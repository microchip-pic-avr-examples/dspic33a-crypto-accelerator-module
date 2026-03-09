/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_util.c

  Summary:
    This source file implements helper functions.
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
#include <xc.h>
#include "app/app_util.h"

void UTIL_StatusPrint(crypto_DigiSign_Status_E status, const char *type)
{
    if (status != CRYPTO_DIGISIGN_SUCCESS)
    {
        (void)printf(RED
            " \r\n ECDSA %s FAIL:: ErrorCode = 0x%X"
            RESET_COLOR, type, (unsigned int) status);
    }
    else
    {
        (void)printf(GREEN" \r\n ECDSA %s PASS "RESET_COLOR, type);
    }
}

void UTIL_SplitterPrint(void)
{
    (void)printf("\r\n\n "
        "__________________________________________________"
        "__________________________________________________"
        "__ \r\n");
}

void UTIL_TestHeaderPrint(const char *curve, const char *blocking)
{
    (void)printf(MAG
        "\r\n\r\n ******************************************"
        "******************************************");
    (void)printf(CYAN
        "\r\n\t ******************** %s ECDSA %s "
        "********************", curve, blocking);
    (void)printf(MAG
        "\r\n ******************************************"
        "******************************************"
        RESET_COLOR);
}

void UTIL_HexArrayPrint(const char *label, uint8_t *data, uint32_t size)
{
    uint8_t const *ldata = data;
    (void)printf("\r\n\r\n\t%s", label);
    for (uint32_t i = 0; i < size; i++)
    {
        if ((0U == (i % 16U)) && (0U != i))
        {
            (void)printf("\r\n\t\t\t");
        }

        (void)printf(" 0x%02X", ldata[i]);
    }
}

uint8_t UTIL_SignatureLengthGet(crypto_EccCurveType_E curve)
{
    uint8_t length = 0;

    switch (curve)
    {
        case CRYPTO_ECC_CURVE_P192:
            length = P192_LENGTH;
            break;
        case CRYPTO_ECC_CURVE_P256:
            length = P256_LENGTH;
            break;
        case CRYPTO_ECC_CURVE_P384:
            length = P384_LENGTH;
            break;
        case CRYPTO_ECC_CURVE_P521:
            length = P521_LENGTH;
            break;
        default:
            break;
    }
    return length;
}