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
#include "mcc_generated_files/system/pins.h"
#include "crypto/common_crypto/crypto_mac_cipher.h"

#define HMAC_SIZE             (64U)
#define KEY_SIZE              (1U)
#define SESSION_ID            (1U)

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

uint8_t data[0] __attribute__((space(prog))) = {};

uint8_t key[1] __attribute__((space(prog))) = { 0x01 };

uint8_t resultHmac[CRYPTO_HASH_MAX_DIGEST_SIZE] __attribute__((space(prog)));

#ifdef ENABLE_ARRAY_COMPARISON
uint8_t expectedHmac[HMAC_SIZE] __attribute__((space(prog))) = {
    0xFA, 0x6E, 0x66, 0x19, 0x28, 0xAD, 0x97, 0x2E, 0xD5, 0xF5, 0x3D, 0xAD, 0x7F, 0x5A, 0xF1, 0xAB,
    0x34, 0x31, 0xEE, 0xA3, 0x44, 0x53, 0x49, 0xF1, 0x96, 0x85, 0xE0, 0xC5, 0x8E, 0x87, 0xB4, 0xC1,
    0x4F, 0x67, 0xFB, 0x34, 0xF7, 0xC2, 0x99, 0x87, 0x39, 0x19, 0x66, 0x6F, 0x60, 0xD4, 0xBA, 0xE3,
    0xA1, 0x5D, 0x86, 0x9A, 0x8C, 0xDB, 0x65, 0x1B, 0xA6, 0xE8, 0x86, 0x90, 0x13, 0x5F, 0x71, 0xC2
};
#endif

#ifdef ENABLE_MULTI_STEP
// Context for HMAC calculations when running individual steps.
// This is too large to put on the stack.
static st_Crypto_Mac_Hmac_ctx macCtx __attribute__((aligned(4), space(prog)));
#endif

void hmac_512_test(void)
{
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;

    #ifdef ENABLE_MULTI_STEP
    status = Crypto_Mac_Hmac_Init(&macCtx, CRYPTO_HANDLER_HW_INTERNAL,
            key, sizeof(key), CRYPTO_HASH_SHA2_512, SESSION_ID);


    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hmac_Cipher(&macCtx, data, sizeof(data));
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hmac_Final(&macCtx, resultHmac);
    }

    #else

    status = Crypto_Mac_Hmac_Direct(CRYPTO_HANDLER_HW_INTERNAL, data, sizeof(data),
            resultHmac, key, sizeof(key), CRYPTO_HASH_SHA2_512, SESSION_ID);

    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((status == CRYPTO_MAC_CIPHER_SUCCESS) &&
        checkArrayEquality(expectedHmac, resultHmac, HMAC_SIZE))
    #else
    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    #endif
    {
        LED_RED_SetLow();
        LED_GREEN_SetHigh();
    }
    else
    {
        LED_GREEN_SetLow();
        LED_RED_SetHigh();
    }
}