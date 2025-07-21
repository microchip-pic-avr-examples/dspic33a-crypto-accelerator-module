/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_kas_cam05346_wrapper.c

  Summary:
    Crypto Framework Library wrapper file for the Shared Secret generation in the
    hardware cryptographic library.

  Description:
    This source file contains the wrapper interface to access the hardware
    cryptographic library in Microchip microcontrollers for Shared Secret generation.
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
#include "crypto/drivers/wrapper/crypto_kas_cam05346_wrapper.h"
#include "crypto/drivers/wrapper/crypto_cam05346_wrapper.h"
#include "crypto/drivers/library/cam_ecdh.h"

static void lDRV_CRYPTO_ECC_InterruptSetup(void)
{
    (void)Crypto_Int_Hw_Register_Handler(CRYPTO3_INT, DRV_CRYPTO_PKE_IsrHelper);
    (void)Crypto_Int_Hw_Enable(CRYPTO3_INT);
}

static CRYPTO_PKE_RESULT lCrypto_Kas_Ecdh_Hw_GetCurve(
    crypto_EccCurveType_E eccCurveType, PKE_ECC_CURVE *hwEccCurve)
{
    CRYPTO_PKE_RESULT eccStatus = CRYPTO_PKE_RESULT_SUCCESS;

    switch (eccCurveType)
    {
        case CRYPTO_ECC_CURVE_P192:
            *hwEccCurve = P192;
            break;

        case CRYPTO_ECC_CURVE_P256:
            *hwEccCurve = P256;
            break;

        case CRYPTO_ECC_CURVE_P384:
            *hwEccCurve = P384;
            break;

        case CRYPTO_ECC_CURVE_P521:
            *hwEccCurve = P521;
            break;

        default:
            eccStatus = CRYPTO_PKE_RESULT_ERROR_CURVE;
            break;
    }

    return eccStatus;
}

static crypto_Kas_Status_E lCrypto_Kas_Ecdh_Hw_MapResult(CRYPTO_PKE_RESULT result)
{
    crypto_Kas_Status_E kasStatus;

    switch (result)
    {
        case CRYPTO_PKE_RESULT_SUCCESS:
            kasStatus = CRYPTO_KAS_SUCCESS;
            break;

        case CRYPTO_PKE_ERROR_PUBKEYCOMPRESS:
            kasStatus = CRYPTO_KAS_ERROR_PUBKEY;
            break;

        case CRYPTO_PKE_RESULT_ERROR_CURVE:
            kasStatus = CRYPTO_KAS_ERROR_CURVE;
            break;

        case CRYPTO_PKE_RESULT_INIT_FAIL:
        case CRYPTO_PKE_RESULT_ERROR_FAIL:
            kasStatus = CRYPTO_KAS_ERROR_FAIL;
            break;
        default:
            kasStatus = CRYPTO_KAS_ERROR_FAIL;
            break;
    }

    return kasStatus;
}

crypto_Kas_Status_E Crypto_Kas_Ecdh_Hw_SharedSecret(uint8_t *privKey,
    uint32_t privKeyLen, uint8_t *pubKey, uint32_t pubKeyLen,
    uint8_t *secret, uint32_t secretLen, crypto_EccCurveType_E eccCurveType_en)
{
    CRYPTO_PKE_RESULT hwResult;
    PKE_CONFIG eccData;
    PKE_ECC_CURVE hwEccCurve;

    /* Get curve */
    hwResult = lCrypto_Kas_Ecdh_Hw_GetCurve(eccCurveType_en, &hwEccCurve);
    if (hwResult == CRYPTO_PKE_RESULT_SUCCESS)
    {
        hwResult = DRV_CRYPTO_ECDH_InitEccParams(&eccData, privKey, privKeyLen,
                                             pubKey, pubKeyLen, hwEccCurve);
    }

    lDRV_CRYPTO_ECC_InterruptSetup();

    if (hwResult == CRYPTO_PKE_RESULT_SUCCESS)
    {
        /* Get shared key */
        hwResult = DRV_CRYPTO_ECDH_GetSharedSecret(&eccData, secret, secretLen);
    }
    return lCrypto_Kas_Ecdh_Hw_MapResult(hwResult);
}