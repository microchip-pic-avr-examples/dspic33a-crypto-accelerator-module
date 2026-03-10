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
for any third-party software incorporated in the software and any a pplicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

#include "test_vectors/sym_test_vector.h"
#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"

#define DATA_SIZE           (48)
#define INIT_VECTOR_SIZE    (16)

/*
 * Test vectors from NIST Block Cipher Modes SP 800-38A:
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/block-ciphers
 */

static uint8_t ptData[DATA_SIZE] = {
    0x22, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF1, 0x1F,
    0x12, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x98, 0xA9, 0xBA, 0xCB, 0xDC, 0xED, 0xFE, 0x1F, 0xFF,
    0x11, 0x23, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0x99, 0xAA, 0xBB, 0xCC
};

static uint8_t ecbCtData[DATA_SIZE] = {
    0xb8, 0xae, 0x5c, 0x05, 0x14, 0xb5, 0xd9, 0x96, 0x9d, 0x32, 0x44, 0x32, 0x7c, 0x70, 0xe9, 0xe3,
    0xd7, 0xe1, 0xf7, 0x08, 0xcc, 0x1e, 0x6d, 0x17, 0xd7, 0x2f, 0xd1, 0x79, 0xb1, 0xb6, 0xb4, 0x21,
    0x50, 0xa8, 0x73, 0x34, 0x2f, 0xce, 0x6e, 0x58, 0x82, 0x89, 0x0c, 0x8a, 0x6c, 0x1a, 0x70, 0x13
};

static uint8_t ctrCtData[DATA_SIZE] = {
    0x8c, 0xa9, 0x4d, 0xa3, 0xbf, 0x7f, 0x40, 0xe1, 0x75, 0x0d, 0xa8, 0x22, 0x40, 0x48, 0xfe, 0xf3,
    0xf6, 0x96, 0x5b, 0x07, 0xc6, 0x13, 0x2e, 0x71, 0x7f, 0x16, 0x67, 0x9b, 0x78, 0x0d, 0x46, 0xd7,
    0x52, 0xad, 0x6c, 0xad, 0x07, 0x5b, 0x8d, 0xdf, 0x5c, 0xc0, 0xc1, 0x0d, 0x40, 0xaa, 0x3a, 0x7f
};

static uint8_t cbcCtData[DATA_SIZE] = {
    0x93, 0x42, 0x96, 0x03, 0x06, 0x68, 0x7b, 0xe0, 0xe6, 0x3b, 0x9e, 0x85, 0x79, 0xaa, 0x11, 0xcb,
    0x0e, 0x0e, 0xb8, 0xfe, 0x09, 0x8d, 0x14, 0xef, 0xaf, 0x64, 0x24, 0x4e, 0x07, 0xa9, 0x9f, 0x7b,
    0x33, 0x54, 0x19, 0x06, 0xcc, 0x6e, 0xfd, 0x41, 0xe8, 0x89, 0xaf, 0xf5, 0x10, 0x59, 0x4e, 0x9a
};

static uint8_t cfb128CtData[DATA_SIZE] = {
    0x8c, 0xa9, 0x4d, 0xa3, 0xbf, 0x7f, 0x40, 0xe1, 0x75, 0x0d, 0xa8, 0x22, 0x40, 0x48, 0xfe, 0xf3,
    0x84, 0xe5, 0x23, 0x03, 0x8a, 0x5a, 0x47, 0x75, 0x4b, 0x1a, 0x47, 0x2d, 0x8a, 0xe0, 0xb8, 0x3d,
    0x7c, 0x2b, 0xf7, 0xce, 0x83, 0xac, 0x22, 0x85, 0xc0, 0xd8, 0xfd, 0x43, 0x7f, 0x9b, 0x31, 0x5b
};

static uint8_t ofbCtData[DATA_SIZE] = {
    0x8c, 0xa9, 0x4d, 0xa3, 0xbf, 0x7f, 0x40, 0xe1, 0x75, 0x0d, 0xa8, 0x22, 0x40, 0x48, 0xfe, 0xf3,
    0x0f, 0x6a, 0xc0, 0x35, 0xfa, 0x11, 0xc9, 0x69, 0xf6, 0xdd, 0xda, 0x8d, 0x1a, 0x2e, 0x05, 0x86,
    0x10, 0x8c, 0xc6, 0x9d, 0xb2, 0xa7, 0x03, 0x50, 0x70, 0xff, 0x57, 0xb6, 0x6a, 0xcd, 0xdb, 0x0c
};
static uint8_t initializationVector[INIT_VECTOR_SIZE] = {
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10
};

static uint8_t keyData[32] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x20
};

static const sym_test_vector_t ecb256Vector35Bytes = {
    0,
    CRYPTO_SYM_OPMODE_ECB,
    ptData,
    ecbCtData,
    35,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t ecb_256_41_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_ECB,
    ptData,
    ecbCtData,
    41,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t ecb_256_48_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_ECB,
    ptData,
    ecbCtData,
    48,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

static sym_test_vector_t ecbTestVectors[3] = {
    [0] = ecb256Vector35Bytes,
    [1] = ecb_256_41_bytes_vector,
    [2] = ecb_256_48_bytes_vector,
};

sym_test_vector_t * TV_EcbTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < TV_EcbTestVectorCountGet(); i++)
    {
        ecbTestVectors[i].id = (i + 1UL);
    }

    return ecbTestVectors;
}

uint32_t TV_EcbTestVectorCountGet(void)
{
    return sizeof(ecbTestVectors) / sizeof(ecbTestVectors[0]);
}

 static const sym_test_vector_t ctr_256_35_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CTR,
    ptData,
    ctrCtData,
    35,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t ctr_256_41_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CTR,
    ptData,
    ctrCtData,
    41,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t ctr_256_48_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CTR,
    ptData,
    ctrCtData,
    48,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

static sym_test_vector_t ctrTestVectors[3] = {
    [0] = ctr_256_35_bytes_vector,
    [1] = ctr_256_41_bytes_vector,
    [2] = ctr_256_48_bytes_vector,
};

sym_test_vector_t * TV_CtrTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < TV_CtrTestVectorCountGet(); i++)
    {
        ctrTestVectors[i].id = (i + 1UL);
    }

    return ctrTestVectors;
}

uint32_t TV_CtrTestVectorCountGet(void)
{
    return sizeof(ctrTestVectors) / sizeof(ctrTestVectors[0]);
}

 static const sym_test_vector_t cbc_256_35_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CBC,
    ptData,
    cbcCtData,
    35,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t cbc_256_41_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CBC,
    ptData,
    cbcCtData,
    41,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t cbc_256_48_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CBC,
    ptData,
    cbcCtData,
    48,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

static sym_test_vector_t cbcTestVectors[3] = {
    [0] = cbc_256_35_bytes_vector,
    [1] = cbc_256_41_bytes_vector,
    [2] = cbc_256_48_bytes_vector,
};

sym_test_vector_t * TV_CbcTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < TV_CbcTestVectorCountGet(); i++)
    {
        cbcTestVectors[i].id = (i + 1UL);
    }

    return cbcTestVectors;
}

uint32_t TV_CbcTestVectorCountGet(void)
{
    return sizeof(cbcTestVectors) / sizeof(cbcTestVectors[0]);
}

 static const sym_test_vector_t cfb128_256_35_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CFB128,
    ptData,
    cfb128CtData,
    35,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t cfb128_256_41_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CFB128,
    ptData,
    cfb128CtData,
    41,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t cfb128_256_48_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CFB128,
    ptData,
    cfb128CtData,
    48,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

static sym_test_vector_t cfb128TestVectors[3] = {
    [0] = cfb128_256_35_bytes_vector,
    [1] = cfb128_256_41_bytes_vector,
    [2] = cfb128_256_48_bytes_vector,
};

sym_test_vector_t * TV_Cfb128TestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < TV_Cfb128TestVectorCountGet(); i++)
    {
        cfb128TestVectors[i].id = (i + 1UL);
    }

    return cfb128TestVectors;
}

uint32_t TV_Cfb128TestVectorCountGet(void)
{
    return sizeof(cfb128TestVectors) / sizeof(cfb128TestVectors[0]);
}

 static const sym_test_vector_t ofb_256_35_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_OFB,
    ptData,
    ofbCtData,
    35,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t ofb_256_41_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_OFB,
    ptData,
    ofbCtData,
    41,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const sym_test_vector_t ofb_256_48_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_OFB,
    ptData,
    ofbCtData,
    48,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    NULL,
    0,
    initializationVector,
    INIT_VECTOR_SIZE
};

static sym_test_vector_t ofbTestVectors[3] = {
    [0] = ofb_256_35_bytes_vector,
    [1] = ofb_256_41_bytes_vector,
    [2] = ofb_256_48_bytes_vector,
};

sym_test_vector_t * TV_OfbTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < TV_OfbTestVectorCountGet(); i++)
    {
        ofbTestVectors[i].id = (i + 1UL);
    }

    return ofbTestVectors;
}

uint32_t TV_OfbTestVectorCountGet(void)
{
    return sizeof(ofbTestVectors) / sizeof(ofbTestVectors[0]);
}
