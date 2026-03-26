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
#include "crypto/common_crypto/crypto_sym_cipher.h"
#include <stdio.h>

#define SESSION_ID            (1U)
#define TWEAK_SIZE            (16U)
#define DATA_SIZE             (16U)

#define SYM_INIT_VECTOR_SIZE  (16U)

uint8_t plainText[DATA_SIZE] __attribute__((space(prog))) = {
    0xeb, 0xab, 0xce, 0x95, 0xb1, 0x4d, 0x3c, 0x8d,
    0x6f, 0xb3, 0x50, 0x39, 0x07, 0x90, 0x31, 0x1c
};
uint8_t encryptionResult[DATA_SIZE] __attribute__((space(prog))) = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t decryptionResult[DATA_SIZE] __attribute__((space(prog))) = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t key[CRYPTO_AESKEYSIZE_256] __attribute__((space(prog))) = {
    0xa1, 0xb9, 0x0c, 0xba, 0x3f, 0x06, 0xac, 0x35,  // key 1
    0x3b, 0x2c, 0x34, 0x38, 0x76, 0x08, 0x17, 0x62,
    0x09, 0x09, 0x23, 0x02, 0x6e, 0x91, 0x77, 0x18,  // key 2
    0x15, 0xf2, 0x9d, 0xab, 0x01, 0x93, 0x2f, 0x2f
};

uint8_t tweak[TWEAK_SIZE] __attribute__((space(prog))) = {
    0x4f, 0xae, 0xf7, 0x11, 0x7c, 0xda, 0x59, 0xc6,
    0x6e, 0x4b, 0x92, 0x01, 0x3e, 0x76, 0x8a, 0xd5
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
    crypto_Sym_Status_E xtsStatus = CRYPTO_SYM_ERROR_CIPFAIL;

    #ifdef ENABLE_MULTI_STEP
    static st_Crypto_Sym_BlockCtx aesXtsCtx __attribute__((aligned(4)));

    xtsStatus = Crypto_Sym_Aes_Init(&aesXtsCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_ENCRYPT,
        CRYPTO_SYM_OPMODE_XTS, key, CRYPTO_AESKEYSIZE_256, NULL, SESSION_ID);

    if (xtsStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        xtsStatus = Crypto_Sym_AesXts_Cipher(&aesXtsCtx, plainText, DATA_SIZE, encryptionResult, tweak);
    }

    if (xtsStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        xtsStatus = Crypto_Sym_Aes_Init(&aesXtsCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_DECRYPT,
            CRYPTO_SYM_OPMODE_XTS, key, CRYPTO_AESKEYSIZE_256, NULL, SESSION_ID);
    }

    if (xtsStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        xtsStatus = Crypto_Sym_AesXts_Cipher(&aesXtsCtx, encryptionResult, DATA_SIZE, decryptionResult, tweak);
    }

    #else

    xtsStatus = Crypto_Sym_Aes_EncryptDirect(CRYPTO_HANDLER_HW_INTERNAL,
        CRYPTO_SYM_OPMODE_XTS, plainText, DATA_SIZE, encryptionResult, key, CRYPTO_AESKEYSIZE_256, tweak, SESSION_ID);

    if(xtsStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        xtsStatus = Crypto_Sym_Aes_DecryptDirect(CRYPTO_HANDLER_HW_INTERNAL,
            CRYPTO_SYM_OPMODE_XTS, encryptionResult, DATA_SIZE, decryptionResult, key, CRYPTO_AESKEYSIZE_256, tweak, SESSION_ID);
    }

    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((xtsStatus == CRYPTO_SYM_CIPHER_SUCCESS) &&
        checkArrayEquality(plainText, decryptionResult, DATA_SIZE))
    #else
    if (xtsStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    #endif
    {
        PASS_setHigh();
    }
    else
    {
        FAIL_setHigh();
    }
}