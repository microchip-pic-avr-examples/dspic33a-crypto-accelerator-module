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
#include <string.h>
#include "app/app_mac.h"
#include "app/app_cmac.h"
#include "app/app_hmac.h"
#include "app/app_gmac.h"

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

    UTILS_TestHeaderPrint(header);
}

void MAC_CmacCryptoErrorPrint(unsigned long keyBitLength,
        const char *testType, const char *description,
        int status)
{
    char errorDesc[80];
    (void) sprintf(errorDesc, "AES-%lu %s %s",
            keyBitLength, testType, description);

    UTILS_CryptoErrorPrint(errorDesc, status);
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

    UTILS_TestHeaderPrint(header);
}

void MAC_HmacCryptoErrorPrint(const char *testType,
        const char *description, int status)
{
    char errorDesc[80];
    (void) sprintf(errorDesc, "%s %s", testType, description);

    UTILS_CryptoErrorPrint(errorDesc, status);
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

    UTILS_TestHeaderPrint(header);
}

void MAC_GmacCryptoErrorPrint(unsigned long keyBitLength,
        const char *testType, const char *description,
        int status)
{
    char errorDesc[80];
    (void) sprintf(errorDesc, "AES-%lu %s %s",
            keyBitLength, testType, description);

    UTILS_CryptoErrorPrint(errorDesc, status);
}
#endif

void MAC_TestRun(void)
{
    #ifdef RUN_MAC_CMAC_TEST
    MAC_CmacTestRun();
    #endif

    #ifdef RUN_MAC_HMAC_TEST
    MAC_HmacTestRun();
    #endif

    #ifdef RUN_MAC_GMAC_TEST
    MAC_GmacTestRun();
    #endif
}
