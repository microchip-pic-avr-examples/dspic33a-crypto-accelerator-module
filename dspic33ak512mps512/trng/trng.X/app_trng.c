/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This source file handles the running of the TRNG examples.
*******************************************************************************/

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

/* cppcheck-suppress misra-c2012-21.6
* 
*  (Rule 21.6) Required: The standard library input/output functions shall not be used
* 
*  Reasoning: This demo uses printf to print out information to a terminal.
*/
#include <stdio.h>
#include <stdint.h>
#include "app_trng.h"
#include "crypto/common_crypto/crypto_rng.h"

static const char * caption = "Random";

static inline void printHexArray(const char* label, void* Data, uint32_t size) 
{
    (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
    const uint8_t *ldata = (uint8_t *) Data;
    (void) printf("\r\n  %s   :  ", label);
    for (uint32_t i = 0; i < size; i++) 
    {
        if (i % 16U == 0U && i != 0U) 
        {
            (void) printf("\r\n              ");
        }
        (void) printf(" 0x%02X", ldata[i]);
    }
}

static void trngRun(uint8_t* randArray, uint32_t randLen)
{
    (void) printf(MAG"\r\n\r\n *********************************************************************************************"RESET_COLOR);
    (void) printf(CYAN"\r\n *************************************** TRNG Size %ld ***************************************"RESET_COLOR, randLen);
    (void) printf(MAG"\r\n *********************************************************************************************"RESET_COLOR);
    
    (void) Crypto_Rng_Generate(CRYPTO_HANDLER_HW_INTERNAL, randArray, randLen, NULL, 0, 1);
    
    printHexArray(caption, randArray, randLen);
}

// *****************************************************************************
// *****************************************************************************
// Section: TRNG Test Functions
// *****************************************************************************
// *****************************************************************************

void trngTest(void) 
{
    uint8_t randArray1[192] = {0};
    trngRun(randArray1, 192);
    uint8_t randArray2[256] = {0};
    trngRun(randArray2, 256);
    uint8_t randArray3[384] = {0};
    trngRun(randArray3, 384);
    uint8_t randArray4[521] = {0};
    trngRun(randArray4, 521);
}
