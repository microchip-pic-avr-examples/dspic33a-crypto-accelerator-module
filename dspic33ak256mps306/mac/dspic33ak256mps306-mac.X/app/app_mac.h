/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_mac.h

  Summary:
    This header file provides prototypes and definitions for MAC algorithms in the application.
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

#ifndef APP_MAC_H
#define APP_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "utils/app_utils.h"

// *****************************************************************************
// *****************************************************************************
// Section: Configuration Options
// *****************************************************************************
// *****************************************************************************

//#define RUN_MAC_CMAC_TEST
#define RUN_MAC_HMAC_TEST
#define RUN_MAC_GMAC_TEST

// Uncomment the #define RUN_ALL_VECTORS below to run with all defined test vectors
// #define RUN_ALL_VECTORS

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Print a formatted CMAC test header banner to the terminal.
 * @param wrapperType The wrapper type label (e.g., "MAC").
 * @param keyBitLength The AES key length in bits.
 * @param testType The test type label (e.g., "CMAC (DIRECT)").
 * @param dataLength The data length in bytes.
 * @param dataInfo Additional data info appended to the header.
 */
void MAC_CmacTestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLength, const char *testType,
        uint32_t dataLength, const char *dataInfo);

/**
 * @brief Print a formatted CMAC crypto error message to the terminal.
 * @param keyBitLength The AES key length in bits.
 * @param testType The test type label.
 * @param description Description of the failed operation.
 * @param status The error status code returned by the crypto API.
 */
void MAC_CmacCryptoErrorPrint(unsigned long keyBitLength,
        const char *testType, const char *description,
        int status);

/**
 * @brief Print a formatted HMAC test header banner to the terminal.
 * @param wrapperType The wrapper type label (e.g., "MAC").
 * @param keyBitLength The key length in bits.
 * @param testType The test type label (e.g., "HMAC (DIRECT)").
 * @param shaType The SHA algorithm variant string.
 * @param dataLength The data length in bytes.
 * @param additionalInfo Additional text appended to the header.
 */
void MAC_HmacTestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLength, const char *testType,
        const char *shaType, uint32_t dataLength,
        const char *additionalInfo);

/**
 * @brief Print a formatted HMAC crypto error message to the terminal.
 * @param testType The test type label.
 * @param description Description of the failed operation.
 * @param status The error status code returned by the crypto API.
 */
void MAC_HmacCryptoErrorPrint(const char *testType,
        const char *description, int status);

/**
 * @brief Print a formatted GMAC test header banner to the terminal.
 * @param wrapperType The wrapper type label (e.g., "MAC").
 * @param keyBitLength The AES key length in bits.
 * @param testType The test type label (e.g., "GMAC (DIRECT)").
 * @param dataLength The data length in bytes.
 * @param dataInfo Additional data info appended to the header.
 */
void MAC_GmacTestHeaderPrint(const char *wrapperType,
        unsigned long keyBitLength, const char *testType,
        uint32_t dataLength, const char *dataInfo);

/**
 * @brief Print a formatted GMAC crypto error message to the terminal.
 * @param keyBitLength The AES key length in bits.
 * @param testType The test type label.
 * @param description Description of the failed operation.
 * @param status The error status code returned by the crypto API.
 */
void MAC_GmacCryptoErrorPrint(unsigned long keyBitLength,
        const char *testType, const char *description,
        int status);

/**
 * @brief Run all enabled MAC tests (CMAC, HMAC, and GMAC).
 */
void MAC_TestRun(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_MAC_H */
