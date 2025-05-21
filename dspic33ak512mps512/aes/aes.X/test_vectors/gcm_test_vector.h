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

#ifndef GCM_TEST_VECTOR_H
#define	GCM_TEST_VECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// Maximum test vector data size.
#define GCM_TEST_VECTOR_SZ      (96u)

#define KEY_TYPE_AES128_SIZE    (16UL)
#define KEY_TYPE_AES192_SIZE    (24UL)
#define KEY_TYPE_AES256_SIZE    (32UL)
#define KEY_TYPE_MAX_SIZE       KEY_TYPE_AES256_SIZE

#define AES_GCM_TAG_LENGTH      (16UL)

typedef struct TEST_VECTOR {
    uint8_t key[KEY_TYPE_MAX_SIZE];
    uint8_t iv[GCM_TEST_VECTOR_SZ];
    uint8_t aad[GCM_TEST_VECTOR_SZ];
    uint8_t pt[GCM_TEST_VECTOR_SZ];
    uint8_t ct[GCM_TEST_VECTOR_SZ];
    uint8_t tag[AES_GCM_TAG_LENGTH];

    uint32_t keylength;
    uint32_t ptlength;
    uint32_t ivlength;
    uint32_t aadlength;

} TEST_VECTOR;

TEST_VECTOR * get_gcm_test_vectors(void);
uint32_t get_gcm_test_count(void);

#ifdef	__cplusplus
}
#endif

#endif	/* GCM_TEST_VECTOR_H */
