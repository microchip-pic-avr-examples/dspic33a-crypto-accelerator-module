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
#include <stdint.h>
#include <string.h>
#include "app/app_aes.h"
#include "app/app_sym.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"
#include "test_vectors/xts_test_vector.h"

static crypto_Sym_Status_E SYM_XtsDirectTestRun(crypto_CipherOper_E encrypt,
        uint8_t *data, uint32_t dataLength, uint8_t *key, uint32_t keyLength,
        uint8_t *tweak, uint8_t *result)
{
    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPFAIL;

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (DIRECT)...");

        if (dataLength != 0UL)
        {
            UTILS_HexArrayPrint("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(UTILS_BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    UTILS_RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Sym_Aes_EncryptDirect(
                CRYPTO_HANDLER_HW_INTERNAL,
                CRYPTO_SYM_OPMODE_XTS, data,
                dataLength, result, key, keyLength,
                tweak, UTILS_SESSION_ID);

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
                CRYPTO_HANDLER_HW_INTERNAL,
                CRYPTO_SYM_OPMODE_XTS, data,
                dataLength, result, key, keyLength,
                tweak, UTILS_SESSION_ID);

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            UTILS_HexArrayPrint("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}

static crypto_Sym_Status_E SYM_XtsStepsTestRun(crypto_CipherOper_E encrypt,
        uint8_t *data, uint32_t dataLength, uint8_t *key, uint32_t keyLength,
        uint8_t *tweak, uint8_t *result)
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

        if (dataLength != 0UL)
        {
            UTILS_HexArrayPrint("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(UTILS_BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    UTILS_RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Sym_Aes_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, encrypt,
                                     CRYPTO_SYM_OPMODE_XTS, key, keyLength, NULL, UTILS_SESSION_ID);
        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_AesXts_Cipher(&aesCtx, data, dataLength, result, tweak);
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

        status = Crypto_Sym_Aes_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, encrypt,
                                     CRYPTO_SYM_OPMODE_XTS, key, keyLength, NULL, UTILS_SESSION_ID);
        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            status = Crypto_Sym_AesXts_Cipher(&aesCtx, data, dataLength, result, tweak);
        }

        if (status == CRYPTO_SYM_CIPHER_SUCCESS)
        {
            UTILS_HexArrayPrint("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}

static void SYM_EncryptionCheck(const uint8_t *text, const uint8_t *result, uint32_t length)
{
    (void) printf(UTILS_BLUE
            "\r\n ---------------------------------"
            "----------------------------------------------"
            "--------------"
            UTILS_RESET_COLOR);

    if (length > 0UL)
    {
        if (true == UTILS_ArrayEqualityCheck(result, text, length))
        {
            (void) printf(UTILS_GREEN"\r\n Encryption: PASS    "UTILS_RESET_COLOR );
        }
        else
        {
            UTILS_HexArrayPrint("Expected Data        ", text, length);
            (void) printf(UTILS_RED"\r\n Encryption: FAILED  "UTILS_RESET_COLOR);
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Symmetric XTS Test Functions
// *****************************************************************************
// *****************************************************************************

void SYM_XtsTestRun(void)
{
    uint8_t encryptedDirectResult[XTS_TEST_VECTOR_SIZE];
    uint8_t decryptedDirectResult[XTS_TEST_VECTOR_SIZE];

    uint8_t encryptedStepsResult[XTS_TEST_VECTOR_SIZE];
    uint8_t decryptedStepsResult[XTS_TEST_VECTOR_SIZE];

    uint8_t xtsKey[KEY_TYPE_MAX_SIZE * 2UL];

    const char *testWrapper = "SYMMETRIC";
    const char *testType = "XTS";

    xts_test_vector_t *test = VECTOR_XtsTestVectorsGet();
    uint32_t testCount = VECTOR_XtsTestCountGet();

    for (uint32_t vectorIndex = 0U; vectorIndex < testCount; vectorIndex++)
    {
        uint8_t *plaintext;
        uint8_t *ciphertext;

        AES_TestHeaderPrint(testWrapper, (test->keyLength * 8UL), testType, test->textLength, "");

        UTILS_HexArrayPrint("Key                  ", test->key, test->keyLength);
        UTILS_HexArrayPrint("Key2                 ", test->key2, test->keyLength);
        UTILS_HexArrayPrint("Tweak                ", test->tweak, XTS_TWEAK_SIZE);

        (void) memcpy(xtsKey, test->key, test->keyLength);
        (void) memcpy(&xtsKey[test->keyLength], test->key2, test->keyLength);

        plaintext = (test->textLength > 0UL) ? test->plaintext : NULL;
        ciphertext = (test->textLength > 0UL) ? encryptedDirectResult : NULL;

        crypto_Sym_Status_E status =
                SYM_XtsDirectTestRun(
                CRYPTO_CIOP_ENCRYPT, plaintext,
                test->textLength, xtsKey,
                (test->keyLength * 2U), test->tweak,
                encryptedDirectResult);
        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Encrypt", status);
        }
        else
        {
            SYM_EncryptionCheck(test->ciphertext, ciphertext, test->textLength);

            if (test->textLength > 0UL)
            {
                status = SYM_XtsDirectTestRun(
                        CRYPTO_CIOP_DECRYPT, ciphertext,
                        test->textLength, xtsKey,
                        (test->keyLength * 2U),
                        test->tweak,
                        decryptedDirectResult);
                if (status != CRYPTO_SYM_CIPHER_SUCCESS)
                {
                    AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Decrypt", status);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    UTILS_ArrayEqualityResultPrint(
                            test->plaintext,
                            decryptedDirectResult,
                            test->textLength);
                }
            }
        }

        (void) printf(UTILS_BLUE
                "\r\n\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                UTILS_RESET_COLOR);
        (void) printf(UTILS_BLUE
                "\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                UTILS_RESET_COLOR);

        ciphertext = (test->textLength > 0UL) ? encryptedStepsResult : NULL;

        status = SYM_XtsStepsTestRun(
                CRYPTO_CIOP_ENCRYPT, plaintext,
                test->textLength, xtsKey,
                (test->keyLength * 2U), test->tweak,
                encryptedStepsResult);
        if (status != CRYPTO_SYM_CIPHER_SUCCESS)
        {
            AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Encrypt", status);
        }
        else
        {
            SYM_EncryptionCheck(test->ciphertext, ciphertext, test->textLength);

            if (test->textLength > 0UL)
            {
                status = SYM_XtsStepsTestRun(
                        CRYPTO_CIOP_DECRYPT, ciphertext,
                        test->textLength, xtsKey,
                        (test->keyLength * 2U),
                        test->tweak,
                        decryptedStepsResult);
                if (status != CRYPTO_SYM_CIPHER_SUCCESS)
                {
                    AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Decrypt", status);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    UTILS_ArrayEqualityResultPrint(
                            test->plaintext,
                            decryptedStepsResult,
                            test->textLength);
                }
            }
        }

        (void) printf(UTILS_BLUE
                "\r\n\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                UTILS_RESET_COLOR);
        (void) printf(UTILS_BLUE
                "\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                UTILS_RESET_COLOR);
        (void) printf("\r\n\r\n COMPARING TESTS...");

        // Verify the direct and stepwise tests generate the same result.
        UTILS_HexArrayPrint("Direct Encryption    ", encryptedDirectResult, test->textLength);
        UTILS_HexArrayPrint("Stepwise Encryption  ", encryptedStepsResult, test->textLength);

        UTILS_ArrayEqualityResultPrint(
                encryptedDirectResult,
                encryptedStepsResult,
                test->textLength);

        test++;
    }
}
