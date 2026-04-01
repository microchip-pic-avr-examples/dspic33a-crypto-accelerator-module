/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_mac_cam06048_wrapper.h

  Summary:
    Crypto Framework Library wrapper file for CAM hardware MAC.

  Description:
    This header file contains the wrapper interface to access the
    MAC (CMAC/HMAC) algorithms in the AES hardware driver for Microchip microcontrollers.
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

#ifndef CRYPTO_MAC_CAM06048_WRAPPER_H
#define	CRYPTO_MAC_CAM06048_WRAPPER_H

#include <stdint.h>

#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_mac_cipher.h"

#include "crypto/drivers/wrapper/crypto_hash_cam06048_wrapper.h"
 
#include "crypto/drivers/wrapper/crypto_aead_cam06048_wrapper.h"
 
#ifdef	__cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// The minimum size to store a CAM library AES-CMAC context data block.
#define MINIMUM_CMAC_CONTEXT_DATA_SIZE  (240UL)

typedef struct
{
  // This is used to store the CAM library context data.
  uint8_t contextData[MINIMUM_CMAC_CONTEXT_DATA_SIZE];

} CRYPTO_CMAC_HW_CONTEXT;

// The maximum size of an HMAC data block.  This size is dependent on the SHA algorithm in use.
#define HMAC_MAX_BLOCK_SIZE (128U)

typedef struct
{
    // Stores the ipad_key / opad_key as used during the HMAC operation.
    uint8_t hmacKeyData[HMAC_MAX_BLOCK_SIZE];

    // Stores the HMAC block size to use for the HMAC operation.
    uint32_t hmacBlockSize;

    // Stores the HASH context to use when performing HMAC HASH operations.
    CRYPTO_HASH_HW_CONTEXT shaContext;

} CRYPTO_HMAC_HW_CONTEXT;

typedef struct
{
    // Key data, provided during init and used during cipher.
    uint8_t keyData[CRYPTO_AESKEYSIZE_256];
    // Length of the key data.
    uint32_t keyLength;

    // GMAC uses GCM with all the data provided as Additional Authentication Data (AAD).
    CRYPTO_AEAD_HW_CONTEXT aeadContext;

} CRYPTO_GMAC_HW_CONTEXT;

 
// *****************************************************************************
// *****************************************************************************
// Section: MAC Algorithms Common Interface
// *****************************************************************************
// *****************************************************************************

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Init(void *contextData, uint8_t *key, uint32_t keyLen);

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Cipher(void *contextData, uint8_t *inputData, uint32_t dataLen);

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Final(void *contextData, uint8_t *outputMac, uint32_t macLen);

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Direct(uint8_t *ptr_inputData, uint32_t dataLen,
                                              uint8_t *ptr_outMac, uint32_t macLen,
                                              uint8_t *ptr_key, uint32_t keyLen);

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Init(void *contextData, uint8_t *key, uint32_t keyLength, crypto_Hash_Algo_E shaAlgorithm);

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Cipher(void *contextData, uint8_t *inputData, uint32_t dataLength);

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Final(void *contextData, uint8_t *outputMac);

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Direct(uint8_t *ptr_inputData, uint32_t dataLength,
                                              uint8_t *ptr_outMac,
                                              uint8_t *ptr_key, uint32_t keyLength, crypto_Hash_Algo_E shaAlgorithm);

crypto_Mac_Status_E Crypto_Mac_Hw_AesGmac_Init(void *ptr_aesGmacCtx, uint8_t *ptr_key, uint32_t keySize);

crypto_Mac_Status_E Crypto_Mac_Hw_AesGmac_Cipher(void *ptr_aesGmacCtx, uint8_t *ptr_initVect,
                                                 uint32_t initVectLen, uint8_t *ptr_aad, uint32_t aadLen,
                                                 uint8_t *ptr_outMac, uint32_t macLen);

crypto_Mac_Status_E Crypto_Mac_Hw_AesGmac_Direct(uint8_t *ptr_initVect, uint32_t initVectLen,
                                                 uint8_t *ptr_outMac, uint32_t macLen, uint8_t *ptr_key,
                                                 uint32_t keyLen, uint8_t *ptr_aad, uint32_t aadLen);

 
#ifdef	__cplusplus
}
#endif

#endif	/* CRYPTO_MAC_CAM06048_WRAPPER_H */

