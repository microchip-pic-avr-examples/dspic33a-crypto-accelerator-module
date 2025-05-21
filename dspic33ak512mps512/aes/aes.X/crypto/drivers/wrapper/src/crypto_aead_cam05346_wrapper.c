/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_aead_cam05346_wrapper.c

  Summary:
    Crypto Framework Library wrapper file for CAM hardware AES.

  Description:
    This source file contains the wrapper interface to access the AEAD
    algorithms in the CAM AES hardware driver for Microchip microcontrollers.
**************************************************************************/

/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <string.h>
#include "crypto/drivers/wrapper/crypto_aead_cam05346_wrapper.h"
#include "crypto/drivers/wrapper/crypto_cam05346_wrapper.h"
#include "crypto/drivers/library/cam_aes.h"

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static void lCrypto_Aead_Hw_Aes_InterruptSetup(void)
{
    (void)Crypto_Int_Hw_Register_Handler(CRYPTO1_INT, DRV_CRYPTO_AES_IsrHelper);
    (void)Crypto_Int_Hw_Enable(CRYPTO1_INT);
}

static crypto_Aead_Status_E lCrypto_Aead_Hw_Aes_GetOperation
    (crypto_CipherOper_E cipherOpType, AESCON_OPERATION* operation)
{
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPOPER;

    switch (cipherOpType)
    {
        case CRYPTO_CIOP_ENCRYPT:
            *operation = OP_ENCRYPT;
            status = CRYPTO_AEAD_CIPHER_SUCCESS;
            break;
        case CRYPTO_CIOP_DECRYPT:
            *operation = OP_DECRYPT;
            status = CRYPTO_AEAD_CIPHER_SUCCESS;
            break;
        default:
            status = CRYPTO_AEAD_ERROR_CIPOPER;
            break;
    }

    return status;
}

static uint32_t lCrypto_Aead_Hw_Aes_GetPadBytes(uint32_t dataLen)
{
    uint32_t mask = (AES_BLOCK_SIZE - 1UL);
    uint32_t pad = ((dataLen + mask) & ~mask) - dataLen;

    return pad;
}

static uint32_t lCrypto_Aead_Hw_CompareAsBytes(uint8_t *cmp1, uint8_t *cmp2, uint32_t cmpLen)
{
    register uint8_t* c1 = cmp1;
    register uint8_t* c2 = cmp2;
    register int32_t len = (int32_t)cmpLen;
    uint32_t result = 0UL;

    while (len > 0L)
    {
        if (*c1 != *c2)
        {
            result = 1UL;
            break;
        }
        else
        {
            len--;
            c1++;
            c2++;
        }
    }

    return result;
}

// *****************************************************************************
// *****************************************************************************
// Section: AEAD Algorithms Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_Init(void *aeadInitCtx,
    crypto_CipherOper_E cipherOper_en, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen)
{
    CRYPTO_AEAD_HW_CONTEXT *aeadCtx = (CRYPTO_AEAD_HW_CONTEXT*) aeadInitCtx;
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    AES_ERROR aesStatus = AES_INITIALIZE_ERROR;

    AESCON_MODE mode = MODE_GCM;
    AESCON_OPERATION operation;

    status = lCrypto_Aead_Hw_Aes_GetOperation(cipherOper_en, &operation);

    if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        // Context data must be cleared as the context may be on a stack versus static memory.
        (void)memset(aeadCtx->contextData, 0, sizeof(aeadCtx->contextData));

        aesStatus = DRV_CRYPTO_AES_Initialize(aeadCtx, mode, operation, key, keyLen, initVect, initVectLen);
    }

    if(aesStatus == AES_NO_ERROR)
    {
        lCrypto_Aead_Hw_Aes_InterruptSetup();
    }
    else
    {
        status = CRYPTO_AEAD_ERROR_CIPFAIL;
    }

    return status;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_AddAadData(void *aeadCipherCtx,
    uint8_t *aad, uint32_t aadLen)
{
    CRYPTO_AEAD_HW_CONTEXT *aeadCtx = (CRYPTO_AEAD_HW_CONTEXT*) aeadCipherCtx;
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    AES_ERROR aesStatus;
    AES_ERROR aesActive;

    aesStatus = DRV_CRYPTO_AES_IsActive(aeadCtx->contextData, &aesActive);
    if ((aesStatus == AES_NO_ERROR) && (aesActive == AES_OPERATION_IS_ACTIVE))
    {
        if (aadLen > 0UL)
        {
            aesStatus = DRV_CRYPTO_AES_AddHeader(aeadCtx->contextData, aad, aadLen);
            if(aesStatus == AES_NO_ERROR)
            {
                /* AES-GCM hardware includes the authentication data in its output.  This data needs to be
                 * discarded from the output stream.  The data is padded to a block size. */
                uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(aadLen);

                aesStatus = DRV_CRYPTO_AES_DiscardData(aeadCtx->contextData, (aadLen + pad));
            }

            if(aesStatus == AES_NO_ERROR)
            {
                status = CRYPTO_AEAD_CIPHER_SUCCESS;
            }
        }
        else
        {
            // Empty AAD data is allowed.
            status = CRYPTO_AEAD_CIPHER_SUCCESS;
        }
}

    return status;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_Cipher(void *aeadCipherCtx,
    uint8_t *inputData, uint32_t dataLen, uint8_t *outData)
{
    CRYPTO_AEAD_HW_CONTEXT *aeadCtx = (CRYPTO_AEAD_HW_CONTEXT*) aeadCipherCtx;
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    AES_ERROR aesStatus;
    AES_ERROR aesActive;

    aesStatus = DRV_CRYPTO_AES_IsActive(aeadCtx->contextData, &aesActive);
    if ((aesStatus == AES_NO_ERROR) && (aesActive == AES_OPERATION_IS_ACTIVE))
    {
        /* AES GCM cipher/decipher accepts the actual number of bytes, and the library will
         * automatically pad to a block size and configure the descriptor to ignore the pad bytes.*/
        aesStatus = DRV_CRYPTO_AES_AddInputData(aeadCtx->contextData, inputData, dataLen);
        if(aesStatus == AES_NO_ERROR)
        {
            aesStatus = DRV_CRYPTO_AES_AddOutputData(aeadCtx->contextData, outData, dataLen);
        }

        if((aesStatus == AES_NO_ERROR) && (dataLen > 0UL))
        {
            /* AES-GCM hardware operates on block size boundaries.  When data size is not aligned to
             * a block size boundary, the excess must be discarded from the output stream.
             * When data is not specified, this is skipped. */
            uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(dataLen);

            aesStatus = DRV_CRYPTO_AES_DiscardData(aeadCtx->contextData, pad);
        }

        if(aesStatus == AES_NO_ERROR)
        {
            status = CRYPTO_AEAD_CIPHER_SUCCESS;
        }
    }

    return status;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_Final(void *aeadFinalCtx,
    uint8_t *authTag, uint32_t authTagLen)
{
    CRYPTO_AEAD_HW_CONTEXT *aeadCtx = (CRYPTO_AEAD_HW_CONTEXT*) aeadFinalCtx;
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    AES_ERROR aesStatus;
    AES_ERROR aesActive;

    aesStatus = DRV_CRYPTO_AES_IsActive(aeadCtx->contextData, &aesActive);
    if ((aesStatus == AES_NO_ERROR) && (aesActive == AES_OPERATION_IS_ACTIVE))
    {
        aesStatus = DRV_CRYPTO_AES_AddOutputData(aeadCtx->contextData, authTag, authTagLen);
        if(aesStatus == AES_NO_ERROR)
        {
            aesStatus = DRV_CRYPTO_AES_AddLenALenC(aeadCtx->contextData);
        }

        if(aesStatus == AES_NO_ERROR)
        {
            aesStatus = DRV_CRYPTO_AES_Execute(aeadCtx->contextData);
        }

        if(aesStatus == AES_NO_ERROR)
        {
            status = CRYPTO_AEAD_CIPHER_SUCCESS;
        }
    }

    return status;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_EncryptAuthDirect(uint8_t *inputData,
    uint32_t dataLen, uint8_t *outData, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen)
{
    CRYPTO_AEAD_HW_CONTEXT aeadCtx;
    crypto_Aead_Status_E result;

    result = Crypto_Aead_Hw_AesGcm_Init(&aeadCtx, CRYPTO_CIOP_ENCRYPT, key, keyLen, initVect, initVectLen);
    if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        result = Crypto_Aead_Hw_AesGcm_AddAadData(&aeadCtx, aad, aadLen);
    }

    if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        result = Crypto_Aead_Hw_AesGcm_Cipher(&aeadCtx, inputData, dataLen, outData);
    }

    if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        result = Crypto_Aead_Hw_AesGcm_Final(&aeadCtx, authTag, authTagLen);
    }

    return result;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_DecryptAuthDirect(uint8_t *inputData,
    uint32_t dataLen, uint8_t *outData, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen)
{
    CRYPTO_AEAD_HW_CONTEXT aeadCtx;
    crypto_Aead_Status_E result;

    result = Crypto_Aead_Hw_AesGcm_Init(&aeadCtx, CRYPTO_CIOP_DECRYPT, key, keyLen, initVect, initVectLen);
    if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        result = Crypto_Aead_Hw_AesGcm_AddAadData(&aeadCtx, aad, aadLen);
    }

    if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        result = Crypto_Aead_Hw_AesGcm_Cipher(&aeadCtx, inputData, dataLen, outData);
    }

    if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        uint8_t generatedAuthTag[AES_GCM_AUTHTAG_SIZE];

        result = Crypto_Aead_Hw_AesGcm_Final(&aeadCtx, generatedAuthTag, authTagLen);

        if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
        {
            // The tag must be verified against what was calculated.
            if (0UL != lCrypto_Aead_Hw_CompareAsBytes(generatedAuthTag, authTag, authTagLen))
            {
                result = CRYPTO_AEAD_ERROR_AUTHFAIL;
            }
        }
    }

    return result;
}
