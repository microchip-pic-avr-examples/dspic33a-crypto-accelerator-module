/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecdsa.c

  Summary:
    This source file handles the running of the ECDSA application.
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
*  Reasoning: This demo uses printf to print out information to a terminal.
*/
#include <stdio.h>
#include "app/app_ecdsa.h"
#include "crypto/common_crypto/crypto_digsign.h"
#include "test_vectors/test_vector.h"
#include "utils/app_utils.h"

#define P192_LENGTH      (48U)
#define P256_LENGTH      (64U)
#define P384_LENGTH      (96U)
#define P521_LENGTH      (132U)
#define SIGNATURE_PRINT  "Signature            "
#define SIGNING_PRINT    "SIGN"
#define VERIFYING_PRINT  "VERIFY"

static uint8_t signature[P521_LENGTH];
static volatile bool signComplete = false;
static volatile bool verifyComplete = false;

/**
 * @brief Clear the sign and verify completion flags.
 */
static void ECDSA_CompleteStatesClear(void)
{
    signComplete = false;
    verifyComplete = false;
}

/**
 * @brief Callback that sets the sign completion flag.
 */
static void ECDSA_SignOperationComplete(void)
{
    signComplete = true;
}

/**
 * @brief Callback that sets the verify completion flag.
 */
static void ECDSA_VerifyOperationComplete(void)
{
    verifyComplete = true;
}

/**
 * @brief Print the ECDSA operation status to the terminal.
 * @param status The digital signature status code.
 * @param type The operation type string (e.g., "SIGN", "VERIFY").
 */
static void ECDSA_StatusPrint(crypto_DigiSign_Status_E status,
        const char *type)
{
    char description[80];
    (void) sprintf(description, "ECDSA %s", type);

    if (status != CRYPTO_DIGISIGN_SUCCESS)
    {
        UTILS_CryptoErrorPrint(description, (int) status);
    }
    else
    {
        UTILS_StatusPrint(description, true);
    }
}

/**
 * @brief Get the signature byte length for the given ECC curve.
 * @param curve The ECC curve type.
 * @return Signature length in bytes, or 0 if the curve is unsupported.
 */
static uint8_t ECDSA_SignatureLengthGet(crypto_EccCurveType_E curve)
{
    uint8_t length = 0U;

    switch (curve)
    {
        case CRYPTO_ECC_CURVE_P192:
            length = P192_LENGTH;
            break;
        case CRYPTO_ECC_CURVE_P256:
            length = P256_LENGTH;
            break;
        case CRYPTO_ECC_CURVE_P384:
            length = P384_LENGTH;
            break;
        case CRYPTO_ECC_CURVE_P521:
            length = P521_LENGTH;
            break;
        default:
            break;
    }

    return length;
}

/**
 * @brief Run blocking ECDSA sign and verify using the hardware accelerator.
 * @param testVector Pointer to the ECDSA test vector.
 */
static void ECDSA_BlockingHardwareTestRun(test_vector_t *testVector)
{
    uint8_t signatureLength = ECDSA_SignatureLengthGet(testVector->curveType);
    crypto_DigiSign_Status_E status = CRYPTO_DIGISIGN_SUCCESS;
    char header[80];
    (void) sprintf(header, " %s ECDSA BLOCKING",
            testVector->curveInformation);
    UTILS_TestHeaderPrint(header);
    (void) printf(UTILS_YELLOW"\r\n\r\n Start Signature Generation"UTILS_RESET_COLOR);

    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA SIGN API
    status = Crypto_DigiSign_Ecdsa_Sign(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        signature, signatureLength,
        testVector->privateKeyData,
        testVector->privateKeyLength,
        testVector->curveType, UTILS_SESSION_ID);

    UTILS_HexArrayPrint(SIGNATURE_PRINT, signature, signatureLength);
    ECDSA_StatusPrint(status, SIGNING_PRINT);
    UTILS_SplitterPrint();
    (void) printf(UTILS_YELLOW"\r\n\r\n Start Verification"UTILS_RESET_COLOR);

    int8_t hashStatus = 0;
    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA VERIFY API
    status = Crypto_DigiSign_Ecdsa_Verify(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        signature, signatureLength,
        testVector->publicKeyData,
        testVector->publicKeyLength,
        &hashStatus, testVector->curveType, UTILS_SESSION_ID);

    ECDSA_StatusPrint(status, VERIFYING_PRINT);
}

/**
 * @brief Run non-blocking ECDSA sign and verify using the hardware accelerator.
 * @param testVector Pointer to the ECDSA test vector.
 */
static void ECDSA_NonBlockingHardwareTestRun(test_vector_t *testVector)
{
    uint8_t signatureLength = ECDSA_SignatureLengthGet(testVector->curveType);
    crypto_DigiSign_Status_E status = CRYPTO_DIGISIGN_SUCCESS;
    char header[80];
    (void) sprintf(header, " %s ECDSA NON-BLOCKING",
            testVector->curveInformation);
    UTILS_TestHeaderPrint(header);

    (void) printf(UTILS_YELLOW"\r\n\r\n Start Signature Generation"UTILS_RESET_COLOR);
    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA SIGN Non-Blocking API
    status = Crypto_DigiSign_Ecdsa_Sign_Start(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        testVector->privateKeyData,
        testVector->privateKeyLength,
        testVector->curveType, UTILS_SESSION_ID);
    if (status == CRYPTO_DIGISIGN_SUCCESS)
    {
        while (!signComplete)
        {
        }
        status = Crypto_DigiSign_Ecdsa_Sign_GetResult(signature, signatureLength);
    }
    UTILS_HexArrayPrint(SIGNATURE_PRINT, signature, signatureLength);
    ECDSA_StatusPrint(status, SIGNING_PRINT);
    UTILS_SplitterPrint();

    (void) printf(UTILS_YELLOW"\r\n\r\n Start Verification"UTILS_RESET_COLOR);
    ECDSA_CompleteStatesClear();
    // Crypto Library ECDSA VERIFY Non-Blocking API
    status = Crypto_DigiSign_Ecdsa_Verify_Start(
        CRYPTO_HANDLER_HW_INTERNAL,
        testVector->hashData, testVector->hashLength,
        signature, signatureLength,
        testVector->publicKeyData,
        testVector->publicKeyLength,
        testVector->curveType, UTILS_SESSION_ID);
    if (status == CRYPTO_DIGISIGN_SUCCESS)
    {
        while (!verifyComplete)
        {
        }
        status = Crypto_DigiSign_Ecdsa_Verify_GetResult();
    }
    ECDSA_StatusPrint(status, VERIFYING_PRINT);
}

/**
 * @brief Run ECDSA tests for the P-192 curve.
 */
static void ECDSA_P192TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP192);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP192);
}

/**
 * @brief Run ECDSA tests for the P-256 curve.
 */
static void ECDSA_P256TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP256);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP256);
}

/**
 * @brief Run ECDSA tests for the P-384 curve.
 */
static void ECDSA_P384TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP384);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP384);
}

/**
 * @brief Run ECDSA tests for the P-521 curve.
 */
static void ECDSA_P521TestRun(void)
{
    ECDSA_BlockingHardwareTestRun(&ecdsaP521);
    ECDSA_NonBlockingHardwareTestRun(&ecdsaP521);
}

void ECDSA_TestRun(void)
{
    Crypto_DigiSign_Ecdsa_SignComplete_CallbackRegister(&ECDSA_SignOperationComplete);
    Crypto_DigiSign_Ecdsa_VerifyComplete_CallbackRegister(&ECDSA_VerifyOperationComplete);
    ECDSA_P192TestRun();
    ECDSA_P256TestRun();
    ECDSA_P384TestRun();
    ECDSA_P521TestRun();
}
