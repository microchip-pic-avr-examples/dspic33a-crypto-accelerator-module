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

#include "../aead_test_vector.h"

/*
 * Test vectors from NIST Block Cipher Modes SP 800-38D (GCM) validation:
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/block-ciphers
 */

static uint8_t aesGcmEmptyKey[KEY_TYPE_AES256_SIZE] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t aesGcmKey[KEY_TYPE_AES256_SIZE] =
{
    0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C, 0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08,
    0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C, 0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08
};

static uint8_t aesGcmEmptyIv[AES_AEAD_IV_LENGTH] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t aesGcmIv[AES_AEAD_IV_LENGTH] =
{
    0xCA, 0xFE, 0xBA, 0xBE, 0xFA, 0xCE, 0xDB, 0xAD, 0xDE, 0xCA, 0xF8, 0x88
};

static uint8_t aesGcmAad20Bytes[20] =
{
    0xFE, 0xED, 0xFA, 0xCE, 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED, 0xFA, 0xCE, 0xDE, 0xAD, 0xBE, 0xEF,
    0xAB, 0xAD, 0xDA, 0xD2
};

static uint8_t aesGcmPlaintextData16Bytes[16] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t aesGcmPlaintextData64Bytes[64] =
{
    0xD9, 0x31, 0x32, 0x25, 0xF8, 0x84, 0x06, 0xE5, 0xA5, 0x59, 0x09, 0xC5, 0xAF, 0xF5, 0x26, 0x9A,
    0x86, 0xA7, 0xA9, 0x53, 0x15, 0x34, 0xF7, 0xDA, 0x2E, 0x4C, 0x30, 0x3D, 0x8A, 0x31, 0x8A, 0x72,
    0x1C, 0x3C, 0x0C, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2F, 0xCF, 0x0E, 0x24, 0x49, 0xA6, 0xB5, 0x25,
    0xB1, 0x6A, 0xED, 0xF5, 0xAA, 0x0D, 0xE6, 0x57, 0xBA, 0x63, 0x7B, 0x39, 0x1A, 0xAF, 0xD2, 0x55
};

static uint8_t aesGcm128CiphertextData16Bytes[16] =
{
    0x03, 0x88, 0xDA, 0xCE, 0x60, 0xB6, 0xA3, 0x92, 0xF3, 0x28, 0xC2, 0xB9, 0x71, 0xB2, 0xFE, 0x78
};
static uint8_t aesGcm128CiphertextData64Bytes[64] =
{
    0x42, 0x83, 0x1E, 0xC2, 0x21, 0x77, 0x74, 0x24, 0x4B, 0x72, 0x21, 0xB7, 0x84, 0xD0, 0xD4, 0x9C,
    0xE3, 0xAA, 0x21, 0x2F, 0x2C, 0x02, 0xA4, 0xE0, 0x35, 0xC1, 0x7E, 0x23, 0x29, 0xAC, 0xA1, 0x2E,
    0x21, 0xD5, 0x14, 0xB2, 0x54, 0x66, 0x93, 0x1C, 0x7D, 0x8F, 0x6A, 0x5A, 0xAC, 0x84, 0xAA, 0x05,
    0x1B, 0xA3, 0x0B, 0x39, 0x6A, 0x0A, 0xAC, 0x97, 0x3D, 0x58, 0xE0, 0x91, 0x47, 0x3F, 0x59, 0x85
};

static uint8_t aesGcm128Tag0Bytes[AES_AEAD_TAG_LENGTH] =
{
    0x58, 0xE2, 0xFC, 0xCE, 0xFA, 0x7E, 0x30, 0x61, 0x36, 0x7F, 0x1D, 0x57, 0xA4, 0xE7, 0x45, 0x5A
};

static uint8_t aesGcm128Tag16Bytes[AES_AEAD_TAG_LENGTH] =
{
    0xAB, 0x6E, 0x47, 0xD4, 0x2C, 0xEC, 0x13, 0xBD, 0xF5, 0x3A, 0x67, 0xB2, 0x12, 0x57, 0xBD, 0xDF
};

static uint8_t aesGcm128Tag60Bytes[AES_AEAD_TAG_LENGTH] =
{
    0x5B, 0xC9, 0x4F, 0xBC, 0x32, 0x21, 0xA5, 0xDB, 0x94, 0xFA, 0xE9, 0x5A, 0xE7, 0x12, 0x1A, 0x47
};

static uint8_t aesGcm128Tag64Bytes[AES_AEAD_TAG_LENGTH] =
{
    0x4D, 0x5C, 0x2A, 0xF3, 0x27, 0xCD, 0x64, 0xA6, 0x2C, 0xF3, 0x5A, 0xBD, 0x2B, 0xA6, 0xFA, 0xB4
};

static uint8_t aesGcm256CiphertextData16Bytes[16] =
{
    0xCE, 0xA7, 0x40, 0x3D, 0x4D, 0x60, 0x6B, 0x6E, 0x07, 0x4E, 0xC5, 0xD3, 0xBA, 0xF3, 0x9D, 0x18
};
static uint8_t aesGcm256CiphertextData64Bytes[64] =
{
    0x52, 0x2D, 0xC1, 0xF0, 0x99, 0x56, 0x7D, 0x07, 0xF4, 0x7F, 0x37, 0xA3, 0x2A, 0x84, 0x42, 0x7D,
    0x64, 0x3A, 0x8C, 0xDC, 0xBF, 0xE5, 0xC0, 0xC9, 0x75, 0x98, 0xA2, 0xBD, 0x25, 0x55, 0xD1, 0xAA,
    0x8C, 0xB0, 0x8E, 0x48, 0x59, 0x0D, 0xBB, 0x3D, 0xA7, 0xB0, 0x8B, 0x10, 0x56, 0x82, 0x88, 0x38,
    0xC5, 0xF6, 0x1E, 0x63, 0x93, 0xBA, 0x7A, 0x0A, 0xBC, 0xC9, 0xF6, 0x62, 0x89, 0x80, 0x15, 0xAD
};

static uint8_t aesGcm256Tag0Bytes[AES_AEAD_TAG_LENGTH] =
{
    0x53, 0x0F, 0x8A, 0xFB, 0xC7, 0x45, 0x36, 0xB9, 0xA9, 0x63, 0xB4, 0xF1, 0xC4, 0xCB, 0x73, 0x8B
};

static uint8_t aesGcm256Tag16Bytes[AES_AEAD_TAG_LENGTH] =
{
    0xD0, 0xD1, 0xC8, 0xA7, 0x99, 0x99, 0x6B, 0xF0, 0x26, 0x5B, 0x98, 0xB5, 0xD4, 0x8A, 0xB9, 0x19
};

static uint8_t aesGcm256Tag60Bytes[AES_AEAD_TAG_LENGTH] =
{
    0x76, 0xFC, 0x6E, 0xCE, 0x0F, 0x4E, 0x17, 0x68, 0xCD, 0xDF, 0x88, 0x53, 0xBB, 0x2D, 0x55, 0x1B
};

static uint8_t aesGcm256Tag64Bytes[AES_AEAD_TAG_LENGTH] =
{
    0xB0, 0x94, 0xDA, 0xC5, 0xD9, 0x34, 0x71, 0xBD, 0xEC, 0x1A, 0x50, 0x22, 0x70, 0xE3, 0xCC, 0x6C
};

// AES-128 GCM, no plaintext, no AAD
static const aead_test_vector_t aesGcm128Vector0Bytes =
{
    .id = 0,
    .inputKey = aesGcmEmptyKey,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aesGcmEmptyIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = NULL,
    .ciphertextData = NULL,
    .dataLength = 0,
    .authTag = aesGcm128Tag0Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// AES-128 GCM, 16-byte plaintext, no AAD
static const aead_test_vector_t aesGcm128Vector16Bytes =
{
    .id = 0,
    .inputKey = aesGcmEmptyKey,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aesGcmEmptyIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = aesGcmPlaintextData16Bytes,
    .ciphertextData = aesGcm128CiphertextData16Bytes,
    .dataLength = 16,
    .authTag = aesGcm128Tag16Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// AES-128 GCM, 60-byte plaintext, 20-byte AAD
static const aead_test_vector_t aesGcm128Vector60Bytes =
{
    .id = 0,
    .inputKey = aesGcmKey,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aesGcmIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = aesGcmAad20Bytes,
    .aadLength = 20,
    .plaintextData = aesGcmPlaintextData64Bytes,
    .ciphertextData = aesGcm128CiphertextData64Bytes,
    .dataLength = 60,
    .authTag = aesGcm128Tag60Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// AES-128 GCM, 64-byte plaintext, no AAD
static const aead_test_vector_t aesGcm128Vector64Bytes =
{
    .id = 0,
    .inputKey = aesGcmKey,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aesGcmIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = aesGcmPlaintextData64Bytes,
    .ciphertextData = aesGcm128CiphertextData64Bytes,
    .dataLength = 64,
    .authTag = aesGcm128Tag64Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// AES-256 GCM, no plaintext, no AAD
static const aead_test_vector_t aesGcm256Vector0Bytes =
{
    .id = 0,
    .inputKey = aesGcmEmptyKey,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = aesGcmEmptyIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = NULL,
    .ciphertextData = NULL,
    .dataLength = 0,
    .authTag = aesGcm256Tag0Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// AES-256 GCM, 16-byte plaintext, no AAD
static const aead_test_vector_t aesGcm256Vector16Bytes =
{
    .id = 0,
    .inputKey = aesGcmEmptyKey,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = aesGcmEmptyIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = aesGcmPlaintextData16Bytes,
    .ciphertextData = aesGcm256CiphertextData16Bytes,
    .dataLength = 16,
    .authTag = aesGcm256Tag16Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// AES-256 GCM, 60-byte plaintext, 20-byte AAD
static const aead_test_vector_t aesGcm256Vector60Bytes =
{
    .id = 0,
    .inputKey = aesGcmKey,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = aesGcmIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = aesGcmAad20Bytes,
    .aadLength = 20,
    .plaintextData = aesGcmPlaintextData64Bytes,
    .ciphertextData = aesGcm256CiphertextData64Bytes,
    .dataLength = 60,
    .authTag = aesGcm256Tag60Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// AES-256 GCM, 64-byte plaintext, no AAD
static const aead_test_vector_t aesGcm256Vector64Bytes =
{
    .id = 0,
    .inputKey = aesGcmKey,
    .inputKeyLength = KEY_TYPE_AES256_SIZE,
    .initializationVector = aesGcmIv,
    .initializationVectorLength = AES_AEAD_IV_LENGTH,
    .aad = NULL,
    .aadLength = 0,
    .plaintextData = aesGcmPlaintextData64Bytes,
    .ciphertextData = aesGcm256CiphertextData64Bytes,
    .dataLength = 64,
    .authTag = aesGcm256Tag64Bytes,
    .authTagLength = AES_AEAD_TAG_LENGTH
};

// 8 GCM test vectors: vectors 0-3 AES-128, vectors 4-7 AES-256
static aead_test_vector_t aesGcmTestVectors[8] =
{
    [0] = aesGcm128Vector0Bytes,
    [1] = aesGcm128Vector16Bytes,
    [2] = aesGcm128Vector60Bytes,
    [3] = aesGcm128Vector64Bytes,
    [4] = aesGcm256Vector0Bytes,
    [5] = aesGcm256Vector16Bytes,
    [6] = aesGcm256Vector60Bytes,
    [7] = aesGcm256Vector64Bytes
};

aead_test_vector_t * VECTOR_GcmTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_GcmTestVectorCountGet(); index++)
    {
        aesGcmTestVectors[index].id = (index + 1UL);
    }

    return aesGcmTestVectors;
}

uint32_t VECTOR_GcmTestVectorCountGet(void)
{
    return sizeof(aesGcmTestVectors) / sizeof(aesGcmTestVectors[0]);
}
