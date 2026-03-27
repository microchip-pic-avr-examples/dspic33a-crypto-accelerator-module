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
#include "crypto/common_crypto/crypto_sym_cipher.h"

#define SESSION_ID            (1U)
#define DATA_SIZE             (16U)
#define KEY_256_SIZE          (32U)

#define SYM_INIT_VECTOR_SIZE  (16U)

uint8_t plainText[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03 };
uint8_t encryptionResult[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t decryptionResult[DATA_SIZE] __attribute__((space(prog))) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t key[KEY_256_SIZE] __attribute__((space(prog))) = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x20
};
uint8_t symInitializationVector[SYM_INIT_VECTOR_SIZE] __attribute__((space(prog))) = {
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10
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
    crypto_Sym_Status_E ctrStatus = CRYPTO_SYM_ERROR_CIPFAIL;

    #ifdef ENABLE_MULTI_STEP
    // Context for AES calculations when running individual steps.
    // This is too large to put on the stack.
    static st_Crypto_Sym_BlockCtx aesCtrCtx __attribute__((aligned(4)));

    // begin encryption
    ctrStatus = Crypto_Sym_Aes_Init(&aesCtrCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_ENCRYPT,
        CRYPTO_SYM_OPMODE_OFB, key, KEY_256_SIZE, symInitializationVector, SESSION_ID);

    if (ctrStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        ctrStatus = Crypto_Sym_Aes_Cipher(&aesCtrCtx, plainText, DATA_SIZE, encryptionResult);
    }

    // begin decryption
    if (ctrStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        ctrStatus = Crypto_Sym_Aes_Init(&aesCtrCtx, CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_CIOP_DECRYPT,
            CRYPTO_SYM_OPMODE_OFB, key, KEY_256_SIZE, symInitializationVector, SESSION_ID);
    }

    if (ctrStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        ctrStatus = Crypto_Sym_Aes_Cipher(&aesCtrCtx, encryptionResult, DATA_SIZE, decryptionResult);
    }

    #else

    ctrStatus = Crypto_Sym_Aes_EncryptDirect(CRYPTO_HANDLER_HW_INTERNAL,
        CRYPTO_SYM_OPMODE_OFB, plainText, DATA_SIZE, encryptionResult, key, KEY_256_SIZE, symInitializationVector, SESSION_ID);

    if(ctrStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        ctrStatus = Crypto_Sym_Aes_DecryptDirect(CRYPTO_HANDLER_HW_INTERNAL,
            CRYPTO_SYM_OPMODE_OFB, encryptionResult, DATA_SIZE, decryptionResult, key, KEY_256_SIZE, symInitializationVector, SESSION_ID);
    }

    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((ctrStatus == CRYPTO_SYM_CIPHER_SUCCESS) &&
        checkArrayEquality(plainText, decryptionResult, DATA_SIZE))
    #else
    if (ctrStatus == CRYPTO_SYM_CIPHER_SUCCESS)
    #endif
    {
        PASS_setHigh();
    }
    else
    {
        FAIL_setHigh();
    }
}