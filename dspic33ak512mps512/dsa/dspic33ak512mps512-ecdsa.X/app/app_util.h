/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecdsa.h

  Summary:
    This header file provide a prototype for the ECDSA test application.
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

#ifndef APP_UTIL_H
#define	APP_UTIL_H

#include "crypto/common_crypto/crypto_digsign.h"

#define RESET_COLOR     "\033""[0m"
#define RED             "\033""[1;31m"
#define GREEN           "\033""[1;32m"
#define YELLOW          "\033""[1;33m"
#define BLUE            "\033""[1;34m"
#define MAG             "\033""[1;35m"
#define CYAN            "\033""[1;36m"
#define SESSION_ID      0x1U
#define P192Length      48U
#define P256Length      64U
#define P384Length      96U
#define P521Length      132U
#define signaturePrint  "Signature : \t"
#define signingPrint    "SIGN"
#define verifyingPrint  "VERIFY"

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************
void printStatus(crypto_DigiSign_Status_E status, const char *type);

void printSplitter(void);

void printTestHeader(const char* curve, const char* blocking);

void printHexArray(const char* label, uint8_t* data, uint32_t size);

uint8_t signatureLengthGet(crypto_EccCurveType_E curve);
#endif	/* APP_UTIL_H */