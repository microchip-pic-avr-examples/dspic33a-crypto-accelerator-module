/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    cam_pke.h

  Summary:
    Common enums and structures used with PKE operations.

  Description:
    This header file defined the enums and structures for the ECDSA and ECDH  
    hardware drivers for the following families of Microchip microcontrollers:
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

#ifndef CAM_PKE_H
#define	CAM_PKE_H

#ifdef	__cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "cam_version.h"
#include "cam_device.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief This enum represents the result of the CAM PKE operation.
 */
typedef enum {
    CRYPTO_PKE_RESULT_SUCCESS,
    CRYPTO_PKE_RESULT_INIT_FAIL,
    CRYPTO_PKE_ERROR_PUBKEYCOMPRESS,
    CRYPTO_PKE_RESULT_ERROR_CURVE,
    CRYPTO_PKE_RESULT_ERROR_RNG,
    CRYPTO_PKE_RESULT_ERROR_FAIL
} CRYPTO_PKE_RESULT;

/**
 * @brief This enum represents the ECC curves supported by the CAM PKE driver.
 */
typedef enum
{
  NO_CURVE = 0,
  P256 = 1,
  P384 = 2,
  P521 = 3,
  P192 = 4,
} PKE_ECC_CURVE;

/**
 * @brief This enum represents the type of operation the PKE engine must complete.
 */
typedef enum
{
    DEFAULT,
    PKE_CLEAR_MEMORY,
    ECDSA_SIGNATURE_GENERATION,
    ECDSA_SIGNATURE_VERIFICATION,
    ECDH_ECC_MULTIPLY,
} PKE_OPERATIONS;

/**
 * @brief This struct is used to store input/output values.
 */
typedef struct
{
    uint8_t* data; /**< @brief List of values used as an input or output.*/
    uint32_t size; /**< @brief Size of the list being stored.*/
} PKE_DATA;

/**
 * @brief This struct is used to store key data.
 */
typedef struct
{
    uint8_t* x;    /**< @brief X component of the public key.*/
    uint8_t* y;    /**< @brief Y component of the public key.*/
    uint32_t size; /**< @brief Size of an individual key component.*/
} PKE_KEY_DATA;

/**
 * @brief This struct stores the inputs and configuration options for the PKE.
 */
typedef struct
{
    PKE_OPERATIONS operation; /**< @brief Operation the PKE needs to compute.*/
    PKE_DATA operand1;        /**< @brief PKE_DATA Input/output value storage for operand 1.*/
    PKE_DATA operand2;        /**< @brief PKE_DATA Input/output value storage for operand 2.*/
    PKE_DATA operand3;        /**< @brief PKE_DATA Input/output value storage for operand 3.*/
    PKE_KEY_DATA publicKey;   /**< @brief PKE_KEY_DATA Public key storage.*/
    uint8_t operandSize;      /**< @brief Size of the input/output operands being used.*/
    PKE_ECC_CURVE curve;      /**< @brief Enum for the curve type being used.*/
} PKE_CONFIG;


// *****************************************************************************
// *****************************************************************************
// Section: PKE Common Interface
// *****************************************************************************
// *****************************************************************************

/**
 *  @brief Resets the interrupt state for PKE operations.
 **/
void DRV_CRYPTO_PKE_IsrHelper(void);

// *****************************************************************************
// *****************************************************************************
// Section: PKE CAM Device Support
// *****************************************************************************
// *****************************************************************************

void MPROTO(DRV_CRYPTO_PKE_IsrHelper)(void);
#define DRV_CRYPTO_PKE_IsrHelper MPROTO(DRV_CRYPTO_PKE_IsrHelper)

#ifdef	__cplusplus
}
#endif

#endif	/* CAM_PKE_H */
