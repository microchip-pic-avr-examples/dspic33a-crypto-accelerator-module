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
#include "app_sha.h"

#define MESSAGE_SIZE 0U
#define SHA_512_DIGEST_LENGTH 64U

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

uint8_t digestResult[SHA_512_DIGEST_LENGTH] __attribute__((space(prog)));

#ifdef ENABLE_MULTI_STEP
st_Crypto_Hash_Sha_Ctx context __attribute__((space(prog)));
#endif

#ifdef ENABLE_ARRAY_COMPARISON
    #ifdef RUN_SHA2
    static uint8_t expectedDigest[SHA_512_DIGEST_LENGTH] __attribute__((space(prog))) = {
        0xCF, 0x83, 0xE1, 0x35, 0x7E, 0xEF, 0xB8, 0xBD, 0xF1, 0x54, 0x28, 0x50, 0xD6, 0x6D, 0x80, 0x07,
        0xD6, 0x20, 0xE4, 0x05, 0x0B, 0x57, 0x15, 0xDC, 0x83, 0xF4, 0xA9, 0x21, 0xD3, 0x6C, 0xE9, 0xCE,
        0x47, 0xD0, 0xD1, 0x3C, 0x5D, 0x85, 0xF2, 0xB0, 0xFF, 0x83, 0x18, 0xD2, 0x87, 0x7E, 0xEC, 0x2F,
        0x63, 0xB9, 0x31, 0xBD, 0x47, 0x41, 0x7A, 0x81, 0xA5, 0x38, 0x32, 0x7A, 0xF9, 0x27, 0xDA, 0x3E
    };
    #elif defined(RUN_SHA3)
    static uint8_t expectedDigest[SHA_512_DIGEST_LENGTH] __attribute__((space(prog))) = {
        0xA6, 0x9F, 0x73, 0xCC, 0xA2, 0x3A, 0x9A, 0xC5, 0xC8, 0xB5, 0x67, 0xDC, 0x18, 0x5A, 0x75, 0x6E,
        0x97, 0xC9, 0x82, 0x16, 0x4F, 0xE2, 0x58, 0x59, 0xE0, 0xD1, 0xDC, 0xC1, 0x47, 0x5C, 0x80, 0xA6,
        0x15, 0xB2, 0x12, 0x3A, 0xF1, 0xF5, 0xF9, 0x4C, 0x11, 0xE3, 0xE9, 0x40, 0x2C, 0x3A, 0xC5, 0x58,
        0xF5, 0x00, 0x19, 0x9D, 0x95, 0xB6, 0xD3, 0xE3, 0x01, 0x75, 0x85, 0x86, 0x28, 0x1D, 0xCD, 0x26
    };
    #endif
#endif
    
void sha_512_test(void)
{
    crypto_Hash_Status_E status;
    
    #ifdef ENABLE_MULTI_STEP
    
    #ifdef RUN_SHA2
    status = Crypto_Hash_Sha_Init(&context, CRYPTO_HASH_SHA2_512, CRYPTO_HANDLER_HW_INTERNAL, 1);
    #elif defined(RUN_SHA3)
    status = Crypto_Hash_Sha_Init(&context, CRYPTO_HASH_SHA3_512, CRYPTO_HANDLER_HW_INTERNAL, 1);
    #endif

    if(status == CRYPTO_HASH_SUCCESS)
    {
        status = Crypto_Hash_Sha_Update(&context, message, MESSAGE_SIZE);
    }

    if(status == CRYPTO_HASH_SUCCESS)
    {
        status = Crypto_Hash_Sha_Final(&context, digestResult);
    }
    #else
    #ifdef RUN_SHA2
    status = Crypto_Hash_Sha_Digest(CRYPTO_HANDLER_HW_INTERNAL, message, MESSAGE_SIZE, digestResult, CRYPTO_HASH_SHA2_512, 1);
    #elif defined(RUN_SHA3)
    status = Crypto_Hash_Sha_Digest(CRYPTO_HANDLER_HW_INTERNAL, message, MESSAGE_SIZE, digestResult, CRYPTO_HASH_SHA3_512, 1);
    #endif

    #endif

    #ifdef ENABLE_ARRAY_COMPARISON
    if ((status == CRYPTO_HASH_SUCCESS) &&
            checkArrayEquality(digestResult, expectedDigest, SHA_512_DIGEST_LENGTH))
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