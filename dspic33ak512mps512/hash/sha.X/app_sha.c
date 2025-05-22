/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This source file handles the running of the Hashing examples.
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
#include "test_vectors/test_vectors_sha1.h"
#include "test_vectors/test_vectors_sha224.h"
#include "test_vectors/test_vectors_sha256.h"
#include "test_vectors/test_vectors_sha384.h"
#include "test_vectors/test_vectors_sha512.h"

#define MAX_DIGEST_SIZE     64
#define SINGLE_STEP         0
#define MULTI_STEP          1
#define SHA_TEST_VECTOR_5   5

extern TEST_VECTOR sha_1_test_vectors[NUMBER_OF_SHA1_VECTORS];
extern TEST_VECTOR sha_224_test_vectors[NUMBER_OF_SHA224_VECTORS];
extern TEST_VECTOR sha_256_test_vectors[NUMBER_OF_SHA256_VECTORS];
extern TEST_VECTOR sha_384_test_vectors[NUMBER_OF_SHA384_VECTORS];
extern TEST_VECTOR sha_512_test_vectors[NUMBER_OF_SHA512_VECTORS];

static void printHexArray(const char* label, const void* data, uint32_t size)
{
    const uint8_t *ldata = (const uint8_t *) data;

    (void) printf(BLUE"\r\n -------------------------------------------------------------------------------------------------"RESET_COLOR);
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


static void checkArrayEquality(const uint8_t *a, const uint8_t *b, size_t size)
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
        printHexArray(expectedHeader, b, size);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: SHA Test Functions
// *****************************************************************************
// *****************************************************************************

static crypto_Hash_Status_E sha_test_digest(bool multistep, uint32_t blockSize, crypto_Hash_Algo_E algo, uint8_t* data, uint32_t dataLen, uint8_t* digestResult)
{
    crypto_Hash_Status_E status;
    uint8_t* localData = data;

    if (multistep)
    {
        st_Crypto_Hash_Sha_Ctx context;

        (void) printf(CYAN"\r\n\r\n MULTI-STEP TEST BLOCK SIZE: %ld"RESET_COLOR, blockSize);

        status = Crypto_Hash_Sha_Init(&context, algo, CRYPTO_HANDLER_HW_INTERNAL, 1);

        if(status == CRYPTO_HASH_SUCCESS)
        {
            uint32_t localLen = dataLen;
            do
            {
                uint32_t dl = (localLen > blockSize) ? blockSize : localLen;
                status = Crypto_Hash_Sha_Update(&context, localData, dl);

                localLen -= dl;
                localData += (uint8_t)dl;

            } while ((localLen > 0U) && (status == CRYPTO_HASH_SUCCESS));
        }

        if(status == CRYPTO_HASH_SUCCESS)
        {
            status = Crypto_Hash_Sha_Final(&context, digestResult);
        }
    }
    else
    {
        (void) printf(CYAN"\r\n\r\n SINGLE-STEP TEST"RESET_COLOR);
        status = Crypto_Hash_Sha_Digest(CRYPTO_HANDLER_HW_INTERNAL, localData, dataLen, digestResult, algo, 1);
    }

    return status;
}

static void sha_test(crypto_Hash_Algo_E algo, uint8_t* data, uint32_t dataLen, const uint8_t* expectedDigest)
{
    const char *inputHeader =    "Input Data     ";
    const char *outputHeader =   "Output Digest  ";

    uint8_t digestResult[MAX_DIGEST_SIZE];
    unsigned long digestLen;

    switch(algo)
    {
        case CRYPTO_HASH_SHA1:
            digestLen = SHA1_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_224:
            digestLen = SHA224_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_256:
            digestLen = SHA256_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_384:
            digestLen = SHA384_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_512:
            digestLen = SHA512_DIGEST_LEN_BYTES;
            break;
        default:
            digestLen = SHA256_DIGEST_LEN_BYTES;
            break;
    }

    unsigned long digestBitLen = digestLen * (uint32_t) 8;

    (void) printf(MAG"\r\n\r\n **************************************************************************************");
    (void) printf(CYAN"\r\n ************************* HASH SHA-%lu TEST - INPUT SIZE %lu **************************%s",
            (digestBitLen == 160U) ? 1U : digestBitLen,
            dataLen,
            (digestBitLen == 160U) ? "**" : "");
    (void) printf(MAG"\r\n **************************************************************************************"RESET_COLOR);

    if (dataLen > 0UL)
    {
        printHexArray(inputHeader, data, dataLen);
    }
    else
    {
        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        (void) printf("\r\n %s  :  None", inputHeader);
    }

    crypto_Hash_Status_E status;

    // Test with a single update() call, using the data length as the block size.
    status = sha_test_digest(SINGLE_STEP, dataLen, algo, data, dataLen, (uint8_t*) &digestResult);
    printHexArray(outputHeader, &digestResult, digestLen);

    if (status != CRYPTO_HASH_SUCCESS)
    {
        (void) printf(RED"\r\n Crypto v4 SHA-%lu Error Result: %d\n\r"RESET_COLOR, digestBitLen, status);
    }
    else
    {
        checkArrayEquality((uint8_t*) &digestResult, expectedDigest, digestLen);
    }

    // Test multiple iterations of the update() call with different block sizes to show that arbitrary sizes are accepted.
    #define BLOCK_SIZE_128 128UL
    #define BLOCK_SIZE_64 64UL
    #define BLOCK_SIZE_32 32UL
    #define BLOCK_SIZE_16 16UL
    #define BLOCK_SIZE_13 13UL
    #define BLOCK_SIZE_8 8UL
    #define BLOCK_SIZE_7 7UL

    uint32_t blockSizes[] = {BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32, BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8, BLOCK_SIZE_7};

    for (uint32_t i = 0; i < (sizeof(blockSizes) / sizeof(blockSizes[0])); i++)
    {
        status = sha_test_digest(MULTI_STEP, blockSizes[i], algo, data, dataLen, (uint8_t*) &digestResult);

        if (status != CRYPTO_HASH_SUCCESS)
        {
            (void) printf(RED"\r\n Crypto v4 SHA-%lu Error Result: %d\n\r"RESET_COLOR, digestBitLen, status);
        }
        else
        {
            printHexArray(outputHeader, &digestResult, digestLen);
            checkArrayEquality((uint8_t*) &digestResult, expectedDigest, digestLen);
        }
    }
}

void sha_1_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &sha_1_test_vectors[0];
    while(vectorArrayPtr < &sha_1_test_vectors[NUMBER_OF_SHA1_VECTORS])
    {
        sha_test(CRYPTO_HASH_SHA1, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_1_test_vectors[SHA_TEST_VECTOR_5];
    sha_test(CRYPTO_HASH_SHA1, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void sha_224_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &sha_224_test_vectors[0];
    while(vectorArrayPtr < &sha_224_test_vectors[NUMBER_OF_SHA224_VECTORS])
    {
        sha_test(CRYPTO_HASH_SHA2_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_224_test_vectors[SHA_TEST_VECTOR_5];
    sha_test(CRYPTO_HASH_SHA2_224, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void sha_256_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &sha_256_test_vectors[0];
    while(vectorArrayPtr < &sha_256_test_vectors[NUMBER_OF_SHA256_VECTORS])
    {
        sha_test(CRYPTO_HASH_SHA2_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_256_test_vectors[SHA_TEST_VECTOR_5];
    sha_test(CRYPTO_HASH_SHA2_256, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void sha_384_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &sha_384_test_vectors[0];
    while(vectorArrayPtr < &sha_384_test_vectors[NUMBER_OF_SHA384_VECTORS])
    {
        sha_test(CRYPTO_HASH_SHA2_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_384_test_vectors[SHA_TEST_VECTOR_5];
    sha_test(CRYPTO_HASH_SHA2_384, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}

void sha_512_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &sha_512_test_vectors[0];
    while(vectorArrayPtr < &sha_512_test_vectors[NUMBER_OF_SHA512_VECTORS])
    {
        sha_test(CRYPTO_HASH_SHA2_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);

        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &sha_512_test_vectors[SHA_TEST_VECTOR_5];
    sha_test(CRYPTO_HASH_SHA2_512, vectorArrayPtr->inputData,
            vectorArrayPtr->inputDataLen, vectorArrayPtr->expectedDigest);
#endif
}