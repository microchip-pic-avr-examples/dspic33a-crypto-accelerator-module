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

#include "../gcm_test_vector.h"

static uint8_t aes_gcm_empty_key[KEY_TYPE_AES256_SIZE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t aes_gcm_key[KEY_TYPE_AES256_SIZE] = {
    0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08,
    0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08
};


static uint8_t aes_gcm_empty_iv[AES_GCM_IV_LENGTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t aes_gcm_iv[AES_GCM_IV_LENGTH] = {
    0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88
};


static uint8_t aes_gcm_aad_20_bytes[20] = {
    0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
    0xab, 0xad, 0xda, 0xd2
};


static uint8_t aes_gcm_plaintext_data_16_bytes[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t aes_gcm_plaintext_data_64_bytes[64] = {
    0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
    0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
    0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
    0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55
};


static uint8_t aes_gcm_128_ciphertext_data_16_bytes[16] = {
    0x03, 0x88, 0xda, 0xce, 0x60, 0xb6, 0xa3, 0x92, 0xf3, 0x28, 0xc2, 0xb9, 0x71, 0xb2, 0xfe, 0x78
};
static uint8_t aes_gcm_128_ciphertext_data_64_bytes[64] = {
    0x42, 0x83, 0x1e, 0xc2, 0x21, 0x77, 0x74, 0x24, 0x4b, 0x72, 0x21, 0xb7, 0x84, 0xd0, 0xd4, 0x9c,
    0xe3, 0xaa, 0x21, 0x2f, 0x2c, 0x02, 0xa4, 0xe0, 0x35, 0xc1, 0x7e, 0x23, 0x29, 0xac, 0xa1, 0x2e,
    0x21, 0xd5, 0x14, 0xb2, 0x54, 0x66, 0x93, 0x1c, 0x7d, 0x8f, 0x6a, 0x5a, 0xac, 0x84, 0xaa, 0x05,
    0x1b, 0xa3, 0x0b, 0x39, 0x6a, 0x0a, 0xac, 0x97, 0x3d, 0x58, 0xe0, 0x91, 0x47, 0x3f, 0x59, 0x85
};

static uint8_t aes_gcm_128_tag_0_bytes[AES_GCM_TAG_LENGTH] = {
    0x58, 0xe2, 0xfc, 0xce, 0xfa, 0x7e, 0x30, 0x61, 0x36, 0x7f, 0x1d, 0x57, 0xa4, 0xe7, 0x45, 0x5a
};

static uint8_t aes_gcm_128_tag_16_bytes[AES_GCM_TAG_LENGTH] = {
    0xab, 0x6e, 0x47, 0xd4, 0x2c, 0xec, 0x13, 0xbd, 0xf5, 0x3a, 0x67, 0xb2, 0x12, 0x57, 0xbd, 0xdf
};

static uint8_t aes_gcm_128_tag_60_bytes[AES_GCM_TAG_LENGTH] = {
    0x5b, 0xc9, 0x4f, 0xbc, 0x32, 0x21, 0xa5, 0xdb, 0x94, 0xfa, 0xe9, 0x5a, 0xe7, 0x12, 0x1a, 0x47
};

static uint8_t aes_gcm_128_tag_64_bytes[AES_GCM_TAG_LENGTH] = {
    0x4d, 0x5c, 0x2a, 0xf3, 0x27, 0xcd, 0x64, 0xa6, 0x2c, 0xf3, 0x5a, 0xbd, 0x2b, 0xa6, 0xfa, 0xb4
};


static uint8_t aes_gcm_256_ciphertext_data_16_bytes[16] = {
    0xce, 0xa7, 0x40, 0x3d, 0x4d, 0x60, 0x6b, 0x6e, 0x07, 0x4e, 0xc5, 0xd3, 0xba, 0xf3, 0x9d, 0x18
};
static uint8_t aes_gcm_256_ciphertext_data_64_bytes[64] = {
    0x52, 0x2d, 0xc1, 0xf0, 0x99, 0x56, 0x7d, 0x07, 0xf4, 0x7f, 0x37, 0xa3, 0x2a, 0x84, 0x42, 0x7d,
    0x64, 0x3a, 0x8c, 0xdc, 0xbf, 0xe5, 0xc0, 0xc9, 0x75, 0x98, 0xa2, 0xbd, 0x25, 0x55, 0xd1, 0xaa,
    0x8c, 0xb0, 0x8e, 0x48, 0x59, 0x0d, 0xbb, 0x3d, 0xa7, 0xb0, 0x8b, 0x10, 0x56, 0x82, 0x88, 0x38,
    0xc5, 0xf6, 0x1e, 0x63, 0x93, 0xba, 0x7a, 0x0a, 0xbc, 0xc9, 0xf6, 0x62, 0x89, 0x80, 0x15, 0xad
};

static uint8_t aes_gcm_256_tag_0_bytes[AES_GCM_TAG_LENGTH] = {
    0x53, 0x0f, 0x8a, 0xfb, 0xc7, 0x45, 0x36, 0xb9, 0xa9, 0x63, 0xb4, 0xf1, 0xc4, 0xcb, 0x73, 0x8b
};

static uint8_t aes_gcm_256_tag_16_bytes[AES_GCM_TAG_LENGTH] = {
    0xd0, 0xd1, 0xc8, 0xa7, 0x99, 0x99, 0x6b, 0xf0, 0x26, 0x5b, 0x98, 0xb5, 0xd4, 0x8a, 0xb9, 0x19
};

static uint8_t aes_gcm_256_tag_60_bytes[AES_GCM_TAG_LENGTH] = {
    0x76, 0xfc, 0x6e, 0xce, 0x0f, 0x4e, 0x17, 0x68, 0xcd, 0xdf, 0x88, 0x53, 0xbb, 0x2d, 0x55, 0x1b
};

static uint8_t aes_gcm_256_tag_64_bytes[AES_GCM_TAG_LENGTH] = {
    0xb0, 0x94, 0xda, 0xc5, 0xd9, 0x34, 0x71, 0xbd, 0xec, 0x1a, 0x50, 0x22, 0x70, 0xe3, 0xcc, 0x6c
};


static const TEST_VECTOR aes_gcm_128_0_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_empty_key,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_empty_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = NULL,
    .ciphertextData = NULL,
    .dataLength = 0,
    .authTag = (uint8_t *)aes_gcm_128_tag_0_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};

static const TEST_VECTOR aes_gcm_128_16_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_empty_key,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_empty_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = (uint8_t *)&aes_gcm_plaintext_data_16_bytes,
    .ciphertextData = (uint8_t *)&aes_gcm_128_ciphertext_data_16_bytes,
    .dataLength = 16,
    .authTag = (uint8_t *)&aes_gcm_128_tag_16_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};

static const TEST_VECTOR aes_gcm_128_60_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_key,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = (uint8_t *)&aes_gcm_aad_20_bytes,
    .aadLength = 20,
    .plaintextData = (uint8_t *)&aes_gcm_plaintext_data_64_bytes,
    .ciphertextData = (uint8_t *)&aes_gcm_128_ciphertext_data_64_bytes,
    .dataLength = 60,
    .authTag = (uint8_t *)&aes_gcm_128_tag_60_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};

static const TEST_VECTOR aes_gcm_128_64_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_key,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = (uint8_t *)&aes_gcm_plaintext_data_64_bytes,
    .ciphertextData = (uint8_t *)&aes_gcm_128_ciphertext_data_64_bytes,
    .dataLength = 64,
    .authTag = (uint8_t *)&aes_gcm_128_tag_64_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};


static const TEST_VECTOR aes_gcm_256_0_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_empty_key,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_empty_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = NULL,
    .ciphertextData = NULL,
    .dataLength = 0,
    .authTag = (uint8_t *)aes_gcm_256_tag_0_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};

static const TEST_VECTOR aes_gcm_256_16_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_empty_key,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_empty_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = (uint8_t *)&aes_gcm_plaintext_data_16_bytes,
    .ciphertextData = (uint8_t *)&aes_gcm_256_ciphertext_data_16_bytes,
    .dataLength = 16,
    .authTag = (uint8_t *)&aes_gcm_256_tag_16_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};

static const TEST_VECTOR aes_gcm_256_60_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_key,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = (uint8_t *)&aes_gcm_aad_20_bytes,
    .aadLength = 20,
    .plaintextData = (uint8_t *)&aes_gcm_plaintext_data_64_bytes,
    .ciphertextData = (uint8_t *)&aes_gcm_256_ciphertext_data_64_bytes,
    .dataLength = 60,
    .authTag = (uint8_t *)&aes_gcm_256_tag_60_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};

static const TEST_VECTOR aes_gcm_256_64_bytes_vector = {
    .id = 0,
    .inputKey = (uint8_t *)&aes_gcm_key,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = (uint8_t *)&aes_gcm_iv,
    .initializationVectorLength = AES_GCM_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = (uint8_t *)&aes_gcm_plaintext_data_64_bytes,
    .ciphertextData = (uint8_t *)&aes_gcm_256_ciphertext_data_64_bytes,
    .dataLength = 64,
    .authTag = (uint8_t *)&aes_gcm_256_tag_64_bytes,
    .authTagLength = AES_GCM_TAG_LENGTH
};

TEST_VECTOR aes_gcm_test_vectors[] = {
    aes_gcm_128_0_bytes_vector,
    aes_gcm_128_16_bytes_vector,
    aes_gcm_128_60_bytes_vector,
    aes_gcm_128_64_bytes_vector,
    aes_gcm_256_0_bytes_vector,
    aes_gcm_256_16_bytes_vector,
    aes_gcm_256_60_bytes_vector,
    aes_gcm_256_64_bytes_vector
};


TEST_VECTOR * get_gcm_test_vectors(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < get_gcm_test_vector_count(); i++)
    {
        aes_gcm_test_vectors[i].id = (i + 1UL);
    }

    return aes_gcm_test_vectors;
}

uint32_t get_gcm_test_vector_count(void)
{
    return sizeof(aes_gcm_test_vectors) / sizeof(aes_gcm_test_vectors[0]);
}