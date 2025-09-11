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
*  Reasoning: printf is required for printing to the terminal in this code example
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "crypto/common_crypto/crypto_common.h"
#include "app_aes.h"

uint8_t checkArrayEquality(const uint8_t *a, const uint8_t *b, size_t size)
{
    uint8_t result = 0;

    if (size > 0U)
    {
        const uint8_t *tempa = a;
        const uint8_t *tempb = b;

        for (unsigned int i = 0; i < size; i++)
        {
            result |= tempa[i] ^ tempb[i];
        }

    }
    return result;
}



void printArrayEqualityResult(const uint8_t *a, const uint8_t *b, size_t size)
{
	uint8_t result = checkArrayEquality(a, b, size);

    if(result == 0U)
    {
        (void) printf(GREEN" \r\n\r\n PASS "RESET_COLOR);
    }
    else
    {
        (void) printf(RED" \r\n\r\n FAIL - Data Mismatch"RESET_COLOR);
    }
}


void printHexArray(const char* label, void* data, uint32_t size)
{
    if (size > 0U)
    {
        const uint8_t *ldata = (uint8_t *) data;

        (void) printf(BLUE"\r\n ---------------------------------------------------------------------------------------------"RESET_COLOR);
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

void printTestHeader(const char* wrapperType, unsigned long keyBitLen, const char* testType, uint32_t dataLen, const char* additionalInfo)
{
    char header[94];
    (void) sprintf(header, " %s AES-%lu %s TEST - DATA: %lu BYTES%s", wrapperType, keyBitLen, testType, dataLen, additionalInfo);
    int headerLen = strlen(header);
    int padding = (94 - headerLen) / 2;

    (void) printf(MAG"\r\n\r\n **********************************************************************************************");
    (void) printf(CYAN"\r\n ");
    (void) printf("%*c", padding, ' ');
    (void) printf("%s", header);
    (void) printf("%*c", padding, ' ');
    (void) printf(MAG"\r\n **********************************************************************************************"RESET_COLOR);
}

void printCryptoError(unsigned long keyBitLen, const char* testType, const char* description, int status)
{
    (void) printf(RED"\r\nCrypto v4 AES-%lu %s %s Result: %d\r\n"RESET_COLOR, keyBitLen, testType, description, status);
}

void aes_test_sym(void)
{
    #ifdef RUN_SYM_ECB_TEST
    aes_sym_ecb_test();
    #endif

    #ifdef RUN_SYM_CTR_TEST
    aes_sym_ctr_test();
    #endif
}

void aes_test_mac(void)
{
    #ifdef RUN_MAC_CMAC_TEST
    aes_mac_cmac_test();
    #endif
}

void aes_test_aead(void)
{
    #ifdef RUN_AEAD_GCM_TEST
    aes_aead_gcm_test();
    #endif
}