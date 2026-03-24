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
#include "test_vectors/test_vectors_sha_1.h"
#include "test_vectors/test_vectors_sha_224.h"
#include "test_vectors/test_vectors_sha_256.h"
#include "test_vectors/test_vectors_sha_384.h"
#include "test_vectors/test_vectors_sha_512.h"

#define MAX_DIGEST_SIZE     (64U)
#define SINGLE_STEP         (false)
#define MULTI_STEP          (true)
#define SHA_TEST_VECTOR_4   (4)
#define SHA_TEST_VECTOR_5   (5)

/**
 * @brief Compare an expected digest with the actual digest and print the result.
 * @param expected Pointer to the expected digest data.
 * @param actual Pointer to the actual digest data.
 * @param size Number of bytes to compare.
 */
static void SHA_ArrayEqualityCheck(const uint8_t *expected,
        const uint8_t *actual, size_t size)
{
    bool passed = UTILS_ArrayEqualityCheck(expected, actual, size);
    UTILS_StatusPrint("SHA DIGEST", passed);

    if (!passed)
    {
        UTILS_HexArrayPrint("Expected Digest",
                actual, (uint32_t) size);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: SHA Test Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Run a SHA hash digest test in single-step or multi-step mode.
 * @param multistep true for multi-step (init/update/final), false for single-step.
 * @param blockSize Block size in bytes for multi-step update calls.
 * @param algorithm The SHA algorithm to use.
 * @param data Pointer to the input data.
 * @param dataLength Length of the input data in bytes.
 * @param digestResult Pointer to the output buffer for the computed digest.
 * @return Hash status code.
 */
static crypto_Hash_Status_E SHA_TestDigestRun(
        bool multistep, uint32_t blockSize,
        crypto_Hash_Algo_E algorithm, uint8_t *data,
        uint32_t dataLength, uint8_t *digestResult)
{
    crypto_Hash_Status_E status;
    uint8_t *localData = data;

    if (multistep)
    {
        static st_Crypto_Hash_Sha_Ctx context;

        (void) printf(UTILS_CYAN
            "\r\n\r\n MULTI-STEP TEST BLOCK SIZE: %lu"
            UTILS_RESET_COLOR, (unsigned long)blockSize);

        status = Crypto_Hash_Sha_Init(&context, algorithm,
                CRYPTO_HANDLER_HW_INTERNAL, UTILS_SESSION_ID);

        if (status == CRYPTO_HASH_SUCCESS)
        {
            uint32_t localLength = dataLength;
            do
            {
                uint32_t blockLength =
                        (localLength > blockSize) ? blockSize : localLength;
                status = Crypto_Hash_Sha_Update(&context, localData, blockLength);

                localLength -= blockLength;
                localData = &localData[blockLength];

            } while ((localLength > 0U) && (status == CRYPTO_HASH_SUCCESS));
        }

        if (status == CRYPTO_HASH_SUCCESS)
        {
            status = Crypto_Hash_Sha_Final(&context, digestResult);
        }
    }
    else
    {
        (void) printf(UTILS_CYAN"\r\n\r\n SINGLE-STEP TEST"UTILS_RESET_COLOR);

        status = Crypto_Hash_Sha_Digest(
                CRYPTO_HANDLER_HW_INTERNAL, localData,
                dataLength, digestResult, algorithm, UTILS_SESSION_ID);
    }

    return status;
}

/**
 * @brief Run a complete SHA test with single-step and multi-step modes.
 * @param algorithm The SHA algorithm to use.
 * @param data Pointer to the input data.
 * @param dataLength Length of the input data in bytes.
 * @param expectedDigest Pointer to the expected digest for verification.
 */
static void SHA_TestRun(crypto_Hash_Algo_E algorithm, uint8_t *data,
        uint32_t dataLength, const uint8_t *expectedDigest)
{
    const char *inputHeader =    "Input Data     ";
    const char *outputHeader =   "Output Digest  ";

    uint8_t digestResult[MAX_DIGEST_SIZE];
    unsigned long digestLength;

    switch (algorithm)
    {
        case CRYPTO_HASH_SHA1:
            digestLength = SHA_1_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_224:
        case CRYPTO_HASH_SHA3_224:
            digestLength = SHA_224_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_256:
        case CRYPTO_HASH_SHA3_256:
            digestLength = SHA_256_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_384:
        case CRYPTO_HASH_SHA3_384:
            digestLength = SHA_384_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_512:
        case CRYPTO_HASH_SHA3_512:
            digestLength = SHA_512_DIGEST_LEN_BYTES;
            break;
        default:
            digestLength = SHA_256_DIGEST_LEN_BYTES;
            break;
    }

    unsigned long digestBitLength = digestLength * 8UL;

    {
        char header[80];

        switch (algorithm)
        {
            case CRYPTO_HASH_SHA1:
                (void) sprintf(header, " HASH SHA-1 TEST - INPUT SIZE %lu",
                        (unsigned long) dataLength);
                break;
            case CRYPTO_HASH_SHA3_224:
            case CRYPTO_HASH_SHA3_256:
            case CRYPTO_HASH_SHA3_384:
            case CRYPTO_HASH_SHA3_512:
                (void) sprintf(header, " HASH SHA3-%lu TEST - INPUT SIZE %lu",
                        digestBitLength, (unsigned long) dataLength);
                break;
            default:
                (void) sprintf(header, " HASH SHA-%lu TEST - INPUT SIZE %lu",
                        digestBitLength, (unsigned long) dataLength);
                break;
        }

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
    status = SHA_TestDigestRun(SINGLE_STEP, dataLength, algorithm,
            data, dataLength, digestResult);

    UTILS_HexArrayPrint(outputHeader, digestResult, digestLength);

    if (status != CRYPTO_HASH_SUCCESS)
    {
        char errorDesc[40];
        (void) sprintf(errorDesc, "SHA-%lu",
                digestBitLength);
        UTILS_CryptoErrorPrint(errorDesc, (int) status);
    }
    else
    {
        SHA_ArrayEqualityCheck(
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
        status = SHA_TestDigestRun(MULTI_STEP, blockSizes[blockIndex],
                algorithm, data, dataLength, digestResult);

        if (status != CRYPTO_HASH_SUCCESS)
        {
            char errorDesc[40];
            (void) sprintf(errorDesc, "SHA-%lu",
                    digestBitLength);
            UTILS_CryptoErrorPrint(errorDesc, (int) status);
        }
        else
        {
            UTILS_HexArrayPrint(outputHeader,
                    digestResult, digestLength);

            SHA_ArrayEqualityCheck(
                    digestResult, expectedDigest, digestLength);
        }
    }
}

void SHA_Sha1TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha1TestVectors[0];
    while (vectorArrayPtr < &sha1TestVectors[NUMBER_OF_SHA_1_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA1, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha1TestVectors[SHA_TEST_VECTOR_5];
    SHA_TestRun(CRYPTO_HASH_SHA1, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha224TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha224TestVectors[0];
    while (vectorArrayPtr < &sha224TestVectors[NUMBER_OF_SHA_224_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA2_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha224TestVectors[SHA_TEST_VECTOR_5];
    SHA_TestRun(CRYPTO_HASH_SHA2_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha256TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha256TestVectors[0];
    while (vectorArrayPtr < &sha256TestVectors[NUMBER_OF_SHA_256_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA2_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha256TestVectors[SHA_TEST_VECTOR_5];
    SHA_TestRun(CRYPTO_HASH_SHA2_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha384TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha384TestVectors[0];
    while (vectorArrayPtr < &sha384TestVectors[NUMBER_OF_SHA_384_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA2_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha384TestVectors[SHA_TEST_VECTOR_5];
    SHA_TestRun(CRYPTO_HASH_SHA2_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha512TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha512TestVectors[0];
    while (vectorArrayPtr < &sha512TestVectors[NUMBER_OF_SHA_512_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA2_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha512TestVectors[SHA_TEST_VECTOR_5];
    SHA_TestRun(CRYPTO_HASH_SHA2_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha3224TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha3_224TestVectors[0];
    while (vectorArrayPtr < &sha3_224TestVectors[NUMBER_OF_SHA3_224_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA3_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha3_224TestVectors[SHA_TEST_VECTOR_5];
    SHA_TestRun(CRYPTO_HASH_SHA3_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha3256TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha3_256TestVectors[0];
    while (vectorArrayPtr < &sha3_256TestVectors[NUMBER_OF_SHA3_256_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA3_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha3_256TestVectors[SHA_TEST_VECTOR_5];
    SHA_TestRun(CRYPTO_HASH_SHA3_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha3384TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha3_384TestVectors[0];
    while (vectorArrayPtr < &sha3_384TestVectors[NUMBER_OF_SHA3_384_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA3_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha3_384TestVectors[SHA_TEST_VECTOR_4];
    SHA_TestRun(CRYPTO_HASH_SHA3_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}

void SHA_Sha3512TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &sha3_512TestVectors[0];
    while (vectorArrayPtr < &sha3_512TestVectors[NUMBER_OF_SHA3_512_VECTORS])
    {
        SHA_TestRun(CRYPTO_HASH_SHA3_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &sha3_512TestVectors[SHA_TEST_VECTOR_4];
    SHA_TestRun(CRYPTO_HASH_SHA3_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLength, vectorArrayPtr->expectedDigest);
#endif
}
