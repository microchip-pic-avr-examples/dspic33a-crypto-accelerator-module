/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sym_xts.c

  Summary:
    This source file handles the running of the symmetric XTS AES examples.
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
#include <string.h>
#include "app_aes.h"
#include "app/app_sym.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"
#include "test_vectors/xts_test_vector.h"

#define TEST_SESSION_ID 0x1

// Context for AES calculations when running individual steps.
// This is too large to put on the stack.
static st_Crypto_Sym_BlockCtx aesCtx __attribute__((aligned(4)));

static crypto_Sym_Status_E sym_xts_test(crypto_CipherOper_E encrypt,
        uint8_t* data, uint32_t dataLength, uint8_t* key, uint32_t keyLength,
        uint8_t* tweak, uint8_t* result)
{
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING...");

        if (dataLength != 0UL)
        {
            printHexArray("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Sym_Aes_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, encrypt,
                                     CRYPTO_SYM_OPMODE_XTS, key, keyLength, NULL, TEST_SESSION_ID);
        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_AesXts_Cipher(&aesCtx, data, dataLength, result, tweak);
        }

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printHexArray("Data (Encrypted)     ", result, dataLength);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING...");

        printHexArray("Data (Input)         ", data, dataLength);

        status = Crypto_Sym_Aes_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, encrypt,
                                     CRYPTO_SYM_OPMODE_XTS, key, keyLength, NULL, TEST_SESSION_ID);
        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_AesXts_Cipher(&aesCtx, data, dataLength, result, tweak);
        }

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printHexArray("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}

static void checkEncryption(uint8_t *text, uint8_t *result, uint32_t length)
{
    (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);

    if (length > 0UL)
    {
        if (0 == memcmp(result, text, length))
        {
            (void)printf(GREEN"\r\n Encryption: PASS    "RESET_COLOR );
        }
        else
        {
            printHexArray("Expected Data        ", text, length);
            (void)printf(RED"\r\n Encryption: FAILED  "RESET_COLOR);
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: AES Test Functions
// *****************************************************************************
// *****************************************************************************

void aes_sym_xts_test(void)
{
    crypto_Sym_Status_E status;

    uint8_t encryptedResult[XTS_TEST_VECTOR_SIZE];
    uint8_t decryptedResult[XTS_TEST_VECTOR_SIZE];
    uint8_t xtsKey[KEY_TYPE_MAX_SIZE * 2UL];

    const char* testWrapper = "SYMMETRIC";
    const char* testType = "XTS";

    XTS_TEST_VECTOR *test = get_xts_test_vectors();
    uint32_t testCount = get_xts_test_count();

    for (uint32_t i = 0; i < testCount; i++)
    {
        uint8_t *plaintext;
        uint8_t *ciphertext;

        printTestHeader(testWrapper, (test->keyLength * 8UL), testType, test->textLength, "");

        printHexArray("Key                  ", test->key, test->keyLength);
        printHexArray("Key2                 ", &test->key2, test->keyLength);

        (void)memcpy(xtsKey, test->key, test->keyLength);
        (void)memcpy(&xtsKey[test->keyLength], test->key2, test->keyLength);

        plaintext = (test->textLength > 0UL) ? test->plaintext : NULL;
        ciphertext = (test->textLength > 0UL) ? encryptedResult : NULL;

        status = sym_xts_test(CRYPTO_CIOP_ENCRYPT, plaintext, test->textLength, xtsKey,
                              (test->keyLength * 2U), test->tweak, encryptedResult);

        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            printCryptoError((test->keyLength * 8UL), testType, "Encrypt", status);
        }
        else
        {
            checkEncryption(test->ciphertext, ciphertext, test->textLength);

            if (test->textLength > 0UL)
            {
                status = sym_xts_test(CRYPTO_CIOP_DECRYPT, ciphertext, test->textLength, xtsKey,
                                     (test->keyLength * 2), test->tweak, decryptedResult);
                if (status != CRYPTO_SYM_CIPHER_SUCCESS)
                {
                    printCryptoError((test->keyLength * 8UL), testType, "Decrypt", status);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    checkArrayEquality(test->plaintext, decryptedResult, test->textLength);
                }
            }
        }

        (void) printf(BLUE"\r\n\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            (void) printf(RED"\r\nTEST FAILED"RESET_COLOR);
        }

        test++;
    }
}
