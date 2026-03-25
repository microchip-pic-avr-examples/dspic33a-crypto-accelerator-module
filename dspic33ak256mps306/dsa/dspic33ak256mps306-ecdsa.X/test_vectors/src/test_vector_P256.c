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
 * Test vectors from NIST Digital Signature Standard (DSS) FIPS 186-4:
 * https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/digital-signatures
 */

static uint8_t privateKeyP256[32] =
{
    0xC9, 0x80, 0x68, 0x98, 0xA0, 0x33, 0x49, 0x16, 0xC8, 0x60, 0x74, 0x88, 0x80, 0xA5, 0x41, 0xF0,
    0x93, 0xB5, 0x79, 0xA9, 0xB1, 0xF3, 0x29, 0x34, 0xD8, 0x6C, 0x36, 0x3C, 0x39, 0x80, 0x03, 0x57
};

static uint8_t publicKeyP256[65] =
{
    0x04, 0xD0, 0x72, 0x0D, 0xC6, 0x91, 0xAA, 0x80, 0x09, 0x6B, 0xA3, 0x2F, 0xED, 0x1C, 0xB9, 0x7C,
    0x2B, 0x62, 0x06, 0x90, 0xD0, 0x6D, 0xE0, 0x31, 0x7B, 0x86, 0x18, 0xD5, 0xCE, 0x65, 0xEB, 0x72,
    0x8F, 0x96, 0x81, 0xB5, 0x17, 0xB1, 0xCD, 0xA1, 0x7D, 0x0D, 0x83, 0xD3, 0x35, 0xD9, 0xC4, 0xA8,
    0xA9, 0xA9, 0xB0, 0xB1, 0xB3, 0xC7, 0x10, 0x6D, 0x8F, 0x3C, 0x72, 0xBC, 0x50, 0x93, 0xDC, 0x27,
    0x5F
};

static uint8_t hashP256[32] =
{
    0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
    0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD
};

static const char CURVE_INFORMATION_P256[] = "P-256";

test_vector_t ecdsaP256 =
{
    privateKeyP256,
    sizeof(privateKeyP256),
    publicKeyP256,
    sizeof(publicKeyP256),
    hashP256,
    sizeof(hashP256),
    CURVE_INFORMATION_P256,
    CRYPTO_ECC_CURVE_P256,
};
