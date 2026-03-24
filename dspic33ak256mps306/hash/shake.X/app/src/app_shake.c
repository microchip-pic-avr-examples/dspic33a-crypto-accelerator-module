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
#include "test_vectors/shake/test_vectors_shake128.h"
#include "test_vectors/shake/test_vectors_shake256.h"

#define SINGLE_STEP             (false)
#define MULTI_STEP              (true)
#define SHAKE_TEST_VECTOR_5     (5)

/**
 * @brief Compare an expected digest with the actual digest and print the result.
 * @param expected Pointer to the expected digest data.
 * @param actual Pointer to the actual digest data.
 * @param size Number of bytes to compare.
 */
static void SHAKE_ArrayEqualityCheck(const uint8_t *expected,
        const uint8_t *actual, size_t size)
{
    bool passed = UTILS_ArrayEqualityCheck(expected, actual, size);
    UTILS_StatusPrint("SHAKE DIGEST", passed);

    if (!passed)
    {
        UTILS_HexArrayPrint("Expected Digest",
                actual, (uint32_t) size);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: SHAKE Test Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Run a SHAKE hash digest test in single-step or multi-step mode.
 * @param multistep true for multi-step (init/update/final), false for single-step.
 * @param blockSize Block size in bytes for multi-step update calls.
 * @param algorithm The SHAKE algorithm to use.
 * @param data Pointer to the input data.
 * @param dataLength Length of the input data in bytes.
 * @param digestResult Pointer to the output buffer for the computed digest.
 * @param digestLength Length of the digest output in bytes.
 * @return Hash status code.
 */
static crypto_Hash_Status_E SHAKE_TestDigestRun(
        bool multistep, uint32_t blockSize,
        crypto_Hash_Algo_E algorithm, uint8_t *data,
        uint32_t dataLength, uint8_t *digestResult,
        uint32_t digestLength)
{
    crypto_Hash_Status_E status;
    uint8_t *localData = data;

    if (multistep)
    {
        static st_Crypto_Hash_Shake_Ctx context;

        (void) printf(UTILS_CYAN
            "\r\n\r\n MULTI-STEP TEST BLOCK SIZE: %lu"
            UTILS_RESET_COLOR, (unsigned long)blockSize);

        status = Crypto_Hash_Shake_Init(&context, algorithm,
                CRYPTO_HANDLER_HW_INTERNAL, digestLength,
                UTILS_SESSION_ID);

        if (status == CRYPTO_HASH_SUCCESS)
        {
            uint32_t localLength = dataLength;
            do
            {
                uint32_t blockLength =
                        (localLength > blockSize) ? blockSize : localLength;
                status = Crypto_Hash_Shake_Update(&context, localData, blockLength);

                localLength -= blockLength;
                localData = &localData[blockLength];

            } while ((localLength > 0U) && (status == CRYPTO_HASH_SUCCESS));
        }

        if (status == CRYPTO_HASH_SUCCESS)
        {
            status = Crypto_Hash_Shake_Final(&context, digestResult);
        }
    }
    else
    {
        (void) printf(UTILS_CYAN"\r\n\r\n SINGLE-STEP TEST"UTILS_RESET_COLOR);

        status = Crypto_Hash_Shake_Digest(
                CRYPTO_HANDLER_HW_INTERNAL, algorithm, localData,
                dataLength, digestResult, digestLength,
                UTILS_SESSION_ID);
    }

    return status;
}

/**
 * @brief Run a complete SHAKE test with single-step and multi-step modes.
 * @param algorithm The SHAKE algorithm to use.
 * @param data Pointer to the input data.
 * @param dataLength Length of the input data in bytes.
 * @param expectedDigest Pointer to the expected digest for verification.
 * @param digestLength Length of the digest output in bytes.
 */
static void SHAKE_TestRun(crypto_Hash_Algo_E algorithm, uint8_t *data,
        uint32_t dataLength, const uint8_t *expectedDigest,
        uint32_t digestLength)
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
    const char *algorithmName;

    switch (algorithm)
    {
        case CRYPTO_HASH_SHA3_SHAKE128:
            algorithmName = "SHAKE128";
            break;
        case CRYPTO_HASH_SHA3_SHAKE256:
            algorithmName = "SHAKE256";
            break;
        default:
            algorithmName = "SHAKE";
            break;
    }

    {
        char header[80];
        (void) sprintf(header, " HASH %s TEST - INPUT SIZE %lu",
                algorithmName, (unsigned long) dataLength);

        UTILS_TestHeaderPrint(header);
    }

    if (dataLength > 0UL)
    {
        UTILS_HexArrayPrint(inputHeader, data, dataLength);
    }
    else
    {
        UTILS_SeparatorPrint(UTILS_BLUE, '-');

        (void) printf("\r\n %s        : None", inputHeader);
    }

    crypto_Hash_Status_E status;

    // Test with a single direct digest call.
    status = SHAKE_TestDigestRun(SINGLE_STEP, dataLength, algorithm,
            data, dataLength, digestResult, digestLength);

    UTILS_HexArrayPrint(outputHeader, digestResult, digestLength);

    if (status != CRYPTO_HASH_SUCCESS)
    {
        UTILS_CryptoErrorPrint(algorithmName, (int) status);
    }
    else
    {
        SHAKE_ArrayEqualityCheck(
                digestResult, expectedDigest, digestLength);
    }

    /*
     * Test multiple iterations of the update() call with
     * different block sizes to show arbitrary sizes are accepted.
     */
    #define BLOCK_SIZE_128 (128UL)
    #define BLOCK_SIZE_64  (64UL)
    #define BLOCK_SIZE_32  (32UL)
    #define BLOCK_SIZE_16  (16UL)
    #define BLOCK_SIZE_13  (13UL)
    #define BLOCK_SIZE_8   (8UL)
    #define BLOCK_SIZE_7   (7UL)

    uint32_t blockSizes[] =
    {
        BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32, BLOCK_SIZE_16,
        BLOCK_SIZE_13, BLOCK_SIZE_8, BLOCK_SIZE_7
    };

    for (uint32_t blockIndex = 0U;
            blockIndex < (sizeof(blockSizes) / sizeof(blockSizes[0]));
            blockIndex++)
    {
        status = SHAKE_TestDigestRun(MULTI_STEP, blockSizes[blockIndex],
                algorithm, data, dataLength, digestResult, digestLength);

        if (status != CRYPTO_HASH_SUCCESS)
        {
            UTILS_CryptoErrorPrint(algorithmName, (int) status);
        }
        else
        {
            UTILS_HexArrayPrint(outputHeader,
                    digestResult, digestLength);

            SHAKE_ArrayEqualityCheck(
                    digestResult, expectedDigest, digestLength);
        }
    }
}

void SHAKE_Shake128TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &shake128TestVectors[0];
    while (vectorArrayPtr < &shake128TestVectors[NUMBER_OF_SHAKE128_VECTORS])
    {
        SHAKE_TestRun(CRYPTO_HASH_SHA3_SHAKE128, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &shake128TestVectors[SHAKE_TEST_VECTOR_5];
    SHAKE_TestRun(CRYPTO_HASH_SHA3_SHAKE128, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);
#endif
}

void SHAKE_Shake256TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &shake256TestVectors[0];
    while (vectorArrayPtr < &shake256TestVectors[NUMBER_OF_SHAKE256_VECTORS])
    {
        SHAKE_TestRun(CRYPTO_HASH_SHA3_SHAKE256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &shake256TestVectors[SHAKE_TEST_VECTOR_5];
    SHAKE_TestRun(CRYPTO_HASH_SHA3_SHAKE256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest,
                vectorArrayPtr->digestLength);
#endif
}
