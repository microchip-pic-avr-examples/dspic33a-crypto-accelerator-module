/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    crypto_mac_cipher.c

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

#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/drivers/wrapper/crypto_mac_cam05346_wrapper.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define CRYPTO_MAC_SESSION_MAX (1)

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

crypto_Mac_Status_E Crypto_Mac_AesCmac_Init(st_Crypto_Mac_Aes_ctx *ptr_aesCmacCtx_st, crypto_HandlerType_E handlerType_en,
                                              uint8_t *ptr_key, uint32_t keyLen, uint32_t sessionID)
{
    crypto_Mac_Status_E ret_aesCmacStat_en = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;

    if(ptr_aesCmacCtx_st == NULL)
    {
        ret_aesCmacStat_en = CRYPTO_MAC_ERROR_CTX;
    }
    else if( (ptr_key == NULL) || (keyLen < (uint32_t)CRYPTO_AESKEYSIZE_128) || (keyLen > (uint32_t)CRYPTO_AESKEYSIZE_256)  )
    {
       ret_aesCmacStat_en =  CRYPTO_MAC_ERROR_KEY;
    }
    else if( (sessionID == 0U) || (sessionID > (uint32_t)CRYPTO_MAC_SESSION_MAX) )
    {
       ret_aesCmacStat_en =  CRYPTO_MAC_ERROR_SID;
    }
    else
    {
        ptr_aesCmacCtx_st->cryptoSessionID =  sessionID;
        ptr_aesCmacCtx_st->macHandlerType_en = handlerType_en;
        ptr_aesCmacCtx_st->ptr_key = ptr_key;
        ptr_aesCmacCtx_st->mackeyLen = keyLen;

        switch(ptr_aesCmacCtx_st->macHandlerType_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
            	ret_aesCmacStat_en = Crypto_Sym_Hw_Cmac_Init((void*)ptr_aesCmacCtx_st->arr_macDataCtx, ptr_aesCmacCtx_st->ptr_key, keyLen);
                break;
            default:
                ret_aesCmacStat_en = CRYPTO_MAC_ERROR_HDLR;
                break;
        }

    }
    return ret_aesCmacStat_en;
}

crypto_Mac_Status_E Crypto_Mac_AesCmac_Cipher(st_Crypto_Mac_Aes_ctx *ptr_aesCmacCtx_st, uint8_t *ptr_inputData, uint32_t dataLen)
{
    crypto_Mac_Status_E ret_aesCmacStat_en = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;

    if(ptr_aesCmacCtx_st == NULL)
    {
        ret_aesCmacStat_en = CRYPTO_MAC_ERROR_CTX;
    }
    else if( (ptr_inputData == NULL) && (dataLen != 0UL) )
    {
        ret_aesCmacStat_en = CRYPTO_MAC_ERROR_INPUTDATA;
    }
    else
    {
        switch(ptr_aesCmacCtx_st->macHandlerType_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
                ret_aesCmacStat_en = Crypto_Sym_Hw_Cmac_Cipher((void*)ptr_aesCmacCtx_st->arr_macDataCtx, ptr_inputData, dataLen);
                break;
            default:
                ret_aesCmacStat_en = CRYPTO_MAC_ERROR_HDLR;
				break;
        }
    }
    return ret_aesCmacStat_en;
}

crypto_Mac_Status_E Crypto_Mac_AesCmac_Final(st_Crypto_Mac_Aes_ctx *ptr_aesCmacCtx_st, uint8_t *ptr_outMac, uint32_t macLen)
{
    crypto_Mac_Status_E ret_aesCmacStat_en = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    if(ptr_aesCmacCtx_st == NULL)
    {
        ret_aesCmacStat_en = CRYPTO_MAC_ERROR_CTX;
    }
    else if((ptr_outMac == NULL) || (macLen == 0U))
    {
        ret_aesCmacStat_en = CRYPTO_MAC_ERROR_MACDATA;
    }
    else
    {
        switch(ptr_aesCmacCtx_st->macHandlerType_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
                ret_aesCmacStat_en = Crypto_Sym_Hw_Cmac_Final((void*)ptr_aesCmacCtx_st->arr_macDataCtx, ptr_outMac, macLen);
                break;
            default:
                ret_aesCmacStat_en = CRYPTO_MAC_ERROR_HDLR;
				break;
        }
    }
    return ret_aesCmacStat_en;
}

crypto_Mac_Status_E Crypto_Mac_AesCmac_Direct(crypto_HandlerType_E macHandlerType_en, uint8_t *ptr_inputData, uint32_t dataLen,
                                                uint8_t *ptr_outMac, uint32_t macLen, uint8_t *ptr_key, uint32_t keyLen, uint32_t sessionID)
{
    crypto_Mac_Status_E ret_aesCmacStat_en = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    if( (ptr_inputData == NULL) && (dataLen != 0UL) )
    {
        ret_aesCmacStat_en = CRYPTO_MAC_ERROR_INPUTDATA;
    }
    else if( (ptr_outMac == NULL) || (macLen == 0UL) )
    {
        ret_aesCmacStat_en = CRYPTO_MAC_ERROR_MACDATA;
    }
    else if( (ptr_key == NULL) || (keyLen < (uint32_t)CRYPTO_AESKEYSIZE_128) || (keyLen > (uint32_t)CRYPTO_AESKEYSIZE_256)  )
    {
       ret_aesCmacStat_en =  CRYPTO_MAC_ERROR_KEY;
    }
    else if( (sessionID == 0U) || (sessionID > (uint32_t)CRYPTO_MAC_SESSION_MAX) )
    {
       ret_aesCmacStat_en =  CRYPTO_MAC_ERROR_SID;
    }
    else
    {
        switch(macHandlerType_en)
        {
            case CRYPTO_HANDLER_HW_INTERNAL:
                ret_aesCmacStat_en = Crypto_Sym_Hw_Cmac_Direct(ptr_inputData, dataLen, ptr_outMac, macLen, ptr_key, keyLen);
                break;
            default:
                ret_aesCmacStat_en = CRYPTO_MAC_ERROR_HDLR;
				break;
        }
    }
    return ret_aesCmacStat_en;
}

// *****************************************************************************
