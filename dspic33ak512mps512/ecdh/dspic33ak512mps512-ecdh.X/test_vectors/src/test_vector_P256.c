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

static uint8_t privateKeyP256[32] =
{
    0x7D, 0x7D, 0xC5, 0xF7, 0x1E, 0xB2, 0x9D, 0xDA, 0xF8, 0x0D, 0x62, 0x14, 0x63, 0x2E, 0xEA, 0xE0,
    0x3D, 0x90, 0x58, 0xAF, 0x1F, 0xB6, 0xD2, 0x2E, 0xD8, 0x0B, 0xAD, 0xB6, 0x2B, 0xC1, 0xA5, 0x34
};

static uint8_t publicKeyP256[65] =
{
    0x04, 0x70, 0x0C, 0x48, 0xF7, 0x7F, 0x56, 0x58, 0x4C, 0x5C, 0xC6, 0x32, 0xCA, 0x65, 0x64, 0x0D,
    0xB9, 0x1B, 0x6B, 0xAC, 0xCE, 0x3A, 0x4D, 0xF6, 0xB4, 0x2C, 0xE7, 0xCC, 0x83, 0x88, 0x33, 0xD2,
    0x87, 0xDB, 0x71, 0xE5, 0x09, 0xE3, 0xFD, 0x9B, 0x06, 0x0D, 0xDB, 0x20, 0xBA, 0x5C, 0x51, 0xDC,
    0xC5, 0x94, 0x8D, 0x46, 0xFB, 0xF6, 0x40, 0xDF, 0xE0, 0x44, 0x17, 0x82, 0xCA, 0xB8, 0x5F, 0xA4,
    0xAC
};

static uint8_t expectedSharedSecretP256[32] =
{
    0x46, 0xFC, 0x62, 0x10, 0x64, 0x20, 0xFF, 0x01, 0x2E, 0x54, 0xA4, 0x34, 0xFB, 0xDD, 0x2D, 0x25,
    0xCC, 0xC5, 0x85, 0x20, 0x60, 0x56, 0x1E, 0x68, 0x04, 0x0D, 0xD7, 0x77, 0x89, 0x97, 0xBD, 0x7B
};

static const char CURVE_INFORMATION_P256[] = "P-256";

ecdh_test_vector_t ecdhP256 =
{
    privateKeyP256,
    sizeof(privateKeyP256),
    publicKeyP256,
    sizeof(publicKeyP256),
    expectedSharedSecretP256,
    sizeof(expectedSharedSecretP256),
    CURVE_INFORMATION_P256,
    CRYPTO_ECC_CURVE_P256,
};
