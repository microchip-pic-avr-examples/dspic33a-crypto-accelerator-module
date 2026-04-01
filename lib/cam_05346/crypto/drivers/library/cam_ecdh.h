/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    cam_ecdh.h

  Summary:
    Hardware abstraction layer header.

  Description:
    This header file defines the functions and structures for the CAM/HSM-Lite ECDH
    driver for Microchip microcontrollers equipped with a Crypto Accelerator Module.

**************************************************************************/

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

/**
 * Crypto Framework Library Source
 * 
 * @file cam_ecdh.h
 * @defgroup  ecdh ECDH
 * 
 * @brief This header file defines the functions and structures for the CAM/HSM-Lite ECDH driver for the Microchip microcontrollers equipped with a Crypto Accelerator Module.
 *
 */
 
#ifndef CAM_ECDH_H
#define	CAM_ECDH_H

#ifdef __cplusplus  // Provide C++ Compatibility
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
 * @ingroup ecdh
 * 
 * This function initializes the ECDH parameters and sets up the Hardware Crypto Engine
 * for the shared secret generation.
 * 
 * @param eccData Configuration structure to store ECDH information.
 * @param privateKey Private key input for the shared secret generation.
 * @param privateKeyLength Length of the private key.
 * @param publicKey Public key input for the shared secret generation.
 * @param publicKeyLength Length of the public key.
 * @param hwEccCurve ECC curve used for the generation.
 * @retval CRYPTO_PKE_RESULT_SUCCESS   Success.
 * @retval CRYPTO_PKE_RESULT_ERROR_FAIL  ECC initialization failure.
 */
CRYPTO_PKE_RESULT DRV_CRYPTO_ECDH_InitEccParams(PKE_CONFIG *eccData, uint8_t *privateKey, 
        uint32_t privateKeyLength, uint8_t *publicKey, uint32_t publicKeyLength, PKE_ECC_CURVE hwEccCurve);

/**
 * @ingroup ecdh
 * 
 * This function generates the shared secret based on the initialized ECDH parameters.
 * 
 * @param eccData Configuration structure for the ECDH information.
 * @param secret Output to store the shared secret.
 * @param secretLength Length of the shared secret.
 * @retval CRYPTO_PKE_RESULT_SUCCESS   Success.
 * @retval CRYPTO_PKE_RESULT_ERROR_FAIL  Shared secret generation failure.
 */
CRYPTO_PKE_RESULT DRV_CRYPTO_ECDH_GetSharedSecret(PKE_CONFIG *eccData, uint8_t *secret, uint32_t secretLength);

// *****************************************************************************
// *****************************************************************************
// Section: ECDH CAM/HSM-Lite Device Support
// *****************************************************************************
// *****************************************************************************

/** @cond INTERNAL */
/**
 * @ingroup ecdh
 * @brief Function prototype for initializing ECDH parameters.
 * 
 * This function is specific to the currently defined hardware version for initializing ECDH parameters for the shared secret generation.
 * If the hardware version is not defined, this function will not be available.
 */
CRYPTO_PKE_RESULT MPROTO(DRV_CRYPTO_ECDH_InitEccParams)(PKE_CONFIG *eccData, uint8_t *privateKey, 
        uint32_t privateKeyLength, uint8_t *publicKey, uint32_t publicKeyLength, PKE_ECC_CURVE hwEccCurve);

/**
 * @ingroup ecdh
 * @def DRV_CRYPTO_ECDH_InitEccParams
 * @brief Macro to create a device-specific function prototype for initializing ECDH parameters.
 * 
 * This macro generates a function prototype specific to the currently defined
 * hardware version for initializing ECDH parameters for the shared secret generation.
 */
#define DRV_CRYPTO_ECDH_InitEccParams MPROTO(DRV_CRYPTO_ECDH_InitEccParams)

/**
 * @ingroup ecdh
 * @brief Function prototype for generating shared secret.
 * 
 * This function is specific to the currently defined hardware version for generating the shared secret.
 * If the hardware version is not defined, this function will not be available.
 */
CRYPTO_PKE_RESULT MPROTO(DRV_CRYPTO_ECDH_GetSharedSecret)(PKE_CONFIG *eccData, uint8_t *secret, uint32_t secretLength);

/**
 * @ingroup ecdh
 * @def DRV_CRYPTO_ECDH_GetSharedSecret
 * @brief Macro to create a device-specific function prototype for generating shared secret.
 * 
 * This macro generates a function prototype specific to the currently defined
 * hardware version for generating the shared secret.
 */
#define DRV_CRYPTO_ECDH_GetSharedSecret MPROTO(DRV_CRYPTO_ECDH_GetSharedSecret)
/** @endcond */

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif	/* CAM_ECDH_H */
