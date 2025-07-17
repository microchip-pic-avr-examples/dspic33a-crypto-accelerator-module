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

#define DATA_SIZE           48
#define INIT_VECTOR_SIZE    16

/*
 The following test vectors were pulled from NIST's Block Cipher Modes SP 800-38B test vectors:
 https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/cavp-testing-block-cipher-modes
*/

static uint8_t ptdata[DATA_SIZE] = {
    0x22, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89,
    0x9A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF1, 0x1F,
    0x12, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x98,
    0xA9, 0xBA, 0xCB, 0xDC, 0xED, 0xFE, 0x1F, 0xFF,
    0x11, 0x23, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC, 0x99, 0xAA, 0xBB, 0xCC
};

static uint8_t ecb_ctdata[DATA_SIZE] = {
    0xb8, 0xae, 0x5c, 0x05, 0x14, 0xb5, 0xd9, 0x96, 0x9d, 0x32, 0x44, 0x32, 0x7c, 0x70, 0xe9, 0xe3,
    0xd7, 0xe1, 0xf7, 0x08, 0xcc, 0x1e, 0x6d, 0x17, 0xd7, 0x2f, 0xd1, 0x79, 0xb1, 0xb6, 0xb4, 0x21,
    0x50, 0xa8, 0x73, 0x34, 0x2f, 0xce, 0x6e, 0x58, 0x82, 0x89, 0x0c, 0x8a, 0x6c, 0x1a, 0x70, 0x13
};

static uint8_t ctr_ctdata[DATA_SIZE] = {
    0x8c, 0xa9, 0x4d, 0xa3, 0xbf, 0x7f, 0x40, 0xe1, 0x75, 0x0d, 0xa8, 0x22, 0x40, 0x48, 0xfe, 0xf3,
    0xf6, 0x96, 0x5b, 0x07, 0xc6, 0x13, 0x2e, 0x71, 0x7f, 0x16, 0x67, 0x9b, 0x78, 0x0d, 0x46, 0xd7,
    0x52, 0xad, 0x6c, 0xad, 0x07, 0x5b, 0x8d, 0xdf, 0x5c, 0xc0, 0xc1, 0x0d, 0x40, 0xaa, 0x3a, 0x7f
};

static uint8_t initializationVector[INIT_VECTOR_SIZE] = {
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10
};

static uint8_t keyData[32] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x20
};


 static const TEST_VECTOR ecb_256_35_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_ECB,
    ptdata,
    ecb_ctdata,
    35,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const TEST_VECTOR ecb_256_41_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_ECB,
    ptdata,
    ecb_ctdata,
    41,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const TEST_VECTOR ecb_256_48_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_ECB,
    ptdata,
    ecb_ctdata,
    48,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    initializationVector,
    INIT_VECTOR_SIZE
};

TEST_VECTOR ecb_test_vectors[] = {
    ecb_256_35_bytes_vector,
    ecb_256_41_bytes_vector,
    ecb_256_48_bytes_vector,
};

TEST_VECTOR * get_ecb_test_vectors(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < get_ecb_test_vector_count(); i++)
    {
        ecb_test_vectors[i].id = (i + 1UL);
    }

    return ecb_test_vectors;
}

uint32_t get_ecb_test_vector_count(void)
{
    return sizeof(ecb_test_vectors) / sizeof(ecb_test_vectors[0]);
}


 static const TEST_VECTOR ctr_256_35_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CTR,
    ptdata,
    ctr_ctdata,
    35,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const TEST_VECTOR ctr_256_41_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CTR,
    ptdata,
    ctr_ctdata,
    41,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    initializationVector,
    INIT_VECTOR_SIZE
};

 static const TEST_VECTOR ctr_256_48_bytes_vector = {
    0,
    CRYPTO_SYM_OPMODE_CTR,
    ptdata,
    ctr_ctdata,
    48,
    keyData,
    CRYPTO_AESKEYSIZE_256,
    initializationVector,
    INIT_VECTOR_SIZE
};

TEST_VECTOR ctr_test_vectors[] = {
    ctr_256_35_bytes_vector,
    ctr_256_41_bytes_vector,
    ctr_256_48_bytes_vector,
};

TEST_VECTOR * get_ctr_test_vectors(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < get_ctr_test_vector_count(); i++)
    {
        ctr_test_vectors[i].id = (i + 1UL);
    }

    return ctr_test_vectors;
}

uint32_t get_ctr_test_vector_count(void)
{
    return sizeof(ctr_test_vectors) / sizeof(ctr_test_vectors[0]);
}