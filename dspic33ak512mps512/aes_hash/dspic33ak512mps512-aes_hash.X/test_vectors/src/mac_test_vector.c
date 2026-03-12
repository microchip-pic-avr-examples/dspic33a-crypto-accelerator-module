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

#include "test_vectors/mac_test_vector.h"

/*
 * Test vectors from NIST Block Cipher Modes SP 800-38B:
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/block-ciphers
 */

// CMAC AES-256, 0-byte message
static uint8_t cmac256Key0Bytes[32] =
{
    0x7F, 0x1E, 0x1F, 0x76, 0x27, 0x3A, 0xEC, 0xB3, 0x64, 0x53, 0xB4, 0x24, 0x5D, 0xA4, 0x7C, 0x8A,
    0xED, 0xAA, 0xBA, 0x75, 0x99, 0x51, 0x08, 0xE8, 0x6A, 0xA9, 0xE3, 0x02, 0xF7, 0x29, 0xCB, 0xED
};

static uint8_t cmac256Cmac0Bytes[8] =
{
    0xB9, 0xB5, 0xAF, 0x31, 0xCE, 0x4A, 0x8E, 0x8F
};

static const mac_test_vector_t cmac256Vector0Bytes =
{
    0,
    (uint8_t[]) {},
    0U,
    (uint8_t *) &cmac256Key0Bytes,
    sizeof(cmac256Key0Bytes),
    (uint8_t *) &cmac256Cmac0Bytes,
    sizeof(cmac256Cmac0Bytes)
};

// CMAC AES-256, 15-byte message
static uint8_t cmac256Key15Bytes[32] =
{
    0x7B, 0x35, 0x2C, 0x47, 0xA4, 0x71, 0x89, 0x38, 0xF8, 0x42, 0xF6, 0x92, 0x46, 0x84, 0xED, 0xB8,
    0xF6, 0x51, 0x2F, 0x33, 0xF9, 0x24, 0x5C, 0xFA, 0x00, 0x3E, 0xC1, 0xF0, 0xEC, 0x49, 0x71, 0xAF
};

static uint8_t cmac256Message15Bytes[15] =
{
    0xE4, 0x8D, 0xFA, 0xA5, 0x3B, 0x68, 0x07, 0xEA, 0x6F, 0x01, 0xD8, 0xDC, 0xA6, 0x79, 0x60
};

static uint8_t cmac256Cmac15Bytes[10] =
{
    0x1B, 0x58, 0x18, 0xE1, 0x3A, 0x62, 0x9C, 0xAF, 0x72, 0x8B
};

static const mac_test_vector_t cmac256Vector15Bytes =
{
    0,
    cmac256Message15Bytes,
    sizeof(cmac256Message15Bytes),
    cmac256Key15Bytes,
    sizeof(cmac256Key15Bytes),
    cmac256Cmac15Bytes,
    sizeof(cmac256Cmac15Bytes)
};

// CMAC AES-256, 16-byte message
static uint8_t cmac256Key16Bytes[32] =
{
    0x20, 0x22, 0x5E, 0x11, 0x31, 0xDD, 0xA7, 0x0E, 0xF8, 0x88, 0xDD, 0xFC, 0xB7, 0xF4, 0xE7, 0x40,
    0x0C, 0xCF, 0xFF, 0x34, 0xBB, 0x63, 0xA0, 0x79, 0xAE, 0x81, 0xA6, 0x8F, 0x3A, 0xB7, 0xB1, 0xA2
};

static uint8_t cmac256Message16Bytes[16] =
{
    0xA2, 0x33, 0xFE, 0xD4, 0x9F, 0x86, 0x6B, 0xA3, 0x2F, 0x1C, 0x14, 0xA5, 0x7B, 0x8E, 0x05, 0x42
};

static uint8_t cmac256Cmac16Bytes[10] =
{
    0x95, 0x6F, 0x17, 0xE8, 0x30, 0x94, 0xB9, 0xD7, 0x62, 0xCA
};

static const mac_test_vector_t cmac256Vector16Bytes =
{
    0,
    cmac256Message16Bytes,
    sizeof(cmac256Message16Bytes),
    cmac256Key16Bytes,
    sizeof(cmac256Key16Bytes),
    cmac256Cmac16Bytes,
    sizeof(cmac256Cmac16Bytes)
};

// CMAC AES-256, 48-byte message
static uint8_t cmac256Key48Bytes[32] =
{
    0xDC, 0xBA, 0x2C, 0xE0, 0x16, 0x33, 0x93, 0x7B, 0x1C, 0xDA, 0xB1, 0x2B, 0x2E, 0x83, 0x59, 0x8A,
    0x49, 0xC5, 0x16, 0x09, 0xEF, 0xAE, 0x0F, 0x40, 0x26, 0xB6, 0x2D, 0x82, 0xC3, 0xF2, 0x80, 0xB5
};

static uint8_t cmac256Message48Bytes[48] =
{
    0x9A, 0xA3, 0xE8, 0xAD, 0x92, 0x77, 0x7D, 0xFE, 0xB1, 0x21, 0xA6, 0x46, 0xCE, 0x2E, 0x91, 0x8D,
    0x1E, 0x12, 0xB3, 0x07, 0x54, 0xBC, 0x09, 0x47, 0x0D, 0x6D, 0xA4, 0xAF, 0x6C, 0xC9, 0x64, 0x2B,
    0x01, 0x2F, 0x04, 0x1F, 0xF0, 0x46, 0x56, 0x9D, 0x4F, 0xD8, 0xD0, 0xDC, 0xCF, 0xE4, 0x48, 0xE5
};

static uint8_t cmac256Cmac48Bytes[5] =
{
    0x81, 0x62, 0x82, 0xFB, 0x33
};

static const mac_test_vector_t cmac256Vector48Bytes =
{
    0,
    cmac256Message48Bytes,
    sizeof(cmac256Message48Bytes),
    cmac256Key48Bytes,
    sizeof(cmac256Key48Bytes),
    cmac256Cmac48Bytes,
    sizeof(cmac256Cmac48Bytes)
};

// 4 CMAC AES-256 test vectors (0-byte, 15-byte, 16-byte, 48-byte messages)
static mac_test_vector_t cmacTestVectors[4] =
{
    [0] = cmac256Vector0Bytes,
    [1] = cmac256Vector15Bytes,
    [2] = cmac256Vector16Bytes,
    [3] = cmac256Vector48Bytes,
};

mac_test_vector_t * VECTOR_CmacTestVectorsGet(void)
{
    // Set test IDs.
    for (uint32_t index = 0U; index < VECTOR_CmacTestVectorCountGet(); index++)
    {
        cmacTestVectors[index].id = (index + 1UL);
    }

    return cmacTestVectors;
}

uint32_t VECTOR_CmacTestVectorCountGet(void)
{
    return sizeof(cmacTestVectors) / sizeof(cmacTestVectors[0]);
}
