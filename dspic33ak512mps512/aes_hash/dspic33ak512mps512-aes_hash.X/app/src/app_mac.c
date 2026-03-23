/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_mac.c

  Summary:
    This source file handles the running of the AES MAC examples.
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

/* cppcheck-suppress misra-c2012-21.6
*
*  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
*
*  Reasoning: printf is required for printing to the terminal in this code example
*/
#include <stdio.h>
#include <string.h>
#include "app/app_aes_hash.h"
#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "app/app_mac.h"

// Context for CMAC calculations.
static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4)));

/**
 * @brief Get the test operation mode string based on the mode.
 * @param opMode The MAC operations mode.
 * @return Pointer to string.
 */
static const char *MAC_TestOpModeGet(mac_oper_mode_t opMode)
{
    const char *opModeString = "???";

    switch (opMode)
    {
        case MAC_OPER_MODE_AES_CMAC:
            opModeString = "AES-CMAC";
            break;

        default:
            break;
    }

    return opModeString;
}

// *****************************************************************************
// *****************************************************************************
// Section: MAC CMAC Test State Machine Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief The MAC CMAC test init state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_INIT_FAILED on failure.
 */
static test_result_t MAC_CmacTestStepInit(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    aes_cmac_test_context_t *testContext = (aes_cmac_test_context_t *)context;
    mac_test_vector_t *test = testContext->test;

    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    test_result_t result;

#ifdef SHOW_TEST_DATA
    (void) printf(UTILS_BLUE "\r\n\r\n %s %s Test %lu",
            MAC_TestOpModeGet(testContext->opMode),
            AESHASH_TestModeGet(testContext->testMode),
            (unsigned long) test->id);

    UTILS_SeparatorPrint(UTILS_BLUE, '-');
    UTILS_HexArrayPrint("Key", test->inputKey, test->inputKeyLength);
    UTILS_SeparatorPrint(UTILS_BLUE, '-');
    UTILS_HexArrayPrint("Data (Input)", test->inputData,
            test->inputDataLength);
    UTILS_SeparatorPrint(UTILS_BLUE, '-');
#endif

    status = Crypto_Mac_AesCmac_Init(&macCtx,
            CRYPTO_HANDLER_HW_INTERNAL, test->inputKey,
            test->inputKeyLength, UTILS_SESSION_ID);
    result = (status == CRYPTO_MAC_CIPHER_SUCCESS)
            ? TEST_RESULT_SUCCESS : TEST_RESULT_INIT_FAILED;

    (void) printf(UTILS_MAG);
    AESHASH_OperationStepPrint("INITIALIZING",
            MAC_TestOpModeGet(testContext->opMode),
            testContext->testMode,
            test->inputDataLength, true);
    AESHASH_CryptoResultPrint(result, status);

    return result;
}

/**
 * @brief The MAC CMAC test cipher state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_CIPHER_FAILED on failure.
 */
static test_result_t MAC_CmacTestCipherRun(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    aes_cmac_test_context_t *testContext = (aes_cmac_test_context_t *)context;
    mac_test_vector_t *test = testContext->test;

    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    test_result_t result;

    (void) printf(UTILS_MAG);
    AESHASH_OperationStepPrint("CIPHERING",
            MAC_TestOpModeGet(testContext->opMode),
            testContext->testMode,
            test->inputDataLength, true);

    status = Crypto_Mac_AesCmac_Cipher(&macCtx,
            test->inputData, test->inputDataLength);
    result = (status == CRYPTO_MAC_CIPHER_SUCCESS)
            ? TEST_RESULT_SUCCESS : TEST_RESULT_CIPHER_FAILED;

    AESHASH_CryptoResultPrint(result, status);

    return result;
}

/**
 * @brief The MAC CMAC test finalize/compare state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success,
 *         TEST_RESULT_FINALIZE_FAILED/TEST_RESULT_DATA_COMPARE_FAILED/TEST_RESULT_TAG_COMPARE_FAILED on failure.
 */
static test_result_t MAC_CmacTestFinalizeRun(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    aes_cmac_test_context_t *testContext = (aes_cmac_test_context_t *)context;
    mac_test_vector_t *test = testContext->test;

    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    test_result_t result = TEST_RESULT_FINALIZE_FAILED;

    (void) printf(UTILS_MAG);
    AESHASH_OperationStepPrint("FINALIZING",
            MAC_TestOpModeGet(testContext->opMode),
            testContext->testMode,
            test->inputDataLength, true);

    status = Crypto_Mac_AesCmac_Final(&macCtx,
            testContext->dataResult,
            test->expectedMacLength);
    result = (status == CRYPTO_MAC_CIPHER_SUCCESS)
            ? TEST_RESULT_SUCCESS
            : TEST_RESULT_FINALIZE_FAILED;

    AESHASH_CryptoResultPrint(result, status);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
#ifdef SHOW_TEST_DATA
        UTILS_SeparatorPrint(UTILS_BLUE, '-');
        AESHASH_LabelPrint("Result Length", "%d bytes", test->expectedMacLength);
        UTILS_HexArrayPrint("Data (Result)", testContext->dataResult, test->expectedMacLength);
        UTILS_SeparatorPrint(UTILS_BLUE, '-');
#endif
        (void) printf(UTILS_MAG);
        AESHASH_OperationStepPrint("VERIFYING",
                MAC_TestOpModeGet(testContext->opMode),
                testContext->testMode,
                test->inputDataLength, true);

        if (true == UTILS_ArrayEqualityCheck(testContext->dataResult,
                test->expectedMac,
                test->expectedMacLength))
        {
            result = TEST_RESULT_SUCCESS;
        }
        else
        {
            result = TEST_RESULT_DATA_COMPARE_FAILED;
        }

        AESHASH_CryptoResultPrint(result, status);

        if (result != TEST_RESULT_SUCCESS)
        {
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
            UTILS_HexArrayPrint("Expected:", test->expectedMac, test->expectedMacLength);
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
            UTILS_HexArrayPrint("Result:", testContext->dataResult, test->expectedMacLength);
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
        }
    }

    return result;
}

/**
 * @brief Defines the states for a AES CMAC test.
 */
enum
{
    AES_CMAC_TEST_STEP_INIT = 0,
    AES_CMAC_TEST_STEP_CIPHER = 1,
    AES_CMAC_TEST_STEP_FINAL = 2,
    AES_CMAC_TEST_STEP_NUMBER_OF_STEPS
};

// *****************************************************************************
// *****************************************************************************
// Section: MAC CMAC Test Functions
// *****************************************************************************
// *****************************************************************************

test_result_t MAC_CmacContextInit(test_context_t *context,
        test_mode_t testMode, mac_oper_mode_t opMode)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    aes_cmac_test_context_t *testContext = (aes_cmac_test_context_t *)context->testContext;
    test_result_t result = TEST_RESULT_SUCCESS;

    static test_state_fn_t aes_cmac_test_states[] =
    {
        MAC_CmacTestStepInit,
        MAC_CmacTestCipherRun,
        MAC_CmacTestFinalizeRun,
    };

    (void) memset(context, 0, sizeof(test_context_t));
    context->testContext = testContext;

    switch (opMode)
    {
        case MAC_OPER_MODE_AES_CMAC:
        {
            context->states = aes_cmac_test_states;
            context->tests = (uint8_t *)VECTOR_CmacTestVectorsGet();
            context->testCount = VECTOR_CmacTestVectorCountGet();
            context->stepCount = sizeof(aes_cmac_test_states) / sizeof(aes_cmac_test_states[0]);
        }
        break;

        default:
        {
            (void) printf(UTILS_RED
                    "\r\n\r\n Unknown MAC test operation mode %d"
                    UTILS_RESET_COLOR, opMode);
            result = TEST_RESULT_INIT_FAILED;
        }
        break;
    }

    if (TEST_RESULT_SUCCESS == result)
    {
        context->testMode = testMode;
        testContext->testMode = context->testMode;
        testContext->opMode = opMode;

        context->test = 0;
        context->step = 0;

        /* cppcheck-suppress misra-c2012-11.5
         *
         *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
         *
         *  Reasoning: A generic pointer allows support for various algorithm test context types.
         */
        mac_test_vector_t *tests = (mac_test_vector_t *)context->tests;

        // Start at the first test.
        testContext->test = &tests[context->test];
    }

    return result;
}

test_result_t MAC_CmacTestAdvance(test_context_t *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    aes_cmac_test_context_t *testContext = (aes_cmac_test_context_t *)context->testContext;

    if (context->test < context->testCount)
    {
        // Execute the next step in the state machine.
        context->lastResult = context->states[context->step](testContext);
        if (context->lastResult != TEST_RESULT_SUCCESS)
        {
            context->step = AES_CMAC_TEST_STEP_NUMBER_OF_STEPS;
        }

        switch (context->step)
        {
            case AES_CMAC_TEST_STEP_INIT:
            {
                context->step = AES_CMAC_TEST_STEP_CIPHER;
            }
            break;

            case AES_CMAC_TEST_STEP_CIPHER:
            {
                context->step = AES_CMAC_TEST_STEP_FINAL;
            }
            break;

            case AES_CMAC_TEST_STEP_FINAL:
            case AES_CMAC_TEST_STEP_NUMBER_OF_STEPS:
            default:
            {
                context->test++;
                if (context->test < context->testCount)
                {
                    context->step = AES_CMAC_TEST_STEP_INIT;

                    /* cppcheck-suppress misra-c2012-11.5
                     *
                     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
                     *
                     *  Reasoning: A generic pointer allows support for various algorithm test context types.
                     */
                    mac_test_vector_t *tests = (mac_test_vector_t *)context->tests;
                    testContext->test = &tests[context->test];
                }
            }
            break;
        }
    }
    else
    {
        context->lastResult = TEST_RESULT_ALL_TESTS_DONE;
    }

    return context->lastResult;
}
