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

#ifndef TEST_VECTORS_SHA_H
#define	TEST_VECTORS_SHA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

// Size of a SHA-512 digest.
#define SHA512_DIGEST_LEN_BYTES     (64U)

typedef struct
{
    uint32_t    id;

    uint8_t *   inputData;
    uint32_t    inputDataLength;

    uint8_t *   expectedDigest;
    uint32_t    expectedDigestLength;
} TEST_VECTOR;

TEST_VECTOR * get_sha_512_test_vectors(void);
uint32_t get_sha_512_test_vector_count(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TEST_VECTORS_SHA512_H */

