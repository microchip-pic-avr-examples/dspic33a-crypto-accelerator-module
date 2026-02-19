/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sha.c

  Summary:
    This source file handles the running of the SHA hashing examples.
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
#include "app/app_sha.h"
#include "app/app_utils.h"
#include "test_vectors/test_vectors_sha_1.h"
#include "test_vectors/test_vectors_sha_224.h"
#include "test_vectors/test_vectors_sha_256.h"
#include "test_vectors/test_vectors_sha_384.h"
#include "test_vectors/test_vectors_sha_512.h"

#define MAX_DIGEST_SIZE    (SHA_512_DIGEST_LEN_BYTES)

extern TEST_VECTOR sha_1_test_vectors[NUMBER_OF_SHA_1_VECTORS];

extern TEST_VECTOR sha_224_test_vectors[NUMBER_OF_SHA_224_VECTORS];
extern TEST_VECTOR sha_256_test_vectors[NUMBER_OF_SHA_256_VECTORS];
extern TEST_VECTOR sha_384_test_vectors[NUMBER_OF_SHA_384_VECTORS];
extern TEST_VECTOR sha_512_test_vectors[NUMBER_OF_SHA_512_VECTORS];

extern TEST_VECTOR sha3_224_test_vectors[NUMBER_OF_SHA3_224_VECTORS];
extern TEST_VECTOR sha3_256_test_vectors[NUMBER_OF_SHA3_256_VECTORS];
extern TEST_VECTOR sha3_384_test_vectors[NUMBER_OF_SHA3_384_VECTORS];
extern TEST_VECTOR sha3_512_test_vectors[NUMBER_OF_SHA3_512_VECTORS];

// *****************************************************************************
// *****************************************************************************
// Section: SHA Test Functions
// *****************************************************************************
// *****************************************************************************

static crypto_Hash_Status_E SHA_DigestGenerate(bool multistep, uint32_t blockSize, crypto_Hash_Algo_E algorithm,
        uint8_t* data, uint32_t dataLength, uint8_t* digestResult)
{
    crypto_Hash_Status_E status;

    uint8_t* localData = data;

    if (multistep)
    {
        static st_Crypto_Hash_Sha_Ctx context;

        (void) printf(CYAN"\r\n\r\n MULTI-STEP TEST BLOCK SIZE: %lu"RESET_COLOR, blockSize);

        status = Crypto_Hash_Sha_Init(&context, algorithm, CRYPTO_HANDLER_HW_INTERNAL, SESSION_ID);

        if(status == CRYPTO_HASH_SUCCESS)
        {
            uint32_t localLength = dataLength;
            do
            {
                uint32_t length = (localLength > blockSize)
                        ? blockSize : localLength;
                status = Crypto_Hash_Sha_Update(&context, localData, length);

                localLength -= length;
                localData = &localData[length];

            } while ((localLength > 0U) && (status == CRYPTO_HASH_SUCCESS));
        }

        if(status == CRYPTO_HASH_SUCCESS)
        {
            status = Crypto_Hash_Sha_Final(&context, digestResult);
        }
    }
    else
    {
        (void) printf(CYAN"\r\n\r\n SINGLE-STEP TEST"RESET_COLOR);
        status = Crypto_Hash_Sha_Digest(CRYPTO_HANDLER_HW_INTERNAL, localData, dataLength, digestResult, algorithm, SESSION_ID);
    }

    return status;
}

static void SHA_Digest(crypto_Hash_Algo_E algorithm, uint8_t* data, uint32_t dataLength, const uint8_t* expectedDigest)
{
    const char *inputHeader =    "Input Data     ";
    const char *outputHeader =   "Output Digest  ";

    uint8_t digestResult[MAX_DIGEST_SIZE];

    uint32_t digestLength = shaDigestLengthGet(algorithm);
    const char* algorithmName = algorithmNameGet(algorithm);

    headerPrint(algorithm, algorithmName, dataLength);

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
    status = SHA_DigestGenerate(SINGLE_STEP, dataLength, algorithm, data, dataLength, digestResult);
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
    const uint32_t blockSizes[] = {BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32, BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8, BLOCK_SIZE_7};

    for (uint32_t index = 0; index < (sizeof(blockSizes) / sizeof(blockSizes[0])); index++)
    {
        status = SHA_DigestGenerate(MULTI_STEP, blockSizes[index], algorithm, data, dataLength, digestResult);

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

void SHA_1_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha_1_test_vectors;
    while(vectorArrayPtr < &sha_1_test_vectors[NUMBER_OF_SHA_1_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA1, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_1_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA1, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_224_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha_224_test_vectors;
    while(vectorArrayPtr < &sha_224_test_vectors[NUMBER_OF_SHA_224_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA2_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_224_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA2_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_256_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha_256_test_vectors;
    while(vectorArrayPtr < &sha_256_test_vectors[NUMBER_OF_SHA_256_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA2_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_256_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA2_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_384_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha_384_test_vectors;
    while(vectorArrayPtr < &sha_384_test_vectors[NUMBER_OF_SHA_384_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA2_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_384_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA2_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_512_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha_512_test_vectors;
    while(vectorArrayPtr < &sha_512_test_vectors[NUMBER_OF_SHA_512_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA2_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_512_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA2_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA3_224_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha3_224_test_vectors;
    while(vectorArrayPtr < &sha3_224_test_vectors[NUMBER_OF_SHA3_224_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA3_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha3_224_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA3_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA3_256_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha3_256_test_vectors;
    while(vectorArrayPtr < &sha3_256_test_vectors[NUMBER_OF_SHA3_256_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA3_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha3_256_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA3_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA3_384_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha3_384_test_vectors;
    while(vectorArrayPtr < &sha3_384_test_vectors[NUMBER_OF_SHA3_384_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA3_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha3_384_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA3_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void SHA3_512_Digest(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = sha3_512_test_vectors;
    while(vectorArrayPtr < &sha3_512_test_vectors[NUMBER_OF_SHA3_512_VECTORS])
    {
        SHA_Digest(CRYPTO_HASH_SHA3_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha3_512_test_vectors[TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA3_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}