/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_hmac.c

  Summary:
    This source file handles the running of the HMAC examples.
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
#include "app/app_mac.h"

#ifdef RUN_MAC_HMAC_TEST

#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_hash.h"
#include "test_vectors/mac_test_vector.h"
#include "app/app_hmac.h"

#ifndef RUN_ALL_VECTORS
#define MAC_TEST_VECTOR_0   (0)
#endif

static const char *hmacTestWrapper = "MAC";

static const char *HMAC_AlgorithmStringGet(crypto_Hash_Algo_E hashAlgorithm)
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

static void HMAC_DirectTestRun(test_vector_t *testVector)
{
    uint8_t generatedHmac[CRYPTO_HASH_MAX_DIGEST_SIZE] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLength = testVector->inputKeyLength * 8U;

    char dataInfo[20];
    (void) sprintf(dataInfo, ", HMAC: %lu BYTES",
        (unsigned long)testVector->expectedMacLength);

    MAC_HmacTestHeaderPrint(hmacTestWrapper, keyBitLength,
        "HMAC (DIRECT)",
        HMAC_AlgorithmStringGet(testVector->hashAlgorithm),
        testVector->inputDataLength, dataInfo);

    UTILS_HexArrayPrint("Key                  ",
        testVector->inputKey, testVector->inputKeyLength);

    UTILS_HexArrayPrint("Data                 ",
        testVector->inputData, testVector->inputDataLength);

    status = Crypto_Mac_Hmac_Direct(
            CRYPTO_HANDLER_HW_INTERNAL,
            testVector->inputData,
            testVector->inputDataLength,
            generatedHmac,
            testVector->inputKey,
            testVector->inputKeyLength,
            testVector->hashAlgorithm,
            UTILS_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        UTILS_HexArrayPrint("HMAC Output          ",
            generatedHmac, testVector->expectedMacLength);

        UTILS_HexArrayPrint("Expected Output      ",
            testVector->expectedMac,
            testVector->expectedMacLength);

        UTILS_ArrayEqualityResultPrint(
            "HMAC OUTPUT",
            generatedHmac,
            testVector->expectedMac,
            testVector->expectedMacLength);
    }
    else
    {
        MAC_HmacCryptoErrorPrint("HMAC", "Generation", status);
    }
}

static void HMAC_StepTestRun(test_vector_t *testVector)
{
    /*
     * Context for HMAC calculations when running individual steps.
     * This is non-reentrant as it is too large to put on the stack.
     */
    static st_Crypto_Mac_Hmac_ctx macCtx __attribute__((aligned(4)));
    uint8_t generatedHmac[CRYPTO_HASH_MAX_DIGEST_SIZE] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLength = testVector->inputKeyLength * 8U;

    char dataInfo[20];
    (void) sprintf(dataInfo, ", HMAC: %lu BYTES",
        (unsigned long)testVector->expectedMacLength);

    MAC_HmacTestHeaderPrint(hmacTestWrapper, keyBitLength,
        "HMAC (STEPWISE)",
        HMAC_AlgorithmStringGet(testVector->hashAlgorithm),
        testVector->inputDataLength, dataInfo);

    UTILS_HexArrayPrint("Key                  ",
        testVector->inputKey, testVector->inputKeyLength);

    UTILS_HexArrayPrint("Data                 ",
        testVector->inputData, testVector->inputDataLength);

    status = Crypto_Mac_Hmac_Init(&macCtx,
            CRYPTO_HANDLER_HW_INTERNAL,
            testVector->inputKey,
            testVector->inputKeyLength,
            testVector->hashAlgorithm,
            UTILS_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hmac_Cipher(&macCtx,
                testVector->inputData,
                testVector->inputDataLength);
    }
    else
    {
        MAC_HmacCryptoErrorPrint("HMAC", "Initialization", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hmac_Final(&macCtx, generatedHmac);
    }
    else
    {
        MAC_HmacCryptoErrorPrint("HMAC", "Cipher", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        UTILS_HexArrayPrint("HMAC Output          ",
            generatedHmac, testVector->expectedMacLength);

        UTILS_HexArrayPrint("Expected Output      ",
            testVector->expectedMac,
            testVector->expectedMacLength);

        UTILS_ArrayEqualityResultPrint(
            "HMAC OUTPUT",
            generatedHmac,
            testVector->expectedMac,
            testVector->expectedMacLength);
    }
    else
    {
        MAC_HmacCryptoErrorPrint("HMAC", "Generation",
            status);
    }
}

static void HMAC_VectorsTestRun(test_vector_t *vectors)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &vectors[0];

    while (vectorArrayPtr < &vectors[NUMBER_OF_HMAC_VECTORS])
    {
        HMAC_DirectTestRun(vectorArrayPtr);
        HMAC_StepTestRun(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &vectors[MAC_TEST_VECTOR_0];
    HMAC_DirectTestRun(vectorArrayPtr);
    HMAC_StepTestRun(vectorArrayPtr);
#endif
}

// *****************************************************************************
// *****************************************************************************
// Section: HMAC Test Functions
// *****************************************************************************
// *****************************************************************************

void HMAC_TestRun(void)
{
    HMAC_VectorsTestRun(&hmac1TestVectors[0]);
    HMAC_VectorsTestRun(&hmac224TestVectors[0]);
    HMAC_VectorsTestRun(&hmac256TestVectors[0]);
    HMAC_VectorsTestRun(&hmac384TestVectors[0]);
    HMAC_VectorsTestRun(&hmac512TestVectors[0]);
}

#endif // RUN_MAC_HMAC_TEST
