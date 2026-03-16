/*******************************************************************************
  Application Utilities Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_utils.h

  Summary:
    This header file provides common utility function prototypes and macro
    definitions shared across all crypto test application projects.
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

#ifndef APP_UTILS_H
#define APP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// *****************************************************************************
// *****************************************************************************
// Section: Terminal Color Macros
// *****************************************************************************
// *****************************************************************************

#define UTILS_RESET_COLOR   "\x1B""[0m"
#define UTILS_RED           "\x1B""[1;31m"
#define UTILS_GREEN         "\x1B""[1;32m"
#define UTILS_YELLOW        "\x1B""[1;33m"
#define UTILS_BLUE          "\x1B""[1;34m"
#define UTILS_MAG           "\x1B""[1;35m"
#define UTILS_CYAN          "\x1B""[1;36m"
#define UTILS_WHITE         "\x1B""[1;37m"

// *****************************************************************************
// *****************************************************************************
// Section: Common Constant Macros
// *****************************************************************************
// *****************************************************************************

#define UTILS_SESSION_ID    (0x1U)

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Compare two byte arrays for equality using constant-time XOR.
 * @param expected Pointer to the expected data array.
 * @param actual Pointer to the actual data array.
 * @param size The number of bytes to compare.
 * @return true if the arrays are identical, false otherwise.
 */
bool UTILS_ArrayEqualityCheck(const uint8_t *expected,
        const uint8_t *actual, size_t size);

/**
 * @brief Compare two arrays and print a labeled PASS or FAIL.
 * @param description Descriptive text printed after the PASS or FAIL label.
 * @param expected Pointer to the expected data array.
 * @param actual Pointer to the actual data array.
 * @param size The number of bytes to compare.
 */
void UTILS_ArrayEqualityResultPrint(const char *description,
        const uint8_t *expected,
        const uint8_t *actual, size_t size);

/**
 * @brief Print a hex dump of a byte array to the terminal.
 * @param label A descriptive label printed before the data.
 * @param data Pointer to the data to print.
 * @param size The number of bytes to print.
 */
void UTILS_HexArrayPrint(const char *label,
        const uint8_t *data, uint32_t size);

/**
 * @brief Print a separator line using a repeated character.
 * @param color The ANSI color escape string to use.
 * @param separator The character used to build the separator line.
 */
void UTILS_SeparatorPrint(const char *color, char separator);

/**
 * @brief Print a decorated test header banner.
 * @param header The header text to display inside the banner.
 */
void UTILS_TestHeaderPrint(const char *header);

/**
 * @brief Print a separator line followed by a newline.
 *
 * Print a reset-color underscore separator and append a trailing newline.
 * Convenience wrapper around UTILS_SeparatorPrint for section breaks.
 */
void UTILS_SplitterPrint(void);

/**
 * @brief Print a labeled PASS or FAIL status.
 *
 * Print " \r\n\r\n PASS: description " in green or
 * " \r\n\r\n FAIL: description " in red.
 *
 * @param description Descriptive text printed after PASS/FAIL.
 * @param passed true for PASS, false for FAIL.
 */
void UTILS_StatusPrint(const char *description, bool passed);

/**
 * @brief Print a crypto operation error message.
 * @param description A description of the failed operation.
 * @param status The error status code returned by the crypto API.
 */
void UTILS_CryptoErrorPrint(const char *description, int status);

#ifdef __cplusplus
}
#endif

#endif /* APP_UTILS_H */
