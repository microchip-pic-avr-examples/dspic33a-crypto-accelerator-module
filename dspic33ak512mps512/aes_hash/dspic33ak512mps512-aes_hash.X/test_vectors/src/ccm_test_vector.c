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
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

#include "../aead_test_vector.h"

static uint8_t aes_ccm_key[KEY_TYPE_AES128_SIZE] = {
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
};

static uint8_t aes_ccm_nonce[AES_AEAD_IV_LENGTH] = {
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b
};

static uint8_t aes_ccm_aad[20] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13
};

static uint8_t aes_ccm_plaintext[24] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
};

static uint8_t aes_ccm_ciphertext_4bytes[4] = {
    0x71, 0x62, 0x01, 0x5b
};

static uint8_t aes_ccm_authtag_4bytes[4] = {
    0x4d, 0xac, 0x25, 0x5d
};

static uint8_t aes_ccm_ciphertext_16bytes[16] = {
    0xd2, 0xa1, 0xf0, 0xe0, 0x51, 0xea, 0x5f, 0x62, 0x08, 0x1a, 0x77, 0x92, 0x07, 0x3d, 0x59, 0x3d
};

static uint8_t aes_ccm_authtag_6bytes[6] = {
    0x1f, 0xc6, 0x4f, 0xbf, 0xac, 0xcd
};

static uint8_t aes_ccm_ciphertext_24bytes[24] = {
    0xe3, 0xb2, 0x01, 0xa9, 0xf5, 0xb7, 0x1a, 0x7a, 0x9b, 0x1c, 0xea, 0xec, 0xcd, 0x97, 0xe7, 0x0b,
    0x61, 0x76, 0xaa, 0xd9, 0xa4, 0x42, 0x8a, 0xa5
};

static uint8_t aes_ccm_authtag_8bytes[8] = {
    0x48, 0x43, 0x92, 0xfb, 0xc1, 0xb0, 0x99, 0x51
};


static const AEAD_TEST_VECTOR aes_ccm_128_4_bytes_vector = {
    .id = 0,
    .inputKey = aes_ccm_key,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aes_ccm_nonce,
    .initializationVectorLength = 7,
    .aad = aes_ccm_aad,
    .aadLength = 8,
    .plaintextData = aes_ccm_plaintext,
    .ciphertextData = aes_ccm_ciphertext_4bytes,
    .dataLength = 4,
    .authTag = aes_ccm_authtag_4bytes,
    .authTagLength = 4
};

static const AEAD_TEST_VECTOR aes_ccm_128_16_bytes_vector = {
    .id = 0,
    .inputKey = aes_ccm_key,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aes_ccm_nonce,
    .initializationVectorLength = 8,
    .aad = aes_ccm_aad,
    .aadLength = 16,
    .plaintextData = aes_ccm_plaintext,
    .ciphertextData = aes_ccm_ciphertext_16bytes,
    .dataLength = 16,
    .authTag = aes_ccm_authtag_6bytes,
    .authTagLength = 6
};

static const AEAD_TEST_VECTOR aes_ccm_128_24_bytes_vector = {
    .id = 0,
    .inputKey = aes_ccm_key,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aes_ccm_nonce,
    .initializationVectorLength = 12,
    .aad = aes_ccm_aad,
    .aadLength = 20,
    .plaintextData = aes_ccm_plaintext,
    .ciphertextData = aes_ccm_ciphertext_24bytes,
    .dataLength = 24,
    .authTag = aes_ccm_authtag_8bytes,
    .authTagLength = 8
};

static AEAD_TEST_VECTOR aes_ccm_test_vectors[3] = {
    [0] = aes_ccm_128_4_bytes_vector,
    [1] = aes_ccm_128_16_bytes_vector,
    [2] = aes_ccm_128_24_bytes_vector,
};

AEAD_TEST_VECTOR * get_ccm_test_vectors(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < get_ccm_test_vector_count(); i++)
    {
        aes_ccm_test_vectors[i].id = (i + 1UL);
    }

    return aes_ccm_test_vectors;
}

uint32_t get_ccm_test_vector_count(void)
{
    return sizeof(aes_ccm_test_vectors) / sizeof(aes_ccm_test_vectors[0]);
}