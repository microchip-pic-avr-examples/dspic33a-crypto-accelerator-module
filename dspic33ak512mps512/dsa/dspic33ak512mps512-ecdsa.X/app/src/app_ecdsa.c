/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecdsa.c

  Summary:
    This source file handles the running of the ECDSA application.
*******************************************************************************/

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

/* cppcheck-suppress misra-c2012-21.6
*
*  (Rule 21.6) Required: The standard library input/output functions shall not be used
*
*  Reasoning: This demo uses printf to print out information to a terminal.
*/
#include <stdio.h>
#include <xc.h>
#include "app/app_ecdsa.h"
#include "crypto/common_crypto/crypto_digsign.h"
#include "mcc_generated_files/system/clock.h"
#include "mcc_generated_files/system/pins.h"
#include "test_vectors/test_vector.h"
#include "app/app_util.h"

static uint8_t signature[P521_LENGTH];
static volatile bool signComplete = false;
static volatile bool verifyComplete = false;

static void ECDSA_CompleteStatesClear(void)
{
    signComplete = false;
    verifyComplete = false;
}

static void ECDSA_SignOperationComplete(void)
{
    signComplete = true;
}

static void ECDSA_VerifyOperationComplete(void)
{
    verifyComplete = true;
}

static void ECDSA_BlockingHardwareTestRun(test_vector_t *testVector)
{
    uint8_t signatureLength = UTIL_SignatureLengthGet(testVector->curveType);
    crypto_DigiSign_Status_E status = CRYPTO_DIGISIGN_SUCCESS;
    UTIL_TestHeaderPrint(testVector->curveInformation, "BLOCKING");
    (void) printf(YELLOW"\r\n\r\n Start Signature Generation"RESET_COLOR);

    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA SIGN API
    status = Crypto_DigiSign_Ecdsa_Sign(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        signature, signatureLength,
        testVector->privateKeyData,
        testVector->privateKeyLength,
        testVector->curveType, SESSION_ID);

    UTIL_HexArrayPrint(SIGNATURE_PRINT, signature, signatureLength);
    UTIL_StatusPrint(status, SIGNING_PRINT);
    UTIL_SplitterPrint();
    (void) printf(YELLOW"\r\n\r\n Start Verification"RESET_COLOR);

    int8_t hashStatus = 0;
    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA VERIFY API
    status = Crypto_DigiSign_Ecdsa_Verify(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        signature, signatureLength,
        testVector->publicKeyData,
        testVector->publicKeyLength,
        &hashStatus, testVector->curveType, SESSION_ID);

    UTIL_StatusPrint(status, VERIFYING_PRINT);
}

static void ECDSA_NonBlockingHardwareTestRun(test_vector_t *testVector)
{
    uint8_t signatureLength = UTIL_SignatureLengthGet(testVector->curveType);
    crypto_DigiSign_Status_E status = CRYPTO_DIGISIGN_SUCCESS;
    UTIL_TestHeaderPrint(testVector->curveInformation, "NON-BLOCKING");

    (void) printf(YELLOW"\r\n\r\n Start Signature Generation"RESET_COLOR);
    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA SIGN Non-Blocking API
    status = Crypto_DigiSign_Ecdsa_Sign_Start(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        testVector->privateKeyData,
        testVector->privateKeyLength,
        testVector->curveType, SESSION_ID);
    if (status == CRYPTO_DIGISIGN_SUCCESS)
    {
        while (!signComplete)
        {
        }
        status = Crypto_DigiSign_Ecdsa_Sign_GetResult(signature, signatureLength);
    }
    UTIL_HexArrayPrint(SIGNATURE_PRINT, signature, signatureLength);
    UTIL_StatusPrint(status, SIGNING_PRINT);
    UTIL_SplitterPrint();

    (void) printf(YELLOW"\r\n\r\n Start Verification"RESET_COLOR);
    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA VERIFY Non-Blocking API
    status = Crypto_DigiSign_Ecdsa_Verify_Start(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        signature, signatureLength,
        testVector->publicKeyData,
        testVector->publicKeyLength,
        testVector->curveType, SESSION_ID);
    if (status == CRYPTO_DIGISIGN_SUCCESS)
    {
        while (!verifyComplete)
        {
        }
        status = Crypto_DigiSign_Ecdsa_Verify_GetResult();
    }
    UTIL_StatusPrint(status, VERIFYING_PRINT);
}

static void ECDSA_P192TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP192);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP192);
}

static void ECDSA_P256TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP256);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP256);
}

static void ECDSA_P384TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP384);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP384);
}

static void ECDSA_P521TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP521);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP521);
}

void ECDSA_TestRun(void)
{
    Crypto_DigiSign_Ecdsa_SignComplete_CallbackRegister(&ECDSA_SignOperationComplete);
    Crypto_DigiSign_Ecdsa_VerifyComplete_CallbackRegister(&ECDSA_VerifyOperationComplete);
    UTIL_SplitterPrint();
    (void) printf("\r\n Starting CRYPTO ECDSA Verify Sign program \r\n");
    ECDSA_P192TestRun();
    ECDSA_P256TestRun();
    ECDSA_P384TestRun();
    ECDSA_P521TestRun();
    UTIL_SplitterPrint();
}
