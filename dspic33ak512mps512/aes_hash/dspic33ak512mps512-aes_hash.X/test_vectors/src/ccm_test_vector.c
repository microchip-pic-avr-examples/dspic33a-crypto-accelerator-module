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
 * Test vectors from NIST Block Cipher Modes SP 800-38C (CCM) validation:
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/block-ciphers
 */

static uint8_t aesCcmKey[KEY_TYPE_AES128_SIZE] =
{
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F
};

static uint8_t aesCcmNonce[AES_AEAD_IV_LENGTH] =
{
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B
};

static uint8_t aesCcmAad[20] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13
};

static uint8_t aesCcmPlaintext[24] =
{
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
};

static uint8_t aesCcmCiphertext4Bytes[4] =
{
    0x71, 0x62, 0x01, 0x5B
};

static uint8_t aesCcmAuthtag4Bytes[4] =
{
    0x4D, 0xAC, 0x25, 0x5D
};

static uint8_t aesCcmCiphertext16Bytes[16] =
{
    0xD2, 0xA1, 0xF0, 0xE0, 0x51, 0xEA, 0x5F, 0x62, 0x08, 0x1A, 0x77, 0x92, 0x07, 0x3D, 0x59, 0x3D
};

static uint8_t aesCcmAuthtag6Bytes[6] =
{
    0x1F, 0xC6, 0x4F, 0xBF, 0xAC, 0xCD
};

static uint8_t aesCcmCiphertext24Bytes[24] =
{
    0xE3, 0xB2, 0x01, 0xA9, 0xF5, 0xB7, 0x1A, 0x7A, 0x9B, 0x1C, 0xEA, 0xEC, 0xCD, 0x97, 0xE7, 0x0B,
    0x61, 0x76, 0xAA, 0xD9, 0xA4, 0x42, 0x8A, 0xA5
};

static uint8_t aesCcmAuthtag8Bytes[8] =
{
    0x48, 0x43, 0x92, 0xFB, 0xC1, 0xB0, 0x99, 0x51
};

// AES-128 CCM, 4-byte plaintext, 8-byte AAD, 4-byte tag, 7-byte nonce
static const aead_test_vector_t aesCcm128Vector4Bytes =
{
    .id = 0,
    .inputKey = aesCcmKey,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aesCcmNonce,
    .initializationVectorLength = 7,
    .aad = aesCcmAad,
    .aadLength = 8,
    .plaintextData = aesCcmPlaintext,
    .ciphertextData = aesCcmCiphertext4Bytes,
    .dataLength = 4,
    .authTag = aesCcmAuthtag4Bytes,
    .authTagLength = 4
};

// AES-128 CCM, 16-byte plaintext, 16-byte AAD, 6-byte tag, 8-byte nonce
static const aead_test_vector_t aesCcm128Vector16Bytes =
{
    .id = 0,
    .inputKey = aesCcmKey,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aesCcmNonce,
    .initializationVectorLength = 8,
    .aad = aesCcmAad,
    .aadLength = 16,
    .plaintextData = aesCcmPlaintext,
    .ciphertextData = aesCcmCiphertext16Bytes,
    .dataLength = 16,
    .authTag = aesCcmAuthtag6Bytes,
    .authTagLength = 6
};

// AES-128 CCM, 24-byte plaintext, 20-byte AAD, 8-byte tag, 12-byte nonce
static const aead_test_vector_t aesCcm128Vector24Bytes =
{
    .id = 0,
    .inputKey = aesCcmKey,
    .inputKeyLength = KEY_TYPE_AES128_SIZE,
    .initializationVector = aesCcmNonce,
    .initializationVectorLength = 12,
    .aad = aesCcmAad,
    .aadLength = 20,
    .plaintextData = aesCcmPlaintext,
    .ciphertextData = aesCcmCiphertext24Bytes,
    .dataLength = 24,
    .authTag = aesCcmAuthtag8Bytes,
    .authTagLength = 8
};

// 3 CCM test vectors: all AES-128 (4-byte, 16-byte, 24-byte plaintext)
static aead_test_vector_t aesCcmTestVectors[3] =
{
    [0] = aesCcm128Vector4Bytes,
    [1] = aesCcm128Vector16Bytes,
    [2] = aesCcm128Vector24Bytes,
};

aead_test_vector_t * VECTOR_CcmTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_CcmTestVectorCountGet(); index++)
    {
        aesCcmTestVectors[index].id = (index + 1UL);
    }

    return aesCcmTestVectors;
}

uint32_t VECTOR_CcmTestVectorCountGet(void)
{
    return sizeof(aesCcmTestVectors) / sizeof(aesCcmTestVectors[0]);
}
