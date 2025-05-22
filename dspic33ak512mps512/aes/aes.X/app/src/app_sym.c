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
#include <stdint.h>
#include "app/app_sym.h"
#include "./app_aes.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"

static uint8_t data[DATA_SIZE] = {
    0x22, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89,
    0x9A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF1, 0x1F,
    0x12, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x98,
    0xA9, 0xBA, 0xCB, 0xDC, 0xED, 0xFE, 0x1F, 0xFF,
    0x11, 0x23, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC, 0x99, 0xAA, 0xBB, 0xCC
};

static uint8_t initializationVector[INIT_VECTOR_SIZE] = {
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10
};

static uint8_t keyData[32] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x20
};

#define TEST_SESSION_ID 0x1

// Context for AES calculations when running individual steps.
// This is too large to put on the stack.
static st_Crypto_Sym_BlockCtx aesCtx __attribute__((aligned(4)));


static crypto_Sym_Status_E aes_sym_test_direct(crypto_CipherOper_E encrypt, crypto_Sym_OpModes_E opMode,
        uint8_t* data, uint32_t dataLen, uint8_t* key, uint32_t keyLen,
        uint8_t* result, uint8_t* initializationVector)
{
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (DIRECT)...");

        printHexArray("Data (Input)         ", data, dataLen);

        status = Crypto_Sym_Aes_EncryptDirect(CRYPTO_HANDLER_HW_INTERNAL,
                opMode, data, dataLen, result, key, keyLen, initializationVector, TEST_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printHexArray("Data (Encrypted)     ", result, dataLen);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (DIRECT)...");

        printHexArray("Data (Input)         ", data, dataLen);

        status = Crypto_Sym_Aes_DecryptDirect(CRYPTO_HANDLER_HW_INTERNAL,
                opMode, data, dataLen, result, key, keyLen, initializationVector, TEST_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printHexArray("Data (Decrypted)     ", result, dataLen);
        }
    }

    return status;
}


static crypto_Sym_Status_E aes_sym_test_steps(crypto_CipherOper_E encrypt, crypto_Sym_OpModes_E opMode,
        uint8_t* data, uint32_t dataLen, uint8_t* key, uint32_t keyLen,
        uint8_t* result, uint8_t* initializationVector)
{
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;


    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (STEPS)...");

        printHexArray("Data (Input)         ", data, dataLen);

        status = Crypto_Sym_Aes_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_ENCRYPT,
                                                    opMode, key, keyLen, initializationVector, TEST_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_Aes_Cipher(&aesCtx, data, dataLen, result);
        }

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printHexArray("Data (Encrypted)     ", result, dataLen);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (STEPS)...");

        printHexArray("Data (Input)         ", data, dataLen);

        status = Crypto_Sym_Aes_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_DECRYPT,
                                                    opMode, key, keyLen, initializationVector, TEST_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_Aes_Cipher(&aesCtx, data, dataLen, result);
        }

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printHexArray("Data (Decrypted)     ", result, dataLen);
        }
    }

    return status;
}

static void aes_sym_test(crypto_Sym_OpModes_E opMode, uint8_t* data, uint32_t dataLen,
        uint8_t* key, uint32_t keyLen, uint8_t* initializationVector)
{
    unsigned long keyBitLen = keyLen * 8U;
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;

    uint8_t encryptedDirectResult[48];
    uint8_t decryptedDirectResult[48];

    uint8_t encryptedStepsResult[48];
    uint8_t decryptedStepsResult[48];

    const char* testWrapper = "SYMMETRIC";
    const char* testType = "???";
    const char* emptyString = "";
    
    switch(opMode)
    {
        case CRYPTO_SYM_OPMODE_ECB:
            testType = "ECB";
            break;
        case CRYPTO_SYM_OPMODE_CTR:
            testType = "CTR";
            break;
        default:
            testType = "???";
            break;
    }

    printTestHeader(testWrapper, keyBitLen, testType, dataLen, emptyString);

    printHexArray("Key                  ", key, keyLen);

    if(initializationVector != NULL)
    {
        printHexArray("Initialization Vector", initializationVector, INIT_VECTOR_SIZE);
    }
    else
    {
        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        (void) printf("\r\n%s", " Initialization Vector  :  None");
    }

    status = aes_sym_test_direct(CRYPTO_CIOP_ENCRYPT, opMode, data, dataLen, key, keyLen, (uint8_t*) &encryptedDirectResult, initializationVector);
    if (status != CRYPTO_SYM_CIPHER_SUCCESS)
    {
        printCryptoError(keyBitLen, testType, "Encrypt", status);
    }
    else
    {
        status = aes_sym_test_direct(CRYPTO_CIOP_DECRYPT, opMode, (uint8_t*) &encryptedDirectResult, dataLen, key, keyLen, (uint8_t*) &decryptedDirectResult, initializationVector);
        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printCryptoError(keyBitLen, testType, "Decrypt", status);
        }
        else
        {
            (void) printf("\r\n\r\n VERIFYING...");
            checkArrayEquality(data, decryptedDirectResult, dataLen);
        }
    }

    (void) printf(BLUE"\r\n\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
    (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);

    status = aes_sym_test_steps(CRYPTO_CIOP_ENCRYPT, opMode, data, dataLen, key, keyLen, (uint8_t*) &encryptedStepsResult, initializationVector);
    if (status != CRYPTO_SYM_CIPHER_SUCCESS)
    {
        printCryptoError(keyBitLen, testType, "Encrypt", status);
    }
    else
    {
        status = aes_sym_test_steps(CRYPTO_CIOP_DECRYPT, opMode, (uint8_t*) &encryptedStepsResult, dataLen, key, keyLen, (uint8_t*) &decryptedStepsResult, initializationVector);
        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printCryptoError(keyBitLen, testType, "Decrypt", status);
        }
        else
        {
            (void) printf("\r\n\r\n VERIFYING...");
            checkArrayEquality(data, decryptedStepsResult, dataLen);
        }
    }

    (void) printf(BLUE"\r\n\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
    (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
    (void) printf("\r\n\r\n COMPARING TESTS...");

    // Verify the direct and stepwise tests generate the same result.
    printHexArray("Direct Encryption    ", encryptedDirectResult, dataLen);
    printHexArray("Stepwise Encryption  ", encryptedStepsResult, dataLen);

    checkArrayEquality(encryptedDirectResult, encryptedStepsResult, dataLen);

}

static void aes_sym_128_test(crypto_Sym_OpModes_E opMode, uint8_t* data, uint8_t* key, uint8_t* initializationVector)
{
#ifdef RUN_ALL_VECTORS
    aes_sym_test(opMode, data, 35, key, CRYPTO_AESKEYSIZE_128, initializationVector);
    aes_sym_test(opMode, data, 41, key, CRYPTO_AESKEYSIZE_128, initializationVector);
#endif
    aes_sym_test(opMode, data, 48, key, CRYPTO_AESKEYSIZE_128, initializationVector);
}

static void aes_sym_192_test(crypto_Sym_OpModes_E opMode, uint8_t* data, uint8_t* key, uint8_t* initializationVector)
{
#ifdef RUN_ALL_VECTORS
    aes_sym_test(opMode, data, 35, key, CRYPTO_AESKEYSIZE_192, initializationVector);
    aes_sym_test(opMode, data, 41, key, CRYPTO_AESKEYSIZE_192, initializationVector);
#endif
    aes_sym_test(opMode, data, 48, key, CRYPTO_AESKEYSIZE_192, initializationVector);
}

static void aes_sym_256_test(crypto_Sym_OpModes_E opMode, uint8_t* data, uint8_t* key, uint8_t* initializationVector)
{
#ifdef RUN_ALL_VECTORS
    aes_sym_test(opMode, data, 35, (uint8_t*) key, CRYPTO_AESKEYSIZE_256, initializationVector);
    aes_sym_test(opMode, data, 41, (uint8_t*) key, CRYPTO_AESKEYSIZE_256, initializationVector);
#endif
    aes_sym_test(opMode, data, 48, (uint8_t*) key, CRYPTO_AESKEYSIZE_256, initializationVector);
}

// *****************************************************************************
// *****************************************************************************
// Section: AES Test Functions
// *****************************************************************************
// *****************************************************************************

void aes_sym_ecb_test(void)
{
    aes_sym_128_test(CRYPTO_SYM_OPMODE_ECB, data, keyData, NULL);
    aes_sym_192_test(CRYPTO_SYM_OPMODE_ECB, data, keyData, NULL);
    aes_sym_256_test(CRYPTO_SYM_OPMODE_ECB, data, keyData, NULL);
}

void aes_sym_ctr_test(void)
{
    aes_sym_128_test(CRYPTO_SYM_OPMODE_CTR, data, keyData, initializationVector);
    aes_sym_192_test(CRYPTO_SYM_OPMODE_CTR, data, keyData, initializationVector);
    aes_sym_256_test(CRYPTO_SYM_OPMODE_CTR, data, keyData, initializationVector);
}

