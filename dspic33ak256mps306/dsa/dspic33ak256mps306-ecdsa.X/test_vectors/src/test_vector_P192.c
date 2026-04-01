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

static uint8_t privateKeyP192[24] =
{
    0xE5, 0xCE, 0x89, 0xA3, 0x4A, 0xDD, 0xDF, 0x25, 0xFF, 0x3B, 0xF1, 0xFF, 0xE6, 0x80, 0x3F, 0x57,
    0xD0, 0x22, 0x0D, 0xE3, 0x11, 0x87, 0x98, 0xEA
};

static uint8_t publicKeyP192[49] =
{
    0x04, 0x8A, 0xBF, 0x7B, 0x3C, 0xEB, 0x2B, 0x02, 0x43, 0x8A, 0xF1, 0x95, 0x43, 0xD3, 0xE5, 0xB1,
    0xD5, 0x73, 0xFA, 0x9A, 0xC6, 0x00, 0x85, 0x84, 0x0F, 0xA8, 0x7F, 0x80, 0x18, 0x2D, 0xCD, 0x56,
    0xA6, 0xA0, 0x61, 0xF8, 0x1F, 0x7D, 0xA3, 0x93, 0xE7, 0xCF, 0xFD, 0x5E, 0x07, 0x38, 0xC6, 0xB2,
    0x45
};

static uint8_t hashP192[32] =
{
    0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
    0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD
};

static const char CURVE_INFORMATION_P192[] = "P-192";

test_vector_t ecdsaP192 =
{
    privateKeyP192,
    sizeof(privateKeyP192),
    publicKeyP192,
    sizeof(publicKeyP192),
    hashP192,
    sizeof(hashP192),
    CURVE_INFORMATION_P192,
    CRYPTO_ECC_CURVE_P192,
};
