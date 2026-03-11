/*******************************************************************************
  Application Utilities Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_utils.c

  Summary:
    This source file implements common utility functions shared across all
    crypto test application projects.
*******************************************************************************/

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/* cppcheck-suppress misra-c2012-21.6
 *
 *  (Rule 21.6) REQUIRED: The standard library input/output functions shall not
 *  be used
 *
 *  Reasoning: printf is required for printing to the terminal in this code
 *  example.
 */
#include <stdio.h>
#include <string.h>
#include "utils/app_utils.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

#define UTILS_SEPARATOR_LENGTH  (100)
#define UTILS_HEX_WRAP_COLUMN   (16U)
#define UTILS_HEADER_WIDTH      (94)

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

bool UTILS_ArrayEqualityCheck(const uint8_t *expected,
        const uint8_t *actual, size_t size)
{
    uint8_t result = 0U;
    bool isEqual = false;

    if (size > 0U)
    {
        for (size_t byteIndex = 0U; byteIndex < size; byteIndex++)
        {
            result |= (uint8_t)(expected[byteIndex]
                    ^ actual[byteIndex]);
        }
    }

    isEqual = (result == 0U);

    return isEqual;
}

void UTILS_ArrayEqualityResultPrint(const uint8_t *expected,
        const uint8_t *actual, size_t size)
{
    if (true == UTILS_ArrayEqualityCheck(expected, actual, size))
    {
        (void) printf(UTILS_GREEN
                " \r\n\r\n PASS " UTILS_RESET_COLOR);
    }
    else
    {
        (void) printf(UTILS_RED
                " \r\n\r\n FAIL - Data Mismatch"
                UTILS_RESET_COLOR);
    }
}

void UTILS_HexArrayPrint(const char *label,
        const uint8_t *data, uint32_t size)
{
    (void) printf(UTILS_BLUE
            "\r\n ----------------------------------------"
            "---------------------------------------------"
            UTILS_RESET_COLOR);

    (void) printf("\r\n %-22s :", label);

    if (size > 0U)
    {
        for (uint32_t index = 0U; index < size; index++)
        {
            if (((index % UTILS_HEX_WRAP_COLUMN) == 0U)
                    && (index != 0U))
            {
                (void) printf("\r\n%-25s", "");
            }

            (void) printf(" 0x%02x", data[index]);
        }
    }
    else
    {
        (void) printf(" None");
    }
}

void UTILS_SeparatorPrint(const char *color, char separator)
{
    (void) printf("\r\n %s", color);

    for (uint8_t charIndex = 0U;
            charIndex < (uint8_t)UTILS_SEPARATOR_LENGTH;
            charIndex++)
    {
        (void) printf("%c", separator);
    }

    (void) printf(UTILS_RESET_COLOR);
}

void UTILS_SplitterPrint(void)
{
    UTILS_SeparatorPrint(UTILS_RESET_COLOR, '_');
    (void) printf("\r\n");
}

void UTILS_TestHeaderPrint(const char *header)
{
    int headerLength = (int) strlen(header);
    int padding = (UTILS_HEADER_WIDTH - headerLength) / 2;

    (void) printf(UTILS_MAG
            "\r\n\r\n *************************************"
            "*********************************************************");

    (void) printf(UTILS_CYAN "\r\n ");
    (void) printf("%*c", padding, ' ');
    (void) printf("%s", header);
    (void) printf("%*c", padding, ' ');

    (void) printf(UTILS_MAG
            "\r\n **************************************"
            "********************************************************"
            UTILS_RESET_COLOR);
}

void UTILS_CryptoErrorPrint(const char *description, int status)
{
    (void) printf(UTILS_RED
            "\r\n Common Crypto %s Result: %d\r\n"
            UTILS_RESET_COLOR, description, status);
}
