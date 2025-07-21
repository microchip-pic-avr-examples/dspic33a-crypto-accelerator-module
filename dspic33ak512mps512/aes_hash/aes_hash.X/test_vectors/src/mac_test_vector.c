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

#include "test_vectors/mac_test_vector.h"

/*
 The following test vectors were pulled from NIST's Block Cipher Modes SP 800-38B test vectors:
 https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/cavp-testing-block-cipher-modes
*/

// CMAC Key size 256 with 0 Byte Message Test Vector
static uint8_t cmac_256_0_bytes_key[] = {
    0x7F, 0x1E, 0x1F, 0x76, 0x27, 0x3A, 0xEC, 0xB3, 0x64, 0x53, 0xB4, 0x24, 0x5D, 0xA4, 0x7C, 0x8A,
    0xED, 0xAA, 0xBA, 0x75, 0x99, 0x51, 0x08, 0xE8, 0x6A, 0xA9, 0xE3, 0x02, 0xF7, 0x29, 0xCB, 0xED
};

static uint8_t cmac_256_0_bytes_cmac[] = {
    0xB9, 0xB5, 0xAF, 0x31, 0xCE, 0x4A, 0x8E, 0x8F
};

static const TEST_VECTOR cmac_256_0_bytes_vector = {
    0,
    (uint8_t*) 0,
    0,
    (uint8_t*) &cmac_256_0_bytes_key,
    sizeof(cmac_256_0_bytes_key),
    (uint8_t*) &cmac_256_0_bytes_cmac,
    sizeof(cmac_256_0_bytes_cmac)
};

// CMAC Key size 256 with 15 Byte Message Test Vector
static uint8_t cmac_256_15_bytes_key[] = {
    0x7b, 0x35, 0x2c, 0x47, 0xa4, 0x71, 0x89, 0x38, 0xf8, 0x42, 0xf6, 0x92,
    0x46, 0x84, 0xed, 0xb8, 0xf6, 0x51, 0x2f, 0x33, 0xf9, 0x24, 0x5c, 0xfa,
    0x00, 0x3e, 0xc1, 0xf0, 0xec, 0x49, 0x71, 0xaf
};

static uint8_t cmac_256_15_bytes_message[] = {
    0xe4, 0x8d, 0xfa, 0xa5, 0x3b, 0x68, 0x07, 0xea, 0x6f, 0x01, 0xd8, 0xdc,
    0xa6, 0x79, 0x60
};

static uint8_t cmac_256_15_bytes_cmac[] = {
    0x1b, 0x58, 0x18, 0xe1, 0x3a, 0x62, 0x9c, 0xaf, 0x72, 0x8b
};

static const TEST_VECTOR cmac_256_15_bytes_vector = {
    0,
    (uint8_t*) &cmac_256_15_bytes_message,
    sizeof(cmac_256_15_bytes_message),
    (uint8_t*) &cmac_256_15_bytes_key,
    sizeof(cmac_256_15_bytes_key),
    (uint8_t*) &cmac_256_15_bytes_cmac,
    sizeof(cmac_256_15_bytes_cmac)
};

// CMAC Key size 256 with 16 Byte Message Test Vector
static uint8_t cmac_256_16_bytes_key[] = {
    0x20, 0x22, 0x5e, 0x11, 0x31, 0xdd, 0xa7, 0x0e, 0xf8, 0x88, 0xdd, 0xfc,
    0xb7, 0xf4, 0xe7, 0x40, 0x0c, 0xcf, 0xff, 0x34, 0xbb, 0x63, 0xa0, 0x79,
    0xae, 0x81, 0xa6, 0x8f, 0x3a, 0xb7, 0xb1, 0xa2
};

static uint8_t cmac_256_16_bytes_message[] = {
    0xa2, 0x33, 0xfe, 0xd4, 0x9f, 0x86, 0x6b, 0xa3, 0x2f, 0x1c, 0x14, 0xa5,
    0x7b, 0x8e, 0x05, 0x42
};

static uint8_t cmac_256_16_bytes_cmac[] = {
    0x95, 0x6f, 0x17, 0xe8, 0x30, 0x94, 0xb9, 0xd7, 0x62, 0xca
};

static const TEST_VECTOR cmac_256_16_bytes_vector = {
    0,
    (uint8_t*) &cmac_256_16_bytes_message,
    sizeof(cmac_256_16_bytes_message),
    (uint8_t*) &cmac_256_16_bytes_key,
    sizeof(cmac_256_16_bytes_key),
    (uint8_t*) &cmac_256_16_bytes_cmac,
    sizeof(cmac_256_16_bytes_cmac)
};

// CMAC Key size 256 with 48 Byte Message Test Vector
static uint8_t cmac_256_48_bytes_key[] = {
    0xdc, 0xba, 0x2c, 0xe0, 0x16, 0x33, 0x93, 0x7b, 0x1c, 0xda, 0xb1, 0x2b,
    0x2e, 0x83, 0x59, 0x8a, 0x49, 0xc5, 0x16, 0x09, 0xef, 0xae, 0x0f, 0x40,
    0x26, 0xb6, 0x2d, 0x82, 0xc3, 0xf2, 0x80, 0xb5
};

static uint8_t cmac_256_48_bytes_message[] = {
    0x9a, 0xa3, 0xe8, 0xad, 0x92, 0x77, 0x7d, 0xfe, 0xb1, 0x21, 0xa6, 0x46,
    0xce, 0x2e, 0x91, 0x8d, 0x1e, 0x12, 0xb3, 0x07, 0x54, 0xbc, 0x09, 0x47,
    0x0d, 0x6d, 0xa4, 0xaf, 0x6c, 0xc9, 0x64, 0x2b, 0x01, 0x2f, 0x04, 0x1f,
    0xf0, 0x46, 0x56, 0x9d, 0x4f, 0xd8, 0xd0, 0xdc, 0xcf, 0xe4, 0x48, 0xe5
};

static uint8_t cmac_256_48_bytes_cmac[] = {
    0x81, 0x62, 0x82, 0xfb, 0x33
};

static const TEST_VECTOR cmac_256_48_bytes_vector = {
    0,
    (uint8_t*) &cmac_256_48_bytes_message,
    sizeof(cmac_256_48_bytes_message),
    (uint8_t*) &cmac_256_48_bytes_key,
    sizeof(cmac_256_48_bytes_key),
    (uint8_t*) &cmac_256_48_bytes_cmac,
    sizeof(cmac_256_48_bytes_cmac)
};


TEST_VECTOR cmac_test_vectors[] = {
    cmac_256_0_bytes_vector,
    cmac_256_15_bytes_vector,
    cmac_256_16_bytes_vector,
    cmac_256_48_bytes_vector,
};

TEST_VECTOR * get_cmac_test_vectors(void)
{
    // Set test IDs.
    for (uint32_t i = 0; i < get_cmac_test_vector_count(); i++)
    {
        cmac_test_vectors[i].id = (i + 1UL);
    }

    return cmac_test_vectors;
}

uint32_t get_cmac_test_vector_count(void)
{
    return sizeof(cmac_test_vectors) / sizeof(cmac_test_vectors[0]);
}
