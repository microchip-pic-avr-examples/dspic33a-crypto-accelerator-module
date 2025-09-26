/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    cam_trng.h

  Summary:
    Crypto Framework Library interface file for CAM TRNG.

  Description:
    This header file contains the interface that make up the CAM TRNG hardware 
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

#ifndef CAM_TRNG_H
#define	CAM_TRNG_H

#ifdef	__cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "cam_version.h"
#include "cam_device.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief This enum represents the result of the TRNG generation.
 */
typedef enum TRNG_ERROR
{
    TRNG_NO_ERROR = 0,
    TRNG_FAILED_TO_GENERATE = 1,
    TRNG_INVALID_OPERATION = 3,
    TRNG_KEY_SETUP = 4,
    TRNG_EXEC_FIFO = 5,
    TRNG_EXEC_STARTUP = 6,
    TRNG_EXEC_PROP = 7,
    TRNG_EXEC_REP = 8,
    TRNG_EXEC_FAIL
} TRNG_ERROR;

// *****************************************************************************
// *****************************************************************************
// Section: CAM TRNG Function Definitions 
// *****************************************************************************
// *****************************************************************************

/*
 *  @brief Resets TRNG error bits whenever an interrupt occurs.
 *  @return none
 *  @note This function is called when one of the error bits is 1. This function resets the built-in error bits so that future errors can be caught.
*/
void DRV_CRYPTO_TRNG_IsrHelper(void);

/*
 *  @brief Creates and sets the key for True Random Number Generation.
 *  @return Returns TRNG_NO_ERROR on success. Returns one of the TRNG_FAILED_TO_GENERATE / TRNG_EXEC_FIFO / TRNG_EXEC_STARTUP 
 *          TRNG_EXEC_PROP / TRNG_EXEC_REP on fail. 
 *  @note This function performs a read of the FIFO to get the key and then stores it for use in the generation of the TRNG values.
*/
TRNG_ERROR DRV_CRYPTO_TRNG_Setup(void);

/*
 *  @brief Reads generated true random number and stores it in the provided buffer.
 *  @param data Buffer to store the TRNG data.
 *  @param size Number of 4-bit words to read and store.
 *  @return Returns TRNG_NO_ERROR on success. Returns one of the TRNG_FAILED_TO_GENERATE / TRNG_EXEC_FIFO / TRNG_EXEC_STARTUP 
 *          TRNG_EXEC_PROP / TRNG_EXEC_REP on fail. 
 *  @note This function reads the specified amount of 4-bit words from the designated read address.
*/
TRNG_ERROR DRV_CRYPTO_TRNG_ReadData(uint8_t* data, uint32_t size) ;

// *****************************************************************************
// *****************************************************************************
// Section: TRNG CAM Device Support
// *****************************************************************************
// *****************************************************************************

void MPROTO(DRV_CRYPTO_TRNG_IsrHelper)(void);
#define DRV_CRYPTO_TRNG_IsrHelper MPROTO(DRV_CRYPTO_TRNG_IsrHelper)

TRNG_ERROR MPROTO(DRV_CRYPTO_TRNG_Setup)(void);
#define DRV_CRYPTO_TRNG_Setup MPROTO(DRV_CRYPTO_TRNG_Setup)

TRNG_ERROR MPROTO(DRV_CRYPTO_TRNG_ReadData)(uint8_t* data, uint32_t size);
#define DRV_CRYPTO_TRNG_ReadData MPROTO(DRV_CRYPTO_TRNG_ReadData)

#ifdef	__cplusplus
}
#endif

#endif	/* CAM_TRNG_H */