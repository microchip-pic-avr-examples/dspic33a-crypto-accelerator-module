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

#ifndef SYM_TEST_VECTOR_H
#define	SYM_TEST_VECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct TEST_VECTOR {
    uint32_t    id;
    uint32_t    mode;

    uint8_t *   ptData;
    uint8_t *   ctData;
    uint32_t    dataLength;

    uint8_t *   key;
    uint32_t    keyLength;

    uint8_t *   tweak;
    uint32_t    tweakLength;

    uint8_t *   initializationVector;
    uint32_t    initializationVectorLength;
} TEST_VECTOR;

TEST_VECTOR * get_ecb_test_vectors(void);
uint32_t get_ecb_test_vector_count(void);

TEST_VECTOR * get_ctr_test_vectors(void);
uint32_t get_ctr_test_vector_count(void);

TEST_VECTOR * get_xts_test_vectors(void);
uint32_t get_xts_test_vector_count(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAC_TEST_VECTOR_H */
