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

#ifndef XTS_TEST_VECTOR_H
#define XTS_TEST_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Maximum test vector data size.
#define XTS_TEST_VECTOR_SIZE    (96U)
#define XTS_TWEAK_SIZE          (16U)

#define KEY_TYPE_AES128_SIZE    (16UL)
#define KEY_TYPE_AES256_SIZE    (32UL)
#define KEY_TYPE_MAX_SIZE       KEY_TYPE_AES256_SIZE

typedef struct xts_test_vector_t
{
    uint8_t *key;
    uint8_t *key2;
    uint8_t *plaintext;
    uint8_t *ciphertext;
    uint8_t *tweak;

    uint32_t keyLength;
    uint32_t textLength;

} xts_test_vector_t;

xts_test_vector_t *VECTOR_XtsTestVectorsGet(void);

uint32_t VECTOR_XtsTestCountGet(void);

#ifdef __cplusplus
}
#endif

#endif /* XTS_TEST_VECTOR_H */
