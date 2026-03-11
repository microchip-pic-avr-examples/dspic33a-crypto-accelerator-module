/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sym.h

  Summary:
    This header file provides prototypes and definitions for symmetric AES
    algorithms in the application.
*******************************************************************************/

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

#ifndef APP_SYM_H
#define APP_SYM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app_aes_hash.h"
#include "test_vectors/sym_test_vector.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// Maximum size of the result data.
#define DATA_RESULT_SIZE    (48U)

typedef struct
{
    // Pointer to current test vector data.
    sym_test_vector_t *test;

    // Contains the data result.
    uint8_t dataResult[DATA_RESULT_SIZE];

    // The test mode (encrypt/decrypt).
    test_mode_t testMode;

    // The symmetric encryption type.
    crypto_Sym_OpModes_E opMode;

} sym_test_context_t;

// *****************************************************************************
// *****************************************************************************
// Section: Configuration Options
// *****************************************************************************
// *****************************************************************************

#define RUN_SYM_ECB_TEST
#define RUN_SYM_CTR_TEST
#define RUN_SYM_CBC_TEST
#define RUN_SYM_CFB128_TEST
#define RUN_SYM_OFB_TEST
#define RUN_SYM_XTS_TEST

typedef enum
{
    SYM_OPER_MODE_AES_ECB = 0,
    SYM_OPER_MODE_AES_CTR = 1,
    SYM_OPER_MODE_AES_CBC = 2,
    SYM_OPER_MODE_AES_CFB128 = 3,
    SYM_OPER_MODE_AES_OFB = 4,
    SYM_OPER_MODE_AES_XTS = 5,
    SYM_OPER_MODE_COUNT = 6
} sym_oper_mode_t;

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initialize an AES symmetric text context.
 * @param context The test context block to use.
 * @param testMode The operation mode (encrypt/decrypt).
 * @param opMode The symmetric test operation mode.
 * @return TEST_RESULT_SUCCESS if initialized, TEST_RESULT_INIT_FAILED if not.
 * @note Do not proceed with the test if the initiaolization result is a failure.
 */
test_result_t SYM_ContextInit(test_context_t *context, test_mode_t testMode,
        sym_oper_mode_t opMode);

/**
 * @brief Advance the AES symmetric test state machine.
 * @param context The test context block to use.
 * @return TEST_RESULT_SUCCESS if the current step was successful, error on failure,
 *         and TEST_RESULT_ALL_TESTS_DONE when all tests have been executed (regardless
 *         of success).
 */
test_result_t SYM_TestAdvance(test_context_t *context);

#ifdef __cplusplus
}
#endif

#endif /* APP_SYM_H */

