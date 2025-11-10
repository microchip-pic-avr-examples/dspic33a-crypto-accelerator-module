/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    cam_aes.h

  Summary:
    Crypto Framework Library interface file for CAM AES.

  Description:
    This header file contains the interface that make up the CAM AES hardware
    driver for the Microchip microcontrollers equipped with a Crypto Accelerator Module.
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

/**
 * Crypto Framework Library Source
 * 
 * @file cam_aes.h
 * @defgroup  aes AES
 * 
 * @brief This header file contains the interface that make up the CAM AES hardware driver for the Microchip microcontrollers equipped with a Crypto Accelerator Module.
 *
 */
 
#ifndef CAM_AES_H
#define	CAM_AES_H

#ifdef	__cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include "cam_version.h"
#include "cam_device.h"

// *****************************************************************************
// *****************************************************************************
// Section: AES Common Data Types
// *****************************************************************************
// *****************************************************************************

/**
 * @ingroup aes
 * @def AES_BLOCK_SIZE
 * @brief This macro defines the size in bytes of an AES block
 */
#define AES_BLOCK_SIZE          (16U)  // bytes

/**
 * @ingroup aes
 * @def CMAC_BLOCK_SIZE
 * @brief This macro defines the size in bytes of the context for the CMAC generation
 */
#define CMAC_BLOCK_SIZE         (16U)  // bytes

/**
 * @ingroup aes
 * @enum AES_ERROR
 * 
 * This enumeration defines the various error states that can occur during
 * AES operations.
 */
typedef enum AES_ERROR
{
    AES_NO_ERROR = 0,               /**< No error occurred */
    AES_CONTEXT_ERROR = 1,          /**< Error with the AES context */
    AES_INVALID_MODE_ERROR = 2,     /**< Invalid AES mode specified */
    AES_KEY_SETUP_ERROR = 3,        /**< Error during key setup */
    AES_INIT_VECTOR_SETUP_ERROR = 4,/**< Error during initialization vector setup */
    AES_COUNTER_MEASURE_SETUP_ERROR = 5, /**< Error during counter measure setup */
    AES_INITIALIZE_ERROR = 6,       /**< Error during initialization */
    AES_WRITE_ERROR = 7,            /**< Error during write operation */
    AES_READ_ERROR = 8,             /**< Error during read operation */
    AES_STATE_ERROR = 9,            /**< Error with the AES state */
    AES_EXECUTE_ERROR = 10,         /**< Error during execution */
    AES_OPERATION_IS_NOT_ACTIVE = 11,/**< No active AES operation */
    AES_OPERATION_IS_ACTIVE = 12     /**< AES operation is currently active */
} AES_ERROR;

/**
 * @ingroup aes
 * @enum AESCON_MODE
 * 
 * This enumeration defines the various modes of operation supported by the AES driver.
 */
typedef enum AESCON_MODE
{
    MODE_ECB  = 0,   /**< Electronic Codebook (ECB) mode */
    MODE_CTR  = 2,   /**< Counter (CTR) mode */
    MODE_GCM  = 6,   /**< Galois Counter Mode (GCM) */
    MODE_XTS  = 7,   /**< XEX Tweakable Block Cipher (XTS) with Ciphertext Stealing (XTS-CS) */
    MODE_CMAC = 8,   /**< Cipher-based Message Authentication Code (CMAC) */
} AESCON_MODE;

/**
 * @ingroup aes
 * @enum AESCON_OPERATION
 * 
 * This enumeration defines the operations that can be performed using the AES driver.
 */
typedef enum AESCON_OPERATION
{
    OP_ENCRYPT = 0, /**< Encrypt operation */
    OP_DECRYPT = 1, /**< Decrypt operation */
} AESCON_OPERATION;


// *****************************************************************************
// *****************************************************************************
// Section: AES Common Interface
// *****************************************************************************
// *****************************************************************************

/**
 * @ingroup aes
 * 
 * This function handles the interrupt service routine for the AES operations.
 * @return None.
 */
void DRV_CRYPTO_AES_IsrHelper(void);


/**
 * @ingroup aes
 * 
 * This function initializes a new AES operation with the specified mode and operation type.
 * 
 * @param contextData The context data for this operation.
 * @param mode The AESCON_MODE mode to use.
 * @param operation The AESCON_OPERATION operation to perform.
 * @param key The AES key to use.
 * @param keyLength The length of the AES key.
 * @param initVector The initialization vector (IV) data to use.
 * @param initVectorLength The length of the IV data.
 * @retval AES_NO_ERROR         Success.
 * @retval AES_STATE_ERROR      Invalid or unexpected AES state.
 * @retval AES_INITIALIZE_ERROR Failure during AES initialization. 
 * @note Reinitializing an AES operation in progress will void the previous operation.
 */
AES_ERROR DRV_CRYPTO_AES_Initialize(void *contextData, AESCON_MODE mode, AESCON_OPERATION operation,
        void* key, uint32_t keyLength, void *initVector, uint32_t initVectorLength);

/**
 * @ingroup aes
 * 
 * This function adds header data (such as Additional Authentication Data) to an AES operation.
 * 
 * @param contextData The context data for this operation.
 * @param headerData The header data to add to the operation.
 * @param headerLength The length of the header data.
 * @retval AES_NO_ERROR     Success.
 * @retval AES_STATE_ERROR  Invalid or unexpected AES state.
 * @retval AES_WRITE_ERROR  Failure while writing header data.
 * @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 */
AES_ERROR DRV_CRYPTO_AES_AddHeader(void *contextData, void * headerData, uint32_t headerLength);

/**
 * @ingroup aes
 * 
 * This function initializes the tweak value for use in AES XTS mode.
 * 
 * @param contextData The context data for this operation.
 * @param tweakData The tweak data to use.
 * @param tweakLength The length of the tweak data.
 * @retval AES_NO_ERROR     Success.
 * @retval AES_STATE_ERROR  Invalid or unexpected AES state.
 * @retval AES_WRITE_ERROR  Failure while writing tweak data.
 * @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 */
AES_ERROR DRV_CRYPTO_AES_AddTweakData(void *contextData, const void *tweakData, uint32_t tweakLength);

/**
 * @ingroup aes
 * 
 * This function adds input data to the ongoing AES operation.
 * 
 * @param contextData The context data for this operation.
 * @param data The data to add to the operation.
 * @param dataLength The length of the data.
 * @retval AES_NO_ERROR     Success.
 * @retval AES_STATE_ERROR  Invalid or unexpected AES state.
 * @retval AES_WRITE_ERROR  Failure while writing input data. 
 * @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 */
AES_ERROR DRV_CRYPTO_AES_AddInputData(void *contextData, void * data, uint32_t dataLength);

/**
 * @ingroup aes
 * 
 * This function specifies the output buffer for the AES operation.
 * 
 * @param contextData The context data for this operation.
 * @param data The data buffer to add to the operation.
 * @param dataLength The length of the data buffer.
 * @retval AES_NO_ERROR     Success.
 * @retval AES_STATE_ERROR  Invalid or unexpected AES state.
 * @retval AES_WRITE_ERROR  Failure while writing output data.
 * @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 */
AES_ERROR DRV_CRYPTO_AES_AddOutputData(void *contextData, void * data, uint32_t dataLength);

/**
 * @ingroup aes
 * 
 * This function discards a specified number of bytes from the output stream of the AES operation.
 * 
 * @param contextData The context data for this operation.
 * @param discardLength The number of bytes to discard from the output stream.
 * @retval AES_NO_ERROR     Success.
 * @retval AES_STATE_ERROR  Invalid or unexpected AES state.
 * @retval AES_WRITE_ERROR  Failure while discarding output data.
 * @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 */
AES_ERROR DRV_CRYPTO_AES_DiscardData(void *contextData, uint32_t discardLength);

/**
 * @ingroup aes
 * 
 * This function calculates the LenALenC input for the AES-GCM and adds it to the operation.
 * 
 * @param contextData The context data for this operation.
 * @retval AES_NO_ERROR     Success.
 * @retval AES_STATE_ERROR  Invalid or unexpected AES state.
 * @retval AES_WRITE_ERROR  Failure while writing LenALenC data.
 * @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 */
AES_ERROR DRV_CRYPTO_AES_AddLenALenC(void *contextData);

/**
 * @ingroup aes
 * 
 * This function executes an AES operation to generate the final result (such as a CMAC or a cipher).
 * 
 * @param contextData The context data for this operation.
 * @retval AES_NO_ERROR      Success.
 * @retval AES_STATE_ERROR   Invalid or unexpected AES state.
 * @retval AES_EXECUTE_ERROR Failure while executing the AES operation.
 * @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 */
AES_ERROR DRV_CRYPTO_AES_Execute(void *contextData);

/**
 * @ingroup aes
 * 
 * This function checks if an AES operation is currently active.
 * 
 * @param contextData The context data for this operation.
 * @param active Pointer to value to contain the active/inactive status.
 * @retval AES_NO_ERROR        Success. The value pointed to by active is set to
 *                             AES_OPERATION_IS_ACTIVE or AES_OPERATION_IS_NOT_ACTIVE.
 * @retval AES_CONTEXT_ERROR   Invalid or corrupted AES context.
 * @retval AES_STATE_ERROR     Invalid or unexpected AES state.
 * @note This sets the value pointed to by active to AES_OPERATION_IS_ACTIVE
 *       or AES_OPERATION_IS_NOT_ACTIVE when the call is successful.
 */
AES_ERROR DRV_CRYPTO_AES_IsActive(void *contextData, AES_ERROR *active);

/**
 * @ingroup aes
 * 
 * This function returns the size of an AES context block based on the AES mode in use.
 * 
 * @param contextData The context data for this operation.
 * @return Size of the AES context block in bytes.
 */
uint32_t DRV_CRYPTO_AES_GetContextSize(void *contextData);

// *****************************************************************************
// *****************************************************************************
// Section: AES CAM Device Support
// *****************************************************************************
// *****************************************************************************

/** @cond INTERNAL */
/**
 * @ingroup aes
 * @brief Function prototype for the AES ISR helper.
 * 
 * This function is specific to the currently defined hardware version for the AES ISR helper.
 * If the hardware version is not defined, this function will not be available.
 */
void MPROTO(DRV_CRYPTO_AES_IsrHelper)(void);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_IsrHelper
 * @brief Macro to create a device-specific function prototype for the AES ISR helper.
 */
#define DRV_CRYPTO_AES_IsrHelper MPROTO(DRV_CRYPTO_AES_IsrHelper)

/**
 * @ingroup aes
 * @brief Function prototype for initializing AES.
 * 
 * This function is specific to the currently defined hardware version for initializing an AES operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_Initialize)(void *contextData, AESCON_MODE mode, AESCON_OPERATION operation, 
        void* key, uint32_t keyLength, void *initVector, uint32_t initVectorLength);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_Initialize
 * @brief Macro to create a device-specific function prototype for initializing AES.
 */
#define DRV_CRYPTO_AES_Initialize MPROTO(DRV_CRYPTO_AES_Initialize)

/**
 * @ingroup aes
 * @brief Function prototype for adding header data to AES.
 * 
 * This function is specific to the currently defined hardware version for adding header data to an AES operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_AddHeader)(void *contextData, void * headerData, uint32_t headerLength);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_AddHeader
 * @brief Macro to create a device-specific function prototype for adding header data to AES.
 */
#define DRV_CRYPTO_AES_AddHeader MPROTO(DRV_CRYPTO_AES_AddHeader)

/**
 * @ingroup aes
 * @brief Function prototype for adding tweak data to AES.
 * 
 * This function is specific to the currently defined hardware version for adding tweak data to an AES operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_AddTweakData)(void *contextData, const void *tweakData, uint32_t tweakLength);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_AddTweakData
 * @brief Macro to create a device-specific function prototype for adding tweak data to AES.
 */
#define DRV_CRYPTO_AES_AddTweakData MPROTO(DRV_CRYPTO_AES_AddTweakData)

/**
 * @ingroup aes
 * @brief Function prototype for adding input data to AES.
 * 
 * This function is specific to the currently defined hardware version for adding input data to an AES operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_AddInputData)(void *contextData, void * data, uint32_t dataLength);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_AddInputData
 * @brief Macro to create a device-specific function prototype for adding input data to AES.
 */
#define DRV_CRYPTO_AES_AddInputData MPROTO(DRV_CRYPTO_AES_AddInputData)

/**
 * @ingroup aes
 * @brief Function prototype for adding output data to AES.
 * 
 * This function is specific to the currently defined hardware version for adding output data to an AES operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_AddOutputData)(void *contextData, void * data, uint32_t dataLength);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_AddOutputData
 * @brief Macro to create a device-specific function prototype for adding output data to AES.
 */
#define DRV_CRYPTO_AES_AddOutputData MPROTO(DRV_CRYPTO_AES_AddOutputData)

/**
 * @ingroup aes
 * @brief Function prototype for discarding data in AES.
 * 
 * This function is specific to the currently defined hardware version for discarding data from an AES operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_DiscardData)(void *contextData, uint32_t discardLength);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_DiscardData
 * @brief Macro to create a device-specific function prototype for discarding data in AES.
 */
#define DRV_CRYPTO_AES_DiscardData MPROTO(DRV_CRYPTO_AES_DiscardData)

/**
 * @ingroup aes
 * @brief Function prototype for adding LenALenC in AES-GCM.
 * 
 * This function is specific to the currently defined hardware version for adding LenALenC input to an AES-GCM operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_AddLenALenC)(void *contextData);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_AddLenALenC
 * @brief Macro to create a device-specific function prototype for adding LenALenC in AES-GCM.
 */
#define DRV_CRYPTO_AES_AddLenALenC MPROTO(DRV_CRYPTO_AES_AddLenALenC)

/**
 * @ingroup aes
 * @brief Function prototype for executing AES.
 * 
 * This function is specific to the currently defined hardware version for executing an AES operation.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_Execute)(void *contextData);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_Execute
 * @brief Macro to create a device-specific function prototype for executing AES.
 */
#define DRV_CRYPTO_AES_Execute MPROTO(DRV_CRYPTO_AES_Execute)

/**
 * @ingroup aes
 * @brief Function prototype for checking AES activity.
 * 
 * This function is specific to the currently defined hardware version for checking if an AES operation is active.
 * If the hardware version is not defined, this function will not be available.
 */
AES_ERROR MPROTO(DRV_CRYPTO_AES_IsActive)(void *contextData, AES_ERROR *active);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_IsActive
 * @brief Macro to create a device-specific function prototype for checking AES activity.
 */
#define DRV_CRYPTO_AES_IsActive MPROTO(DRV_CRYPTO_AES_IsActive)

/**
 * @ingroup aes
 * @brief Function prototype for getting AES context size.
 * 
 * This function is specific to the currently defined hardware version for retrieving the size of the AES context block.
 * If the hardware version is not defined, this function will not be available.
 */
uint32_t MPROTO(DRV_CRYPTO_AES_GetContextSize)(void *contextData);

/**
 * @ingroup aes
 * @def DRV_CRYPTO_AES_GetContextSize
 * @brief Macro to create a device-specific function prototype for getting AES context size.
 */
#define DRV_CRYPTO_AES_GetContextSize MPROTO(DRV_CRYPTO_AES_GetContextSize)
/** @endcond */

#ifdef	__cplusplus
}
#endif

#endif	/* CAM_AES_H */

