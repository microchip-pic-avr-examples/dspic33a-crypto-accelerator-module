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

#define CMAC_SIZE             (8U)
#define KEY_256_SIZE          (32U)
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
    0x7F, 0x1E, 0x1F, 0x76, 0x27, 0x3A, 0xEC, 0xB3, 0x64, 0x53, 0xB4, 0x24, 0x5D, 0xA4, 0x7C, 0x8A,
    0xED, 0xAA, 0xBA, 0x75, 0x99, 0x51, 0x08, 0xE8, 0x6A, 0xA9, 0xE3, 0x02, 0xF7, 0x29, 0xCB, 0xED
};

uint8_t resultCmac[CMAC_SIZE] __attribute__((space(prog)));

#ifdef ENABLE_ARRAY_COMPARISON
uint8_t expectedCmac[CMAC_SIZE] __attribute__((space(prog))) = { 0xB9, 0xB5, 0xAF, 0x31, 0xCE, 0x4A, 0x8E, 0x8F };
#endif

#ifdef ENABLE_MULTI_STEP
// Context for CMAC calculations when running individual steps.
// This is too large to put on the stack.
static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4), space(prog)));
#endif

void cmac_256_test(void)
{
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;

    #ifdef ENABLE_MULTI_STEP
    status = Crypto_Mac_AesCmac_Init(&macCtx, CRYPTO_HANDLER_HW_INTERNAL,
            key, sizeof(key), SESSION_ID);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesCmac_Cipher(&macCtx, data, sizeof(data));
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_AesCmac_Final(&macCtx, resultCmac, CMAC_SIZE);
    }

    #else

    status = Crypto_Mac_AesCmac_Direct(CRYPTO_HANDLER_HW_INTERNAL, data, sizeof(data),
            resultCmac, CMAC_SIZE, key, sizeof(key), SESSION_ID);

    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((status == CRYPTO_MAC_CIPHER_SUCCESS) &&
        checkArrayEquality(expectedCmac, resultCmac, CMAC_SIZE))
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