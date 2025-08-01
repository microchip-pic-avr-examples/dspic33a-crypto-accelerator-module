/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_rng_cam05346_wrapper.c

  Summary:
    Crypto Framework Library wrapper file for hardware TRNG.

  Description:
    This source file contains the wrapper interface to access the TRNG
    hardware driver for Microchip microcontrollers.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <xc.h>
#include "crypto/drivers/wrapper/crypto_rng_cam05346_wrapper.h"
#include "crypto/drivers/wrapper/crypto_cam05346_wrapper.h"
#include "crypto/drivers/library/cam_trng.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

static void lDRV_CRYPTO_TRNG_InterruptSetup(void)
{
    (void)Crypto_Int_Hw_Register_Handler(CRYPTO2_INT, DRV_CRYPTO_TRNG_IsrHelper);
    (void)Crypto_Int_Hw_Enable(CRYPTO2_INT);
}

// *****************************************************************************
// *****************************************************************************
// Section: TRNG Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

crypto_Rng_Status_E Crypto_Rng_Hw_Trng_Generate(uint8_t *rngData, uint32_t rngLen)
{

    (void) DRV_CRYPTO_TRNG_Setup();
    lDRV_CRYPTO_TRNG_InterruptSetup();
    (void) DRV_CRYPTO_TRNG_ReadData(rngData, rngLen);

    return CRYPTO_RNG_SUCCESS;
}
