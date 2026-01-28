
/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_gmac.c

  Summary:
    This source file handles the running of the AES GMAC examples.
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
#include "./app_mac.h"

#ifdef RUN_MAC_GMAC_TEST

#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "test_vectors/mac_test_vector.h"
#include "app/app_gmac.h"

#define TEST_SESSION_ID     0x1
#define MAC_TEST_VECTOR_0   0

// Context for HMAC calculations when running individual steps.
// This is too large to put on the stack.
static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4)));
static const char* testWrapper = "MAC";

extern TEST_VECTOR gmac_test_vectors[NUMBER_OF_GMAC_VECTORS];


static void mac_gmac_direct_test(TEST_VECTOR* testVector)
{
    uint8_t generatedGmac[CRYPTO_AESKEYSIZE_256] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLen = testVector->inputKeyLen * 8U;

    char dataInfo[20];
    (void)sprintf(dataInfo, ", GMAC: %lu BYTES", testVector->expectedMacLen);

    printGmacTestHeader(testWrapper, keyBitLen, "GMAC (DIRECT)", testVector->inputDataLen, dataInfo);

    printHexArray("Key                  ", testVector->inputKey, testVector->inputKeyLen);
    printHexArray("Data                 ", testVector->inputData, testVector->inputDataLen);


    status = Crypto_Mac_AesGmac_Direct(CRYPTO_HANDLER_HW_INTERNAL, testVector->initVect, testVector->initVectLen,
                                       generatedGmac, testVector->expectedMacLen,
                                       testVector->inputKey, testVector->inputKeyLen,
                                       testVector->inputData, testVector->inputDataLen, TEST_SESSION_ID);

    if(status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        printHexArray("GMAC Output          ", generatedGmac, testVector->expectedMacLen);
        printHexArray("Expected Output      ", testVector->expectedMac, testVector->expectedMacLen);
        printArrayEqualityResult(generatedGmac, testVector->expectedMac, testVector->expectedMacLen);
    }
    else
    {
        printGmacCryptoError(keyBitLen, "GMAC", "Generation", status);
    }
}

static void mac_gmac_step_test(TEST_VECTOR* testVector)
{
    uint8_t generatedGmac[CRYPTO_AESKEYSIZE_256] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLen = testVector->inputKeyLen * 8U;


    char dataInfo[20];
    (void)sprintf(dataInfo, ", GMAC: %lu BYTES", testVector->expectedMacLen);

    printGmacTestHeader(testWrapper, keyBitLen, "GMAC (STEPWISE)", testVector->inputDataLen, dataInfo);

    printHexArray("Key                  ", testVector->inputKey, testVector->inputKeyLen);
    printHexArray("Data                 ", testVector->inputData, testVector->inputDataLen);

    status = Crypto_Mac_AesGmac_Init(&macCtx, CRYPTO_HANDLER_HW_INTERNAL, testVector->inputKey, testVector->inputKeyLen, TEST_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesGmac_Cipher(&macCtx, testVector-> initVect, testVector->initVectLen,
                                           testVector->inputData, testVector->inputDataLen,
                                           generatedGmac, testVector->expectedMacLen);
    }
    else
    {
        printGmacCryptoError(keyBitLen, "GMAC", "Initialization", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        printHexArray("GMAC Output          ", generatedGmac, testVector->expectedMacLen);
        printHexArray("Expected Output      ", testVector->expectedMac, testVector->expectedMacLen);
        printArrayEqualityResult(generatedGmac, testVector->expectedMac, testVector->expectedMacLen);
    }
    else
    {
        printGmacCryptoError(keyBitLen, "GMAC", "Generation", status);
    }
}

static void mac_gmac_vectors_test(TEST_VECTOR *vectors)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &vectors[0];

    while(vectorArrayPtr < &vectors[NUMBER_OF_GMAC_VECTORS])
    {
        mac_gmac_direct_test(vectorArrayPtr);
        mac_gmac_step_test(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &vectors[MAC_TEST_VECTOR_0];
    mac_gmac_direct_test(vectorArrayPtr);
    mac_gmac_step_test(vectorArrayPtr);
#endif
}

// *****************************************************************************
// *****************************************************************************
// Section: AES MAC Test Functions
// *****************************************************************************
// *****************************************************************************

void mac_gmac_test(void)
{
    mac_gmac_vectors_test(&gmac_test_vectors[0]);
}

#endif // RUN_MAC_HMAC_TEST
