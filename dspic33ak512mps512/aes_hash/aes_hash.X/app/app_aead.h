/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_aead.h

  Summary:
    This header file provides prototypes and definitions for AEAD AES
    algorithms in the application.
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

#ifndef APP_AEAD_H
#define	APP_AEAD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "app_aes_hash.h"

// *****************************************************************************
// *****************************************************************************
// Section: Configuration Options
// *****************************************************************************
// *****************************************************************************

#define RUN_AEAD_GCM_TEST

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef enum AEAD_OPER_MODE
{
  AEAD_OPER_MODE_AES_GCM = 0,
  AEAD_OPER_MODE_AES_CCM = 1,
  AEAD_OPER_MODE_AES_XTS = 2
} AEAD_OPER_MODE;

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initialize an AEAD GCM text context.
 * @param context The test context block to use.
 * @param testMode The operation mode (encrypt/decrypt).
 * @param opMode The AEAD test operation mode.
 * @return TEST_RESULT_SUCCESS if initialized, TEST_RESULT_INIT_FAILED if not.
 * @note Do not proceed with the test if the initiaolization result is a failure.
 */
TEST_RESULT aead_test_init(TEST_CONTEXT *context, TEST_MODE mode, AEAD_OPER_MODE opMode);

/**
 * @brief Advance the AEAD test state machine.
 * @param context The test context block to use.
 * @return TEST_RESULT_SUCCESS if the current step was successful, error on failure,
 *         and TEST_RESULT_ALL_TESTS_DONE when all tests have been executed (regardless
 *         of success).
 */
TEST_RESULT aead_test_advance(TEST_CONTEXT *context);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_AEAD_H */

