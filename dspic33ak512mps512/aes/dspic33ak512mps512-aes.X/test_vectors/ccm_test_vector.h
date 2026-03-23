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

#ifndef CCM_TEST_VECTOR_H
#define CCM_TEST_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Maximum test vector data size.
#define CCM_TEST_VECTOR_SIZE    (96U)

#define KEY_TYPE_AES128_SIZE    (16UL)
#define KEY_TYPE_AES256_SIZE    (32UL)
#define KEY_TYPE_MAX_SIZE       KEY_TYPE_AES256_SIZE

#define AES_CCM_TAG_LENGTH      (32UL)

typedef struct ccm_test_vector_t
{
    uint8_t *key;
    uint8_t *nonce;
    uint8_t *aad;
    uint8_t *plaintext;
    uint8_t *ciphertext;
    uint8_t *tag;

    uint32_t keyLength;
    uint32_t textLength;
    uint32_t nonceLength;
    uint32_t aadLength;
    uint32_t tagLength;

} ccm_test_vector_t;

/**
 * @brief Get the array of AES-CCM test vectors.
 * @return Pointer to the first CCM test vector.
 */
ccm_test_vector_t *VECTOR_CcmTestVectorsGet(void);

/**
 * @brief Get the number of AES-CCM test vectors.
 * @return Number of CCM test vectors.
 */
uint32_t VECTOR_CcmTestCountGet(void);

#ifdef __cplusplus
}
#endif

#endif /* CCM_TEST_VECTOR_H */
