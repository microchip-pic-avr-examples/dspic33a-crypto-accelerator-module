/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_kas.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2026 Microchip Technology Inc., and its subsidiaries. All rights reserved.

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

#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_kas.h"
#include "crypto/drivers/wrapper/crypto_kas_cam06048_wrapper.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define CRYPTO_KAS_SESSION_MAX (1) 

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

crypto_Kas_Status_E Crypto_Kas_Ecdh_SharedSecret(crypto_HandlerType_E ecdhHandlerType_en, uint8_t *ptr_privKey, uint32_t privKeyLen, uint8_t *ptr_pubKey, uint32_t pubKeyLen,
                                                    uint8_t *ptr_sharedSecret, uint32_t sharedSecretLen, crypto_EccCurveType_E eccCurveType_en, uint32_t ecdhSessionId)
{
    crypto_Kas_Status_E ret_ecdhStat_en = CRYPTO_KAS_ERROR_ALGONOTSUPPTD;
    
    if( (ptr_pubKey == NULL) || (pubKeyLen == 0u) )
    {
        ret_ecdhStat_en = CRYPTO_KAS_ERROR_PUBKEY;
    }
    else if( (ptr_privKey == NULL) || (privKeyLen == 0u) || (privKeyLen > (uint32_t)CRYPTO_ECC_MAX_KEY_LENGTH) )
    {
         ret_ecdhStat_en = CRYPTO_KAS_ERROR_PRIVKEY;
    }
    else if( (eccCurveType_en <= CRYPTO_ECC_CURVE_INVALID) || (eccCurveType_en >= CRYPTO_ECC_CURVE_MAX) )
    {
         ret_ecdhStat_en = CRYPTO_KAS_ERROR_CURVE;
    }
    else if((ecdhSessionId == 0u) || (ecdhSessionId > (uint32_t)CRYPTO_KAS_SESSION_MAX) )
    {
        ret_ecdhStat_en = CRYPTO_KAS_ERROR_SID;
    }
    else
    {
        switch(ecdhHandlerType_en)
        {            
            case CRYPTO_HANDLER_HW_INTERNAL:
	              ret_ecdhStat_en = Crypto_Kas_Ecdh_Hw_SharedSecret(ptr_privKey, privKeyLen, ptr_pubKey, pubKeyLen, ptr_sharedSecret,
	                                                                    sharedSecretLen, eccCurveType_en);
	            break;
            default:
                ret_ecdhStat_en = CRYPTO_KAS_ERROR_HDLR;
                break;
        }
    }
    return ret_ecdhStat_en;
}
