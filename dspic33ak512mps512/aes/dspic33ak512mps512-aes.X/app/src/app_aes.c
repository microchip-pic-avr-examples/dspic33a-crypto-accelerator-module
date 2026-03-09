/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_aes.c

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

/* cppcheck-suppress misra-c2012-21.6
*
*  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
*
*  Reasoning: printf and sprintf are required for printing to the terminal
*  in this code example
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "crypto/common_crypto/crypto_common.h"
#include "app/app_aes.h"

uint8_t AES_ArrayEqualityCheck(const uint8_t *a, const uint8_t *b, size_t size)
{
    uint8_t result = 0U;

    if (size > 0U)
    {
        for (size_t i = 0U; i < size; i++)
        {
            result |= (uint8_t)(a[i] ^ b[i]);
        }
    }
    return result;
}

void AES_ArrayEqualityResultPrint(const uint8_t *a, const uint8_t *b, size_t size)
{
    uint8_t result = AES_ArrayEqualityCheck(a, b, size);

    if (result == 0U)
    {
        (void) printf(GREEN" \r\n\r\n PASS "RESET_COLOR);
    }
    else
    {
        (void) printf(RED" \r\n\r\n FAIL - Data Mismatch"RESET_COLOR);
    }
}

void AES_HexArrayPrint(const char *label, const uint8_t *data,
        uint32_t size)
{
    if (size > 0U)
    {
        (void) printf(BLUE
            "\r\n -----------------------------------"
            "--------------------------------------------------"
            RESET_COLOR);
        (void) printf("\r\n %s  : ", label);

        for (uint32_t index = 0U; index < size; index++)
        {
            if (((index % 16U) == 0U) && (index != 0U))
            {
                (void) printf("\r\n                          ");
            }

            (void) printf(" 0x%02x", data[index]);
        }
    }
}

void AES_TestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLen, const char *testType,
        uint32_t dataLen, const char *additionalInfo)
{
    char header[94U];
    (void) sprintf(header,
        " %s AES-%lu %s TEST - DATA: %lu BYTES%s",
        wrapperType, keyBitLen, testType,
        (unsigned long) dataLen, additionalInfo);
    int headerLen = (int) strlen(header);
    int padding = (94 - headerLen) / 2;

    (void) printf(MAG
        "\r\n\r\n ************************************"
        "******************************************");
    (void) printf(CYAN "\r\n ");
    (void) printf("%*c", padding, ' ');
    (void) printf("%s", header);
    (void) printf("%*c", padding, ' ');
    (void) printf(MAG
        "\r\n ************************************"
        "******************************************"
        RESET_COLOR);
}

void AES_CryptoErrorPrint(unsigned long keyBitLen,
        const char *testType, const char *description,
        int status)
{
    (void) printf(RED
        "\r\nCrypto v4 AES-%lu %s %s Result: %d\r\n"
        RESET_COLOR, keyBitLen, testType,
        description, status);
}

void AES_SymTestRun(void)
{
    #ifdef RUN_SYM_ECB_TEST
    AES_SymEcbTestRun();
    #endif

    #ifdef RUN_SYM_CTR_TEST
    AES_SymCtrTestRun();
    #endif

    #ifdef RUN_SYM_CBC_TEST
    AES_SymCbcTestRun();
    #endif

    #ifdef RUN_SYM_CFB_TEST
    AES_SymCfbTestRun();
    #endif

    #ifdef RUN_SYM_OFB_TEST
    AES_SymOfbTestRun();
    #endif

    #ifdef RUN_SYM_XTS_TEST
    AES_SymXtsTestRun();
    #endif
}

void AES_AeadTestRun(void)
{
    #ifdef RUN_AEAD_GCM_TEST
    AES_AeadGcmTestRun();
    #endif

    #ifdef RUN_AEAD_CCM_TEST
    AES_AeadCcmTestRun();
    #endif
}
