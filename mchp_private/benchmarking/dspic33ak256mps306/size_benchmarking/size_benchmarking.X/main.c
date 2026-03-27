/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This source file starts the application to demonstrate ECDSA sign and verify.
 *******************************************************************************/

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

/* cppcheck-suppress misra-c2012-21.6
* 
*  (Rule 21.6) Required: The standard library input/output functions shall not be used
* 
*  Reasoning: This demo uses printf to print out information to a terminal.
*/
#include <stdbool.h>
#include "mcc_generated_files/system/system.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_digsign.h"
#include "crypto/common_crypto/crypto_hash.h"
#include "crypto/common_crypto/crypto_kas.h"
#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_rng.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"

int main( void ) {
    
    SYSTEM_Initialize();
    
    //AES CMAC
    Crypto_Mac_AesCmac_Init(NULL, CRYPTO_HANDLER_HW_INTERNAL, NULL, NULL, NULL);
    Crypto_Mac_AesCmac_Cipher(NULL, NULL, NULL);
    Crypto_Mac_AesCmac_Final(NULL, NULL, NULL);
    
    //AES ECB
    Crypto_Sym_Aes_EncryptDirect(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_SYM_OPMODE_ECB, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Crypto_Sym_Aes_DecryptDirect(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_SYM_OPMODE_ECB, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    //AES CTR
    Crypto_Sym_Aes_EncryptDirect(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_SYM_OPMODE_CTR, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Crypto_Sym_Aes_DecryptDirect(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_SYM_OPMODE_CTR, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    //AES GCM
    Crypto_Aead_AesGcm_EncryptAuthDirect(CRYPTO_HANDLER_HW_INTERNAL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Crypto_Aead_AesGcm_DecryptAuthDirect(CRYPTO_HANDLER_HW_INTERNAL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    //AES XTS
    Crypto_Sym_Aes_EncryptDirect(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_SYM_OPMODE_XTS, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Crypto_Sym_Aes_DecryptDirect(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_SYM_OPMODE_XTS, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    //ECDH
    Crypto_Kas_Ecdh_SharedSecret(CRYPTO_HANDLER_HW_INTERNAL,NULL, NULL, NULL, NULL, NULL, NULL, CRYPTO_ECC_CURVE_P521, NULL);
    
    //ECDSA
    Crypto_DigiSign_Ecdsa_Sign(CRYPTO_HANDLER_HW_INTERNAL, NULL, NULL, NULL, NULL, NULL, NULL, CRYPTO_ECC_CURVE_P521, NULL);
    Crypto_DigiSign_Ecdsa_Verify(CRYPTO_HANDLER_HW_INTERNAL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, CRYPTO_ECC_CURVE_P521, NULL);
    
    //HASH
    Crypto_Hash_Sha_Digest(CRYPTO_HANDLER_HW_INTERNAL, NULL, NULL, NULL, CRYPTO_HASH_SHA2_512, NULL);
    Crypto_Hash_Shake_Digest(CRYPTO_HANDLER_HW_INTERNAL, CRYPTO_HASH_SHA3_SHAKE256, NULL, NULL, NULL, NULL, NULL);

    //TRNG
    Crypto_Rng_Generate(CRYPTO_HANDLER_HW_INTERNAL, NULL, NULL, NULL, NULL, NULL);
    
    while (1)
    {
    }
    
    return 0;
}