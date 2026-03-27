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
#include "crypto/common_crypto/crypto_aead_cipher.h"

#define GMAC_SIZE             (16U)
#define KEY_256_SIZE          (32U)
#define IV_SIZE               (12U)
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

uint8_t key[KEY_256_SIZE] __attribute__((space(prog))) = {
    0xE3, 0xC0, 0x8A, 0x8F, 0x06, 0xC6, 0xE3, 0xAD, 0x95, 0xA7, 0x05, 0x57, 0xB2, 0x3F, 0x75, 0x48,
    0x3C, 0xE3, 0x30, 0x21, 0xA9, 0xC7, 0x2B, 0x70, 0x25, 0x66, 0x62, 0x04, 0xC6, 0x9C, 0x0B, 0x72
};

uint8_t iv[IV_SIZE] __attribute__((space(prog))) = {
    0x12, 0x15, 0x35, 0x24, 0xC0, 0x89, 0x5E, 0x81, 0xB2, 0xC2, 0x84, 0x65
};

uint8_t resultGmac[GMAC_SIZE] __attribute__((space(prog)));

#ifdef ENABLE_ARRAY_COMPARISON
uint8_t expectedGmac[GMAC_SIZE] __attribute__((space(prog))) = {
    0x71, 0x4D, 0x54, 0xFD, 0xCF, 0xCE, 0xE3, 0x7D, 0x57, 0x29, 0xCD, 0xDA, 0xB3, 0x83, 0xA0, 0x16
};
#endif

#ifdef ENABLE_MULTI_STEP
 // Context for GMAC calculations when running individual steps.
 // This is too large to put on the stack.
 static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4), space(prog)));
#endif
 
void gmac_256_test(void)
{
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;

    #ifdef ENABLE_MULTI_STEP
    status = Crypto_Mac_AesGmac_Init(&macCtx, CRYPTO_HANDLER_HW_INTERNAL, key, sizeof(key), SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesGmac_Cipher(&macCtx, iv, sizeof(iv), data, sizeof(data), resultGmac, GMAC_SIZE);
    }
    #else

    status = Crypto_Mac_AesGmac_Direct(CRYPTO_HANDLER_HW_INTERNAL, iv, sizeof(iv),
            resultGmac, GMAC_SIZE, key, sizeof(key), data, sizeof(data), SESSION_ID);

    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((status == CRYPTO_MAC_CIPHER_SUCCESS) &&
        checkArrayEquality(expectedGmac, resultGmac, GMAC_SIZE))
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