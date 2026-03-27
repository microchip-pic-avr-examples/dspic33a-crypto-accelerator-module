/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This source file handles the running of the AES examples.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2026 Microchip Technology Inc., and its subsidiaries. All rights reserved.

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

#include "app.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"

#define SESSION_ID            (1U)
#define DATA_SIZE             (16U)
#define KEY_256_SIZE          (32U)

#define GCM_INIT_VECTOR_SIZE  (12U)
#define GCM_AAD_SIZE          (20U)
#define GCM_TAG_SIZE          (16UL)

uint8_t plainText[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03 };
uint8_t encryptionResult[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t decryptionResult[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t key[KEY_256_SIZE] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x20
};

uint8_t gcmInitializationVector[GCM_INIT_VECTOR_SIZE] __attribute__((space(prog)))  = {
    0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88
};
uint8_t aad[GCM_AAD_SIZE] __attribute__((space(prog))) = {
    0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed,
    0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xab, 0xad, 0xda, 0xd2
};
uint8_t tag[GCM_TAG_SIZE] __attribute__((space(prog))) = {
    0x76, 0xfc, 0x6e, 0xce, 0x0f, 0x4e, 0x17, 0x68, 0xcd, 0xdf,
    0x88, 0x53, 0xbb, 0x2d, 0x55, 0x1b
};

// #define ENABLE_MULTI_STEP
// #define ENABLE_ARRAY_COMPARISON

#ifdef ENABLE_ARRAY_COMPARISON
static uint8_t checkArrayEquality(const uint8_t *a, const uint8_t *b, size_t size)
{
	const uint8_t *tempa = a;
	const uint8_t *tempb = b;
	uint8_t result = 0;

	for (unsigned int i = 0; i < size; i++)
    {
		result |= tempa[i] ^ tempb[i];
	}

    return (result == 0);
}
#endif

void aes_256_test(void)
{
    crypto_Aead_Status_E gcmStatus = CRYPTO_AEAD_ERROR_CIPFAIL;

    #ifdef ENABLE_MULTI_STEP

    // Context for AES calculations when running individual steps.
    // This is too large to put on the stack.
    static st_Crypto_Aead_AesGcm_ctx aesGcmCtx __attribute__((aligned(4)));

    // begin encryption
    gcmStatus = Crypto_Aead_AesGcm_Init(&aesGcmCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_ENCRYPT,
        key, KEY_256_SIZE, gcmInitializationVector, GCM_INIT_VECTOR_SIZE, SESSION_ID);

    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_AddAadData(&aesGcmCtx, aad, GCM_AAD_SIZE);
    }

    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_Cipher(&aesGcmCtx, plainText, DATA_SIZE, encryptionResult);
    }

    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_Final(&aesGcmCtx, tag, GCM_TAG_SIZE);
    }

    // begin decryption
    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_Init(&aesGcmCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_DECRYPT,
            key, KEY_256_SIZE, gcmInitializationVector, GCM_INIT_VECTOR_SIZE, SESSION_ID);
    }

    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_AddAadData(&aesGcmCtx, aad, GCM_AAD_SIZE);
    }

    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_Cipher(&aesGcmCtx, encryptionResult, DATA_SIZE, decryptionResult);
    }

    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_Final(&aesGcmCtx, tag, GCM_TAG_SIZE);
    }
    #else

    gcmStatus = Crypto_Aead_AesGcm_EncryptAuthDirect(CRYPTO_HANDLER_HW_INTERNAL,
        plainText, DATA_SIZE, encryptionResult, key, KEY_256_SIZE, gcmInitializationVector,
        GCM_INIT_VECTOR_SIZE, aad, GCM_AAD_SIZE, tag, GCM_TAG_SIZE, SESSION_ID);

    if(gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        gcmStatus = Crypto_Aead_AesGcm_DecryptAuthDirect(CRYPTO_HANDLER_HW_INTERNAL,
            encryptionResult, DATA_SIZE, decryptionResult, key, KEY_256_SIZE,
            gcmInitializationVector, GCM_INIT_VECTOR_SIZE, aad, GCM_AAD_SIZE, tag, GCM_TAG_SIZE, SESSION_ID);
    }

    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS) &&
            checkArrayEquality(plainText, decryptionResult, DATA_SIZE))
    #else
    if (gcmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    #endif
    {
        PASS_setHigh();
    }
    else
    {
        FAIL_setHigh();
    }
}