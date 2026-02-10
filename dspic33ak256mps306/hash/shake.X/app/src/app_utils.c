/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_utils.c

  Summary:
    This source file provides utility functions for SHA and SHAKE demos.
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

const char* algorithmNameGet(crypto_Hash_Algo_E algorithm)
{
    const char* algorithmType;

    switch(algorithm)
    {
        case CRYPTO_HASH_SHA3_SHAKE128:
            algorithmType = "SHAKE128";
            break;
        case CRYPTO_HASH_SHA3_SHAKE256:
            algorithmType = "SHAKE256";
            break;
        default:
            algorithmType = "???";
            break;
    }

    return algorithmType;
}

void headerPrint(const char* algorithmName, uint32_t dataLength)
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
    (void) printf(CYAN"\r\n ********************************%s HASH %s TEST - INPUT SIZE %lu ******************************",
            sizePadding, algorithmName, dataLength);
    (void) printf(MAG"\r\n %s", headerDivider);
}
