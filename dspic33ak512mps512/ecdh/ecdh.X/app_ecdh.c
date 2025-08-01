
/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecdh.c

  Summary:
    This source file handles the running of the ECDH examples.
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
#include "crypto/common_crypto/crypto_kas.h"
#include "app_ecdh.h"

#define RESET_COLOR     "\033""[0m"
#define RED             "\033""[1;31m"
#define GREEN           "\033""[1;32m"
#define YELLOW          "\033""[1;33m"
#define BLUE            "\033""[1;34m"
#define MAG             "\033""[1;35m"
#define CYAN            "\033""[1;36m"
#define SESSION_ID      0x1U

static void printSplitter(void)
{
    (void)printf("\r\n\n ______________________________________________________________________________________________________ \r\n");
}

static void printTestHeader(const char* label)
{
    (void)printf(MAG"\r\n\r\n ************************************************************************************");
    (void)printf(CYAN"\r\n\t\t ******************** %s ********************", label);
    (void)printf(MAG"\r\n ************************************************************************************"RESET_COLOR);
}

static void printHexArray(const char* label, void* Data, uint32_t size) 
{
    (void)printf(BLUE"\r\n --------------------------------------------------------------------------------------------------"RESET_COLOR);
    uint8_t *ldata = (uint8_t *) Data;
    (void)printf("\r\n %s", label);
    for (uint32_t i = 0; i < size; i++) 
    {
        if (0U == i % 16U && 0U != i) 
        {
            (void)printf("\r\n\t\t");
        }
        
        (void)printf(" 0x%02X", ldata[i]);
    }
}

static void checkArrayEquality(const uint8_t *a, const uint8_t *b, size_t size)
{
	uint8_t result = 0;

    if (size > 0U)
    {
        const uint8_t *tempa = a;
        const uint8_t *tempb = b;

        for (unsigned int i = 0; i < size; i++)
        {
            result |= tempa[i] ^ tempb[i];
        }

        if(result == 0U)
        {
            (void) printf(GREEN" \r\n\r\n PASS - Generated X point matches expected output"RESET_COLOR);
        }
        else
        {
            (void) printf(RED" \r\n\r\n FAIL - Data Mismatch"RESET_COLOR);
        }
    }
}


static inline void runHardwareTest(uint8_t *publicKey, uint8_t *privateKey,
        uint8_t *sharedSecret, uint32_t operationLength,
        uint8_t *expectedOutput, uint32_t expectedOutputLength, 
        crypto_EccCurveType_E eccCurveType)
{
    uint32_t length = operationLength * 2U;
    
    crypto_Kas_Status_E error = Crypto_Kas_Ecdh_SharedSecret(CRYPTO_HANDLER_HW_INTERNAL, 
            privateKey, operationLength, publicKey, operationLength * 2U,
            sharedSecret, length, eccCurveType, SESSION_ID);
    
    printHexArray("Shared Secret :", sharedSecret, operationLength * 2U);
    
    if(error == CRYPTO_KAS_SUCCESS)
    {
        checkArrayEquality(expectedOutput, sharedSecret, expectedOutputLength);
    }
    else
    {
        (void)printf(RED"\r\nERROR in driver was thrown\r\n"RESET_COLOR);
    }

}

static void TEST_ECDH_SHARED_SECRET_p192(void)
{
    printTestHeader("P-192 ECDH");
    ////NIST Defined Private Key********************************************************************************************
    uint8_t testPrivateKey[24] = {
        0xf1, 0x7d, 0x3f, 0xea, 0x36, 0x7b, 0x74, 0xd3,
        0x40, 0x85, 0x1c, 0xa4, 0x27, 0x0d, 0xcb, 0x24,
        0xc2, 0x71, 0xf4, 0x45, 0xbe, 0xd9, 0xd5, 0x27
    };

    ////NIST Defined Public Key**********************************************************************************************
    uint8_t testPublicKey[48] = {
        0x42, 0xEA, 0x6D, 0xD9, 0x96, 0x9D, 0xD2, 0xA6,
        0x1F, 0xEA, 0x1A, 0xAC, 0x7F, 0x8E, 0x98, 0xED,
        0xCC, 0x89, 0x6C, 0x6E, 0x55, 0x85, 0x7C, 0xC0,
        0xDF, 0xBE, 0x5D, 0x7C, 0x61, 0xFA, 0xC8, 0x8B,
        0x11, 0x81, 0x1B, 0xDE, 0x32, 0x8E, 0x8A, 0x0D,
        0x12, 0xBF, 0x01, 0xA9, 0xD2, 0x04, 0xB5, 0x23
    };
    
    ////NIST Defined Shared Secret**********************************************************************************************
    uint8_t expectedOutput[24] = {
        0x80, 0x3D, 0x8A, 0xB2, 0xE5, 0xB6, 0xE6, 0xFC,
        0xA7, 0x15, 0x73, 0x7C, 0x3A, 0x82, 0xF7, 0xCE,
        0x3C, 0x78, 0x31, 0x24, 0xF6, 0xD5, 0x1C, 0xD0
    };
    
    uint8_t sharedSecret[48];
    
    runHardwareTest(testPublicKey, testPrivateKey, sharedSecret, 
            sizeof(testPrivateKey), expectedOutput, sizeof(expectedOutput),
            CRYPTO_ECC_CURVE_P192);
}

static void TEST_ECDH_SHARED_SECRET_p256(void)
{
    printTestHeader("P-256 ECDH");
    
    ////NIST Defined Private Key********************************************************************************************
    uint8_t testPrivateKey[32] = {
        0x7d, 0x7d, 0xc5, 0xf7, 0x1e, 0xb2, 0x9d, 0xda,
        0xf8, 0x0d, 0x62, 0x14, 0x63, 0x2e, 0xea, 0xe0,
        0x3d, 0x90, 0x58, 0xaf, 0x1f, 0xb6, 0xd2, 0x2e,
        0xd8, 0x0b, 0xad, 0xb6, 0x2b, 0xc1, 0xa5, 0x34
    };

    ////NIST Defined Public Key**********************************************************************************************
    uint8_t testPublicKey[64] = {
        0x70, 0x0c, 0x48, 0xf7, 0x7f, 0x56, 0x58, 0x4c,
        0x5c, 0xc6, 0x32, 0xca, 0x65, 0x64, 0x0d, 0xb9,
        0x1b, 0x6b, 0xac, 0xce, 0x3a, 0x4d, 0xf6, 0xb4,
        0x2c, 0xe7, 0xcc, 0x83, 0x88, 0x33, 0xd2, 0x87,
        0xdb, 0x71, 0xe5, 0x09, 0xe3, 0xfd, 0x9b, 0x06,
        0x0d, 0xdb, 0x20, 0xba, 0x5c, 0x51, 0xdc, 0xc5,
        0x94, 0x8d, 0x46, 0xfb, 0xf6, 0x40, 0xdf, 0xe0,
        0x44, 0x17, 0x82, 0xca, 0xb8, 0x5f, 0xa4, 0xac
    };

    ////NIST Defined Shared Secret**********************************************************************************************
    uint8_t expectedOutput[32] = {
        0x46, 0xfc, 0x62, 0x10, 0x64, 0x20, 0xff, 0x01,
        0x2e, 0x54, 0xa4, 0x34, 0xfb, 0xdd, 0x2d, 0x25,
        0xcc, 0xc5, 0x85, 0x20, 0x60, 0x56, 0x1e, 0x68,
        0x04, 0x0d, 0xd7, 0x77, 0x89, 0x97, 0xbd, 0x7b
    };
    
    uint8_t sharedSecret[64];
    
    runHardwareTest(testPublicKey, testPrivateKey, sharedSecret, 
            sizeof(testPrivateKey), expectedOutput, sizeof(expectedOutput),
            CRYPTO_ECC_CURVE_P256);

}

static void TEST_ECDH_SHARED_SECRET_p384(void)
{
    printTestHeader("P-384 ECDH");
    
    ////NIST Defined Private Key********************************************************************************************
    uint8_t testPrivateKey[48]  = {
        0x3c, 0xc3, 0x12, 0x2a, 0x68, 0xf0, 0xd9, 0x50, 
        0x27, 0xad, 0x38, 0xc0, 0x67, 0x91, 0x6b, 0xa0,
        0xeb, 0x8c, 0x38, 0x89, 0x4d, 0x22, 0xe1, 0xb1, 
        0x56, 0x18, 0xb6, 0x81, 0x8a, 0x66, 0x17, 0x74,
        0xad, 0x46, 0x3b, 0x20, 0x5d, 0xa8, 0x8c, 0xf6, 
        0x99, 0xab, 0x4d, 0x43, 0xc9, 0xcf, 0x98, 0xa1
    };
    ////NIST Defined Public Key**********************************************************************************************
    uint8_t testPublicKey[96] = {
        0xa7, 0xc7, 0x6b, 0x97, 0x0c, 0x3b, 0x5f, 0xe8, 
        0xb0, 0x5d, 0x28, 0x38, 0xae, 0x04, 0xab, 0x47,
        0x69, 0x7b, 0x9e, 0xaf, 0x52, 0xe7, 0x64, 0x59, 
        0x2e, 0xfd, 0xa2, 0x7f, 0xe7, 0x51, 0x32, 0x72,
        0x73, 0x44, 0x66, 0xb4, 0x00, 0x09, 0x1a, 0xdb, 
        0xf2, 0xd6, 0x8c, 0x58, 0xe0, 0xc5, 0x00, 0x66,
        0xac, 0x68, 0xf1, 0x9f, 0x2e, 0x1c, 0xb8, 0x79, 
        0xae, 0xd4, 0x3a, 0x99, 0x69, 0xb9, 0x1a, 0x08,
        0x39, 0xc4, 0xc3, 0x8a, 0x49, 0x74, 0x9b, 0x66, 
        0x1e, 0xfe, 0xdf, 0x24, 0x34, 0x51, 0x91, 0x5e,
        0xd0, 0x90, 0x5a, 0x32, 0xb0, 0x60, 0x99, 0x2b, 
        0x46, 0x8c, 0x64, 0x76, 0x6f, 0xc8, 0x43, 0x7a
    };
    
    ////NIST Defined Shared Secret**********************************************************************************************
    uint8_t expectedOutput[48] = {
        0x5f, 0x9d, 0x29, 0xdc, 0x5e, 0x31, 0xa1, 0x63, 
        0x06, 0x03, 0x56, 0x21, 0x36, 0x69, 0xc8, 0xce,
        0x13, 0x2e, 0x22, 0xf5, 0x7c, 0x9a, 0x04, 0xf4, 
        0x0b, 0xa7, 0xfc, 0xea, 0xd4, 0x93, 0xb4, 0x57,
        0xe5, 0x62, 0x1e, 0x76, 0x6c, 0x40, 0xa2, 0xe3, 
        0xd4, 0xd6, 0xa0, 0x4b, 0x25, 0xe5, 0x33, 0xf1
    };
    
    uint8_t sharedSecret[96];
    
    runHardwareTest(testPublicKey, testPrivateKey, sharedSecret, 
            sizeof(testPrivateKey), expectedOutput, sizeof(expectedOutput),
            CRYPTO_ECC_CURVE_P384);
}

static void TEST_ECDH_SHARED_SECRET_p521(void)
{
    printTestHeader("P-521 ECDH");
    
    ////NIST Defined Private Key********************************************************************************************
    uint8_t testPrivateKey[66] = {
        0x01, 0x7e, 0xec, 0xc0, 0x7a, 0xb4, 0xb3, 0x29,
        0x06, 0x8f, 0xba, 0x65, 0xe5, 0x6a, 0x1f, 0x88,
        0x90, 0xaa, 0x93, 0x5e, 0x57, 0x13, 0x4a, 0xe0, 
        0xff, 0xcc, 0xe8, 0x02, 0x73, 0x51, 0x51, 0xf4, 
        0xea, 0xc6, 0x56, 0x4f, 0x6e, 0xe9, 0x97, 0x4c, 
        0x5e, 0x68, 0x87, 0xa1, 0xfe, 0xfe, 0xe5, 0x74,
        0x3a, 0xe2, 0x24, 0x1b, 0xfe, 0xb9, 0x5d, 0x5c,
        0xe3, 0x1d, 0xdc, 0xb6, 0xf9, 0xed, 0xb4, 0xd6, 
        0xfc, 0x47
    };

    ////NIST Defined Public Key**********************************************************************************************
    uint8_t testPublicKey[132] = {
        0x00, 0x68, 0x5a, 0x48, 0xe8, 0x6c, 0x79, 0xf0,
        0xf0, 0x87, 0x5f, 0x7b, 0xc1, 0x8d, 0x25, 0xeb, 
        0x5f, 0xc8, 0xc0, 0xb0, 0x7e, 0x5d, 0xa4, 0xf4,
        0x37, 0x0f, 0x3a, 0x94, 0x90, 0x34, 0x08, 0x54, 
        0x33, 0x4b, 0x1e, 0x1b, 0x87, 0xfa, 0x39, 0x54,
        0x64, 0xc6, 0x06, 0x26, 0x12, 0x4a, 0x4e, 0x70,
        0xd0, 0xf7, 0x85, 0x60, 0x1d, 0x37, 0xc0, 0x98,
        0x70, 0xeb, 0xf1, 0x76, 0x66, 0x68,0x77, 0xa2, 
        0x04, 0x6d, 0x01, 0xba, 0x52, 0xc5, 0x6f, 0xc8, 
        0x77, 0x6d, 0x9e, 0x8f, 0x5d, 0xb4, 0xf0, 0xcc, 
        0x27, 0x63, 0x6d, 0x0b, 0x74, 0x1b, 0xbe, 0x05,
        0x40, 0x06, 0x97, 0x94, 0x2e, 0x80, 0xb7, 0x39,
        0x88, 0x4a, 0x83, 0xbd, 0xe9, 0x9e, 0x0f, 0x67, 
        0x16, 0x93, 0x9e, 0x63, 0x2b, 0xc8, 0x98, 0x6f, 
        0xa1, 0x8d, 0xcc, 0xd4, 0x43, 0xa3, 0x48, 0xb6,
        0xc3, 0xe5, 0x22, 0x49, 0x79, 0x55, 0xa4, 0xf3,
        0xc3, 0x02, 0xf6, 0x76
    };
   
    ////NIST Defined Shared Secret**********************************************************************************************
    uint8_t expectedOutput[66] = {
        0x00, 0x5f, 0xc7, 0x04, 0x77, 0xc3, 0xe6, 0x3b,
        0xc3, 0x95, 0x4b, 0xd0, 0xdf, 0x3e, 0xa0, 0xd1,
        0xf4, 0x1e, 0xe2, 0x17, 0x46, 0xed, 0x95, 0xfc,
        0x5e, 0x1f, 0xdf, 0x90, 0x93, 0x0d, 0x5e, 0x13,
        0x66, 0x72, 0xd7, 0x2c, 0xc7, 0x70, 0x74, 0x2d,
        0x17, 0x11, 0xc3, 0xc3, 0xa4, 0xc3, 0x34, 0xa0,
        0xad, 0x97, 0x59, 0x43, 0x6a, 0x4d, 0x3c, 0x5b,
        0xf6, 0xe7, 0x4b, 0x95, 0x78, 0xfa, 0xc1, 0x48,
        0xc8, 0x31
    };
    
    uint8_t sharedSecret[132];
    
    runHardwareTest(testPublicKey, testPrivateKey, sharedSecret, 
            sizeof(testPrivateKey), expectedOutput, sizeof(expectedOutput),
            CRYPTO_ECC_CURVE_P521);
}


void test_ecdh(void)
{
    printSplitter();
    (void)printf("\r\n Starting CRYPTO ECDH Shared Secret program \r\n");
    TEST_ECDH_SHARED_SECRET_p192();
    TEST_ECDH_SHARED_SECRET_p256();
    TEST_ECDH_SHARED_SECRET_p384();
    TEST_ECDH_SHARED_SECRET_p521();
    printSplitter();
}