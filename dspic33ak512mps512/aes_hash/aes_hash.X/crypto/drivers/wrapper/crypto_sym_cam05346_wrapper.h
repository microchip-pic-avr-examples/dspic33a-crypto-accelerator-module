/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_sym_cam05346_wrapper.h

  Summary:
    Crypto Framework Library wrapper file for CAM hardware AES.

  Description:
    This header file contains the wrapper interface to access the symmetric
    AES algorithms in the AES hardware driver for Microchip microcontrollers.
**************************************************************************/

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

#ifndef CRYPTO_SYM_CAM05346_WRAPPER_H
#define CRYPTO_SYM_CAM05346_WRAPPER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "crypto/common_crypto/crypto_sym_cipher.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// The minimum size to store a CAM library AES context data block.
#define MINIMUM_AES_CONTEXT_DATA_SIZE  (252UL)

typedef struct
{
  // This is used to store the CAM library context data.
  uint8_t contextData[MINIMUM_AES_CONTEXT_DATA_SIZE];

} CRYPTO_AES_HW_CONTEXT;

// *****************************************************************************
// *****************************************************************************
// Section: Symmetric Algorithms Common Interface
// *****************************************************************************
// *****************************************************************************

crypto_Sym_Status_E Crypto_Sym_Hw_Aes_Init(void *aesInitCtx, crypto_CipherOper_E cipherOpType_en,
    crypto_Sym_OpModes_E opMode_en, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect);

crypto_Sym_Status_E Crypto_Sym_Hw_Aes_Cipher(void *aesCipherCtx, uint8_t *inputData,
    uint32_t dataLen, uint8_t *outData);

crypto_Sym_Status_E Crypto_Sym_Hw_Aes_EncryptDirect(crypto_Sym_OpModes_E opMode_en,
    uint8_t *inputData, uint32_t dataLen, uint8_t *outData,
    uint8_t *key, uint32_t keyLen, uint8_t *initVect);

crypto_Sym_Status_E Crypto_Sym_Hw_Aes_DecryptDirect(crypto_Sym_OpModes_E opMode_en,
    uint8_t *inputData, uint32_t dataLen, uint8_t *outData,
    uint8_t *key, uint32_t keyLen, uint8_t *initVect);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif /* CRYPTO_SYM_CAM05346_WRAPPER_H */
