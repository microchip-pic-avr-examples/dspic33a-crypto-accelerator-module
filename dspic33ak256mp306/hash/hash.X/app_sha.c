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
Copyright (C) 2025, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
#include "app_sha.h"
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


static void hexArrayPrint(const char* label, const void* data, uint32_t size)
{
    const uint8_t *ldata = (const uint8_t *) data;

    (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------------"RESET_COLOR);
    (void) printf("\r\n %s  : ", label);

    for (uint32_t index = 0U; index < size; index++)
    {
        if (((index % 16U) == 0U) && (index != 0U))
        {
            (void) printf("\r\n                    ");
        }
        (void) printf(" 0x%02X", ldata[index]);
    }
}

static void arrayEqualityCheck(const uint8_t *a, const uint8_t *b, size_t size)
{
    const char *expectedHeader = "Expected Digest";
	const uint8_t *tempa = a;
	const uint8_t *tempb = b;
	uint8_t result = 0;

	for (unsigned int i = 0; i < size; i++)
    {
		result |= tempa[i] ^ tempb[i];
	}

    if(result == 0U)
    {
        (void) printf(GREEN" \r\n PASS "RESET_COLOR);
    }
    else
    {
        (void) printf(RED" \r\n FAIL - Data Mismatch"RESET_COLOR);
        hexArrayPrint(expectedHeader, b, size);
    }
}

static uint32_t shaDigestLengthGet(crypto_Hash_Algo_E algorithm)
{
    uint32_t digestLength;

    switch(algorithm)
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
            digestLength = 0U;
            break;
    }

    return digestLength;
}

static char* shaTypeGet(crypto_Hash_Algo_E algorithm)
{
    char* shaType;

    switch(algorithm)
    {
        case CRYPTO_HASH_SHA1:
            shaType = "SHA-1";
            break;
        case CRYPTO_HASH_SHA2_224:
            shaType = "SHA-224";
            break;
        case CRYPTO_HASH_SHA2_256:
            shaType = "SHA-256";
            break;
        case CRYPTO_HASH_SHA2_384:
            shaType = "SHA-384";
            break;
        case CRYPTO_HASH_SHA2_512:
            shaType = "SHA-512";
            break;
        case CRYPTO_HASH_SHA3_224:
            shaType = "SHA3-224";
            break;
        case CRYPTO_HASH_SHA3_256:
            shaType = "SHA3-256";
            break;
        case CRYPTO_HASH_SHA3_384:
            shaType = "SHA3-384";
            break;
        case CRYPTO_HASH_SHA3_512:
            shaType = "SHA3-512";
            break;
        default:
            shaType = "SHA-1";
            break;
    }

    return shaType;
}

static char* headerPaddingGet(crypto_Hash_Algo_E algorithm)
{
    char* padding;

    switch(algorithm)
    {
        case CRYPTO_HASH_SHA1:
            padding = "***";
            break;
        case CRYPTO_HASH_SHA2_224:
        case CRYPTO_HASH_SHA2_256:
        case CRYPTO_HASH_SHA2_384:
        case CRYPTO_HASH_SHA2_512:
            padding = "*";
            break;
        default:
            padding = "";
            break;
    }

    return padding;
}

static void headerPrint(crypto_Hash_Algo_E algorithm, char* shaType, uint32_t dataLength)
{
    char* headerDivider = "***************************************************************************************************";
    char* sizePadding = "";

    if(dataLength == 0)
    {
        sizePadding = "**";
    }
    else if(dataLength < 100)
    {
        sizePadding = "*";
    }

    (void) printf(MAG"\r\n\r\n %s", headerDivider);
    (void) printf(CYAN"\r\n ********************************%s HASH %s TEST - INPUT SIZE %lu ******************************%s",
            sizePadding, shaType, dataLength, headerPaddingGet(algorithm));
    (void) printf(MAG"\r\n %s", headerDivider);
}

// *****************************************************************************
// *****************************************************************************
// Section: SHA Test Functions
// *****************************************************************************
// *****************************************************************************

static crypto_Hash_Status_E SHA_DigestGenerate(bool multistep, uint32_t blockSize, crypto_Hash_Algo_E algorithm,
        const uint8_t* data, const uint32_t dataLength, uint8_t* digestResult)
{
    crypto_Hash_Status_E status;
    uint8_t* localData = (uint8_t *) data;

    if (multistep)
    {
        st_Crypto_Hash_Sha_Ctx context;

        (void) printf(CYAN"\r\n\r\n MULTI-STEP TEST BLOCK SIZE: %ld"RESET_COLOR, blockSize);

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
                localData += (uint8_t) length;

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

static void SHA_Digest(crypto_Hash_Algo_E algorithm, const uint8_t* data, uint32_t dataLength, const uint8_t* expectedDigest)
{
    const char *inputHeader =    "Input Data     ";
    const char *outputHeader =   "Output Digest  ";

    uint8_t digestResult[MAX_DIGEST_SIZE];

    uint32_t digestLength = shaDigestLengthGet(algorithm);
    char* shaType = shaTypeGet(algorithm);

    headerPrint(algorithm, shaType, dataLength);

    if (dataLength > 0UL)
    {
        hexArrayPrint(inputHeader, data, dataLength);
    }
    else
    {
        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        (void) printf("\r\n %s  :  None", inputHeader);
    }

    crypto_Hash_Status_E status;

    // Test with a single update() call, using the data length as the block size.
    status = SHA_DigestGenerate(SINGLE_STEP, dataLength, algorithm, data, dataLength, digestResult);
    hexArrayPrint(outputHeader, digestResult, digestLength);

    if (status != CRYPTO_HASH_SUCCESS)
    {
        (void) printf(RED"\r\n Crypto v4 %s Error Result: %d\n\r"RESET_COLOR, shaType, status);
    }
    else
    {
        arrayEqualityCheck(digestResult, expectedDigest, digestLength);
    }

    // Test multiple iterations of the update() call with different block sizes to show that arbitrary sizes are accepted.
    uint32_t blockSizes[] = {BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32, BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8, BLOCK_SIZE_7};

    for (uint32_t index = 0; index < (sizeof(blockSizes) / sizeof(blockSizes[0])); index++)
    {
        status = SHA_DigestGenerate(MULTI_STEP, blockSizes[index], algorithm, data, dataLength, digestResult);

        if (status != CRYPTO_HASH_SUCCESS)
        {
            (void) printf(RED"\r\n Crypto v4 %s Error Result: %d\n\r"RESET_COLOR, shaType, status);
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
    TEST_VECTOR* vectorArrayPtr = &sha_1_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha_224_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha_256_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha_384_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha_512_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha3_224_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha3_256_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha3_384_test_vectors[SHA_TEST_VECTOR_4];
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
    TEST_VECTOR* vectorArrayPtr = &sha3_512_test_vectors[SHA_TEST_VECTOR_4];
    SHA_Digest(CRYPTO_HASH_SHA3_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}