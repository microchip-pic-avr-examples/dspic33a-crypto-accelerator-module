
/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecdsa.c

  Summary:
    This source file handles the running of the ECDSA application.
*******************************************************************************/

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

/* cppcheck-suppress misra-c2012-21.6
* 
*  (Rule 21.6) Required: The standard library input/output functions shall not be used
* 
*  Reasoning: This demo uses printf to print out information to a terminal.
*/
#include <stdio.h>
#include <xc.h>
#include "app_ecdsa.h"
#include "crypto/common_crypto/crypto_digsign.h"
#include "crypto/drivers/wrapper/crypto_cam05346_wrapper.h"
#include "mcc_generated_files/system/clock.h"
#include "mcc_generated_files/system/pins.h"
#include "test_vectors/test_vector.h"
#include "app/app_util.h"

uint8_t signature[P521Length] = {0};
volatile bool signComplete = false;
volatile bool verifyComplete = false;

static void clearCompleteStates(void)
{
    signComplete = false;
    verifyComplete = false;
}

void signOperationComplete(void)
{
    signComplete = true;
}

void verifyOperationComplete(void)
{
    verifyComplete = true;
}

static void runBlockingHardwareTest(TEST_VECTOR* testVector)
{
    uint8_t signatureLength = signatureLengthGet(testVector->curveType);
    crypto_DigiSign_Status_E status = CRYPTO_DIGISIGN_SUCCESS;
    printTestHeader(testVector->curveInformation, "BLOCKING");
    printf(YELLOW"\r\n\r\n Start Signature Generation"RESET_COLOR);
    
    clearCompleteStates();
    ////Crypto Library ECDSA SIGN API***********************************************************************************************************************
    status = Crypto_DigiSign_Ecdsa_Sign(CRYPTO_HANDLER_HW_INTERNAL, testVector->hashData, testVector->hashLen, signature, signatureLength, testVector->privateKeyData, testVector->privateKeyLen, testVector->curveType, SESSION_ID);
 
    printHexArray(signaturePrint, signature, signatureLength);
    printStatus(status, signingPrint);
    printSplitter();
    printf(YELLOW"\r\n\r\n Start Verification"RESET_COLOR);  
    
    int8_t hashStatus = 0;   
    clearCompleteStates();          
    ////Crypto Library ECDSA VERIFY API*********************************************************************************************************************
    status = Crypto_DigiSign_Ecdsa_Verify(CRYPTO_HANDLER_HW_INTERNAL, testVector->hashData, testVector->hashLen, signature, signatureLength, testVector->publicKeyData, testVector->publicKeyLen, &hashStatus, testVector->curveType, SESSION_ID);
    
    printStatus(status, verifyingPrint);
}

static void runNonBlockingHardwareTest(TEST_VECTOR* testVector)
{
    uint8_t signatureLength = signatureLengthGet(testVector->curveType);
    crypto_DigiSign_Status_E status = CRYPTO_DIGISIGN_SUCCESS;
    printTestHeader(testVector->curveInformation, "NON-BLOCKING");
    
    printf(YELLOW"\r\n\r\n Start Signature Generation"RESET_COLOR);
    clearCompleteStates();  
    //Crypto Library ECDSA SIGN Non-Blocking API***********************************************************************************************************************
    status = Crypto_DigiSign_Ecdsa_Sign_Start(CRYPTO_HANDLER_HW_INTERNAL, testVector->hashData, testVector->hashLen, testVector->privateKeyData, testVector->privateKeyLen, testVector->curveType, SESSION_ID);
    if(status == CRYPTO_DIGISIGN_SUCCESS){
        while(!signComplete)
        {
        }
        status = Crypto_DigiSign_Ecdsa_Sign_GetResult(signature, signatureLength);
    }
    printHexArray(signaturePrint, signature, signatureLength);
    printStatus(status, signingPrint);
    printSplitter();
    
    printf(YELLOW"\r\n\r\n Start Verification"RESET_COLOR);  
    clearCompleteStates();
    ////Crypto Library ECDSA VERIFY Non-Blocking API*********************************************************************************************************************
    status = Crypto_DigiSign_Ecdsa_Verify_Start(CRYPTO_HANDLER_HW_INTERNAL, testVector->hashData, testVector->hashLen, signature, signatureLength, testVector->publicKeyData, testVector->publicKeyLen, testVector->curveType, SESSION_ID);
    if(status == CRYPTO_DIGISIGN_SUCCESS){
        while(!verifyComplete)
        {
        }
        status = Crypto_DigiSign_Ecdsa_Verify_GetResult();
    }
    printStatus(status, verifyingPrint);
}

extern TEST_VECTOR ecdsa_p192;
static void TEST_ECDSA_p192(void) 
{    
    runBlockingHardwareTest(&ecdsa_p192);
    runNonBlockingHardwareTest(&ecdsa_p192);
}

extern TEST_VECTOR ecdsa_p256;
static void TEST_ECDSA_p256(void) 
{    
    runBlockingHardwareTest(&ecdsa_p256);
    runNonBlockingHardwareTest(&ecdsa_p256);
}

extern TEST_VECTOR ecdsa_p384;
static void TEST_ECDSA_p384(void) 
{
    runBlockingHardwareTest(&ecdsa_p384);
    runNonBlockingHardwareTest(&ecdsa_p384);
}

extern TEST_VECTOR ecdsa_p521;
static void TEST_ECDSA_p521(void) 
{        
    runBlockingHardwareTest(&ecdsa_p521);
    runNonBlockingHardwareTest(&ecdsa_p521);
}

void test_ecdsa(void)
{
    CRYPTO3_SignOperationCompleteCallbackRegister(&signOperationComplete);
    CRYPTO3_VerifyOperationCompleteCallbackRegister(&verifyOperationComplete);
    printSplitter();
    (void)printf("\r\n Starting CRYPTO ECDSA Verify Sign program \r\n");
    TEST_ECDSA_p192();
    TEST_ECDSA_p256();
    TEST_ECDSA_p384();
    TEST_ECDSA_p521();
    printSplitter();
}