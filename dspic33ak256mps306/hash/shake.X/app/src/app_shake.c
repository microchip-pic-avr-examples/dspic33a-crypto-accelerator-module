/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_shake.c

  Summary:
    This source file handles the running of the SHAKE hashing examples.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2026, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

/* cppcheck-suppress misra-c2012-21.6
*
*  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
*
*  Reasoning: printf is required for printing to the terminal in this code example
*/
#include <stdio.h>
#include <string.h>
#include "crypto/common_crypto/crypto_hash.h"
#include "app/app_shake.h"
#include "app/app_utils.h"
#include "test_vectors/shake/test_vectors_shake128.h"
#include "test_vectors/shake/test_vectors_shake256.h"

extern TEST_VECTOR shake128_test_vectors[NUMBER_OF_SHAKE128_VECTORS];
extern TEST_VECTOR shake256_test_vectors[NUMBER_OF_SHAKE256_VECTORS];

// *****************************************************************************
// *****************************************************************************
// Section: SHAKE Test Functions
// *****************************************************************************
// *****************************************************************************

static crypto_Hash_Status_E SHAKE_DigestGenerate(bool multistep, uint32_t blockSize, crypto_Hash_Algo_E algorithm,
        const uint8_t* data, const uint32_t dataLength, uint8_t* digestResult, uint32_t digestLength)
{
    crypto_Hash_Status_E status;

    /* cppcheck-suppress misra-c2012-18.8
    *
    *  (Rule 18.8) REQUIRED: Variable-length array types shall not be used
    *
    *  Reasoning: Using a variable-length array allows the demo test vectors to be
    *               adjusted independent of the application code.
    */
    uint8_t localData[dataLength];
    uint8_t* localData_ptr = (uint8_t*) &localData;

    (void )memcpy(localData, data, dataLength);

    if (multistep)
    {
        st_Crypto_Hash_Shake_Ctx context;

        (void) printf(CYAN"\r\n\r\n MULTI-STEP TEST BLOCK SIZE: %lu"RESET_COLOR, blockSize);

        status = Crypto_Hash_Shake_Init(&context, algorithm, CRYPTO_HANDLER_HW_INTERNAL, digestLength, SESSION_ID);

        if(status == CRYPTO_HASH_SUCCESS)
        {
            uint32_t localLength = dataLength;
            do
            {
                uint32_t length = (localLength > blockSize)
                        ? blockSize : localLength;
                status = Crypto_Hash_Shake_Update(&context, localData_ptr, length);

                localLength -= length;
                localData_ptr = &localData_ptr[length];

            } while ((localLength > 0U) && (status == CRYPTO_HASH_SUCCESS));
        }

        if(status == CRYPTO_HASH_SUCCESS)
        {
            status = Crypto_Hash_Shake_Final(&context, digestResult);
        }
    }
    else
    {
        (void) printf(CYAN"\r\n\r\n SINGLE-STEP TEST"RESET_COLOR);
        status = Crypto_Hash_Shake_Digest(CRYPTO_HANDLER_HW_INTERNAL, algorithm, localData_ptr, dataLength, digestResult, digestLength, SESSION_ID);
    }

    return status;
}

static void SHAKE_Digest(crypto_Hash_Algo_E algorithm, const uint8_t* data, uint32_t dataLength, const uint8_t* expectedDigest, uint32_t digestLength)
{
    const char *inputHeader =    "Input Data     ";
    const char *outputHeader =   "Output Digest  ";

    /* cppcheck-suppress misra-c2012-18.8
    *
    *  (Rule 18.8) REQUIRED: Variable-length array types shall not be used
    *
    *  Reasoning: The SHAKE algorithm supports digests of varying sizes.
    *                Using a variable-length array allows the demo test vectors to be
    *                adjusted independent of the application code.
    */
    uint8_t digestResult[digestLength];
    const char* algorithmName = algorithmNameGet(algorithm);

    headerPrint(algorithmName, dataLength);

    if (dataLength > 0UL)
    {
        hexArrayPrint(inputHeader, data, dataLength);
    }
    else
    {
        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------------"RESET_COLOR);
        (void) printf("\r\n %s  :  None", inputHeader);
    }

    crypto_Hash_Status_E status;

    // Test with a single update() call, using the data length as the block size.
    status = SHAKE_DigestGenerate(SINGLE_STEP, dataLength, algorithm, data, dataLength, digestResult, digestLength);
    hexArrayPrint(outputHeader, digestResult, digestLength);

    if (status != CRYPTO_HASH_SUCCESS)
    {
        (void) printf(RED"\r\n Crypto v4 %s Error Result: %d\n\r"RESET_COLOR, algorithmName, status);
    }
    else
    {
        arrayEqualityCheck(digestResult, expectedDigest, digestLength);
    }

    // Test multiple iterations of the update() call with different block sizes to show that arbitrary sizes are accepted.
    uint32_t blockSizes[] = {BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32, BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8, BLOCK_SIZE_7};

    for (uint32_t index = 0; index < (sizeof(blockSizes) / sizeof(blockSizes[0])); index++)
    {
        status = SHAKE_DigestGenerate(MULTI_STEP, blockSizes[index], algorithm, data, dataLength, digestResult, digestLength);

        if (status != CRYPTO_HASH_SUCCESS)
        {
            (void) printf(RED"\r\n Crypto v4 %s Error Result: %d\n\r"RESET_COLOR, algorithmName, status);
        }
        else
        {
            hexArrayPrint(outputHeader, digestResult, digestLength);
            arrayEqualityCheck(digestResult, expectedDigest, digestLength);
        }
    }
}

void SHAKE128_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = shake128_test_vectors;
    while(vectorArrayPtr < &shake128_test_vectors[NUMBER_OF_SHAKE128_VECTORS])
    {
        SHAKE_Digest(CRYPTO_HASH_SHA3_SHAKE128, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &shake128_test_vectors[TEST_VECTOR_4];
    SHAKE_Digest(CRYPTO_HASH_SHA3_SHAKE128, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);
#endif
}

void SHAKE256_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = shake256_test_vectors;
    while(vectorArrayPtr < &shake256_test_vectors[NUMBER_OF_SHAKE256_VECTORS])
    {
        SHAKE_Digest(CRYPTO_HASH_SHA3_SHAKE256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &shake256_test_vectors[TEST_VECTOR_4];
    SHAKE_Digest(CRYPTO_HASH_SHA3_SHAKE256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);
#endif
}
