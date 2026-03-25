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

#include "test_vectors/test_vector.h"

/*
 * Test vectors from NIST SP 800-56A (ECDH) validation:
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/key-management
 */

static uint8_t privateKeyP192[24] =
{
    0xF1, 0x7D, 0x3F, 0xEA, 0x36, 0x7B, 0x74, 0xD3, 0x40, 0x85, 0x1C, 0xA4, 0x27, 0x0D, 0xCB, 0x24,
    0xC2, 0x71, 0xF4, 0x45, 0xBE, 0xD9, 0xD5, 0x27
};

static uint8_t publicKeyP192[49] =
{
    0x04, 0x42, 0xEA, 0x6D, 0xD9, 0x96, 0x9D, 0xD2, 0xA6, 0x1F, 0xEA, 0x1A, 0xAC, 0x7F, 0x8E, 0x98,
    0xED, 0xCC, 0x89, 0x6C, 0x6E, 0x55, 0x85, 0x7C, 0xC0, 0xDF, 0xBE, 0x5D, 0x7C, 0x61, 0xFA, 0xC8,
    0x8B, 0x11, 0x81, 0x1B, 0xDE, 0x32, 0x8E, 0x8A, 0x0D, 0x12, 0xBF, 0x01, 0xA9, 0xD2, 0x04, 0xB5,
    0x23
};

static uint8_t expectedSharedSecretP192[24] =
{
    0x80, 0x3D, 0x8A, 0xB2, 0xE5, 0xB6, 0xE6, 0xFC, 0xA7, 0x15, 0x73, 0x7C, 0x3A, 0x82, 0xF7, 0xCE,
    0x3C, 0x78, 0x31, 0x24, 0xF6, 0xD5, 0x1C, 0xD0
};

static const char CURVE_INFORMATION_P192[] = "P-192";

ecdh_test_vector_t ecdhP192 =
{
    privateKeyP192,
    sizeof(privateKeyP192),
    publicKeyP192,
    sizeof(publicKeyP192),
    expectedSharedSecretP192,
    sizeof(expectedSharedSecretP192),
    CURVE_INFORMATION_P192,
    CRYPTO_ECC_CURVE_P192,
};
