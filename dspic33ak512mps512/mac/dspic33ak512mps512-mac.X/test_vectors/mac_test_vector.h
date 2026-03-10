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

#ifndef MAC_TEST_VECTOR_H
#define MAC_TEST_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "crypto/common_crypto/crypto_hash.h"

typedef struct
{
    uint8_t *inputData;
    uint32_t inputDataLength;
    uint8_t *inputKey;
    uint32_t inputKeyLength;
    uint8_t *expectedMac;
    uint32_t expectedMacLength;
    uint8_t *initVect;
    uint32_t initVectLength;
    crypto_Hash_Algo_E hashAlgorithm;
} test_vector_t;

#define NUMBER_OF_CMAC_VECTORS 4
extern test_vector_t cmac128TestVectors[NUMBER_OF_CMAC_VECTORS];
extern test_vector_t cmac192TestVectors[NUMBER_OF_CMAC_VECTORS];
extern test_vector_t cmac256TestVectors[NUMBER_OF_CMAC_VECTORS];

#define NUMBER_OF_HMAC_VECTORS 2
extern test_vector_t hmac1TestVectors[NUMBER_OF_HMAC_VECTORS];
extern test_vector_t hmac224TestVectors[NUMBER_OF_HMAC_VECTORS];
extern test_vector_t hmac256TestVectors[NUMBER_OF_HMAC_VECTORS];
extern test_vector_t hmac384TestVectors[NUMBER_OF_HMAC_VECTORS];
extern test_vector_t hmac512TestVectors[NUMBER_OF_HMAC_VECTORS];

#define NUMBER_OF_GMAC_VECTORS 4
extern test_vector_t gmacTestVectors[NUMBER_OF_GMAC_VECTORS];

#ifdef __cplusplus
}
#endif

#endif /* MAC_TEST_VECTOR_H */
