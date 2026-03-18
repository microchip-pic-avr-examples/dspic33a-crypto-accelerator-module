/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_trng.c

  Summary:
    This source file handles the running of the TRNG examples.
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
*  Reasoning: This demo uses printf to print out information to a terminal.
*/
#include <stdio.h>
#include <string.h>
#include "app/app_trng.h"
#include "crypto/common_crypto/crypto_rng.h"

static void TRNG_Run(uint8_t *randomArray, uint32_t randomLength)
{
    const char *caption = "Random";
    char header[80];

    (void) sprintf(header, " TRNG Size %lu",
            (unsigned long) randomLength);

    UTILS_TestHeaderPrint(header);

    (void) Crypto_Rng_Generate(CRYPTO_HANDLER_HW_INTERNAL, randomArray, randomLength, NULL, 0U, 1U);

    UTILS_HexArrayPrint(caption, randomArray, randomLength);
}

// *****************************************************************************
// *****************************************************************************
// Section: TRNG Test Functions
// *****************************************************************************
// *****************************************************************************

void TRNG_TestRun(void)
{
    uint8_t randomArray1[192] = {0};
    TRNG_Run(randomArray1, 192U);

    uint8_t randomArray2[256] = {0};
    TRNG_Run(randomArray2, 256U);

    uint8_t randomArray3[384] = {0};
    TRNG_Run(randomArray3, 384U);

    uint8_t randomArray4[521] = {0};
    TRNG_Run(randomArray4, 521U);
}
