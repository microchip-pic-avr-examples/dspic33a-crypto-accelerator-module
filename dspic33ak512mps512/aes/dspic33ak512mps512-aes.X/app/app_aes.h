/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_aes.h

  Summary:
    This header file provides prototypes and definitions for AES utility
    functions, test runners, and terminal output formatting.
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

#ifndef APP_AES_H
#define APP_AES_H

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "utils/app_utils.h"
#include "app/app_sym.h"
#include "app/app_aead.h"

// *****************************************************************************
// *****************************************************************************
// Section: Configuration Options
// *****************************************************************************
// *****************************************************************************

#define RUN_SYM_TEST
#define RUN_AEAD_TEST

// Uncomment the #define RUN_ALL_VECTORS below to run with all defined test vectors
// #define RUN_ALL_VECTORS

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Print a formatted AES test header banner to the terminal.
 * @param wrapperType The wrapper type label (e.g., "SYMMETRIC", "AEAD").
 * @param keyBitLength The AES key length in bits.
 * @param testType The test type label (e.g., "ECB", "GCM").
 * @param dataLength The data length in bytes.
 * @param additionalInfo Additional text appended to the header.
 */
void AES_TestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLength, const char *testType,
        uint32_t dataLength, const char *additionalInfo);

/**
 * @brief Print a formatted AES crypto error message to the terminal.
 * @param keyBitLength The AES key length in bits.
 * @param testType The test type label (e.g., "ECB", "GCM").
 * @param description Description of the failed operation.
 * @param status The error status code returned by the crypto API.
 */
void AES_CryptoErrorPrint(unsigned long keyBitLength,
        const char *testType, const char *description,
        int status);

/**
 * @brief Run all enabled symmetric AES cipher mode tests.
 */
void AES_SymTestRun(void);

/**
 * @brief Run all enabled AEAD AES cipher mode tests.
 */
void AES_AeadTestRun(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_AES_H */
