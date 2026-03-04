
/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_mac.c

  Summary:
    This source file handles the running of the AES MAC examples.
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
#include "app/app_mac.h"

#ifdef RUN_MAC_HMAC_TEST

#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_hash.h"
#include "test_vectors/mac_test_vector.h"
#include "app/app_hmac.h"

#define TEST_SESSION_ID     0x1
#define MAC_TEST_VECTOR_0   0

static const char* hmacTestWrapper = "MAC";

static const char *mac_hmac_get_algorithm_string(crypto_Hash_Algo_E hashAlgorithm)
{
    const char *algorithmString = "UNKNOWN";

    switch (hashAlgorithm)
    {
        case CRYPTO_HASH_SHA1:
            algorithmString = "1";
            break;

        case CRYPTO_HASH_SHA2_224:
            algorithmString = "224";
            break;

        case CRYPTO_HASH_SHA2_256:
            algorithmString = "256";
            break;

        case CRYPTO_HASH_SHA2_384:
            algorithmString = "384";
            break;

        case CRYPTO_HASH_SHA2_512:
            algorithmString = "512";
            break;

        default:
            break;
    }

    return algorithmString;
}

static void mac_hmac_direct_test(TEST_VECTOR* testVector)
{
    uint8_t generatedHmac[CRYPTO_HASH_MAX_DIGEST_SIZE] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLen = testVector->inputKeyLen * 8U;

    char dataInfo[20];
    (void)sprintf(dataInfo, ", HMAC: %lu BYTES", (unsigned long)testVector->expectedMacLen);

    printHmacTestHeader(hmacTestWrapper, keyBitLen, "HMAC (DIRECT)",
                        mac_hmac_get_algorithm_string(testVector->hashAlgorithm), testVector->inputDataLen, dataInfo);

    printHexArray("Key                  ", testVector->inputKey, testVector->inputKeyLen);
    printHexArray("Data                 ", testVector->inputData, testVector->inputDataLen);

    status = Crypto_Mac_Hmac_Direct(CRYPTO_HANDLER_HW_INTERNAL, testVector->inputData, testVector->inputDataLen,
            generatedHmac, testVector->inputKey, testVector->inputKeyLen, testVector->hashAlgorithm, TEST_SESSION_ID);

    if(status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        printHexArray("HMAC Output          ", generatedHmac, testVector->expectedMacLen);
        printHexArray("Expected Output      ", testVector->expectedMac, testVector->expectedMacLen);
        printArrayEqualityResult(generatedHmac, testVector->expectedMac, testVector->expectedMacLen);
    }
    else
    {
        printHmacCryptoError("HMAC", "Generation", status);
    }
}

static void mac_hmac_step_test(TEST_VECTOR* testVector)
{
    // Context for HMAC calculations when running individual steps.
    // This is non-reentrant as it is too large to put on the stack.
    static st_Crypto_Mac_Hmac_ctx macCtx __attribute__((aligned(4)));
    uint8_t generatedHmac[CRYPTO_HASH_MAX_DIGEST_SIZE] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLen = testVector->inputKeyLen * 8U;

    char dataInfo[20];
    (void)sprintf(dataInfo, ", HMAC: %lu BYTES", (unsigned long)testVector->expectedMacLen);

    printHmacTestHeader(hmacTestWrapper, keyBitLen, "HMAC (STEPWISE)",
            mac_hmac_get_algorithm_string(testVector->hashAlgorithm), testVector->inputDataLen, dataInfo);

    printHexArray("Key                  ", testVector->inputKey, testVector->inputKeyLen);
    printHexArray("Data                 ", testVector->inputData, testVector->inputDataLen);

    status = Crypto_Mac_Hmac_Init(&macCtx, CRYPTO_HANDLER_HW_INTERNAL, testVector->inputKey, testVector->inputKeyLen, testVector->hashAlgorithm, TEST_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hmac_Cipher(&macCtx, testVector->inputData, testVector->inputDataLen);
    }
    else
    {
        printHmacCryptoError("HMAC", "Initialization", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hmac_Final(&macCtx, generatedHmac);
    }
    else
    {
        printHmacCryptoError("HMAC", "Cipher", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        printHexArray("HMAC Output          ", generatedHmac, testVector->expectedMacLen);
        printHexArray("Expected Output      ", testVector->expectedMac, testVector->expectedMacLen);
        printArrayEqualityResult(generatedHmac, testVector->expectedMac, testVector->expectedMacLen);
    }
    else
    {
        printHmacCryptoError("HMAC", "Generation", status);
    }
}

static void mac_hmac_vectors_test(TEST_VECTOR *vectors)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &vectors[0];

    while(vectorArrayPtr < &vectors[NUMBER_OF_HMAC_VECTORS])
    {
        mac_hmac_direct_test(vectorArrayPtr);
        mac_hmac_step_test(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &vectors[MAC_TEST_VECTOR_0];
    mac_hmac_direct_test(vectorArrayPtr);
    mac_hmac_step_test(vectorArrayPtr);
#endif
}

// *****************************************************************************
// *****************************************************************************
// Section: HMAC Test Functions
// *****************************************************************************
// *****************************************************************************

void mac_hmac_test(void)
{
    mac_hmac_vectors_test(&hmac_1_test_vectors[0]);
    mac_hmac_vectors_test(&hmac_224_test_vectors[0]);
    mac_hmac_vectors_test(&hmac_256_test_vectors[0]);
    mac_hmac_vectors_test(&hmac_384_test_vectors[0]);
    mac_hmac_vectors_test(&hmac_512_test_vectors[0]);
}

#endif // RUN_MAC_HMAC_TEST
