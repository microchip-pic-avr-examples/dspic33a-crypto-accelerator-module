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
#include "crypto/common_crypto/crypto_hash.h"
#include "app/app_sha.h"
#include "test_vectors/test_vectors_sha1.h"
#include "test_vectors/test_vectors_sha224.h"
#include "test_vectors/test_vectors_sha256.h"
#include "test_vectors/test_vectors_sha384.h"
#include "test_vectors/test_vectors_sha512.h"

#define MAX_DIGEST_SIZE     (64U)
#define SINGLE_STEP         (false)
#define MULTI_STEP          (true)
#define SHA_TEST_VECTOR_5   (5)

static void SHA_HexArrayPrint(const char *label, const uint8_t *data, uint32_t size)
{
    (void) printf(BLUE
        "\r\n ------------------------------------------"
        "---------------------------------------------------"
        RESET_COLOR);

    (void) printf("\r\n %s  : ", label);

    for (uint32_t index = 0U; index < size; index++)
    {
        if (((index % 16U) == 0U) && (index != 0U))
        {
            (void) printf("\r\n                    ");
        }
        (void) printf(" 0x%02X", data[index]);
    }
}


static void SHA_ArrayEqualityCheck(const uint8_t *expected,
        const uint8_t *actual, size_t size)
{
    uint8_t result = 0U;

    for (size_t byteIndex = 0U; byteIndex < size; byteIndex++)
    {
        result |= (uint8_t)(expected[byteIndex] ^ actual[byteIndex]);
    }

    if (result == 0U)
    {
        (void) printf(GREEN" \r\n PASS "RESET_COLOR);
    }
    else
    {
        const char *expectedHeader = "Expected Digest";
        (void) printf(RED" \r\n FAIL - Data Mismatch"RESET_COLOR);
        SHA_HexArrayPrint(expectedHeader, actual, size);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: SHA Test Functions
// *****************************************************************************
// *****************************************************************************

static crypto_Hash_Status_E SHA_TestDigestRun(
        bool multistep, uint32_t blockSize,
        crypto_Hash_Algo_E algo, uint8_t *data,
        uint32_t dataLength, uint8_t *digestResult)
{
    crypto_Hash_Status_E status;
    uint8_t *localData = data;

    if (multistep)
    {
        st_Crypto_Hash_Sha_Ctx context;

        (void) printf(CYAN
            "\r\n\r\n MULTI-STEP TEST BLOCK SIZE: %lu"
            RESET_COLOR, (unsigned long)blockSize);

        status = Crypto_Hash_Sha_Init(&context, algo, CRYPTO_HANDLER_HW_INTERNAL, 1U);

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
        (void) printf(CYAN"\r\n\r\n SINGLE-STEP TEST"RESET_COLOR);

        status = Crypto_Hash_Sha_Digest(
                CRYPTO_HANDLER_HW_INTERNAL, localData,
                dataLength, digestResult, algo, 1U);
    }

    return status;
}

static void SHA_TestRun(crypto_Hash_Algo_E algo, uint8_t *data,
        uint32_t dataLength, const uint8_t *expectedDigest)
{
    const char *inputHeader =    "Input Data     ";
    const char *outputHeader =   "Output Digest  ";

    uint8_t digestResult[MAX_DIGEST_SIZE];
    unsigned long digestLength;

    switch (algo)
    {
        case CRYPTO_HASH_SHA1:
            digestLength = SHA1_DIGEST_LENGTH_BYTES;
            break;
        case CRYPTO_HASH_SHA2_224:
            digestLength = SHA224_DIGEST_LENGTH_BYTES;
            break;
        case CRYPTO_HASH_SHA2_256:
            digestLength = SHA256_DIGEST_LENGTH_BYTES;
            break;
        case CRYPTO_HASH_SHA2_384:
            digestLength = SHA384_DIGEST_LENGTH_BYTES;
            break;
        case CRYPTO_HASH_SHA2_512:
            digestLength = SHA512_DIGEST_LENGTH_BYTES;
            break;
        default:
            digestLength = SHA256_DIGEST_LENGTH_BYTES;
            break;
    }

    unsigned long digestBitLength = digestLength * 8UL;

    (void) printf(MAG
        "\r\n\r\n *****************************"
        "***************************************************");

    (void) printf(CYAN
        "\r\n ************************* HASH SHA-%lu"
        " TEST - INPUT SIZE %lu "
        "**************************%s",
        (unsigned long)((digestBitLength == 160UL) ? 1UL : digestBitLength),
        (unsigned long)dataLength,
        (digestBitLength == 160UL) ? "**" : "");

    (void) printf(MAG
        "\r\n *****************************"
        "***************************************************"
        RESET_COLOR);

    if (dataLength > 0UL)
    {
        SHA_HexArrayPrint(inputHeader, data, dataLength);
    }
    else
    {
        (void) printf(BLUE
            "\r\n --------------------------------"
            "-----------------------------------------------------"
            RESET_COLOR);

        (void) printf("\r\n %s  :  None", inputHeader);
    }

    crypto_Hash_Status_E status;

    // Test with a single update() call, using the data length as the block size.
    status = SHA_TestDigestRun(SINGLE_STEP, dataLength, algo,
            data, dataLength, digestResult);

    SHA_HexArrayPrint(outputHeader, digestResult, digestLength);

    if (status != CRYPTO_HASH_SUCCESS)
    {
        (void) printf(RED
            "\r\n Crypto v4 SHA-%lu Error Result: %d\n\r"
            RESET_COLOR, digestBitLength, status);
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
                algo, data, dataLength, digestResult);

        if (status != CRYPTO_HASH_SUCCESS)
        {
            (void) printf(RED
                "\r\n Crypto v4 SHA-%lu Error Result: %d\n\r"
                RESET_COLOR, digestBitLength, status);
        }
        else
        {
            SHA_HexArrayPrint(outputHeader,
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
    while (vectorArrayPtr < &sha1TestVectors[NUMBER_OF_SHA1_VECTORS])
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
    while (vectorArrayPtr < &sha224TestVectors[NUMBER_OF_SHA224_VECTORS])
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
    while (vectorArrayPtr < &sha256TestVectors[NUMBER_OF_SHA256_VECTORS])
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
    while (vectorArrayPtr < &sha384TestVectors[NUMBER_OF_SHA384_VECTORS])
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
    while (vectorArrayPtr < &sha512TestVectors[NUMBER_OF_SHA512_VECTORS])
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
