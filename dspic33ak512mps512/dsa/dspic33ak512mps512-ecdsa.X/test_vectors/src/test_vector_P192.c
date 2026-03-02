/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "test_vectors/test_vector.h"

static uint8_t privateKey_p192[] = {
    0xe5, 0xce, 0x89, 0xa3, 0x4a, 0xdd, 0xdf, 0x25, 
    0xff, 0x3b, 0xf1, 0xff, 0xe6, 0x80, 0x3f, 0x57, 
    0xd0, 0x22, 0x0d, 0xe3, 0x11, 0x87, 0x98, 0xea
};

static uint8_t publicKey_p192[] = {
    0x04, 0x8a, 0xbf, 0x7b, 0x3c, 0xeb, 0x2b, 0x02, 
    0x43, 0x8a, 0xf1, 0x95, 0x43, 0xd3, 0xe5, 0xb1, 
    0xd5, 0x73, 0xfa, 0x9a, 0xc6, 0x00, 0x85, 0x84, 
    0x0f, 0xa8, 0x7f, 0x80, 0x18, 0x2d, 0xcd, 0x56, 
    0xa6, 0xa0, 0x61, 0xf8, 0x1f, 0x7d, 0xa3, 0x93, 
    0xe7, 0xcf, 0xfd, 0x5e, 0x07, 0x38, 0xc6, 0xb2, 
    0x45
};

static uint8_t hash_p192[] = {
    0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 
    0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23, 
    0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
    0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad
};

static const char curveInformation_p192[] = "P-192";

TEST_VECTOR ecdsa_p192 = {
    (uint8_t *)&privateKey_p192,
    sizeof(privateKey_p192),
    (uint8_t *)&publicKey_p192,
    sizeof(publicKey_p192),
    (uint8_t *)&hash_p192,
    sizeof(hash_p192),
    (const char*) curveInformation_p192,
    CRYPTO_ECC_CURVE_P192,
};
