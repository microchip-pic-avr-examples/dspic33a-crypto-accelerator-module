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

#include "app.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"

#define SESSION_ID            (1U)
#define DATA_SIZE             (32U)
#define KEY_256_SIZE          (32U)

#define CCM_NONCE_SIZE        (12U)
#define CCM_AAD_SIZE          (32U)
#define CCM_TAG_SIZE          (14UL)

uint8_t plainText[DATA_SIZE] __attribute__((space(prog))) = { 0x60, 0xEF, 0x7B, 0x51, 0xC9, 0xC3, 0xED, 0x5B, 0xB5, 0x3D, 0xFA, 0xC0, 0x0E, 0x96, 0x6E, 0x8A, 0xAC,
                                 0x1A, 0xA7, 0x2F, 0x2C, 0x65, 0x43, 0x37, 0x8D, 0x93, 0xA4, 0xAC, 0xDE, 0x65, 0xF7, 0x02 };
uint8_t encryptionResult[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t decryptionResult[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t key[KEY_256_SIZE] __attribute__((space(prog))) = { 0xBC, 0x3C, 0xC9, 0xE1, 0xBE, 0x83, 0xB0, 0xB9, 0x07, 0x4C, 0x31, 0xF8, 0xAE, 0xFA, 0xA4, 0xB1,
                              0x57, 0x65, 0x97, 0x70, 0xD8, 0xAE, 0xC3, 0x06, 0xF4, 0x29, 0x81, 0xC9, 0x98, 0xBD, 0xD4, 0x1E };

uint8_t nonce[CCM_NONCE_SIZE]  __attribute__((space(prog))) = { 0xFC, 0x08, 0xA1, 0x44, 0x08, 0xE3, 0x82, 0xE9, 0x5A, 0x0E, 0x04, 0x54 };
uint8_t aad[CCM_AAD_SIZE] __attribute__((space(prog))) = { 0x57, 0x69, 0xF1, 0xC1, 0x2F, 0x6D, 0x0A, 0x89, 0x6B, 0x83, 0xF2, 0x87, 0x3C, 0x13, 0x6B, 0xFD,
                              0x19, 0xDE, 0xB5, 0x88, 0xEC, 0x68, 0x40, 0x80, 0xAD, 0x18, 0x6B, 0x88, 0xC3, 0x44, 0x64, 0x4B };
uint8_t tag[CCM_TAG_SIZE] __attribute__((space(prog))) = { 0xE4, 0xD6, 0x30, 0xE3, 0xEE, 0xEC, 0xA0, 0xA5, 0xD3, 0x76, 0x18, 0xE4, 0x62, 0x07 };

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
    crypto_Aead_Status_E ccmStatus = CRYPTO_AEAD_ERROR_CIPFAIL;

    // Context for AES calculations.  This is too large to put on the stack.
    static st_Crypto_Aead_AesCcm_ctx aesCcmCtx __attribute__((aligned(4)));

    // begin encryption
    ccmStatus = Crypto_Aead_AesCcm_Init(&aesCcmCtx, CRYPTO_HANDLER_HW_INTERNAL, key, KEY_256_SIZE, SESSION_ID);
    if (ccmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        ccmStatus = Crypto_Aead_AesCcm_Cipher(&aesCcmCtx, CRYPTO_CIOP_ENCRYPT,
                                              plainText, DATA_SIZE, encryptionResult, nonce, CCM_NONCE_SIZE,
                                              tag, CCM_TAG_SIZE, aad, CCM_AAD_SIZE);
    }

    // begin decryption
    if (ccmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        ccmStatus = Crypto_Aead_AesCcm_Init(&aesCcmCtx, CRYPTO_HANDLER_HW_INTERNAL, key, KEY_256_SIZE, SESSION_ID);
    }

    if (ccmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        ccmStatus = Crypto_Aead_AesCcm_Cipher(&aesCcmCtx, CRYPTO_CIOP_DECRYPT,
                                              encryptionResult, DATA_SIZE, decryptionResult, nonce, CCM_NONCE_SIZE,
                                              tag, CCM_TAG_SIZE, aad, CCM_AAD_SIZE);
    }

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((ccmStatus == CRYPTO_AEAD_CIPHER_SUCCESS) &&
            checkArrayEquality(plainText, decryptionResult, DATA_SIZE))
    #else
    if (ccmStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    #endif
    {
        PASS_setHigh();
    }
    else
    {
        FAIL_setHigh();
    }
}