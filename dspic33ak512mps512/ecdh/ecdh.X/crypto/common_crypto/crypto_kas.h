/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_kas.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef CRYPTO_KAS_H
#define CRYPTO_KAS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "crypto_common.h"
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
typedef enum
{
    CRYPTO_KAS_ERROR_ALGONOTSUPPTD = -127,
    CRYPTO_KAS_ERROR_PUBKEY = -126,
    CRYPTO_KAS_ERROR_PRIVKEY = -125,
    CRYPTO_KAS_ERROR_HDLR = -124,
    CRYPTO_KAS_ERROR_SID = -123,  ////session ID Error
    CRYPTO_KAS_ERROR_CURVE = -122,
    CRYPTO_KAS_ERROR_ARG = -121,
    CRYPTO_KAS_ERROR_FAIL = -120,
    CRYPTO_KAS_SUCCESS = 0,        
}crypto_Kas_Status_E;

crypto_Kas_Status_E Crypto_Kas_Ecdh_SharedSecret(crypto_HandlerType_E ecdhHandlerType_en, uint8_t *ptr_privKey, uint32_t privKeyLen, uint8_t *ptr_pubKey, uint32_t pubKeyLen,
                                                    uint8_t *ptr_sharedSecret, uint32_t sharedSecretLen, crypto_EccCurveType_E eccCurveType_en, uint32_t ecdhSessionId);

#endif /* CRYPTO_KAS_H */