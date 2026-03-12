/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_cmac.c

  Summary:
    This source file handles the running of the AES CMAC examples.
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

#ifdef RUN_MAC_CMAC_TEST

#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "test_vectors/mac_test_vector.h"
#include "app/app_cmac.h"

/*
*
*  (Rule 2.5) ADVISORY: A project should not contain unused macro declarations
*
*  Reasoning: This macro is in sections that are not defined by default, but are needed.
*/
/* cppcheck-suppress misra-c2012-2.5 */
#define MAC_TEST_VECTOR_3   (3)

static const char *cmacTestWrapper = "MAC";

static void CMAC_DirectTestRun(test_vector_t *testVector)
{
    uint8_t generatedCmac[16] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLength = testVector->inputKeyLength * 8U;

    char dataInfo[20];
    (void) sprintf(dataInfo, ", CMAC: %lu BYTES",
        (unsigned long)testVector->expectedMacLength);

    MAC_CmacTestHeaderPrint(cmacTestWrapper, keyBitLength,
        "CMAC (DIRECT)", testVector->inputDataLength,
        dataInfo);

    UTILS_HexArrayPrint("Key                  ",
            testVector->inputKey,
            testVector->inputKeyLength);

    UTILS_HexArrayPrint("Data                 ",
            testVector->inputData,
            testVector->inputDataLength);

    status = Crypto_Mac_AesCmac_Direct(
            CRYPTO_HANDLER_HW_INTERNAL,
            testVector->inputData,
            testVector->inputDataLength,
            generatedCmac,
            testVector->expectedMacLength,
            testVector->inputKey,
            testVector->inputKeyLength,
            UTILS_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        UTILS_HexArrayPrint("CMAC Output          ", generatedCmac, testVector->expectedMacLength);
        UTILS_ArrayEqualityResultPrint(
            "AES-CMAC OUTPUT",
            generatedCmac,
            testVector->expectedMac,
            testVector->expectedMacLength);
    }
    else
    {
        MAC_CmacCryptoErrorPrint(keyBitLength, "CMAC",
            "Generation", status);
    }
}

static void CMAC_StepTestRun(test_vector_t *testVector)
{
    /*
     * Context for CMAC calculations when running individual steps.
     * This is non-reentrant as it is too large to put on the stack.
     */
    static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4)));
    uint8_t generatedCmac[16] = {0};
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    unsigned long keyBitLength = testVector->inputKeyLength * 8U;

    char dataInfo[20];
    (void) sprintf(dataInfo, ", CMAC: %lu BYTES",
        (unsigned long)testVector->expectedMacLength);

    MAC_CmacTestHeaderPrint(cmacTestWrapper, keyBitLength,
        "CMAC (STEPWISE)", testVector->inputDataLength,
        dataInfo);

    UTILS_HexArrayPrint("Key                  ",
            testVector->inputKey,
            testVector->inputKeyLength);

    UTILS_HexArrayPrint("Data                 ",
            testVector->inputData,
            testVector->inputDataLength);

    status = Crypto_Mac_AesCmac_Init(&macCtx,
            CRYPTO_HANDLER_HW_INTERNAL,
            testVector->inputKey,
            testVector->inputKeyLength,
            UTILS_SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesCmac_Cipher(&macCtx,
                testVector->inputData,
                testVector->inputDataLength);
    }
    else
    {
        MAC_CmacCryptoErrorPrint(keyBitLength, "CMAC", "Initialization", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesCmac_Final(&macCtx,
                generatedCmac,
                testVector->expectedMacLength);
    }
    else
    {
        MAC_CmacCryptoErrorPrint(keyBitLength, "CMAC", "Cipher", status);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        UTILS_HexArrayPrint("CMAC Output          ",
            generatedCmac, testVector->expectedMacLength);

        UTILS_ArrayEqualityResultPrint(
            "AES-CMAC OUTPUT",
            generatedCmac,
            testVector->expectedMac,
            testVector->expectedMacLength);
    }
    else
    {
        MAC_CmacCryptoErrorPrint(keyBitLength, "CMAC",
            "Generation", status);
    }
}

static void CMAC_128TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &cmac128TestVectors[0];

    while (vectorArrayPtr < &cmac128TestVectors[NUMBER_OF_CMAC_VECTORS])
    {
        CMAC_DirectTestRun(vectorArrayPtr);
        CMAC_StepTestRun(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &cmac128TestVectors[MAC_TEST_VECTOR_3];
    CMAC_DirectTestRun(vectorArrayPtr);
    CMAC_StepTestRun(vectorArrayPtr);
#endif
}

static void CMAC_192TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &cmac192TestVectors[0];

    while (vectorArrayPtr < &cmac192TestVectors[NUMBER_OF_CMAC_VECTORS])
    {
        CMAC_DirectTestRun(vectorArrayPtr);
        CMAC_StepTestRun(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &cmac192TestVectors[MAC_TEST_VECTOR_3];
    CMAC_DirectTestRun(vectorArrayPtr);
    CMAC_StepTestRun(vectorArrayPtr);
#endif
}

static void CMAC_256TestRun(void)
{
#ifdef RUN_ALL_VECTORS
    test_vector_t *vectorArrayPtr = &cmac256TestVectors[0];

    while (vectorArrayPtr < &cmac256TestVectors[NUMBER_OF_CMAC_VECTORS])
    {
        CMAC_DirectTestRun(vectorArrayPtr);
        CMAC_StepTestRun(vectorArrayPtr);
        vectorArrayPtr++;
    }
#else
    test_vector_t *vectorArrayPtr = &cmac256TestVectors[MAC_TEST_VECTOR_3];
    CMAC_DirectTestRun(vectorArrayPtr);
    CMAC_StepTestRun(vectorArrayPtr);
#endif
}

// *****************************************************************************
// *****************************************************************************
// Section: CMAC Test Functions
// *****************************************************************************
// *****************************************************************************

void CMAC_TestRun(void)
{
    CMAC_128TestRun();
    CMAC_192TestRun();
    CMAC_256TestRun();
}

#endif // RUN_MAC_CMAC_TEST
