/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_utils.c

  Summary:
    This source file provides utility functions for the SHA demo.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2026, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
#include "app/app_utils.h"
#include "test_vectors/test_vectors_sha_1.h"
#include "test_vectors/test_vectors_sha_224.h"
#include "test_vectors/test_vectors_sha_256.h"
#include "test_vectors/test_vectors_sha_384.h"
#include "test_vectors/test_vectors_sha_512.h"

void hexArrayPrint(const char* label, const uint8_t* data, uint32_t size)
{
    const uint8_t *ldata = data;

    (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------------"RESET_COLOR);
    (void) printf("\r\n %s  : ", label);

    for (uint32_t index = 0U; index < size; index++)
    {
        if (((index % 16U) == 0U) && (index != 0U))
        {
            (void) printf("\r\n                    ");
        }
        (void) printf(" 0x%02X", ldata[index]);
    }
}

void arrayEqualityCheck(const uint8_t *a, const uint8_t *b, size_t size)
{
	const uint8_t *tempa = a;
	const uint8_t *tempb = b;
	uint8_t result = 0;

	for (unsigned int i = 0; i < size; i++)
    {
		result |= tempa[i] ^ tempb[i];
	}

    if(result == 0U)
    {
        (void) printf(GREEN" \r\n PASS "RESET_COLOR);
    }
    else
    {
        (void) printf(RED" \r\n FAIL - Data Mismatch"RESET_COLOR);
        hexArrayPrint("Expected Digest", b, size);
    }
}

uint32_t shaDigestLengthGet(crypto_Hash_Algo_E algorithm)
{
    uint32_t digestLength;

    switch(algorithm)
    {
        case CRYPTO_HASH_SHA1:
            digestLength = SHA_1_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_224:
        case CRYPTO_HASH_SHA3_224:
            digestLength = SHA_224_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_256:
        case CRYPTO_HASH_SHA3_256:
            digestLength = SHA_256_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_384:
        case CRYPTO_HASH_SHA3_384:
            digestLength = SHA_384_DIGEST_LEN_BYTES;
            break;
        case CRYPTO_HASH_SHA2_512:
        case CRYPTO_HASH_SHA3_512:
            digestLength = SHA_512_DIGEST_LEN_BYTES;
            break;
        default:
            digestLength = 0U;
            break;
    }

    return digestLength;
}

const char* algorithmNameGet(crypto_Hash_Algo_E algorithm)
{
    const char* algorithmType;

    switch(algorithm)
    {
        case CRYPTO_HASH_SHA1:
            algorithmType = "SHA-1";
            break;
        case CRYPTO_HASH_SHA2_224:
            algorithmType = "SHA-224";
            break;
        case CRYPTO_HASH_SHA2_256:
            algorithmType = "SHA-256";
            break;
        case CRYPTO_HASH_SHA2_384:
            algorithmType = "SHA-384";
            break;
        case CRYPTO_HASH_SHA2_512:
            algorithmType = "SHA-512";
            break;
        case CRYPTO_HASH_SHA3_224:
            algorithmType = "SHA3-224";
            break;
        case CRYPTO_HASH_SHA3_256:
            algorithmType = "SHA3-256";
            break;
        case CRYPTO_HASH_SHA3_384:
            algorithmType = "SHA3-384";
            break;
        case CRYPTO_HASH_SHA3_512:
            algorithmType = "SHA3-512";
            break;
        default:
            algorithmType = "???";
            break;
    }

    return algorithmType;
}

const char* headerPaddingGet(crypto_Hash_Algo_E algorithm)
{
    const char* padding;

    switch(algorithm)
    {
        case CRYPTO_HASH_SHA1:
            padding = "***";
            break;
        case CRYPTO_HASH_SHA2_224:
        case CRYPTO_HASH_SHA2_256:
        case CRYPTO_HASH_SHA2_384:
        case CRYPTO_HASH_SHA2_512:
            padding = "*";
            break;
        default:
            padding = "";
            break;
    }

    return padding;
}

void headerPrint(crypto_Hash_Algo_E algorithm, const char* algorithmName, uint32_t dataLength)
{
    const char* headerDivider = "***************************************************************************************************";
    const char* sizePadding = "";

    if(dataLength == 0U)
    {
        sizePadding = "**";
    }
    else if(dataLength < 100U)
    {
        sizePadding = "*";
    }
    else
    {
        /* MISRA */
    }

    (void) printf(MAG"\r\n\r\n %s", headerDivider);
    (void) printf(CYAN"\r\n ********************************%s HASH %s TEST - INPUT SIZE %lu ******************************%s",
            sizePadding, algorithmName, dataLength, headerPaddingGet(algorithm));
    (void) printf(MAG"\r\n %s", headerDivider);
}
