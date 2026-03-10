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

static uint8_t aesCcmKey[KEY_TYPE_AES128_SIZE] = {
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
};

static uint8_t aesCcmNonce[AES_AEAD_IV_LENGTH] = {
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b
};

static uint8_t aesCcmAad[20] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13
};

static uint8_t aesCcmPlaintext[24] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
};

static uint8_t aesCcmCiphertext4Bytes[4] = {
    0x71, 0x62, 0x01, 0x5b
};

static uint8_t aesCcmAuthtag4Bytes[4] = {
    0x4d, 0xac, 0x25, 0x5d
};

static uint8_t aesCcmCiphertext16Bytes[16] = {
    0xd2, 0xa1, 0xf0, 0xe0, 0x51, 0xea, 0x5f, 0x62, 0x08, 0x1a, 0x77, 0x92, 0x07, 0x3d, 0x59, 0x3d
};

static uint8_t aesCcmAuthtag6Bytes[6] = {
    0x1f, 0xc6, 0x4f, 0xbf, 0xac, 0xcd
};

static uint8_t aesCcmCiphertext24Bytes[24] = {
    0xe3, 0xb2, 0x01, 0xa9, 0xf5, 0xb7, 0x1a, 0x7a, 0x9b, 0x1c, 0xea, 0xec, 0xcd, 0x97, 0xe7, 0x0b,
    0x61, 0x76, 0xaa, 0xd9, 0xa4, 0x42, 0x8a, 0xa5
};

static uint8_t aesCcmAuthtag8Bytes[8] = {
    0x48, 0x43, 0x92, 0xfb, 0xc1, 0xb0, 0x99, 0x51
};

static const aead_test_vector_t aesCcm128Vector4Bytes = {
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

static const aead_test_vector_t aesCcm128Vector16Bytes = {
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

static const aead_test_vector_t aesCcm128Vector24Bytes = {
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

static aead_test_vector_t aesCcmTestVectors[3] = {
    [0] = aesCcm128Vector4Bytes,
    [1] = aesCcm128Vector16Bytes,
    [2] = aesCcm128Vector24Bytes,
};

aead_test_vector_t * TV_CcmTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < TV_CcmTestVectorCountGet(); i++)
    {
        aesCcmTestVectors[i].id = (i + 1UL);
    }

    return aesCcmTestVectors;
}

uint32_t TV_CcmTestVectorCountGet(void)
{
    return sizeof(aesCcmTestVectors) / sizeof(aesCcmTestVectors[0]);
}
