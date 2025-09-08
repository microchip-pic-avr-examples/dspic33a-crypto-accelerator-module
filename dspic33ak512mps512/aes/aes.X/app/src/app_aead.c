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
#include "./app_aes.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"
#include "test_vectors/gcm_test_vector.h"

#define TEST_SESSION_ID 0x1

// Context for AES calculations when running individual steps.
// This is too large to put on the stack.
static st_Crypto_Aead_AesGcm_ctx aesCtx __attribute__((aligned(4)));

static crypto_Aead_Status_E aead_gcm_test_direct(crypto_CipherOper_E encrypt,
        uint8_t* data, uint32_t dataLength, uint8_t* key, uint32_t keyLength,
        uint8_t* iv, uint32_t ivLength, uint8_t *aad, uint32_t aadLength,
        uint8_t* result, uint8_t* tag)
{
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (DIRECT)...");

        if (dataLength != 0UL)
        {
            printHexArray("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Aead_AesGcm_EncryptAuthDirect(CRYPTO_HANDLER_HW_INTERNAL,
                data, dataLength, result, key, keyLength, iv, ivLength, aad, aadLength, tag, AES_GCM_TAG_LENGTH, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            printHexArray("Data (Encrypted)     ", result, dataLength);
            printHexArray("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (DIRECT)...");

        printHexArray("Data (Input)         ", data, dataLength);

        status = Crypto_Aead_AesGcm_DecryptAuthDirect(CRYPTO_HANDLER_HW_INTERNAL,
            data, dataLength, result, key, keyLength, iv, ivLength, aad, aadLength, tag, AES_GCM_TAG_LENGTH, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            printHexArray("Data (Decrypted)     ", result, dataLength);
        }
    }

    return status;
}


static crypto_Aead_Status_E aead_gcm_test_steps(crypto_CipherOper_E encrypt,
    uint8_t* data, uint32_t dataLength, uint8_t* key, uint32_t keyLength,
    uint8_t* iv, uint32_t ivLength, uint8_t *aad, uint32_t aadLength,
    uint8_t* result, uint8_t* tag)
{
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;

    if (encrypt == CRYPTO_CIOP_ENCRYPT)
    {
        (void) printf("\r\n\r\n ENCRYPTING (STEPS)...");

        if (dataLength != 0UL)
        {
            printHexArray("Data (Input)         ", data, dataLength);
        }
        else
        {
            (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
            (void) printf("\r\n%s", " Data (Input)           :  None");
        }

        status = Crypto_Aead_AesGcm_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_ENCRYPT,
                                            key, keyLength, iv, ivLength, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_AddAadData(&aesCtx, aad, aadLength);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Cipher(&aesCtx, data, dataLength, result);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Final(&aesCtx, tag, AES_GCM_TAG_LENGTH);

            if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
            {
                printHexArray("Data (Encrypted)     ", result, dataLength);
                printHexArray("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);
            }
        }
    }
    else
    {
        (void) printf("\r\n\r\n DECRYPTING (STEPS)...");

        printHexArray("Data (Input)         ", data, dataLength);
        printHexArray("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);

        status = Crypto_Aead_AesGcm_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_DECRYPT,
                                            key, keyLength, iv, ivLength, TEST_SESSION_ID);

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_AddAadData(&aesCtx, aad, aadLength);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Cipher(&aesCtx, data, dataLength, result);
        }

        if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            status = Crypto_Aead_AesGcm_Final(&aesCtx, tag, AES_GCM_TAG_LENGTH);

            if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
            {
                printHexArray("Data (Decrypted)     ", result, dataLength);
                printHexArray("Authentication Tag   ", tag, AES_GCM_TAG_LENGTH);
            }
        }
    }

    return status;
}

static uint8_t checkArrayEquality(const uint8_t *a, const uint8_t *b, size_t size)
{
	uint8_t result = 0;

    if (size > 0U)
    {
        const uint8_t *tempa = a;
        const uint8_t *tempb = b;

        for (unsigned int i = 0; i < size; i++)
        {
            result |= tempa[i] ^ tempb[i];
        }

    }
    return result;
}

static void checkEncryptionAndTag(uint8_t *ct, uint8_t *result, uint32_t length, uint8_t *tag, uint8_t *tagResult)
{
    (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);

    if (length > 0UL)
    {
        if (0u == checkArrayEquality(result, ct, length))
        {
            (void)printf(GREEN"\r\n Encryption: PASS    "RESET_COLOR );
        }
        else
        {
            (void)printf(RED"\r\n Encryption: FAILED  "RESET_COLOR);
        }
    }

    if (0u == checkArrayEquality(tagResult, tag, AES_GCM_TAG_LENGTH))
    {
        (void)printf(GREEN"\r\n Authentication Tag: PASS"RESET_COLOR);
    }
    else
    {
        (void)printf(RED"\r\n Authentication Tag: FAILED"RESET_COLOR);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: AES Test Functions
// *****************************************************************************
// *****************************************************************************

void aes_aead_gcm_test(void)
{
    crypto_Aead_Status_E status;
    crypto_Aead_Status_E statusSteps;

    uint8_t encryptedDirectResult[GCM_TEST_VECTOR_SZ];
    uint8_t decryptedDirectResult[GCM_TEST_VECTOR_SZ];
    uint8_t tagDirectResult[AES_GCM_TAG_LENGTH];

    uint8_t encryptedStepsResult[GCM_TEST_VECTOR_SZ];
    uint8_t decryptedStepsResult[GCM_TEST_VECTOR_SZ];
    uint8_t tagStepsResult[AES_GCM_TAG_LENGTH];

    const char* testWrapper = "AEAD";
    const char* testType = "GCM";
    const char* emptyString = "";

#ifdef RUN_ALL_VECTORS    
    TEST_VECTOR *test = get_gcm_test_vectors();
    uint32_t testCount = get_gcm_test_count();
#else
    TEST_VECTOR *test = &get_gcm_test_vectors()[22];
    uint32_t testCount = 1;
#endif

    for (uint32_t i = 0; i < testCount; i++)
    {
        uint8_t *pt;
        uint8_t *ct;
        uint8_t *aad;

        printTestHeader(testWrapper, (test->keylength * 8UL), testType, test->ptlength, emptyString);

        printHexArray("Key                  ", test->key, test->keylength);

        if(test->ivlength != 0UL)
        {
            printHexArray("Initialization Vector", test->iv, test->ivlength);
        }
        else
        {
            (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
            (void) printf("\r\n%s", " Initialization Vector  :  None");
        }

        if(test->aadlength != 0UL)
        {
            printHexArray("Authentication Data  ", test->aad, test->aadlength);
        }
        else
        {
            (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
            (void) printf("\r\n%s", " Authentication Data    :  None");
        }

        pt = (test->ptlength > 0UL) ? test->pt : NULL;
        aad = (test->aadlength > 0UL) ? test->aad : NULL;
        ct = (test->ptlength > 0UL) ? encryptedDirectResult : NULL;

        status = aead_gcm_test_direct(CRYPTO_CIOP_ENCRYPT, pt, test->ptlength, test->key, test->keylength,
                                      test->iv, test->ivlength, aad, test->aadlength, ct, tagDirectResult);

        if (status != CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            printCryptoError((test->keylength * 8UL), testType, "Encrypt", status);
        }
        else
        {
            checkEncryptionAndTag(test->ct, ct, test->ptlength, test->tag, tagDirectResult);

            if (test->ptlength > 0UL)
            {
                status = aead_gcm_test_direct(CRYPTO_CIOP_DECRYPT, ct, test->ptlength, test->key, test->keylength,
                                              test->iv, test->ivlength, aad, test->aadlength, decryptedDirectResult, tagDirectResult);
                if (status != CRYPTO_AEAD_CIPHER_SUCCESS)
                {
                    printCryptoError((test->keylength * 8UL), testType, "Decrypt", status);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    checkArrayEquality(test->pt, decryptedDirectResult, test->ptlength);
                }
            }
        }

        (void) printf(BLUE"\r\n\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);

        ct = (test->ptlength > 0UL) ? encryptedStepsResult : NULL;

        statusSteps = aead_gcm_test_steps(CRYPTO_CIOP_ENCRYPT, pt, test->ptlength, test->key, test->keylength,
                                          test->iv, test->ivlength, aad, test->aadlength, ct, tagStepsResult);

        if (statusSteps != CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            printCryptoError((test->keylength * 8UL), testType, "Encrypt", statusSteps);
        }
        else
        {
            checkEncryptionAndTag(test->ct, ct, test->ptlength, test->tag, tagStepsResult);

            if (test->ptlength > 0UL)
            {
                statusSteps = aead_gcm_test_steps(CRYPTO_CIOP_DECRYPT, ct, test->ptlength, test->key, test->keylength,
                                                  test->iv, test->ivlength, aad, test->aadlength, decryptedStepsResult, tagStepsResult);

                if (statusSteps != CRYPTO_AEAD_CIPHER_SUCCESS)
                {
                    printCryptoError((test->keylength * 8UL), testType, "Decrypt", statusSteps);
                }
                else
                {
                    (void) printf("\r\n\r\n VERIFYING...");
                    checkArrayEquality(test->pt, decryptedStepsResult, test->ptlength);
                }
            }
        }

        (void) printf(BLUE"\r\n\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
        if ((status == CRYPTO_AEAD_CIPHER_SUCCESS) && (statusSteps == CRYPTO_AEAD_CIPHER_SUCCESS))
        {
            // Verify the direct and stepwise tests generate the same result.
            (void) printf("\r\n\r\n COMPARING TESTS...");

            if (test->ptlength > 0UL)
            {
                printHexArray("Direct Encryption    ", encryptedDirectResult, test->ptlength);
                printHexArray("Stepwise Encryption  ", encryptedStepsResult, test->ptlength);
                checkArrayEquality(encryptedDirectResult, encryptedStepsResult, test->ptlength);
            }

            printHexArray("Direct Tag           ", tagDirectResult, AES_GCM_TAG_LENGTH);
            printHexArray("Stepwise Tag         ", tagStepsResult, AES_GCM_TAG_LENGTH);
            checkArrayEquality(tagDirectResult, tagStepsResult, AES_GCM_TAG_LENGTH);
        }
        else
        {
            (void) printf(RED"\r\nTEST FAILED"RESET_COLOR);
        }

        test++;
    }
}
