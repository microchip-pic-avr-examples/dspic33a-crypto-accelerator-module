/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sha.c

  Summary:
    This source file handles the running of the Hashing examples.
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

#include "crypto/common_crypto/crypto_hash.h"
#include "app_shake.h"

#define MESSAGE_SIZE 0U
#define SHAKE_DIGEST_LENGTH 64U

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

// *****************************************************************************
// *****************************************************************************
// Section: SHA Test Functions
// *****************************************************************************
// *****************************************************************************

uint8_t message[MESSAGE_SIZE] __attribute__((space(prog))) = {};

uint8_t digestResult[SHAKE_DIGEST_LENGTH] __attribute__((space(prog)));

#ifdef ENABLE_MULTI_STEP
st_Crypto_Hash_Shake_Ctx context __attribute__((space(prog)));
#endif

#ifdef ENABLE_ARRAY_COMPARISON
    static uint8_t expectedDigest[SHAKE_DIGEST_LENGTH] __attribute__((space(prog))) = {
        0x46, 0xB9, 0xDD, 0x2B, 0x0B, 0xA8, 0x8D, 0x13, 0x23, 0x3B, 0x3F, 0xEB, 0x74, 0x3E, 0xEB, 0x24,
        0x3F, 0xCD, 0x52, 0xEA, 0x62, 0xB8, 0x1B, 0x82, 0xB5, 0x0C, 0x27, 0x64, 0x6E, 0xD5, 0x76, 0x2F,
        0xD7, 0x5D, 0xC4, 0xDD, 0xD8, 0xC0, 0xF2, 0x00, 0xCB, 0x05, 0x01, 0x9D, 0x67, 0xB5, 0x92, 0xF6,
        0xFC, 0x82, 0x1C, 0x49, 0x47, 0x9A, 0xB4, 0x86, 0x40, 0x29, 0x2E, 0xAC, 0xB3, 0xB7, 0xC4, 0xBE

    };
#endif
    
void shake_test(void)
{
    crypto_Hash_Status_E status;

    #ifdef ENABLE_MULTI_STEP

    status = Crypto_Hash_Shake_Init(&context, CRYPTO_HASH_SHA3_SHAKE256, CRYPTO_HANDLER_HW_INTERNAL, SHAKE_DIGEST_LENGTH, 1);

    if(status == CRYPTO_HASH_SUCCESS)
    {
        status = Crypto_Hash_Shake_Update(&context, message, MESSAGE_SIZE);
    }

    if(status == CRYPTO_HASH_SUCCESS)
    {
        status = Crypto_Hash_Shake_Final(&context, digestResult);
    }
    #else
    status = Crypto_Hash_Shake_Digest(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_HASH_SHA3_SHAKE256, message, MESSAGE_SIZE, digestResult, SHAKE_DIGEST_LENGTH, 1);
    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((status == CRYPTO_HASH_SUCCESS) &&
            checkArrayEquality(digestResult, expectedDigest, SHAKE_DIGEST_LENGTH))
    #else
    if (status == CRYPTO_HASH_SUCCESS)
    #endif
    {
        PASS_setHigh();
    }
    else
    {
        FAIL_setHigh();
    }
}