/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    cam_ecdh.h

  Summary:
    Hardware abstraction layer header.

  Description:
    This header file defined the functions and structures for the CAM ECDH
    hardware driver for the following families of Microchip microcontrollers:
    dsPIC33AK with Crypto Accelerator Module.
**************************************************************************/

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
#ifndef CAM_ECDH_H
#define	CAM_ECDH_H

#ifdef	__cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "cam_pke.h"
#include "cam_version.h"
#include "cam_device.h"

// *****************************************************************************
// *****************************************************************************
// Section: ECDH Common Interface
// *****************************************************************************
// *****************************************************************************
    
/**
 *  @brief Initialize the ECDH parameters used for Shared Secret generation.
 *  @param eccData Configuration structure to store ECDH information.
 *  @param privateKey Private Key input for Shared Secret generation.
 *  @param privateKeyLength Length of the private key.
 *  @param publicKey Public key input for Shared Secret generation.
 *  @param publicKeyLength Length of the private key.
 *  @param hwEccCurve ECC Curve used for generation.
 *  @return CRYPTO_PKE_RESULT_SUCCESS on success.  CRYPTO_PKE_RESULT_ERROR_FAIL on failure.
 *  @note This function initializes eccData with ECDH parameters and sets up the Hardware Crypto Engine.
 **/
CRYPTO_PKE_RESULT DRV_CRYPTO_ECDH_InitEccParams(PKE_CONFIG *eccData, uint8_t *privateKey, 
        uint32_t privateKeyLength, uint8_t *publicKey, uint32_t publicKeyLength, PKE_ECC_CURVE hwEccCurve);

/**
 *  @brief Generate the Shared Secret.
 *  @param eccData Configuration structure for ECDH information.
 *  @param secret Ouput to store the Shared Secret to.
 *  @param secretLength Length of the Shared Secret.
 *  @return CRYPTO_PKE_RESULT_SUCCESS on success.  CRYPTO_PKE_RESULT_ERROR_FAIL on failure.
 **/
CRYPTO_PKE_RESULT DRV_CRYPTO_ECDH_GetSharedSecret(PKE_CONFIG *eccData, uint8_t *secret, uint32_t secretLength);

// *****************************************************************************
// *****************************************************************************
// Section: ECDH CAM Device Support
// *****************************************************************************
// *****************************************************************************

CRYPTO_PKE_RESULT MPROTO(DRV_CRYPTO_ECDH_InitEccParams)(PKE_CONFIG *eccData, uint8_t *privateKey, 
        uint32_t privateKeyLength, uint8_t *publicKey, uint32_t publicKeyLength, PKE_ECC_CURVE hwEccCurve);
#define DRV_CRYPTO_ECDH_InitEccParams MPROTO(DRV_CRYPTO_ECDH_InitEccParams)

CRYPTO_PKE_RESULT MPROTO(DRV_CRYPTO_ECDH_GetSharedSecret)(PKE_CONFIG *eccData, uint8_t *secret, uint32_t secretLength);
#define DRV_CRYPTO_ECDH_GetSharedSecret MPROTO(DRV_CRYPTO_ECDH_GetSharedSecret)

#ifdef	__cplusplus
}
#endif

#endif	/* CAM_ECDH_H */
