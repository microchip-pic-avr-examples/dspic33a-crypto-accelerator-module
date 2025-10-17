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
    driver for the following families of Microchip microcontrollers:
    dsPIC33AK with Crypto Accelerator Module.
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

#define AES_BLOCK_SIZE          (16U)  // bytes
#define CMAC_BLOCK_SIZE         (16U)  // bytes

/**
 * @brief Defines AES error codes.
 **/
typedef enum AES_ERROR
{
    AES_NO_ERROR = 0,
    AES_CONTEXT_ERROR = 1,
    AES_INVALID_MODE_ERROR = 2,
    AES_KEY_SETUP_ERROR = 3,
    AES_INIT_VECTOR_SETUP_ERROR = 4,
    AES_COUNTER_MEASURE_SETUP_ERROR = 5,
    AES_INITIALIZE_ERROR = 6,
    AES_WRITE_ERROR = 7,
    AES_READ_ERROR = 8,
    AES_STATE_ERROR = 9,
    AES_EXECUTE_ERROR = 10,
    AES_OPERATION_IS_NOT_ACTIVE = 11,
    AES_OPERATION_IS_ACTIVE = 12
} AES_ERROR;

/**
 * @brief Defines allowed AES modes.
 **/
typedef enum AESCON_MODE
{
    MODE_ECB  = 0,   // Electronic Codebook (ECB)
    MODE_CTR  = 2,   // Counter (CTR)
    MODE_GCM  = 6,   // Galois Counter Mode (GCM)
    MODE_XTS  = 7,   // XEX Tweakable Block Cipher (XTS) with Ciphertext Stealing (XTS-CS)
    MODE_CMAC = 8,   // Cipher-based Message Authentication Code (CMAC)
} AESCON_MODE;

/**
 * @brief Defines allowed AES operations.
 **/
typedef enum AESCON_OPERATION
{
    OP_ENCRYPT = 0,
    OP_DECRYPT = 1,
} AESCON_OPERATION;

// *****************************************************************************
// *****************************************************************************
// Section: AES Common Interface
// *****************************************************************************
// *****************************************************************************

/**
 *  @brief The ISR processor for AES operations.
 **/
void DRV_CRYPTO_AES_IsrHelper(void);

/**
 *  @brief Initialize a new AES operation.
 *  @param contextData The context data for this operation.
 *  @param mode The AES_MODE mode to use.
 *  @param operation The AESCON_OPERATION operation to perform.
 *  @param key The AES key to use.
 *  @param keyLength The length of the AES key.
 *  @param initVector The initalization vector (IV) data to use.
 *  @param initVectorLength The length if the IV data.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_INITIALIZE_ERROR on failure.
 *  @note Reinitializing an AES operation in progress will void the previous operation.
 **/
AES_ERROR DRV_CRYPTO_AES_Initialize(void *contextData, AESCON_MODE mode, AESCON_OPERATION operation,
        void* key, uint32_t keyLength, void *initVector, uint32_t initVectorLength);

/**
 *  @brief Add header data (such as Additional Authentication Data) to an AES operation.
 *  @param contextData The context data for this operation.
 *  @param headerData The header data to add to the operation.
 *  @param HeaderLength The length of the header data.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_WRITE_ERROR on failure.
 *  @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 **/
AES_ERROR DRV_CRYPTO_AES_AddHeader(void *contextData, void * headerData, uint32_t headerLength);

/**
 *  @brief Initialize the tweak value for AES XTS support.
 *  @param contextData The context data for this operation.
 *  @param tweakData The tweak data to use.
 *  @param tweakLength The length of the tweak data.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_WRITE_ERROR on failure.
 *  @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 **/
AES_ERROR DRV_CRYPTO_AES_AddTweakData(void *contextData, const void *tweakData, uint32_t tweakLength);

/**
 *  @brief Add data to an AES operation.
 *  @param contextData The context data for this operation.
 *  @param data The data to add to the operation.
 *  @param dataLength The length of the data.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_WRITE_ERROR on failure.
 *  @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 **/
AES_ERROR DRV_CRYPTO_AES_AddInputData(void *contextData, void * data, uint32_t dataLength);

/**
 *  @brief Add a data output buffer to an AES operation.
 *  @param contextData The context data for this operation.
 *  @param data The data buffer to add to the operation.
 *  @param dataLength The length of the data buffer.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_WRITE_ERROR on failure.
 *  @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 **/
AES_ERROR DRV_CRYPTO_AES_AddOutputData(void *contextData, void * data, uint32_t dataLength);

/**
 *  @brief Tell the AES operation to discard a number of bytes from the output stream.
 *  @param contextData The context data for this operation.
 *  @param discardLength The number of bytes to discard from the output stream.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_WRITE_ERROR on failure.
 *  @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 **/
AES_ERROR DRV_CRYPTO_AES_DiscardData(void *contextData, uint32_t discardLength);

/**
 *  @brief Calculate the LenALenC input for an AES-GCM operation and add to the descriptor chain.
 *  @param contextData The context data for this operation.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_WRITE_ERROR on failure.
 *  @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 **/
AES_ERROR DRV_CRYPTO_AES_AddLenALenC(void *contextData);

/**
 *  @brief Execute an AES operation to generate the final result (such as a CMAC or a cipher).
 *  @param contextData The context data for this operation.
 *  @return AES_NO_ERROR on success, AES_STATE_ERROR/AES_EXECUTE_ERROR on failure.
 *  @note An AES operation must be in progress (initialized by DRV_CRYPTO_AES_Initialize()).
 **/
AES_ERROR DRV_CRYPTO_AES_Execute(void *contextData);

/**
 *  @brief Returns whether an AES operation is active.
 *  @param contextData The context data for this operation.
 *  @param active Pointer to value to contain the active/inactive status.
 *  @return AES_NO_ERROR on success, AES_CONTEXT_ERROR/AES_STATE_ERROR on failure.
 *  @note This sets the value pointed to by \p active to AES_OPERATION_IS_ACTIVE
 *        or AES_OPERATION_IS_NOT_ACTIVE when the call is successful.
 **/
AES_ERROR DRV_CRYPTO_AES_IsActive(void *contextData, AES_ERROR *active);

/**
 *  @brief Returns the size of an AES context block based on the AES mode in use.
 *  @param contextData The context data for this operation.
 *  @return Size of the AES context block in bytes.
 **/
uint32_t DRV_CRYPTO_AES_GetContextSize(void *contextData);

// *****************************************************************************
// *****************************************************************************
// Section: AES CAM Device Support
// *****************************************************************************
// *****************************************************************************

/** @cond INTERNAL **/
void MPROTO(DRV_CRYPTO_AES_IsrHelper)(void);
#define DRV_CRYPTO_AES_IsrHelper MPROTO(DRV_CRYPTO_AES_IsrHelper)

AES_ERROR MPROTO(DRV_CRYPTO_AES_Initialize)(void *contextData, AESCON_MODE mode, AESCON_OPERATION operation, 
        void* key, uint32_t keyLength, void *initVector, uint32_t initVectorLength);
#define DRV_CRYPTO_AES_Initialize MPROTO(DRV_CRYPTO_AES_Initialize)

AES_ERROR MPROTO(DRV_CRYPTO_AES_AddHeader)(void *contextData, void * headerData, uint32_t headerLength);
#define DRV_CRYPTO_AES_AddHeader MPROTO(DRV_CRYPTO_AES_AddHeader)

AES_ERROR MPROTO(DRV_CRYPTO_AES_AddTweakData)(void *contextData, const void *tweakData, uint32_t tweakLength);
#define DRV_CRYPTO_AES_AddTweakData MPROTO(DRV_CRYPTO_AES_AddTweakData)

AES_ERROR MPROTO(DRV_CRYPTO_AES_AddInputData)(void *contextData, void * data, uint32_t dataLength);
#define DRV_CRYPTO_AES_AddInputData MPROTO(DRV_CRYPTO_AES_AddInputData)

AES_ERROR MPROTO(DRV_CRYPTO_AES_AddOutputData)(void *contextData, void * data, uint32_t dataLength);
#define DRV_CRYPTO_AES_AddOutputData MPROTO(DRV_CRYPTO_AES_AddOutputData)

AES_ERROR MPROTO(DRV_CRYPTO_AES_DiscardData)(void *contextData, uint32_t discardLength);
#define DRV_CRYPTO_AES_DiscardData MPROTO(DRV_CRYPTO_AES_DiscardData)

AES_ERROR MPROTO(DRV_CRYPTO_AES_AddLenALenC)(void *contextData);
#define DRV_CRYPTO_AES_AddLenALenC MPROTO(DRV_CRYPTO_AES_AddLenALenC)

AES_ERROR MPROTO(DRV_CRYPTO_AES_Execute)(void *contextData);
#define DRV_CRYPTO_AES_Execute MPROTO(DRV_CRYPTO_AES_Execute)

AES_ERROR MPROTO(DRV_CRYPTO_AES_IsActive)(void *contextData, AES_ERROR *active);
#define DRV_CRYPTO_AES_IsActive MPROTO(DRV_CRYPTO_AES_IsActive)

uint32_t MPROTO(DRV_CRYPTO_AES_GetContextSize)(void *contextData);
#define DRV_CRYPTO_AES_GetContextSize MPROTO(DRV_CRYPTO_AES_GetContextSize)
/** @endcond **/

#ifdef	__cplusplus
}
#endif

#endif	/* CAM_AES_H */

