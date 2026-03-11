/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_mac.c

  Summary:
    This source file handles the running of the MAC examples.
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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "crypto/common_crypto/crypto_common.h"
#include "app/app_mac.h"
#include "app/app_cmac.h"
#include "app/app_hmac.h"
#include "app/app_gmac.h"

static uint8_t MAC_ArrayEqualityCheck(const uint8_t *expected, const uint8_t *actual, size_t size)
{
    uint8_t result = 0;

    if (size > 0U)
    {
        const uint8_t *tempExpected = expected;
        const uint8_t *tempActual = actual;

        for (size_t byteIndex = 0U; byteIndex < size; byteIndex++)
        {
            result |= (uint8_t)(tempExpected[byteIndex] ^ tempActual[byteIndex]);
        }
    }
    return result;
}

void MAC_ArrayEqualityResultPrint(const uint8_t *expected, const uint8_t *actual, size_t size)
{
    uint8_t result = MAC_ArrayEqualityCheck(expected, actual, size);

    if (result == 0U)
    {
        (void) printf(GREEN " \r\n\r\n PASS " RESET_COLOR);
    }
    else
    {
        (void) printf(RED " \r\n\r\n FAIL - Data Mismatch" RESET_COLOR);
    }
}

void MAC_HexArrayPrint(const char *label, uint8_t *data, uint32_t size)
{
    if (size > 0U)
    {
        const uint8_t *ldata = data;

        (void) printf(BLUE
            "\r\n -----------------------------------------"
            "--------------------------------------------"
            "----" RESET_COLOR);

        (void) printf("\r\n %s  : ", label);

        for (uint32_t index = 0U; index < size; index++)
        {
            if (((index % 16U) == 0U) && (index != 0U))
            {
                (void) printf("\r\n                          ");
            }

            (void) printf(" 0x%02x", ldata[index]);
        }
    }
}

#ifdef RUN_MAC_CMAC_TEST
void MAC_CmacTestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLength, const char *testType,
        uint32_t dataLength, const char *additionalInfo)
{
    char header[94];

    (void) sprintf(header,
        " %s AES-%lu %s TEST - DATA: %lu BYTES%s",
        wrapperType, keyBitLength, testType,
        (unsigned long)dataLength, additionalInfo);

    int headerLength = (int)strlen(header);
    int padding = (94 - headerLength) / 2;

    (void) printf(MAG
        "\r\n\r\n *************************************"
        "*********************************************************");

    (void) printf(CYAN "\r\n ");
    (void) printf("%*c", padding, ' ');
    (void) printf("%s", header);
    (void) printf("%*c", padding, ' ');

    (void) printf(MAG
        "\r\n **************************************"
        "********************************************************"
        RESET_COLOR);
}

void MAC_CmacCryptoErrorPrint(unsigned long keyBitLength,
        const char *testType, const char *description,
        int status)
{
    (void) printf(RED
        "\r\nCrypto v4 AES-%lu %s %s Result: %d\r\n"
        RESET_COLOR, keyBitLength, testType,
        description, status);
}
#endif

#ifdef RUN_MAC_HMAC_TEST
void MAC_HmacTestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLength, const char *testType,
        const char *shaType, uint32_t dataLength,
        const char *additionalInfo)
{
    char header[94];

    (void) sprintf(header,
        " %s %s SHA-%s TEST - KEY: %lu BITS,"
        " DATA: %lu BYTES%s",
        wrapperType, testType, shaType, keyBitLength,
        (unsigned long)dataLength, additionalInfo);

    int headerLength = (int)strlen(header);
    int padding = (94 - headerLength) / 2;

    (void) printf(MAG
        "\r\n\r\n *************************************"
        "*********************************************************");

    (void) printf(CYAN "\r\n ");
    (void) printf("%*c", padding, ' ');
    (void) printf("%s", header);
    (void) printf("%*c", padding, ' ');

    (void) printf(MAG
        "\r\n **************************************"
        "********************************************************"
        RESET_COLOR);
}

void MAC_HmacCryptoErrorPrint(const char *testType,
        const char *description, int status)
{
    (void) printf(RED
        "\r\nCrypto v4 %s %s Result: %d\r\n"
        RESET_COLOR, testType, description, status);
}
#endif

#ifdef RUN_MAC_GMAC_TEST
void MAC_GmacTestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLength, const char *testType,
        uint32_t dataLength, const char *additionalInfo)
{
    char header[94];

    (void) sprintf(header,
        " %s AES-%lu %s TEST - DATA: %lu BYTES%s",
        wrapperType, keyBitLength, testType,
        (unsigned long)dataLength, additionalInfo);

    int headerLength = (int)strlen(header);
    int padding = (94 - headerLength) / 2;

    (void) printf(MAG
        "\r\n\r\n *************************************"
        "*********************************************************");

    (void) printf(CYAN "\r\n ");
    (void) printf("%*c", padding, ' ');
    (void) printf("%s", header);
    (void) printf("%*c", padding, ' ');

    (void) printf(MAG
        "\r\n **************************************"
        "********************************************************"
        RESET_COLOR);
}

void MAC_GmacCryptoErrorPrint(unsigned long keyBitLength,
        const char *testType, const char *description,
        int status)
{
    (void) printf(RED
        "\r\nCrypto v4 AES-%lu %s %s Result: %d\r\n"
        RESET_COLOR, keyBitLength, testType,
        description, status);
}
#endif

#ifdef RUN_MAC_CMAC_TEST
void MAC_CmacTestRun(void)
{
    CMAC_TestRun();
}
#endif

#ifdef RUN_MAC_HMAC_TEST
void MAC_HmacTestRun(void)
{
    HMAC_TestRun();
}
#endif

#ifdef RUN_MAC_GMAC_TEST
void MAC_GmacTestRun(void)
{
    GMAC_TestRun();
}
#endif
