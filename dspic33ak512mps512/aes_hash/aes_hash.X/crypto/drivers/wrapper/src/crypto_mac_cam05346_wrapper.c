/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_mac_cam05346_wrapper.c

  Summary:
    Crypto Framework Library wrapper file for CAM hardware AES MAC.

  Description:
    This source file contains the wrapper interface to access the
    AES MAC algorithms in the AES hardware driver for Microchip microcontrollers.
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
#include <string.h>
#include "crypto/drivers/wrapper/crypto_mac_cam05346_wrapper.h"
#include "crypto/drivers/wrapper/crypto_cam05346_wrapper.h"
#include "crypto/drivers/library/cam_aes.h"

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static void lDRV_CRYPTO_AES_InterruptSetup(void)
{
    (void)Crypto_Int_Hw_Register_Handler(CRYPTO1_INT, DRV_CRYPTO_AES_IsrHelper);
    (void)Crypto_Int_Hw_Enable(CRYPTO1_INT);
}

// *****************************************************************************
// *****************************************************************************
// Section: MAC Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Init(void *cmacInitCtx, uint8_t *key, uint32_t keyLen)
{
    CRYPTO_CMAC_HW_CONTEXT *cmacCtx = (CRYPTO_CMAC_HW_CONTEXT*) cmacInitCtx;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;
    AES_ERROR aesStatus = AES_INITIALIZE_ERROR;

    AESCON_MODE mode = MODE_CMAC;
    AESCON_OPERATION operation = OP_ENCRYPT;

    // Context data must be cleared as the context may be on a stack versus static memory.
    (void)memset(cmacCtx->contextData, 0, sizeof(cmacCtx->contextData));

    aesStatus = DRV_CRYPTO_AES_Initialize(cmacCtx->contextData, mode, operation, key, keyLen, NULL, 0U);

    if(aesStatus == AES_NO_ERROR)
    {
        lDRV_CRYPTO_AES_InterruptSetup();
        status = CRYPTO_MAC_CIPHER_SUCCESS;
    }
    else
    {
        status = CRYPTO_MAC_ERROR_CIPFAIL;
    }

    return status;
}

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Cipher(void *cmacCipherCtx, uint8_t *inputData, uint32_t dataLen)
{
    CRYPTO_CMAC_HW_CONTEXT *cmacCtx = (CRYPTO_CMAC_HW_CONTEXT*) cmacCipherCtx;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;
    AES_ERROR aesStatus;
    AES_ERROR aesActive;

    aesStatus = DRV_CRYPTO_AES_IsActive(cmacCtx->contextData, &aesActive);
    if ((aesStatus == AES_NO_ERROR) && (aesActive == AES_OPERATION_IS_ACTIVE))
    {
        aesStatus = DRV_CRYPTO_AES_AddInputData(cmacCtx->contextData, inputData, dataLen);
        if(aesStatus == AES_NO_ERROR)
        {
            status = CRYPTO_MAC_CIPHER_SUCCESS;
        }
    }

    return status;
}

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Final(void *cmacFinalCtx, uint8_t *outputMac, uint32_t macLen)
{
    CRYPTO_CMAC_HW_CONTEXT *cmacCtx = (CRYPTO_CMAC_HW_CONTEXT*) cmacFinalCtx;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;

    AES_ERROR aesStatus;
    AES_ERROR aesActive;

    aesStatus = DRV_CRYPTO_AES_IsActive(cmacCtx->contextData, &aesActive);
    if ((aesStatus == AES_NO_ERROR) && (aesActive == AES_OPERATION_IS_ACTIVE))
    {
        if ((NULL != outputMac) && (0UL != macLen))
        {
            aesStatus = DRV_CRYPTO_AES_AddOutputData(cmacCtx->contextData, outputMac, macLen);
            if(aesStatus == AES_NO_ERROR)
            {
                aesStatus = DRV_CRYPTO_AES_Execute(cmacCtx->contextData);
            }
        }
    }

    if(aesStatus == AES_NO_ERROR)
    {
        status = CRYPTO_MAC_CIPHER_SUCCESS;
    }

    return status;
}

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Direct(uint8_t *ptr_inputData, uint32_t dataLen, uint8_t *ptr_outMac, uint32_t macLen,
                                              uint8_t *ptr_key, uint32_t keyLen)
{
    CRYPTO_CMAC_HW_CONTEXT cmacCtx;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;

    status = Crypto_Sym_Hw_Cmac_Init(&cmacCtx, ptr_key, keyLen);

    if(status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Sym_Hw_Cmac_Cipher(&cmacCtx, ptr_inputData, dataLen);
    }

    if(status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Sym_Hw_Cmac_Final(&cmacCtx, ptr_outMac, macLen);
    }

    return status;
}
