/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_hash.c

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

#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_hash.h"
#include "crypto/drivers/wrapper/crypto_hash_shake_cam06048_wrapper.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define CRYPTO_HASH_SESSION_MAX (1)

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************


//SHAKE
crypto_Hash_Status_E Crypto_Hash_Shake_Digest(crypto_HandlerType_E shakeHandlerType_en, crypto_Hash_Algo_E shakeAlgorithm_en,
                                                    uint8_t *ptr_data, uint32_t dataLen, uint8_t *ptr_digest, uint32_t digestLen, uint32_t shakeSessionId)
{
    crypto_Hash_Status_E ret_shakeStat_en = CRYPTO_HASH_ERROR_NOTSUPPTED;

    if( (shakeAlgorithm_en <= CRYPTO_HASH_INVALID) || (shakeAlgorithm_en >= CRYPTO_HASH_MAX))
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_ALGO;
    }
    else if( (ptr_data == NULL) && (dataLen != 0u) )
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_INPUTDATA;
    }
    else if( (ptr_digest == NULL) || (digestLen == 0u) )
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_OUTPUTDATA;
    }
    else if( (shakeSessionId == 0u) || (shakeSessionId > (uint32_t)CRYPTO_HASH_SESSION_MAX) )
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_SID;
    }
    else
    {
        switch(shakeHandlerType_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
                ret_shakeStat_en = Crypto_Hash_Hw_Shake_Digest(ptr_data, dataLen, ptr_digest, digestLen, shakeAlgorithm_en);
                break;
            default:
                ret_shakeStat_en = CRYPTO_HASH_ERROR_HDLR;
                break;
        }
    }
	return ret_shakeStat_en;
}

crypto_Hash_Status_E Crypto_Hash_Shake_Init(st_Crypto_Hash_Shake_Ctx* ptr_shakeCtx_st, crypto_Hash_Algo_E shakeAlgorithm_en,
                                                crypto_HandlerType_E shakeHandlerType_en, uint32_t digestLen, uint32_t shakeSessionId)
{
  	crypto_Hash_Status_E ret_shakeStat_en = CRYPTO_HASH_ERROR_NOTSUPPTED;

    if(ptr_shakeCtx_st == NULL)
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_CTX;
    }
    if( (shakeAlgorithm_en <= CRYPTO_HASH_INVALID) || (shakeAlgorithm_en >= CRYPTO_HASH_MAX))
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_ALGO;
    }
    else if(digestLen == 0u)
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_ARG;
    }
    else if( (shakeSessionId == 0u) || (shakeSessionId > (uint32_t)CRYPTO_HASH_SESSION_MAX) )
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_SID;
    }
    else
    {
        ptr_shakeCtx_st->shakeSessionId = shakeSessionId;
        ptr_shakeCtx_st->shakeAlgo_en = shakeAlgorithm_en;
        ptr_shakeCtx_st->shakeHandler_en = shakeHandlerType_en;
        ptr_shakeCtx_st->digestLen = digestLen;

        switch(ptr_shakeCtx_st->shakeHandler_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
                ret_shakeStat_en = Crypto_Hash_Hw_Shake_Init((void*)ptr_shakeCtx_st->arr_shakeDataCtx, ptr_shakeCtx_st->shakeAlgo_en, ptr_shakeCtx_st->digestLen);
                break;
            default:
                ret_shakeStat_en = CRYPTO_HASH_ERROR_HDLR;
                break;
        }
    }
	return ret_shakeStat_en;
}

crypto_Hash_Status_E Crypto_Hash_Shake_Update(st_Crypto_Hash_Shake_Ctx* ptr_shakeCtx_st, uint8_t *ptr_data, uint32_t dataLen)
{
  	crypto_Hash_Status_E ret_shakeStat_en = CRYPTO_HASH_ERROR_NOTSUPPTED;

    if(ptr_shakeCtx_st == NULL)
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_CTX;
    }
    else if( (ptr_data == NULL) && (dataLen != 0u) )
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_INPUTDATA;
    }
	else
    {
        switch(ptr_shakeCtx_st->shakeHandler_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
                ret_shakeStat_en = Crypto_Hash_Hw_Shake_Update((void*)ptr_shakeCtx_st->arr_shakeDataCtx, ptr_data, dataLen);
                break;
            default:
                ret_shakeStat_en = CRYPTO_HASH_ERROR_HDLR;
                break;
        }
    }
	return ret_shakeStat_en;
}

crypto_Hash_Status_E Crypto_Hash_Shake_Final(st_Crypto_Hash_Shake_Ctx* ptr_shakeCtx_st, uint8_t *ptr_digest)
{
  	crypto_Hash_Status_E ret_shakeStat_en = CRYPTO_HASH_ERROR_NOTSUPPTED;

    if(ptr_shakeCtx_st == NULL)
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_CTX;
    }
    else if(ptr_digest == NULL)
    {
        ret_shakeStat_en = CRYPTO_HASH_ERROR_OUTPUTDATA;
    }
    else
    {
        switch(ptr_shakeCtx_st->shakeHandler_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
                ret_shakeStat_en = Crypto_Hash_Hw_Shake_Final((void*)ptr_shakeCtx_st->arr_shakeDataCtx, ptr_digest);
                break;
            default:
                ret_shakeStat_en = CRYPTO_HASH_ERROR_HDLR;
                break;
        }
    }
	return ret_shakeStat_en;
}

