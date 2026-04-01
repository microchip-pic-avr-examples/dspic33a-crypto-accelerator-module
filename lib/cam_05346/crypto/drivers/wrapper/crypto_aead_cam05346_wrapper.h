/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_aead_cam05346_wrapper.h

  Summary:
    Crypto Framework Library wrapper file for CAM hardware AES.

  Description:
    This header file contains the wrapper interface to access the AEAD
    algorithms in the CAM AES hardware driver for Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2026 Microchip Technology Inc., and its subsidiaries. All rights reserved.

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

#ifndef CRYPTO_AEAD_CAM05346_WRAPPER_H
#define CRYPTO_AEAD_CAM05346_WRAPPER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"

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

// The minimum size to store a CAM library AEAD context data block.
#define MINIMUM_AEAD_CONTEXT_DATA_SIZE  (256U)

// The size of the AES authentication tags.
#define AES_GCM_AUTHTAG_SIZE            (16U)
#define AES_CCM_AUTHTAG_SIZE            (16U)

typedef struct
{
  // This is used to store the CAM library context data.
  uint8_t contextData[MINIMUM_AEAD_CONTEXT_DATA_SIZE];

} CRYPTO_AEAD_HW_CONTEXT;

// *****************************************************************************
// *****************************************************************************
// Section: AEAD Algorithms Common Interface
// *****************************************************************************
// *****************************************************************************

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_Init(void *aeadInitCtx,
    crypto_CipherOper_E cipherOper_en, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen);

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_AddAadData(void *aeadCipherCtx,
      uint8_t *aad, uint32_t aadLen);

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_Cipher(void *aeadCipherCtx,
    uint8_t *inputData, uint32_t dataLen, uint8_t *outData);

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_Final(void *aeadCipherCtx,
      uint8_t *authTag, uint32_t authTagLen);

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_EncryptAuthDirect(uint8_t *inputData,
    uint32_t dataLen, uint8_t *outData, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen);

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_DecryptAuthDirect(uint8_t *inputData,
    uint32_t dataLen, uint8_t *outData, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen);

crypto_Aead_Status_E Crypto_Aead_Hw_AesCcm_Init(void *aeadInitCtx,
    uint8_t *key, uint32_t keyLen);

crypto_Aead_Status_E Crypto_Aead_Hw_AesCcm_Cipher(void *aeadCipherCtx,
    crypto_CipherOper_E cipherOper_en,
    uint8_t *inputData, uint32_t dataLen, uint8_t *outData,
    uint8_t *nonce, uint32_t nonceLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif /* CRYPTO_AEAD_CAM05346_WRAPPER_H */
