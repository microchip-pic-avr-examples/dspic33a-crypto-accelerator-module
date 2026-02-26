/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_mac_cam05346_wrapper.c

  Summary:
    Crypto Framework Library wrapper file for CAM hardware MAC operations.

  Description:
    This source file contains the wrapper interface to access the
    MAC algorithms in the AES hardware driver for
    Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2026, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
#include "crypto/drivers/library/cam_hash.h"

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initialize the CAM library's AES interrupt handlers.
 */
static void lDRV_CRYPTO_AES_InterruptSetup(void)
{
    (void)Crypto_Int_Hw_Register_Handler(CRYPTO1_INT, DRV_CRYPTO_AES_IsrHelper);
    (void)Crypto_Int_Hw_Enable(CRYPTO1_INT);
}

/**
 * @brief XOR data with the given input byte value to generate the output.
 * @param dataBuf The source data buffer.
 * @param value The value to XOR into the buffer.
 * @param length The length of the buffer.
 * @note Parameters are presumed to have been prevalidated.
 */
static void lCrypto_Mac_Hw_XorBuf(uint8_t *dataBuf, uint8_t value, int16_t length)
{
    register int16_t i = length;
    register uint8_t *data = dataBuf;

    while (i > 0)
    {
        *data = (*data ^ value);
        data++;
        i--;
    }
}

/**
 * @brief Fetch the HMAC block size for a given SHA algorithm type.
 * @param shaAlgorithm The SHA algorithm.
 * @param blockSize Pionter to variable to hold the block size.
 * @return CRYPTO_MAC_CIPHER_SUCCESS on success, CRYPTO_MAC_ERROR_HASHTYPE on failure.
 */
static crypto_Mac_Status_E lCrypto_Mac_Hw_GetHmacBlockSize(crypto_Hash_Algo_E shaAlgorithm, uint32_t *blockSize)
{
    crypto_Mac_Status_E status = CRYPTO_MAC_CIPHER_SUCCESS;

    switch (shaAlgorithm)
    {
        case CRYPTO_HASH_SHA1:
        case CRYPTO_HASH_SHA2_224:
        case CRYPTO_HASH_SHA2_256:
        {
            *blockSize = 64UL;
        }
        break;

        case CRYPTO_HASH_SHA2_384:
        case CRYPTO_HASH_SHA2_512:
        {
            *blockSize = 128UL;
        }
        break;

        default:
        {
            *blockSize = 0UL;
            status = CRYPTO_MAC_ERROR_HASHTYPE;
        }
        break;
    }

    return status;
}

// *****************************************************************************
// *****************************************************************************
// Section: MAC Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

crypto_Mac_Status_E Crypto_Sym_Hw_Cmac_Init(void *cmacInitCtx, uint8_t *key, uint32_t keyLen)
{
    /* MISRA C:2012 Rule 11.5 deviation:
    * Reason: Conversion from void* to the CMAC context defined by the
    *         CAM Hardware Driver pre-compiled library is required since
    *         the library does not have access to the upper context structures
    *         defined by the Crypto APIs.
    */
    /* cppcheck-suppress misra-c2012-11.5 */
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
    /* MISRA C:2012 Rule 11.5 deviation:
    * Reason: Conversion from void* to the CMAC context defined by the
    *         CAM Hardware Driver pre-compiled library is required since
    *         the library does not have access to the upper context structures
    *         defined by the Crypto APIs.
    */
    /* cppcheck-suppress misra-c2012-11.5 */
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
    /* MISRA C:2012 Rule 11.5 deviation:
    * Reason: Conversion from void* to the CMAC context defined by the
    *         CAM Hardware Driver pre-compiled library is required since
    *         the library does not have access to the upper context structures
    *         defined by the Crypto APIs.
    */
    /* cppcheck-suppress misra-c2012-11.5 */
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
    register uint8_t *cmacContext = cmacCtx.contextData;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;
    AES_ERROR aesStatus = AES_INITIALIZE_ERROR;

    // Context data must be cleared.
    (void)memset(cmacContext, 0, sizeof(cmacCtx.contextData));

    aesStatus = DRV_CRYPTO_AES_Initialize(cmacContext, MODE_CMAC, OP_ENCRYPT, ptr_key, keyLen, NULL, 0U);
    if(aesStatus == AES_NO_ERROR)
    {
        lDRV_CRYPTO_AES_InterruptSetup();
        aesStatus = DRV_CRYPTO_AES_AddInputData(cmacContext, ptr_inputData, dataLen);
    }

    if(aesStatus == AES_NO_ERROR)
    {
        if ((NULL != ptr_outMac) && (0UL != macLen))
        {
            aesStatus = DRV_CRYPTO_AES_AddOutputData(cmacContext, ptr_outMac, macLen);
            if(aesStatus == AES_NO_ERROR)
            {
                aesStatus = DRV_CRYPTO_AES_Execute(cmacContext);
            }
        }
    }

    if (aesStatus == AES_NO_ERROR)
    {
        status = CRYPTO_MAC_CIPHER_SUCCESS;
    }
    return status;
}

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Init(void *contextData, uint8_t *key, uint32_t keyLength, crypto_Hash_Algo_E shaAlgorithm)
{
    register CRYPTO_HMAC_HW_CONTEXT *hmacCtx = (CRYPTO_HMAC_HW_CONTEXT*) contextData;
    register CRYPTO_HASH_HW_CONTEXT *shaContext = &hmacCtx->shaContext;
    register uint8_t *keyData = hmacCtx->hmacKeyData;
    uint32_t hmacBlockSize;

    // Fetch the block size to use for this operation, based on the hash algorithm to use.
    crypto_Mac_Status_E status = lCrypto_Mac_Hw_GetHmacBlockSize(shaAlgorithm, &hmacBlockSize);
    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        // Hold onto the block size for the finalization step.
        hmacCtx->hmacBlockSize = hmacBlockSize;

        // Store the algorithm in the hash context.  This will be retained thoughout the HMAC operation.
        shaContext->algorithm = shaAlgorithm;

        /* Pre-clear the key (stored in the context) as it is more likely the key is not exactly the buffer size,
         * and it needs to be zero-padded. */
        (void)memset(keyData, 0, hmacBlockSize);

        // The input key (ipad_key) is formed by either copying or hashing the key data.
        if (keyLength <= hmacBlockSize)
        {
            // For keys up to a block size, the key is copied into the buffer.
            for (register uint8_t i = 0; i < keyLength; i++)
            {
                keyData[i] = key[i];
            }

            status = CRYPTO_MAC_CIPHER_SUCCESS;
        }
        else
        {
            /* For keys larger than a block size, hash the key.
             * Calling the hash driver directly avoids extra overhead of a static HASH context. */
            uint32_t digestLength = 0;
            HASHCON_MODE mode;

            crypto_Hash_Status_E hashStatus = Crypto_Hash_Hw_Sha_GetAlgorithm(shaContext->algorithm, &mode);
            if (hashStatus == CRYPTO_HASH_SUCCESS)
            {
                // Calling the digest function directly means the hash context block must be cleared.
                (void)memset(shaContext->contextData, 0, sizeof(shaContext->contextData));

                // Get the size of the digest so the key's digest can be calculated.
                hashStatus = Crypto_Hash_Hw_Sha_GetDigestLength(shaContext->algorithm, &digestLength);
            }

            if (hashStatus == CRYPTO_HASH_SUCCESS)
            {
                // Set up the hash interrupt handler before calling the digest function.
                (void)Crypto_Int_Hw_Register_Handler(CRYPTO1_INT, DRV_CRYPTO_HASH_IsrHelper);
                (void)Crypto_Int_Hw_Enable(CRYPTO1_INT);

                hashStatus = DRV_CRYPTO_HASH_Digest(shaContext->contextData, mode, key, keyLength, keyData, digestLength);
            }

            if (hashStatus == CRYPTO_HASH_SUCCESS)
            {
                status = CRYPTO_MAC_CIPHER_SUCCESS;
            }
        }
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        // Finally, the SHA context is initialized for hash data.  The init calls clears the context data.
        crypto_Hash_Status_E hashStatus = Crypto_Hash_Hw_Sha_Init(shaContext, shaContext->algorithm);
        if (hashStatus == CRYPTO_HASH_SUCCESS)
        {
            // The key data is XORd with 0x36 to generate the 'ipad_key'.
            lCrypto_Mac_Hw_XorBuf(keyData, 0x36U, hmacBlockSize);

            // Feed the ipad_key into the hash.
            hashStatus = Crypto_Hash_Hw_Sha_Update(shaContext, keyData, hmacBlockSize);
            if (hashStatus != CRYPTO_HASH_SUCCESS)
            {
                status = CRYPTO_MAC_ERROR_CIPFAIL;
            }
        }
        else
        {
            status = CRYPTO_MAC_ERROR_CIPFAIL;
        }
    }

    return status;
}

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Cipher(void *contextData, uint8_t *inputData, uint32_t dataLength)
{
    register CRYPTO_HMAC_HW_CONTEXT *hmacCtx = (CRYPTO_HMAC_HW_CONTEXT*) contextData;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;

    // Run the hash with the input data.
    crypto_Hash_Status_E hashStatus = Crypto_Hash_Hw_Sha_Update(&hmacCtx->shaContext, inputData, dataLength);
    if (hashStatus == CRYPTO_HASH_SUCCESS)
    {
        status = CRYPTO_MAC_CIPHER_SUCCESS;
    }

    return status;
}

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Final(void *contextData, uint8_t *outputMac)
{
    register CRYPTO_HMAC_HW_CONTEXT *hmacCtx = (CRYPTO_HMAC_HW_CONTEXT*) contextData;
    register CRYPTO_HASH_HW_CONTEXT *shaContext = &hmacCtx->shaContext;

    // Fetch the block size for use in finalizing the opad_key and HMAC.
    register uint32_t hmacBlockSize = hmacCtx->hmacBlockSize;

    uint8_t digest[CRYPTO_HASH_MAX_DIGEST_SIZE];
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;

    // Finish the current HASH operation.
    crypto_Hash_Status_E hashStatus = Crypto_Hash_Hw_Sha_Final(shaContext, digest);
    if (hashStatus == CRYPTO_HASH_SUCCESS)
    {
        /* Convert the input key (ipad_key) to the output key (opad_key).
         * This is done with a bit of XOR trickery to yield an output XOR value of 0x5c. */
        register uint8_t *keyData = hmacCtx->hmacKeyData;
        lCrypto_Mac_Hw_XorBuf(keyData, (0x36U ^ 0x5CU), hmacBlockSize);

        /* Initialize a new hash to run the opad_key with the digest.
         * Since two pieces of data are input, it cannot be a single digest call. */
        hashStatus = Crypto_Hash_Hw_Sha_Init(shaContext, shaContext->algorithm);
    }

    // Feed the opad_key into the hash.
    if (hashStatus == CRYPTO_HASH_SUCCESS)
    {
        hashStatus = Crypto_Hash_Hw_Sha_Update(shaContext, hmacCtx->hmacKeyData, hmacBlockSize);
    }

    // Feed the digest into the hash.
    if (hashStatus == CRYPTO_HASH_SUCCESS)
    {
        uint32_t digestLength;
        hashStatus = Crypto_Hash_Hw_Sha_GetDigestLength(shaContext->algorithm, &digestLength);

        if (hashStatus == CRYPTO_HASH_SUCCESS)
        {
            hashStatus = Crypto_Hash_Hw_Sha_Update(shaContext, digest, digestLength);
        }
    }

    // Finalize the hash.
    if (hashStatus == CRYPTO_HASH_SUCCESS)
    {
        hashStatus = Crypto_Hash_Hw_Sha_Final(shaContext, outputMac);
        if (hashStatus == CRYPTO_HASH_SUCCESS)
        {
            status = CRYPTO_MAC_CIPHER_SUCCESS;
        }
    }

    return status;
}

crypto_Mac_Status_E Crypto_Mac_Hw_Hmac_Direct(uint8_t *ptr_inputData, uint32_t dataLength,
                                              uint8_t *ptr_outMac,
                                              uint8_t *ptr_key, uint32_t keyLength, crypto_Hash_Algo_E shaAlgorithm)
{
    CRYPTO_HMAC_HW_CONTEXT contextData;
    register CRYPTO_HMAC_HW_CONTEXT *hmacCtx = &contextData;

    crypto_Mac_Status_E status = Crypto_Mac_Hw_Hmac_Init(hmacCtx, ptr_key, keyLength, shaAlgorithm);
    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hw_Hmac_Cipher(hmacCtx, ptr_inputData, dataLength);
    }

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
        status = Crypto_Mac_Hw_Hmac_Final(hmacCtx, ptr_outMac);
    }

    return status;
}
 
crypto_Mac_Status_E Crypto_Mac_Hw_AesGmac_Init(void *ptr_aesGmacCtx, uint8_t *ptr_key, uint32_t keySize)
{
    CRYPTO_GMAC_HW_CONTEXT *gmacCtx = (CRYPTO_GMAC_HW_CONTEXT*) ptr_aesGmacCtx;
    crypto_Mac_Status_E status = CRYPTO_MAC_CIPHER_SUCCESS;

    // Save the key and length, to be used during the cipher operation.
    (void)memcpy(gmacCtx->keyData, ptr_key, keySize);
    gmacCtx->keyLength = keySize;

    return status;
}

crypto_Mac_Status_E Crypto_Mac_Hw_AesGmac_Cipher(void *ptr_aesGmacCtx, uint8_t *ptr_initVect,
                                                 uint32_t initVectLen, uint8_t *ptr_aad, uint32_t aadLen,
                                                 uint8_t *ptr_outMac, uint32_t macLen)
{
    CRYPTO_GMAC_HW_CONTEXT *gmacCtx = (CRYPTO_GMAC_HW_CONTEXT*) ptr_aesGmacCtx;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;

    /* Initialization is the same as for GCM, but always set to decrypt.
     * Use the key data stored during the initi call. */
    crypto_Aead_Status_E aeadStatus = Crypto_Aead_Hw_AesGcm_Init(&gmacCtx->aeadContext, CRYPTO_CIOP_DECRYPT,
                                                                 gmacCtx->keyData, gmacCtx->keyLength, ptr_initVect, initVectLen);
    if (aeadStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        // Add the AAD data.  There is no encipherment data (this is an authentication only operation).
        aeadStatus = Crypto_Aead_Hw_AesGcm_AddAadData(&gmacCtx->aeadContext, ptr_aad, aadLen);
    }

    if (aeadStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        // Finalize the operation to generate the GMAC.
        aeadStatus = Crypto_Aead_Hw_AesGcm_Final(&gmacCtx->aeadContext, ptr_outMac, macLen);
    }

    if (aeadStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        status = CRYPTO_MAC_CIPHER_SUCCESS;
    }

    return status;
}

crypto_Mac_Status_E Crypto_Mac_Hw_AesGmac_Direct(uint8_t *ptr_initVect, uint32_t initVectLen,
                                                 uint8_t *ptr_outMac, uint32_t macLen, uint8_t *ptr_key,
                                                 uint32_t keyLen, uint8_t *ptr_aad, uint32_t aadLen)
{
    CRYPTO_AEAD_HW_CONTEXT aeadContext;
    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPFAIL;

    /* The AEAD context can be used directly.
     * Initialization is the same as for GCM, but always set to decrypt. */
    crypto_Aead_Status_E aeadStatus = Crypto_Aead_Hw_AesGcm_Init(&aeadContext, CRYPTO_CIOP_DECRYPT,
                                                                 ptr_key, keyLen, ptr_initVect, initVectLen);
    if (aeadStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        // Add the AAD data.  There is no encipherment data (this is an authentication only operation).
        aeadStatus = Crypto_Aead_Hw_AesGcm_AddAadData(&aeadContext, ptr_aad, aadLen);
    }

    if (aeadStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        // Finalize the operation to generate the GMAC.
        aeadStatus = Crypto_Aead_Hw_AesGcm_Final(&aeadContext, ptr_outMac, macLen);
    }

    if (aeadStatus == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
        status = CRYPTO_MAC_CIPHER_SUCCESS;
    }

    return status;
}
 