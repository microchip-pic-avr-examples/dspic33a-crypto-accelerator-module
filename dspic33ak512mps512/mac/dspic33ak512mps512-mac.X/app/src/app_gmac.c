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

#ifdef RUN_MAC_GMAC_TEST

#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "test_vectors/mac_test_vector.h"
#include "app/app_gmac.h"

#define TEST_SESSION_ID     0x1U

/*
*
*  (Rule 2.5) ADVISORY: A project should not contain unused macro declarations
*
*  Reasoning: This macro is in sections that are not defined by default, but are needed.
*/
/* cppcheck-suppress misra-c2012-2.5 */
#define MAC_TEST_VECTOR_0   0

static const char *gmacTestWrapper = "MAC";

static void GMAC_DirectTestRun(test_vector_t *testVector)
{
    uint8_t generatedGmac[CRYPTO_AESKEYSIZE_256] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLength = testVector->inputKeyLength * 8U;

    char dataInfo[20];
    (void) sprintf(dataInfo, ", GMAC: %lu BYTES",
        (unsigned long)testVector->expectedMacLength);

    MAC_GmacTestHeaderPrint(gmacTestWrapper, keyBitLength,
        "GMAC (DIRECT)", testVector->inputDataLength,
        dataInfo);

    MAC_HexArrayPrint("Key                  ",
        testVector->inputKey, testVector->inputKeyLength);

    MAC_HexArrayPrint("Data                 ",
        testVector->inputData,
        testVector->inputDataLength);

    status = Crypto_Mac_AesGmac_Direct(
            CRYPTO_HANDLER_HW_INTERNAL,
            testVector->initVect,
            testVector->initVectLength,
            generatedGmac,
            testVector->expectedMacLength,
            testVector->inputKey,
            testVector->inputKeyLength,
            testVector->inputData,
            testVector->inputDataLength,
            TEST_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        MAC_HexArrayPrint("GMAC Output          ",
            generatedGmac,
            testVector->expectedMacLength);

        MAC_HexArrayPrint("Expected Output      ",
            testVector->expectedMac,
            testVector->expectedMacLength);

        MAC_ArrayEqualityResultPrint(generatedGmac,
            testVector->expectedMac,
            testVector->expectedMacLength);
    }
    else
    {
        MAC_GmacCryptoErrorPrint(keyBitLength, "GMAC",
            "Generation", status);
    }
}

static void GMAC_StepTestRun(test_vector_t *testVector)
{
    // Context for GMAC calculations when running individual steps.
    // This is non-reentrant as it is too large to put on the stack.
    static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4)));
    uint8_t generatedGmac[CRYPTO_AESKEYSIZE_256] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLength = testVector->inputKeyLength * 8U;

    char dataInfo[20];

    (void) sprintf(dataInfo, ", GMAC: %lu BYTES",
        (unsigned long)testVector->expectedMacLength);

    MAC_GmacTestHeaderPrint(gmacTestWrapper, keyBitLength,
        "GMAC (STEPWISE)", testVector->inputDataLength,
        dataInfo);

    MAC_HexArrayPrint("Key                  ",
        testVector->inputKey, testVector->inputKeyLength);

    MAC_HexArrayPrint("Data                 ",
        testVector->inputData,
        testVector->inputDataLength);

    status = Crypto_Mac_AesGmac_Init(&macCtx,
            CRYPTO_HANDLER_HW_INTERNAL,
            testVector->inputKey,
            testVector->inputKeyLength,
            TEST_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesGmac_Cipher(&macCtx,
                testVector->initVect,
                testVector->initVectLength,
                testVector->inputData,
                testVector->inputDataLength,
                generatedGmac,
                testVector->expectedMacLength);
    }
    else
    {
        MAC_GmacCryptoErrorPrint(keyBitLength, "GMAC",
            "Initialization", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        MAC_HexArrayPrint("GMAC Output          ",
            generatedGmac,
            testVector->expectedMacLength);

        MAC_HexArrayPrint("Expected Output      ",
            testVector->expectedMac,
            testVector->expectedMacLength);

        MAC_ArrayEqualityResultPrint(generatedGmac,
            testVector->expectedMac,
            testVector->expectedMacLength);
    }
    else
    {
        MAC_GmacCryptoErrorPrint(keyBitLength, "GMAC",
            "Generation", status);
    }
}

static void GMAC_VectorsTestRun(test_vector_t *vectors)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &vectors[0];

    while (vectorArrayPtr < &vectors[NUMBER_OF_GMAC_VECTORS])
    {
        GMAC_DirectTestRun(vectorArrayPtr);
        GMAC_StepTestRun(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &vectors[MAC_TEST_VECTOR_0];
    GMAC_DirectTestRun(vectorArrayPtr);
    GMAC_StepTestRun(vectorArrayPtr);
#endif
}

// *****************************************************************************
// *****************************************************************************
// Section: GMAC Test Functions
// *****************************************************************************
// *****************************************************************************

void GMAC_TestRun(void)
{
    GMAC_VectorsTestRun(&gmacTestVectors[0]);
}

#endif // RUN_MAC_GMAC_TEST
