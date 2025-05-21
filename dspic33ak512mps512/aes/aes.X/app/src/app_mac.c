
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
#include "./app_aes.h"
#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "test_vectors/mac_test_vector.h"
#include "app/app_mac.h"

#define TEST_SESSION_ID     0x1
#define MAC_TEST_VECTOR_3   3

// Context for CMAC calculations when running individual steps.
// This is too large to put on the stack.
static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4)));
static const char* testWrapper = "MAC";

extern TEST_VECTOR cmac_128_test_vectors[NUMBER_OF_CMAC_VECTORS];
extern TEST_VECTOR cmac_192_test_vectors[NUMBER_OF_CMAC_VECTORS];
extern TEST_VECTOR cmac_256_test_vectors[NUMBER_OF_CMAC_VECTORS];

static void aes_cmac_direct_test(TEST_VECTOR* testVector)
{
    uint8_t generatedCmac[16] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLen = testVector->inputKeyLen * 8U;

    char dataInfo[20];
    (void)sprintf(dataInfo, ", CMAC: %lu BYTES", testVector->expectedMacLen);

    printTestHeader(testWrapper, keyBitLen, "CMAC (DIRECT)", testVector->inputDataLen, dataInfo);

    printHexArray("Key                  ", testVector->inputKey, testVector->inputKeyLen);
    printHexArray("Data (Original)      ", testVector->inputData, testVector->inputDataLen);

    status = Crypto_Mac_AesCmac_Direct(CRYPTO_HANDLER_HW_INTERNAL, testVector->inputData, testVector->inputDataLen,
            generatedCmac, testVector->expectedMacLen, testVector->inputKey, testVector->inputKeyLen, TEST_SESSION_ID);

    if(status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        printHexArray("CMAC Output          ", generatedCmac, testVector->expectedMacLen);
        checkArrayEquality(generatedCmac, testVector->expectedMac, testVector->expectedMacLen);
    }
    else
    {
        printCryptoError(keyBitLen, "CMAC", "Generation", status);
    }
}

static void aes_cmac_step_test(TEST_VECTOR* testVector)
{
    uint8_t generatedCmac[16] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLen = testVector->inputKeyLen * 8U;


    char dataInfo[20];
    (void)sprintf(dataInfo, ", CMAC: %lu BYTES", testVector->expectedMacLen);

    printTestHeader(testWrapper, keyBitLen, "CMAC (STEPWISE)", testVector->inputDataLen, dataInfo);

    printHexArray("Key                  ", testVector->inputKey, testVector->inputKeyLen);
    printHexArray("Data (Original)      ", testVector->inputData, testVector->inputDataLen);

    status = Crypto_Mac_AesCmac_Init(&macCtx, CRYPTO_HANDLER_HW_INTERNAL, testVector->inputKey, testVector->inputKeyLen, TEST_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesCmac_Cipher(&macCtx, testVector->inputData, testVector->inputDataLen);
    }
    else
    {
        printCryptoError(keyBitLen, "CMAC", "Initialization", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesCmac_Final(&macCtx, generatedCmac, testVector->expectedMacLen);
    }
    else
    {
        printCryptoError(keyBitLen, "CMAC", "Cipher", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        printHexArray("CMAC Output          ", generatedCmac, testVector->expectedMacLen);
        checkArrayEquality(generatedCmac, testVector->expectedMac, testVector->expectedMacLen);
    }
    else
    {
        printCryptoError(keyBitLen, "CMAC", "Generation", status);
    }
}

static void aes_mac_cmac_128_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &cmac_128_test_vectors[0];

    while(vectorArrayPtr < &cmac_128_test_vectors[NUMBER_OF_CMAC_VECTORS])
    {
        aes_cmac_direct_test(vectorArrayPtr);
        aes_cmac_step_test(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &cmac_128_test_vectors[MAC_TEST_VECTOR_3];
    aes_cmac_direct_test(vectorArrayPtr);
    aes_cmac_step_test(vectorArrayPtr);
#endif
}

static void aes_mac_cmac_192_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &cmac_192_test_vectors[0];

    while(vectorArrayPtr < &cmac_192_test_vectors[NUMBER_OF_CMAC_VECTORS])
    {
       aes_cmac_direct_test(vectorArrayPtr);
       aes_cmac_step_test(vectorArrayPtr);
       vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &cmac_192_test_vectors[MAC_TEST_VECTOR_3];
    aes_cmac_direct_test(vectorArrayPtr);
    aes_cmac_step_test(vectorArrayPtr);
#endif
}

static void aes_mac_cmac_256_test(void)
{
#ifdef RUN_ALL_VECTORS
    TEST_VECTOR* vectorArrayPtr = &cmac_256_test_vectors[0];
    
    while(vectorArrayPtr < &cmac_256_test_vectors[NUMBER_OF_CMAC_VECTORS])
    {
        aes_cmac_direct_test(vectorArrayPtr);
        aes_cmac_step_test(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    TEST_VECTOR* vectorArrayPtr = &cmac_256_test_vectors[MAC_TEST_VECTOR_3];
    aes_cmac_direct_test(vectorArrayPtr);
    aes_cmac_step_test(vectorArrayPtr);
#endif
}

// *****************************************************************************
// *****************************************************************************
// Section: AES MAC Test Functions
// *****************************************************************************
// *****************************************************************************

void aes_mac_cmac_test(void)
{
    aes_mac_cmac_128_test();
    aes_mac_cmac_192_test();
    aes_mac_cmac_256_test();
}

