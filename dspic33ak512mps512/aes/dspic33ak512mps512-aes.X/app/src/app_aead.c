/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_aead.c

  Summary:
    This source file handles the running of the AEAD AES examples.
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
#include "app/app_aead.h"
#include "app/app_aes.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"
#include "test_vectors/gcm_test_vector.h"
#include "test_vectors/ccm_test_vector.h"

#define TEST_SESSION_ID (0x1U)

static crypto_Aead_Status_E AEAD_GcmDirectTestRun(crypto_CipherOper_E encrypt,
        uint8_t *data, uint32_t dataLength, uint8_t *key, uint32_t keyLength,
        uint8_t *iv, uint32_t ivLength, uint8_t *aad, uint32_t aadLength,
        uint8_t *result, uint8_t *tag)
{
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (DIRECT)...");

        if (dataLength != 0UL)
        {
            AES_HexArrayPrint("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Aead_AesGcm_EncryptAuthDirect(
                CRYPTO_HANDLER_HW_INTERNAL,
                data, dataLength, result,
                key, keyLength, iv, ivLength,
                aad, aadLength, tag,
                AES_GCM_TAG_LENGTH, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            AES_HexArrayPrint("Data (Encrypted)     ", result, dataLength);
            AES_HexArrayPrint("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (DIRECT)...");

        AES_HexArrayPrint("Data (Input)         ", data, dataLength);

        status = Crypto_Aead_AesGcm_DecryptAuthDirect(
                CRYPTO_HANDLER_HW_INTERNAL,
                data, dataLength, result,
                key, keyLength, iv, ivLength,
                aad, aadLength, tag,
                AES_GCM_TAG_LENGTH, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            AES_HexArrayPrint("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}

static crypto_Aead_Status_E AEAD_GcmStepsTestRun(crypto_CipherOper_E encrypt,
    uint8_t *data, uint32_t dataLength, uint8_t *key, uint32_t keyLength,
    uint8_t *iv, uint32_t ivLength, uint8_t *aad, uint32_t aadLength,
    uint8_t *result, uint8_t *tag)
{
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;

    /*
     * Context for AES calculations when running individual steps.
     * This is non-reentrant as it is too large to put on the stack.
     */
    static st_Crypto_Aead_AesGcm_ctx aesGcmCtx __attribute__((aligned(4)));

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (STEPS)...");

        if (dataLength != 0UL)
        {
            AES_HexArrayPrint("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Aead_AesGcm_Init(
                &aesGcmCtx, CRYPTO_HANDLER_HW_INTERNAL,
                CRYPTO_CIOP_ENCRYPT, key, keyLength,
                iv, ivLength, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_AddAadData(&aesGcmCtx, aad, aadLength);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Cipher(&aesGcmCtx, data, dataLength, result);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Final(&aesGcmCtx, tag, AES_GCM_TAG_LENGTH);

            if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
            {
                AES_HexArrayPrint("Data (Encrypted)     ", result, dataLength);
                AES_HexArrayPrint("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);
            }
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (STEPS)...");

        AES_HexArrayPrint("Data (Input)         ", data, dataLength);
        AES_HexArrayPrint("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);

        status = Crypto_Aead_AesGcm_Init(
                &aesGcmCtx, CRYPTO_HANDLER_HW_INTERNAL,
                CRYPTO_CIOP_DECRYPT, key, keyLength,
                iv, ivLength, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_AddAadData(&aesGcmCtx, aad, aadLength);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Cipher(&aesGcmCtx, data, dataLength, result);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Final(&aesGcmCtx, tag, AES_GCM_TAG_LENGTH);

            if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
            {
                AES_HexArrayPrint("Data (Decrypted)     ", result, dataLength);
                AES_HexArrayPrint("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);
            }
        }
    }

    return status;
}

static crypto_Aead_Status_E AEAD_CcmCipherRun(crypto_CipherOper_E encrypt,
        uint8_t *data, uint32_t dataLength, uint8_t *key, uint32_t keyLength,
        uint8_t *nonce, uint32_t nonceLength, uint8_t *aad, uint32_t aadLength,
        uint8_t *result, uint8_t *tag, uint32_t tagLength)
{
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;

    /*
     * Context for AES calculations when running individual steps.
     * This is non-reentrant as it is too large to put on the stack.
     */
    static st_Crypto_Aead_AesCcm_ctx aesCcmCtx __attribute__((aligned(4)));

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING...");

        if (dataLength != 0UL)
        {
            AES_HexArrayPrint("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Aead_AesCcm_Init(
                &aesCcmCtx, CRYPTO_HANDLER_HW_INTERNAL,
                key, keyLength, TEST_SESSION_ID);
        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesCcm_Cipher(
                    &aesCcmCtx, encrypt,
                    data, dataLength, result,
                    nonce, nonceLength,
                    tag, tagLength, aad, aadLength);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            AES_HexArrayPrint("Data (Encrypted)     ",
                    result, dataLength);
            AES_HexArrayPrint("Authentication Tag   ",
                    tag, tagLength);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING...");

        AES_HexArrayPrint("Data (Input)         ",
                data, dataLength);

        status = Crypto_Aead_AesCcm_Init(
                &aesCcmCtx, CRYPTO_HANDLER_HW_INTERNAL,
                key, keyLength, TEST_SESSION_ID);
        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesCcm_Cipher(&aesCcmCtx, encrypt,
                                                data, dataLength, result, nonce, nonceLength,
                                                tag, tagLength, aad, aadLength);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            AES_HexArrayPrint("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}

static void AEAD_EncryptionAndTagCheck(
        const uint8_t *text, const uint8_t *result,
        uint32_t length, const uint8_t *tag,
        const uint8_t *tagResult, uint32_t tagLength)
{
    (void) printf(BLUE
            "\r\n ---------------------------------"
            "----------------------------------------------"
            "--------------"
            RESET_COLOR);

    if (length > 0UL)
    {
        if (0U == AES_ArrayEqualityCheck(result, text, length))
        {
            (void) printf(GREEN"\r\n Encryption: PASS    "RESET_COLOR );
        }
        else
        {
            (void) printf(RED"\r\n Encryption: FAILED  "RESET_COLOR);
        }
    }

    if (0U == AES_ArrayEqualityCheck(tagResult, tag, tagLength))
    {
        (void) printf(GREEN"\r\n Authentication Tag: PASS"RESET_COLOR);
    }
    else
    {
        (void) printf(RED"\r\n Authentication Tag: FAILED"RESET_COLOR);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: AEAD Test Functions
// *****************************************************************************
// *****************************************************************************

void AEAD_GcmTestRun(void)
{
    crypto_Aead_Status_E status;
    crypto_Aead_Status_E statusSteps;

    uint8_t encryptedDirectResult[GCM_TEST_VECTOR_SIZE];
    uint8_t decryptedDirectResult[GCM_TEST_VECTOR_SIZE];
    uint8_t tagDirectResult[AES_GCM_TAG_LENGTH];

    uint8_t encryptedStepsResult[GCM_TEST_VECTOR_SIZE];
    uint8_t decryptedStepsResult[GCM_TEST_VECTOR_SIZE];
    uint8_t tagStepsResult[AES_GCM_TAG_LENGTH];

    const char *testWrapper = "AEAD";
    const char *testType = "GCM";
    const char *emptyString = "";

#ifdef RUN_ALL_VECTORS
    gcm_test_vector_t *test = TV_GcmTestVectorsGet();
    uint32_t testCount = TV_GcmTestCountGet();
#else
    gcm_test_vector_t *test = &TV_GcmTestVectorsGet()[22];
    uint32_t testCount = 1U;
#endif

    for (uint32_t vectorIndex = 0U; vectorIndex < testCount; vectorIndex++)
    {
        uint8_t *pt;
        uint8_t *ct;
        uint8_t *aad;

        AES_TestHeaderPrint(testWrapper,
                (test->keyLength * 8UL), testType,
                test->textLength, emptyString);

        AES_HexArrayPrint("Key                  ",
                test->key, test->keyLength);

        if (test->ivLength != 0UL)
        {
            AES_HexArrayPrint("Initialization Vector",
                    test->iv, test->ivLength);
        }
        else
        {
            (void) printf(BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    RESET_COLOR);
            (void) printf("\r\n%s", " Initialization Vector  :  None");
        }

        if (test->aadLength != 0UL)
        {
            AES_HexArrayPrint("Authentication Data  ", test->aad, test->aadLength);
        }
        else
        {
            (void) printf(BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    RESET_COLOR);
            (void) printf("\r\n%s", " Authentication Data    :  None");
        }

        pt = (test->textLength > 0UL) ? test->plaintext : NULL;
        aad = (test->aadLength > 0UL) ? test->aad : NULL;
        ct = (test->textLength > 0UL) ? encryptedDirectResult : NULL;

        status = AEAD_GcmDirectTestRun(
                CRYPTO_CIOP_ENCRYPT, pt,
                test->textLength, test->key,
                test->keyLength, test->iv,
                test->ivLength, aad,
                test->aadLength, ct, tagDirectResult);

        if (status != CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Encrypt", status);
        }
        else
        {
            AEAD_EncryptionAndTagCheck(test->ciphertext,
                    ct, test->textLength, test->tag,
                    tagDirectResult, test->tagLength);

            if (test->textLength > 0UL)
            {
                status = AEAD_GcmDirectTestRun(
                        CRYPTO_CIOP_DECRYPT, ct,
                        test->textLength, test->key,
                        test->keyLength, test->iv,
                        test->ivLength, aad,
                        test->aadLength,
                        decryptedDirectResult,
                        tagDirectResult);
                if (status != CRYPTO_AEAD_CIPHER_SUCCESS)
                {
                    AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Decrypt", status);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    AES_ArrayEqualityResultPrint(
                            test->plaintext,
                            decryptedDirectResult,
                            test->textLength);
                }
            }
        }

        (void) printf(BLUE
                "\r\n\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                RESET_COLOR);
        (void) printf(BLUE
                "\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                RESET_COLOR);

        ct = (test->textLength > 0UL) ? encryptedStepsResult : NULL;

        statusSteps = AEAD_GcmStepsTestRun(
                CRYPTO_CIOP_ENCRYPT, pt,
                test->textLength, test->key,
                test->keyLength, test->iv,
                test->ivLength, aad,
                test->aadLength, ct, tagStepsResult);

        if (statusSteps != CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Encrypt", statusSteps);
        }
        else
        {
            AEAD_EncryptionAndTagCheck(test->ciphertext,
                    ct, test->textLength, test->tag,
                    tagStepsResult, test->tagLength);

            if (test->textLength > 0UL)
            {
                statusSteps = AEAD_GcmStepsTestRun(
                        CRYPTO_CIOP_DECRYPT, ct,
                        test->textLength, test->key,
                        test->keyLength, test->iv,
                        test->ivLength, aad,
                        test->aadLength,
                        decryptedStepsResult,
                        tagStepsResult);

                if (statusSteps != CRYPTO_AEAD_CIPHER_SUCCESS)
                {
                    AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Decrypt", statusSteps);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    AES_ArrayEqualityResultPrint(
                            test->plaintext,
                            decryptedStepsResult,
                            test->textLength);
                }
            }
        }

        (void) printf(BLUE
                "\r\n\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                RESET_COLOR);
        (void) printf(BLUE
                "\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                RESET_COLOR);
        if ((status == CRYPTO_AEAD_CIPHER_SUCCESS) && (statusSteps == CRYPTO_AEAD_CIPHER_SUCCESS))
        {
            // Verify the direct and stepwise tests generate the same result.
            (void) printf("\r\n\r\n COMPARING TESTS...");

            if (test->textLength > 0UL)
            {
                AES_HexArrayPrint("Direct Encryption    ", encryptedDirectResult, test->textLength);
                AES_HexArrayPrint("Stepwise Encryption  ", encryptedStepsResult, test->textLength);
                AES_ArrayEqualityResultPrint(
                        encryptedDirectResult,
                        encryptedStepsResult,
                        test->textLength);
            }

            AES_HexArrayPrint("Direct Tag           ", tagDirectResult, AES_GCM_TAG_LENGTH);
            AES_HexArrayPrint("Stepwise Tag         ", tagStepsResult, AES_GCM_TAG_LENGTH);
            AES_ArrayEqualityResultPrint(tagDirectResult, tagStepsResult, AES_GCM_TAG_LENGTH);
        }
        else
        {
            (void) printf(RED"\r\nTEST FAILED"RESET_COLOR);
        }

        test++;
    }
}

void AEAD_CcmTestRun(void)
{
    uint8_t encryptedResult[CCM_TEST_VECTOR_SIZE] = {0U};
    uint8_t decryptedResult[CCM_TEST_VECTOR_SIZE] = {0U};
    uint8_t tagResult[AES_CCM_TAG_LENGTH] = {0U};

    const char *testWrapper = "AEAD";
    const char *testType = "CCM";
    const char *emptyString = "";

    ccm_test_vector_t *test = TV_CcmTestVectorsGet();
    uint32_t testCount = TV_CcmTestCountGet();

    for (uint32_t vectorIndex = 0U; vectorIndex < testCount; vectorIndex++)
    {
        uint8_t *pt;
        uint8_t *ct;
        uint8_t *aad;

        AES_TestHeaderPrint(testWrapper,
                (test->keyLength * 8UL), testType,
                test->textLength, emptyString);

        AES_HexArrayPrint("Key                  ",
                test->key, test->keyLength);

        if (test->nonceLength != 0UL)
        {
            AES_HexArrayPrint("Nonce                ", test->nonce, test->nonceLength);
        }
        else
        {
            (void) printf(BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    RESET_COLOR);
            (void) printf("\r\n%s", " Nonce                  :  None");
        }

        if (test->aadLength != 0UL)
        {
            AES_HexArrayPrint("Authentication Data  ", test->aad, test->aadLength);
        }
        else
        {
            (void) printf(BLUE
                    "\r\n ---------------------------------"
                    "----------------------------------------------"
                    "--------------"
                    RESET_COLOR);
            (void) printf("\r\n%s", " Authentication Data    :  None");
        }

        pt = (test->textLength > 0UL) ? test->plaintext : NULL;
        aad = (test->aadLength > 0UL) ? test->aad : NULL;
        ct = (test->textLength > 0UL) ? encryptedResult : NULL;

        crypto_Aead_Status_E status = AEAD_CcmCipherRun(
                CRYPTO_CIOP_ENCRYPT, pt,
                test->textLength, test->key,
                test->keyLength, test->nonce,
                test->nonceLength, aad,
                test->aadLength, ct, tagResult,
                test->tagLength);

        if (status != CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Encrypt", status);
        }
        else
        {
            AEAD_EncryptionAndTagCheck(test->ciphertext,
                    ct, test->textLength, test->tag,
                    tagResult, test->tagLength);

            if (test->textLength > 0UL)
            {
                status = AEAD_CcmCipherRun(
                        CRYPTO_CIOP_DECRYPT, ct,
                        test->textLength, test->key,
                        test->keyLength, test->nonce,
                        test->nonceLength, aad,
                        test->aadLength, decryptedResult,
                        tagResult, test->tagLength);
                if (status != CRYPTO_AEAD_CIPHER_SUCCESS)
                {
                    AES_CryptoErrorPrint((test->keyLength * 8UL), testType, "Decrypt", status);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    AES_ArrayEqualityResultPrint(test->plaintext,
                            decryptedResult, test->textLength);
                }
            }
        }

        (void) printf(BLUE
                "\r\n\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                RESET_COLOR);
        (void) printf(BLUE
                "\r\n ---------------------------------"
                "----------------------------------------------"
                "--------------"
                RESET_COLOR);
        if (status != CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            (void) printf(RED"\r\nTEST FAILED"RESET_COLOR);
        }

        test++;
    }
}
