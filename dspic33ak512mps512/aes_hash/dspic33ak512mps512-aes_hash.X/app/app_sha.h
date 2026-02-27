/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.
*******************************************************************************/

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

#ifndef APP_SHA_H
#define	APP_SHA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "app_aes_hash.h"
#include "crypto/common_crypto/crypto_hash.h"

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initialize a HASH text context.
 * @param context The test context block to use.
 * @param mode The operation mode (hash).
 * @param algorithm The hash algorithm to use.
 * @return TEST_RESULT_SUCCESS if initialized, TEST_RESULT_INIT_FAILED if not.
 * @note Do not proceed with the test if the initiaolization result is a failure.
 */
TEST_RESULT hash_test_init(TEST_CONTEXT *context, TEST_MODE mode, crypto_Hash_Algo_E algorithm, uint32_t blockSize);

/**
 * @brief Advance the HASH test state machine.
 * @param context The test context block to use.
 * @return TEST_RESULT_SUCCESS if the current step was successful, error on failure,
 *         and TEST_RESULT_ALL_TESTS_DONE when all tests have been executed (regardless
 *         of success).
 */
TEST_RESULT hash_test_advance(TEST_CONTEXT *context);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_SHA_H */

