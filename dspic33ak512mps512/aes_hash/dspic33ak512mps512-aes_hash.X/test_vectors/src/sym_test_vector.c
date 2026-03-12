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

static uint8_t ptData[DATA_SIZE] =
{
    0x22, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF1, 0x1F,
    0x12, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x98, 0xA9, 0xBA, 0xCB, 0xDC, 0xED, 0xFE, 0x1F, 0xFF,
    0x11, 0x23, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0x99, 0xAA, 0xBB, 0xCC
};

static uint8_t ecbCtData[DATA_SIZE] =
{
    0xB8, 0xAE, 0x5C, 0x05, 0x14, 0xB5, 0xD9, 0x96, 0x9D, 0x32, 0x44, 0x32, 0x7C, 0x70, 0xE9, 0xE3,
    0xD7, 0xE1, 0xF7, 0x08, 0xCC, 0x1E, 0x6D, 0x17, 0xD7, 0x2F, 0xD1, 0x79, 0xB1, 0xB6, 0xB4, 0x21,
    0x50, 0xA8, 0x73, 0x34, 0x2F, 0xCE, 0x6E, 0x58, 0x82, 0x89, 0x0C, 0x8A, 0x6C, 0x1A, 0x70, 0x13
};

static uint8_t ctrCtData[DATA_SIZE] =
{
    0x8C, 0xA9, 0x4D, 0xA3, 0xBF, 0x7F, 0x40, 0xE1, 0x75, 0x0D, 0xA8, 0x22, 0x40, 0x48, 0xFE, 0xF3,
    0xF6, 0x96, 0x5B, 0x07, 0xC6, 0x13, 0x2E, 0x71, 0x7F, 0x16, 0x67, 0x9B, 0x78, 0x0D, 0x46, 0xD7,
    0x52, 0xAD, 0x6C, 0xAD, 0x07, 0x5B, 0x8D, 0xDF, 0x5C, 0xC0, 0xC1, 0x0D, 0x40, 0xAA, 0x3A, 0x7F
};

static uint8_t cbcCtData[DATA_SIZE] =
{
    0x93, 0x42, 0x96, 0x03, 0x06, 0x68, 0x7B, 0xE0, 0xE6, 0x3B, 0x9E, 0x85, 0x79, 0xAA, 0x11, 0xCB,
    0x0E, 0x0E, 0xB8, 0xFE, 0x09, 0x8D, 0x14, 0xEF, 0xAF, 0x64, 0x24, 0x4E, 0x07, 0xA9, 0x9F, 0x7B,
    0x33, 0x54, 0x19, 0x06, 0xCC, 0x6E, 0xFD, 0x41, 0xE8, 0x89, 0xAF, 0xF5, 0x10, 0x59, 0x4E, 0x9A
};

static uint8_t cfb128CtData[DATA_SIZE] =
{
    0x8C, 0xA9, 0x4D, 0xA3, 0xBF, 0x7F, 0x40, 0xE1, 0x75, 0x0D, 0xA8, 0x22, 0x40, 0x48, 0xFE, 0xF3,
    0x84, 0xE5, 0x23, 0x03, 0x8A, 0x5A, 0x47, 0x75, 0x4B, 0x1A, 0x47, 0x2D, 0x8A, 0xE0, 0xB8, 0x3D,
    0x7C, 0x2B, 0xF7, 0xCE, 0x83, 0xAC, 0x22, 0x85, 0xC0, 0xD8, 0xFD, 0x43, 0x7F, 0x9B, 0x31, 0x5B
};

static uint8_t ofbCtData[DATA_SIZE] =
{
    0x8C, 0xA9, 0x4D, 0xA3, 0xBF, 0x7F, 0x40, 0xE1, 0x75, 0x0D, 0xA8, 0x22, 0x40, 0x48, 0xFE, 0xF3,
    0x0F, 0x6A, 0xC0, 0x35, 0xFA, 0x11, 0xC9, 0x69, 0xF6, 0xDD, 0xDA, 0x8D, 0x1A, 0x2E, 0x05, 0x86,
    0x10, 0x8C, 0xC6, 0x9D, 0xB2, 0xA7, 0x03, 0x50, 0x70, 0xFF, 0x57, 0xB6, 0x6A, 0xCD, 0xDB, 0x0C
};
static uint8_t initializationVector[INIT_VECTOR_SIZE] =
{
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10
};

static uint8_t keyData[32] =
{
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x20
};

// AES-256 ECB, 35-byte plaintext
static const sym_test_vector_t ecb256Vector35Bytes =
{
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

// AES-256 ECB, 41-byte plaintext
static const sym_test_vector_t ecb256Vector41Bytes =
{
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

// AES-256 ECB, 48-byte plaintext
static const sym_test_vector_t ecb256Vector48Bytes =
{
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

// 3 ECB AES-256 test vectors (35-byte, 41-byte, 48-byte plaintext)
static sym_test_vector_t ecbTestVectors[3] =
{
    [0] = ecb256Vector35Bytes,
    [1] = ecb256Vector41Bytes,
    [2] = ecb256Vector48Bytes,
};

sym_test_vector_t * VECTOR_EcbTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_EcbTestVectorCountGet(); index++)
    {
        ecbTestVectors[index].id = (index + 1UL);
    }

    return ecbTestVectors;
}

uint32_t VECTOR_EcbTestVectorCountGet(void)
{
    return sizeof(ecbTestVectors) / sizeof(ecbTestVectors[0]);
}

// AES-256 CTR, 35-byte plaintext
static const sym_test_vector_t ctr256Vector35Bytes =
{
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

// AES-256 CTR, 41-byte plaintext
static const sym_test_vector_t ctr256Vector41Bytes =
{
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

// AES-256 CTR, 48-byte plaintext
static const sym_test_vector_t ctr256Vector48Bytes =
{
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

// 3 CTR AES-256 test vectors (35-byte, 41-byte, 48-byte plaintext)
static sym_test_vector_t ctrTestVectors[3] =
{
    [0] = ctr256Vector35Bytes,
    [1] = ctr256Vector41Bytes,
    [2] = ctr256Vector48Bytes,
};

sym_test_vector_t * VECTOR_CtrTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_CtrTestVectorCountGet(); index++)
    {
        ctrTestVectors[index].id = (index + 1UL);
    }

    return ctrTestVectors;
}

uint32_t VECTOR_CtrTestVectorCountGet(void)
{
    return sizeof(ctrTestVectors) / sizeof(ctrTestVectors[0]);
}

// AES-256 CBC, 35-byte plaintext
static const sym_test_vector_t cbc256Vector35Bytes =
{
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

// AES-256 CBC, 41-byte plaintext
static const sym_test_vector_t cbc256Vector41Bytes =
{
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

// AES-256 CBC, 48-byte plaintext
static const sym_test_vector_t cbc256Vector48Bytes =
{
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

// 3 CBC AES-256 test vectors (35-byte, 41-byte, 48-byte plaintext)
static sym_test_vector_t cbcTestVectors[3] =
{
    [0] = cbc256Vector35Bytes,
    [1] = cbc256Vector41Bytes,
    [2] = cbc256Vector48Bytes,
};

sym_test_vector_t * VECTOR_CbcTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_CbcTestVectorCountGet(); index++)
    {
        cbcTestVectors[index].id = (index + 1UL);
    }

    return cbcTestVectors;
}

uint32_t VECTOR_CbcTestVectorCountGet(void)
{
    return sizeof(cbcTestVectors) / sizeof(cbcTestVectors[0]);
}

// AES-256 CFB128, 35-byte plaintext
static const sym_test_vector_t cfb128Vector35Bytes =
{
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

// AES-256 CFB128, 41-byte plaintext
static const sym_test_vector_t cfb128Vector41Bytes =
{
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

// AES-256 CFB128, 48-byte plaintext
static const sym_test_vector_t cfb128Vector48Bytes =
{
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

// 3 CFB128 AES-256 test vectors (35-byte, 41-byte, 48-byte plaintext)
static sym_test_vector_t cfb128TestVectors[3] =
{
    [0] = cfb128Vector35Bytes,
    [1] = cfb128Vector41Bytes,
    [2] = cfb128Vector48Bytes,
};

sym_test_vector_t * VECTOR_Cfb128TestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_Cfb128TestVectorCountGet(); index++)
    {
        cfb128TestVectors[index].id = (index + 1UL);
    }

    return cfb128TestVectors;
}

uint32_t VECTOR_Cfb128TestVectorCountGet(void)
{
    return sizeof(cfb128TestVectors) / sizeof(cfb128TestVectors[0]);
}

// AES-256 OFB, 35-byte plaintext
static const sym_test_vector_t ofb256Vector35Bytes =
{
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

// AES-256 OFB, 41-byte plaintext
static const sym_test_vector_t ofb256Vector41Bytes =
{
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

// AES-256 OFB, 48-byte plaintext
static const sym_test_vector_t ofb256Vector48Bytes =
{
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

// 3 OFB AES-256 test vectors (35-byte, 41-byte, 48-byte plaintext)
static sym_test_vector_t ofbTestVectors[3] =
{
    [0] = ofb256Vector35Bytes,
    [1] = ofb256Vector41Bytes,
    [2] = ofb256Vector48Bytes,
};

sym_test_vector_t * VECTOR_OfbTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_OfbTestVectorCountGet(); index++)
    {
        ofbTestVectors[index].id = (index + 1UL);
    }

    return ofbTestVectors;
}

uint32_t VECTOR_OfbTestVectorCountGet(void)
{
    return sizeof(ofbTestVectors) / sizeof(ofbTestVectors[0]);
}
