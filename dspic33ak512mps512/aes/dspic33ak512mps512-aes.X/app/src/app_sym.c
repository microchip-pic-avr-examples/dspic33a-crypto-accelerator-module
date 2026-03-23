/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sym.c

  Summary:
    This source file handles the running of the symmetric AES examples.
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
#include "app/app_sym.h"
#include "app/app_aes.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"

static uint8_t data[DATA_SIZE] =
{
    0x22, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF1, 0x1F,
    0x12, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x98, 0xA9, 0xBA, 0xCB, 0xDC, 0xED, 0xFE, 0x1F, 0xFF,
    0x11, 0x23, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0x99, 0xAA, 0xBB, 0xCC
};

static uint8_t initializationVector[INIT_VECTOR_SIZE] =
{
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10
};

static uint8_t keyData[32U] =
{
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x20
};

/**
 * @brief Run a direct (single-call) AES symmetric encrypt or decrypt operation.
 * @param encrypt The cipher operation (encrypt or decrypt).
 * @param opMode The AES operation mode (ECB, CTR, CBC, etc.).
 * @param data Pointer to the input data.
 * @param dataLength Length of the input data in bytes.
 * @param key Pointer to the AES key.
 * @param keyLength Length of the AES key in bytes.
 * @param result Pointer to the output buffer.
 * @param initializationVector Pointer to the IV (NULL for ECB mode).
 * @return Symmetric cipher status code.
 */
static crypto_Sym_Status_E SYM_DirectTestRun(
        crypto_CipherOper_E encrypt,
        crypto_Sym_OpModes_E opMode,
        uint8_t *data, uint32_t dataLength,
        uint8_t *key, uint32_t keyLength,
        uint8_t *result, uint8_t *initializationVector)
{
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (DIRECT)...");

        UTILS_HexArrayPrint("Data (Input)         ", data, dataLength);

        status = Crypto_Sym_Aes_EncryptDirect(
                CRYPTO_HANDLER_HW_INTERNAL, opMode, data,
                dataLength, result, key, keyLength,
                initializationVector, UTILS_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            UTILS_HexArrayPrint("Data (Encrypted)     ", result, dataLength);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (DIRECT)...");

        UTILS_HexArrayPrint("Data (Input)         ", data, dataLength);

        status = Crypto_Sym_Aes_DecryptDirect(
                CRYPTO_HANDLER_HW_INTERNAL, opMode, data,
                dataLength, result, key, keyLength,
                initializationVector, UTILS_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            UTILS_HexArrayPrint("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}

/**
 * @brief Run a stepwise (init/cipher) AES symmetric encrypt or decrypt operation.
 * @param encrypt The cipher operation (encrypt or decrypt).
 * @param opMode The AES operation mode (ECB, CTR, CBC, etc.).
 * @param data Pointer to the input data.
 * @param dataLength Length of the input data in bytes.
 * @param key Pointer to the AES key.
 * @param keyLength Length of the AES key in bytes.
 * @param result Pointer to the output buffer.
 * @param initializationVector Pointer to the IV (NULL for ECB mode).
 * @return Symmetric cipher status code.
 */
static crypto_Sym_Status_E SYM_StepsTestRun(
        crypto_CipherOper_E encrypt,
        crypto_Sym_OpModes_E opMode,
        uint8_t *data, uint32_t dataLength,
        uint8_t *key, uint32_t keyLength,
        uint8_t *result, uint8_t *initializationVector)
{
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;

    /*
     * Context for AES calculations when running individual steps.
     * This is non-reentrant as it is too large to put on the stack.
     */
    static st_Crypto_Sym_BlockCtx aesCtx __attribute__((aligned(4)));

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (STEPS)...");

        UTILS_HexArrayPrint("Data (Input)         ", data, dataLength);

        status = Crypto_Sym_Aes_Init(
                &aesCtx, CRYPTO_HANDLER_HW_INTERNAL,
                CRYPTO_CIOP_ENCRYPT, opMode, key,
                keyLength, initializationVector,
                UTILS_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_Aes_Cipher(&aesCtx, data, dataLength, result);
        }

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            UTILS_HexArrayPrint("Data (Encrypted)     ", result, dataLength);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (STEPS)...");

        UTILS_HexArrayPrint("Data (Input)         ", data, dataLength);

        status = Crypto_Sym_Aes_Init(
                &aesCtx, CRYPTO_HANDLER_HW_INTERNAL,
                CRYPTO_CIOP_DECRYPT, opMode, key,
                keyLength, initializationVector,
                UTILS_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_Aes_Cipher(&aesCtx, data, dataLength, result);
        }

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            UTILS_HexArrayPrint("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}

/**
 * @brief Run direct and stepwise AES tests for a given mode and compare results.
 * @param opMode The AES operation mode (ECB, CTR, CBC, etc.).
 * @param data Pointer to the input data.
 * @param dataLength Length of the input data in bytes.
 * @param key Pointer to the AES key.
 * @param keyLength Length of the AES key in bytes.
 * @param initializationVector Pointer to the IV (NULL for ECB mode).
 */
static void SYM_ModeTestRun(crypto_Sym_OpModes_E opMode, uint8_t *data, uint32_t dataLength,
        uint8_t *key, uint32_t keyLength, uint8_t *initializationVector)
{
    unsigned long keyBitLength = keyLength * 8UL;
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;

    uint8_t encryptedDirectResult[DATA_SIZE];
    uint8_t decryptedDirectResult[DATA_SIZE];

    uint8_t encryptedStepsResult[DATA_SIZE];
    uint8_t decryptedStepsResult[DATA_SIZE];

    const char *testWrapper = "SYMMETRIC";
    const char *testType = "???";
    const char *emptyString = "";

    switch (opMode)
    {
        case CRYPTO_SYM_OPMODE_ECB:
            testType = "ECB";
            break;
        case CRYPTO_SYM_OPMODE_CTR:
            testType = "CTR";
            break;
        case CRYPTO_SYM_OPMODE_CBC:
            testType = "CBC";
            break;
        case CRYPTO_SYM_OPMODE_CFB128:
            testType = "CFB";
            break;
        case CRYPTO_SYM_OPMODE_OFB:
            testType = "OFB";
            break;
        default:
            testType = "???";
            break;
    }

    AES_TestHeaderPrint(testWrapper, keyBitLength, testType, dataLength, emptyString);

    UTILS_HexArrayPrint("Key                  ", key, keyLength);

    if (initializationVector != NULL)
    {
        UTILS_HexArrayPrint("Initialization Vector", initializationVector, INIT_VECTOR_SIZE);
    }
    else
    {
        UTILS_SeparatorPrint(UTILS_BLUE, '-');
        (void) printf("\r\n%s", " Initialization Vector  : None");
    }

    status = SYM_DirectTestRun(CRYPTO_CIOP_ENCRYPT,
            opMode, data, dataLength, key, keyLength,
            encryptedDirectResult,
            initializationVector);
    if (status != CRYPTO_SYM_CIPHER_SUCCESS)
    {
        AES_CryptoErrorPrint(keyBitLength, testType, "Encrypt", status);
    }
    else
    {
        status = SYM_DirectTestRun(
                CRYPTO_CIOP_DECRYPT, opMode,
                encryptedDirectResult,
                dataLength, key, keyLength,
                decryptedDirectResult,
                initializationVector);
        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            AES_CryptoErrorPrint(keyBitLength, testType, "Decrypt", status);
        }
        else
        {
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
            (void) printf("\r\n\r\n VERIFYING...");
            UTILS_ArrayEqualityResultPrint(
                    "AES ENCRYPT/DECRYPT",
                    data, decryptedDirectResult, dataLength);
        }
    }

    (void) printf("\r\n");
    UTILS_SeparatorPrint(UTILS_BLUE, '-');
    UTILS_SeparatorPrint(UTILS_BLUE, '-');

    status = SYM_StepsTestRun(CRYPTO_CIOP_ENCRYPT,
            opMode, data, dataLength, key, keyLength,
            encryptedStepsResult,
            initializationVector);
    if (status != CRYPTO_SYM_CIPHER_SUCCESS)
    {
        AES_CryptoErrorPrint(keyBitLength, testType, "Encrypt", status);
    }
    else
    {
        status = SYM_StepsTestRun(
                CRYPTO_CIOP_DECRYPT, opMode,
                encryptedStepsResult,
                dataLength, key, keyLength,
                decryptedStepsResult,
                initializationVector);
        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            AES_CryptoErrorPrint(keyBitLength, testType, "Decrypt", status);
        }
        else
        {
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
            (void) printf("\r\n\r\n VERIFYING...");
            UTILS_ArrayEqualityResultPrint(
                    "AES ENCRYPT/DECRYPT",
                    data, decryptedStepsResult, dataLength);
        }
    }

    (void) printf("\r\n");
    UTILS_SeparatorPrint(UTILS_BLUE, '-');
    UTILS_SeparatorPrint(UTILS_BLUE, '-');
    (void) printf("\r\n\r\n COMPARING TESTS...");

    // Verify the direct and stepwise tests generate the same result.
    UTILS_HexArrayPrint("Direct Encryption    ", encryptedDirectResult, dataLength);
    UTILS_HexArrayPrint("Stepwise Encryption  ", encryptedStepsResult, dataLength);

    UTILS_ArrayEqualityResultPrint(
            "AES DIRECT vs STEPWISE",
            encryptedDirectResult, encryptedStepsResult, dataLength);

}

/**
 * @brief Run AES-128 symmetric tests for the given operation mode.
 * @param opMode The AES operation mode (ECB, CTR, CBC, etc.).
 * @param data Pointer to the input data.
 * @param key Pointer to the AES key.
 * @param initializationVector Pointer to the IV (NULL for ECB mode).
 */
static void SYM_128TestRun(
        crypto_Sym_OpModes_E opMode, uint8_t *data,
        uint8_t *key, uint8_t *initializationVector)
{
#ifdef RUN_ALL_VECTORS
    SYM_ModeTestRun(opMode, data, 35U, key, CRYPTO_AESKEYSIZE_128, initializationVector);
    SYM_ModeTestRun(opMode, data, 41U, key, CRYPTO_AESKEYSIZE_128, initializationVector);
#endif
    SYM_ModeTestRun(opMode, data, 48U, key, CRYPTO_AESKEYSIZE_128, initializationVector);
}

/**
 * @brief Run AES-192 symmetric tests for the given operation mode.
 * @param opMode The AES operation mode (ECB, CTR, CBC, etc.).
 * @param data Pointer to the input data.
 * @param key Pointer to the AES key.
 * @param initializationVector Pointer to the IV (NULL for ECB mode).
 */
static void SYM_192TestRun(
        crypto_Sym_OpModes_E opMode, uint8_t *data,
        uint8_t *key, uint8_t *initializationVector)
{
#ifdef RUN_ALL_VECTORS
    SYM_ModeTestRun(opMode, data, 35U, key, CRYPTO_AESKEYSIZE_192, initializationVector);
    SYM_ModeTestRun(opMode, data, 41U, key, CRYPTO_AESKEYSIZE_192, initializationVector);
#endif
    SYM_ModeTestRun(opMode, data, 48U, key, CRYPTO_AESKEYSIZE_192, initializationVector);
}

/**
 * @brief Run AES-256 symmetric tests for the given operation mode.
 * @param opMode The AES operation mode (ECB, CTR, CBC, etc.).
 * @param data Pointer to the input data.
 * @param key Pointer to the AES key.
 * @param initializationVector Pointer to the IV (NULL for ECB mode).
 */
static void SYM_256TestRun(
        crypto_Sym_OpModes_E opMode, uint8_t *data,
        uint8_t *key, uint8_t *initializationVector)
{
#ifdef RUN_ALL_VECTORS
    SYM_ModeTestRun(opMode, data, 35U, key, CRYPTO_AESKEYSIZE_256, initializationVector);
    SYM_ModeTestRun(opMode, data, 41U, key, CRYPTO_AESKEYSIZE_256, initializationVector);
#endif
    SYM_ModeTestRun(opMode, data, 48U, key, CRYPTO_AESKEYSIZE_256, initializationVector);
}

// *****************************************************************************
// *****************************************************************************
// Section: Symmetric Test Functions
// *****************************************************************************
// *****************************************************************************

void SYM_EcbTestRun(void)
{
    SYM_128TestRun(CRYPTO_SYM_OPMODE_ECB, data, keyData, NULL);
    SYM_192TestRun(CRYPTO_SYM_OPMODE_ECB, data, keyData, NULL);
    SYM_256TestRun(CRYPTO_SYM_OPMODE_ECB, data, keyData, NULL);
}

void SYM_CtrTestRun(void)
{
    SYM_128TestRun(CRYPTO_SYM_OPMODE_CTR, data, keyData, initializationVector);
    SYM_192TestRun(CRYPTO_SYM_OPMODE_CTR, data, keyData, initializationVector);
    SYM_256TestRun(CRYPTO_SYM_OPMODE_CTR, data, keyData, initializationVector);
}

void SYM_CbcTestRun(void)
{
    SYM_128TestRun(CRYPTO_SYM_OPMODE_CBC, data, keyData, initializationVector);
    SYM_192TestRun(CRYPTO_SYM_OPMODE_CBC, data, keyData, initializationVector);
    SYM_256TestRun(CRYPTO_SYM_OPMODE_CBC, data, keyData, initializationVector);
}

void SYM_CfbTestRun(void)
{
    SYM_128TestRun(CRYPTO_SYM_OPMODE_CFB128, data, keyData, initializationVector);
    SYM_192TestRun(CRYPTO_SYM_OPMODE_CFB128, data, keyData, initializationVector);
    SYM_256TestRun(CRYPTO_SYM_OPMODE_CFB128, data, keyData, initializationVector);
}

void SYM_OfbTestRun(void)
{
    SYM_128TestRun(CRYPTO_SYM_OPMODE_OFB, data, keyData, initializationVector);
    SYM_192TestRun(CRYPTO_SYM_OPMODE_OFB, data, keyData, initializationVector);
    SYM_256TestRun(CRYPTO_SYM_OPMODE_OFB, data, keyData, initializationVector);
}
