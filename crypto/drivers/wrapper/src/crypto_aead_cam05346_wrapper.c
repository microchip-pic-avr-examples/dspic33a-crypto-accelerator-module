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
* Copyright (C) 2026 Microchip Technology Inc. and its subsidiaries.
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
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

#define AAD_PRESENT_FLAG    (1U << 6)   // Indicates that AAD is present in the data set.
#define AES_CCM_HEADER_SIZE (22UL)      // The size of the AES CCM data header.

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initialize the CAM library's AES interrupt handler.
 */
static void lCrypto_Aead_Hw_Aes_InterruptSetup(void)
{
    (void)Crypto_Int_Hw_Register_Handler(CRYPTO1_INT, DRV_CRYPTO_AES_IsrHelper);
    (void)Crypto_Int_Hw_Enable(CRYPTO1_INT);
}

/**
 * @brief Determine the AES operation type to pass to the CAM library.
 * @param cipherOpType Crypto operation type.
 * @param operation Pointer to a value to hold the equivalent CAM library operation type.
 * @return CRYPTO_AEAD_CIPHER_SUCCESS on success, CRYPTO_AEAD_ERROR_CIPOPER on failure.
 */
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

/**
 * @brief Calculate the number of pad bytes for an AES block.
 * @param dataLen Size of the data.
 * @return Number of pad bytes to complete the AES block.
 */
static uint32_t lCrypto_Aead_Hw_Aes_GetPadBytes(uint32_t dataLen)
{
    uint32_t mask = (AES_BLOCK_SIZE - 1UL);
    uint32_t pad = ((dataLen + mask) & ~mask) - dataLen;

    return pad;
}

/**
 * @brief Direct byte-compare function.
 * @param cmp1 Pointer to first byte stream.
 * @param cmp2 Pointer to second byte stream.
 * @param cmpLen Number of bytes to compare.
 * @return 0 on success, 1 on failure.
 */
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


static void lCrypto_Aead_Hw_BuildCcmHeader(uint8_t *header, uint32_t *headerLen,
    uint8_t *nonce, uint32_t nonceLen,
    uint32_t aadLen, uint64_t dataLen, uint32_t authTagLen)
{
    // The format of this header is defined in RFC 1310, paragraph 2.2.
    uint8_t *localHeader = header;
    uint8_t *localNonce = nonce;
    uint8_t flags = 0U;
    uint8_t tagSize = ((uint8_t)authTagLen - 2U) / 2U;
    uint8_t lengthSize = (0x0FU - (uint8_t)nonceLen);

    // Encode the auth tag length and nonce length.
    flags |= (tagSize & 0x07U) << 3U;
    flags |= (lengthSize - 1U) & 0x7U;

    // If there is AAD data, set the flag at bit 6.
    if (aadLen > 0UL)
    {
        flags |= AAD_PRESENT_FLAG;
    }

    // Add the flags as the first byte.
    *localHeader = flags; localHeader++;

    // Add the nonce starting at the second byte.
    for (uint8_t i = 0U; i < nonceLen; i++)
    {
        *localHeader = *localNonce;
        localHeader++;
        localNonce++;
    }

    // Add the data length.  dataLen is 64 bits since this size can be up to 7 bytes.
    for (int8_t i = ((lengthSize - 1U) * 8U); i >= 0; i -= 8)
    {
        uint64_t m = (0xFFULL << (uint64_t)i);
        uint64_t b = (dataLen & m);

        *localHeader = (b >> (uint64_t)i); localHeader++;
    }

    // Encode additional authentication data length if present.
    localHeader = &header[16];
    if (aadLen > 0U)
    {
        if (aadLen < 0xFF00UL)
        {
            *localHeader = (aadLen & 0x0000FF00UL) >> 8; localHeader++;
            *localHeader = (aadLen & 0x000000FFUL); localHeader++;
        }
        else
        {
            *localHeader = 0xFF; localHeader++;
            *localHeader = 0xFE; localHeader++;
            *localHeader = (aadLen & 0xFF000000UL) >> 24; localHeader++;
            *localHeader = (aadLen & 0x00FF0000UL) >> 16; localHeader++;
            *localHeader = (aadLen & 0x0000FF00UL) >> 8; localHeader++;
            *localHeader = (aadLen & 0x000000FFUL); localHeader++;
        }
    }

    *headerLen = (localHeader - header);
}


/**
 * @brief Common AES-GCM direct-cipher function.
 * @param mode The cipher mode.
 * @param operation The operation type.
 * @param inputData Pointer to input data.
 * @param dataLen Length of the input/output data.
 * @param outData Pointer to a buffer to hold the output data.
 * @param key Pointer to the key.
 * @param keyLen Length of the key.
 * @param initVect Pointer to the initialization vector data.
 * @param initVectLen Length of the initialization vector.
 * @param aad Pointer to the additional authentication data.
 * @param aadLen Length of the AAD.
 * @param authTag Pointer to a buffer to hold the authentication tag.
 * @param authTagLen Length of the authentication tag.
 * @return CRYPTO_AEAD_CIPHER_SUCCESS on success, other on failure.
 */
static crypto_Aead_Status_E lCrypto_Aead_Hw_AesGcm_Direct(AESCON_MODE mode, AESCON_OPERATION operation,
    uint8_t *inputData, uint32_t dataLen, uint8_t *outData, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen)
{
    CRYPTO_AEAD_HW_CONTEXT aeadCtx;
    register uint8_t *aesContext = aeadCtx.contextData;
    AES_ERROR aesStatus;
    crypto_Aead_Status_E result = CRYPTO_AEAD_ERROR_CIPFAIL;
    // Context data must be cleared.
    (void)memset(aesContext, 0, sizeof(aeadCtx.contextData));
    aesStatus = DRV_CRYPTO_AES_Initialize(aesContext, mode, operation, key, keyLen, initVect, initVectLen);
    if (aesStatus == AES_NO_ERROR)
    {
        lCrypto_Aead_Hw_Aes_InterruptSetup();
        if (aadLen > 0UL)
        {
            aesStatus = DRV_CRYPTO_AES_AddHeader(aesContext, aad, aadLen);
            if(aesStatus == AES_NO_ERROR)
            {
                /* AES-GCM hardware includes the authentication data in its output.  This data needs to be
                 * discarded from the output stream.  The data is padded to a block size. */
                uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(aadLen);
                aesStatus = DRV_CRYPTO_AES_DiscardData(aesContext, (aadLen + pad));
            }
        }
    }
    if(aesStatus == AES_NO_ERROR)
    {
        /* AES GCM cipher/decipher accepts the actual number of bytes, and the library will
         * automatically pad to a block size and configure the descriptor to ignore the pad bytes.*/
        aesStatus = DRV_CRYPTO_AES_AddInputData(aesContext, inputData, dataLen);
        if(aesStatus == AES_NO_ERROR)
        {
            aesStatus = DRV_CRYPTO_AES_AddOutputData(aesContext, outData, dataLen);
        }
        if((aesStatus == AES_NO_ERROR) && (dataLen > 0UL))
        {
            /* AES-GCM hardware operates on block size boundaries.  When data size is not aligned to
             * a block size boundary, the excess must be discarded from the output stream.
             * When data is not specified, this is skipped. */
            uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(dataLen);
            aesStatus = DRV_CRYPTO_AES_DiscardData(aesContext, pad);
        }
    }
    if(aesStatus == AES_NO_ERROR)
    {
        aesStatus = DRV_CRYPTO_AES_AddOutputData(aesContext, authTag, authTagLen);
        if(aesStatus == AES_NO_ERROR)
        {
            aesStatus = DRV_CRYPTO_AES_AddLenALenC(aesContext);
        }
        if(aesStatus == AES_NO_ERROR)
        {
            aesStatus = DRV_CRYPTO_AES_Execute(aesContext);
        }
    }
    if (aesStatus == AES_NO_ERROR)
    {
        result = CRYPTO_AEAD_CIPHER_SUCCESS;
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
    /* MISRA C:2012 Rule 11.5 deviation:
    * Reason: Conversion from void* to the AEAD context defined by the
    *         CAM Hardware Driver pre-compiled library is required since
    *         the library does not have access to the upper context structures
    *         defined by the Crypto APIs.
    */
    /* cppcheck-suppress misra-c2012-11.5 */
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
    /* MISRA C:2012 Rule 11.5 deviation:
    * Reason: Conversion from void* to the AEAD context defined by the
    *         CAM Hardware Driver pre-compiled library is required since
    *         the library does not have access to the upper context structures
    *         defined by the Crypto APIs.
    */
    /* cppcheck-suppress misra-c2012-11.5 */
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
    /* MISRA C:2012 Rule 11.5 deviation:
    * Reason: Conversion from void* to the AEAD context defined by the
    *         CAM Hardware Driver pre-compiled library is required since
    *         the library does not have access to the upper context structures
    *         defined by the Crypto APIs.
    */
    /* cppcheck-suppress misra-c2012-11.5 */
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
    /* MISRA C:2012 Rule 11.5 deviation:
    * Reason: Conversion from void* to the AEAD context defined by the
    *         CAM Hardware Driver pre-compiled library is required since
    *         the library does not have access to the upper context structures
    *         defined by the Crypto APIs.
    */
    /* cppcheck-suppress misra-c2012-11.5 */
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
    crypto_Aead_Status_E result = lCrypto_Aead_Hw_AesGcm_Direct(MODE_GCM, OP_ENCRYPT,
                                                                inputData, dataLen, outData,
                                                                key, keyLen, initVect, initVectLen,
                                                                aad, aadLen, authTag, authTagLen);

    return result;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesGcm_DecryptAuthDirect(uint8_t *inputData,
    uint32_t dataLen, uint8_t *outData, uint8_t *key, uint32_t keyLen,
    uint8_t *initVect, uint32_t initVectLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen)
{
    uint8_t generatedAuthTag[AES_GCM_AUTHTAG_SIZE];
    crypto_Aead_Status_E result = lCrypto_Aead_Hw_AesGcm_Direct(MODE_GCM, OP_DECRYPT,
                                                                inputData, dataLen, outData,
                                                                key, keyLen, initVect, initVectLen,
                                                                aad, aadLen, generatedAuthTag, authTagLen);
    if (result == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        // The tag must be verified against what was calculated.
        if (0UL != lCrypto_Aead_Hw_CompareAsBytes(generatedAuthTag, authTag, authTagLen))
        {
            result = CRYPTO_AEAD_ERROR_AUTHFAIL;
        }
    }

    return result;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesCcm_Init(void *aeadInitCtx,
    uint8_t *key, uint32_t keyLen)
{
    CRYPTO_AEAD_HW_CONTEXT *aeadCtx = (CRYPTO_AEAD_HW_CONTEXT*) aeadInitCtx;
    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    AES_ERROR aesStatus = AES_INITIALIZE_ERROR;

    AESCON_MODE mode = MODE_CCM;

    // Context data must be cleared as the context may be on a stack versus static memory.
    (void)memset(aeadCtx->contextData, 0, sizeof(aeadCtx->contextData));

    /* CCM does not use an initialization vector, instead using a nonce provided during the call to cipher.
     * The 'operation' is dummied as OP_ENCRYPT as it will be overwritten by the 'DRV_Crypto_AES_SetOperation'
     * call in the cipher step. */
    aesStatus = DRV_CRYPTO_AES_Initialize(aeadCtx, mode, OP_ENCRYPT, key, keyLen, NULL, 0UL);
    if(aesStatus == AES_NO_ERROR)
    {
        lCrypto_Aead_Hw_Aes_InterruptSetup();
        status = CRYPTO_AEAD_CIPHER_SUCCESS;
    }

    return status;
}

crypto_Aead_Status_E Crypto_Aead_Hw_AesCcm_Cipher(void *aeadCipherCtx,
    crypto_CipherOper_E cipherOper_en,
    uint8_t *inputData, uint32_t dataLen, uint8_t *outData,
    uint8_t *nonce, uint32_t nonceLen, uint8_t *aad, uint32_t aadLen,
    uint8_t *authTag, uint32_t authTagLen)
{
    CRYPTO_AEAD_HW_CONTEXT *aeadCtx = (CRYPTO_AEAD_HW_CONTEXT*) aeadCipherCtx;

    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    AESCON_OPERATION operation;

    if(lCrypto_Aead_Hw_Aes_GetOperation(cipherOper_en, &operation) == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        AES_ERROR aesStatus;
        AES_ERROR aesActive;

        aesStatus = DRV_CRYPTO_AES_IsActive(aeadCtx->contextData, &aesActive);
        if ((aesStatus == AES_NO_ERROR) && (aesActive == AES_OPERATION_IS_ACTIVE))
        {
            uint8_t generatedAuthTag[AES_CCM_AUTHTAG_SIZE];

            aesStatus = DRV_CRYPTO_AES_SetOperation(aeadCtx->contextData, operation);
            if(aesStatus == AES_NO_ERROR)
            {
                uint8_t headerData[AES_CCM_HEADER_SIZE];
                uint32_t headerLen;

                // Build the CCM header.
                lCrypto_Aead_Hw_BuildCcmHeader(headerData, &headerLen, nonce, nonceLen, aadLen, dataLen, authTagLen);

                // The CCM header data is inserted without alignment or padding.
                aesStatus = DRV_CRYPTO_AES_AddRawHeader(aeadCtx->contextData, headerData, headerLen, AES_HEADER_DO_NOT_ALIGN);
                if (aesStatus == AES_NO_ERROR)
                {
                    if (aadLen > 0UL)
                    {
                        // AAD is inserted without padding, but with alignment.
                        aesStatus = DRV_CRYPTO_AES_AddRawHeader(aeadCtx->contextData, aad, aadLen, AES_HEADER_ALIGN);
                    }

                    if (aesStatus == AES_NO_ERROR)
                    {
                        /* The header + AAD is "the header" (even though it is in two pieces), and has a size.
                         * This may not align to a block size, so the hardware will pad automatically.
                         * The pad must be marked as 'ignore'. */
                        uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(headerLen + aadLen);
                        aesStatus = DRV_CRYPTO_AES_IgnoreData(aeadCtx->contextData, pad);
                    }

                    if (aesStatus == AES_NO_ERROR)
                    {
                        /* AES hardware includes (header + authentication) data in its output.  This data needs to be
                         * discarded from the output stream.  The data to discard is padded to a block size. */
                        uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(headerLen + aadLen);

                        aesStatus = DRV_CRYPTO_AES_DiscardData(aeadCtx->contextData, (headerLen + aadLen + pad));
                    }
                }
            }

            // Add the input plaintext.
            if (aesStatus == AES_NO_ERROR)
            {
                /* AES hardware operates on block size boundaries.  When input data size is not aligned to
                 * a block size, the input must be padded to a block size and the pad 'ignored'.  The
                 * driver call performs this padding. */
                aesStatus = DRV_CRYPTO_AES_AddInputData(aeadCtx->contextData, inputData, dataLen);
            }

            // Add the output ciphertext buffer.
            if (aesStatus == AES_NO_ERROR)
            {
                aesStatus = DRV_CRYPTO_AES_AddOutputData(aeadCtx->contextData, outData, dataLen);
            }

            if ((aesStatus == AES_NO_ERROR) && (dataLen > 0UL))
            {
                /* AES hardware operates on block size boundaries.  When output data size is not aligned to
                 * a block size boundary, the excess must be discarded from the output stream.
                 * When data is not specified, this is skipped. */
                uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(dataLen);
                aesStatus = DRV_CRYPTO_AES_DiscardData(aeadCtx->contextData, pad);
            }

            if (aesStatus == AES_NO_ERROR)
            {
                // Add the authtag output buffer.
                uint8_t *tagPtr = (operation == OP_ENCRYPT) ? authTag : generatedAuthTag;
                aesStatus = DRV_CRYPTO_AES_AddOutputData(aeadCtx->contextData, tagPtr, authTagLen);

                if(aesStatus == AES_NO_ERROR)
                {
                    /* AES hardware operates on block size boundaries.  When output data size is not aligned to
                     * a block size boundary, the excess must be discarded from the output stream. */
                    uint32_t pad = lCrypto_Aead_Hw_Aes_GetPadBytes(authTagLen);
                    aesStatus = DRV_CRYPTO_AES_DiscardData(aeadCtx->contextData, pad);
                }
            }

            /* On decryption, add the authtag as an input.  The tag is needed as part of the decryption data.
             * It will also be generated as an output for comparison. */
            if ((aesStatus == AES_NO_ERROR) && (operation == OP_DECRYPT))
            {
                /* AES hardware operates on block size boundaries.  When input data size is not aligned to
                 * a block size, the input must be padded to a block size and the pad 'ignored'.  The
                 * driver call performs this padding. */
                aesStatus = DRV_CRYPTO_AES_AddInputData(aeadCtx->contextData, authTag, authTagLen);
            }

            if (aesStatus == AES_NO_ERROR)
            {
                aesStatus = DRV_CRYPTO_AES_Execute(aeadCtx->contextData);
            }

            if (aesStatus == AES_NO_ERROR)
            {
                // On decryption, the tag must be verified against what was calculated.
                if (operation == OP_DECRYPT)
                {
                    if (0UL != lCrypto_Aead_Hw_CompareAsBytes(generatedAuthTag, authTag, authTagLen))
                    {
                        status = CRYPTO_AEAD_CIPHER_SUCCESS;
                    }
                }
                else
                {
                    status = CRYPTO_AEAD_CIPHER_SUCCESS;
                }
            }
        }
    }

    return status;
}
