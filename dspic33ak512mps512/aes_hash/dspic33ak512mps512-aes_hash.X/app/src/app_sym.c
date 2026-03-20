/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sym.c

  Summary:
    This source file handles the running of the symmetric AES examples.
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

#include "app/app_sym.h"

#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"

// Context for AES calculations.
static st_Crypto_Sym_BlockCtx aesCtx __attribute__((aligned(4)));

// *****************************************************************************
// *****************************************************************************
// Section: File level Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Compares the result of an encryption operation.
 * @param expected Pointer to expected data value.
 * @param result Pointer to generated data value.
 * @param length Length of the data.
 * @return TEST_RESULT_SUCCESS if both pass, TEST_RESULT_DATA_COMPARE_FAILED on failure.
 */
static test_result_t SYM_ResultCheck(const uint8_t *expected,
        const uint8_t *result, uint32_t length)
{
    test_result_t retval = TEST_RESULT_SUCCESS;

    if (length > 0UL)
    {
        if (0 != memcmp(result, expected, length))
        {
            retval = TEST_RESULT_DATA_COMPARE_FAILED;
        }
    }

    return retval;
}

/**
 * @brief Get the test operation mode string based on the mode.
 * @param opMode The AES symmetric operations mode.
 * @return Pointer to string.
 */
static const char *SYM_TestOpModeGet(crypto_Sym_OpModes_E opMode)
{
    const char *opModeString = "???";

    switch (opMode)
    {
        case CRYPTO_SYM_OPMODE_ECB:
            opModeString = "AES-ECB";
            break;

        case CRYPTO_SYM_OPMODE_CTR:
            opModeString = "AES-CTR";
            break;

        case CRYPTO_SYM_OPMODE_CBC:
            opModeString = "AES-CBC";
            break;

        case CRYPTO_SYM_OPMODE_CFB128:
            opModeString = "AES-CFB128";
            break;

        case CRYPTO_SYM_OPMODE_OFB:
            opModeString = "AES-OFB";
            break;

        case CRYPTO_SYM_OPMODE_XTS:
            opModeString = "AES-XTS";
            break;

        default:
            break;
    }

    return opModeString;
}

// *****************************************************************************
// *****************************************************************************
// Section: Symmetric AES Test State Machine Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Defines the states for an AES symmetric cipher test.
 */
enum
{
    SYM_TEST_STEP_INIT = 0,
    SYM_TEST_STEP_FINAL = 1,
    SYM_TEST_STEP_NUMBER_OF_STEPS
};

/**
 * @brief The AES symmetric test init state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_INIT_FAILED on failure.
 */
static test_result_t SYM_TestStepInit(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    sym_test_context_t *testContext = (sym_test_context_t *)context;
    sym_test_vector_t *test = testContext->test;
    crypto_CipherOper_E testMode =
            (testContext->testMode == TEST_MODE_ENCRYPT)
            ? CRYPTO_CIOP_ENCRYPT : CRYPTO_CIOP_DECRYPT;

    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPNOTSUPPTD;
    test_result_t result;

#ifdef SHOW_TEST_DATA
    uint8_t *data = (testContext->testMode == TEST_MODE_ENCRYPT)
            ? test->ptData : test->ctData;

    (void) printf(UTILS_BLUE "\r\n\r\n %s %s Test %lu",
            SYM_TestOpModeGet(testContext->opMode),
            AESHASH_TestModeGet(testContext->testMode),
            (unsigned long) test->id);

    UTILS_SeparatorPrint(UTILS_BLUE, '-');
    UTILS_HexArrayPrint("Key", test->key, test->keyLength);
    UTILS_SeparatorPrint(UTILS_BLUE, '-');
    UTILS_HexArrayPrint("Data (Input)", data, test->dataLength);
    UTILS_SeparatorPrint(UTILS_BLUE, '-');
#endif

    status = Crypto_Sym_Aes_Init(&aesCtx,
            CRYPTO_HANDLER_HW_INTERNAL, testMode,
            testContext->opMode, test->key, test->keyLength,
            test->initializationVector, UTILS_SESSION_ID);
    result = (status == CRYPTO_SYM_CIPHER_SUCCESS)
            ? TEST_RESULT_SUCCESS : TEST_RESULT_INIT_FAILED;

    (void) printf(UTILS_CYAN);
    AESHASH_OperationStepPrint("INITIALIZING",
            SYM_TestOpModeGet(testContext->opMode),
            testContext->testMode, test->dataLength, true);
    AESHASH_CryptoResultPrint(result, status);

    return result;
}

/**
 * @brief The AES symmetric test finalize/compare state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_CIPHER_FAILED on failure.
 */
static test_result_t SYM_TestFinalizeRun(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    sym_test_context_t *testContext = (sym_test_context_t *)context;
    sym_test_vector_t *test = testContext->test;
    uint8_t *data = (testContext->testMode == TEST_MODE_ENCRYPT)
            ? test->ptData : test->ctData;

    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPNOTSUPPTD;
    test_result_t result = TEST_RESULT_FINALIZE_FAILED;

    (void) printf(UTILS_CYAN);
    AESHASH_OperationStepPrint("FINALIZING",
            SYM_TestOpModeGet(testContext->opMode),
            testContext->testMode, test->dataLength, true);

    if (test->mode == (uint32_t) CRYPTO_SYM_OPMODE_XTS)
    {
        status = Crypto_Sym_AesXts_Cipher(&aesCtx, data,
                test->dataLength, testContext->dataResult,
                test->tweak);
    }
    else
    {
        status = Crypto_Sym_Aes_Cipher(&aesCtx, data,
                test->dataLength, testContext->dataResult);
    }

    result = (status == CRYPTO_SYM_CIPHER_SUCCESS)
            ? TEST_RESULT_SUCCESS
            : TEST_RESULT_FINALIZE_FAILED;

    AESHASH_CryptoResultPrint(result, status);

    if (status == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        uint8_t *resultData =
                (testContext->testMode == TEST_MODE_ENCRYPT)
                ? test->ctData : test->ptData;

#ifdef SHOW_TEST_DATA
        UTILS_SeparatorPrint(UTILS_BLUE, '-');
        UTILS_HexArrayPrint("Data (Result)", data, test->dataLength);
        UTILS_SeparatorPrint(UTILS_BLUE, '-');
#endif
        (void) printf(UTILS_CYAN);
        AESHASH_OperationStepPrint("VERIFYING",
                SYM_TestOpModeGet(testContext->opMode),
                testContext->testMode,
                test->dataLength, true);

        result = SYM_ResultCheck(resultData, testContext->dataResult, test->dataLength);

        AESHASH_CryptoResultPrint(result, status);

        if (result != TEST_RESULT_SUCCESS)
        {
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
            UTILS_HexArrayPrint("Expected:", resultData, test->dataLength);
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
            UTILS_HexArrayPrint("Result: ", testContext->dataResult, test->dataLength);
            UTILS_SeparatorPrint(UTILS_BLUE, '-');
        }
    }

    return result;
}

// *****************************************************************************
// *****************************************************************************
// Section: Symmetric AES Test Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Defines the state functions for an AES symmetric cipher test.
 */

test_result_t AESHASH_SymContextInit(test_context_t *context,
        test_mode_t testMode, sym_oper_mode_t opMode)
{

    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    sym_test_context_t *testContext = (sym_test_context_t *)context->testContext;
    test_result_t result = TEST_RESULT_SUCCESS;

    static test_state_fn_t sym_test_states[] =
    {
        SYM_TestStepInit,
        SYM_TestFinalizeRun,
    };

    (void) memset(context, 0, sizeof(test_context_t));
    context->testContext = testContext;

    switch (opMode)
    {
        case SYM_OPER_MODE_AES_ECB:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)VECTOR_EcbTestVectorsGet();
            context->testCount = VECTOR_EcbTestVectorCountGet();
            testContext->opMode = CRYPTO_SYM_OPMODE_ECB;
        }
        break;

        case SYM_OPER_MODE_AES_CTR:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)VECTOR_CtrTestVectorsGet();
            context->testCount = VECTOR_CtrTestVectorCountGet();
            testContext->opMode = CRYPTO_SYM_OPMODE_CTR;
        }
        break;

        case SYM_OPER_MODE_AES_CBC:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)VECTOR_CbcTestVectorsGet();
            context->testCount = VECTOR_CbcTestVectorCountGet();
            testContext->opMode = CRYPTO_SYM_OPMODE_CBC;
        }
        break;

        case SYM_OPER_MODE_AES_CFB128:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)VECTOR_Cfb128TestVectorsGet();
            context->testCount = VECTOR_Cfb128TestVectorCountGet();
            testContext->opMode = CRYPTO_SYM_OPMODE_CFB128;
        }
        break;

        case SYM_OPER_MODE_AES_OFB:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)VECTOR_OfbTestVectorsGet();
            context->testCount = VECTOR_OfbTestVectorCountGet();
            testContext->opMode = CRYPTO_SYM_OPMODE_OFB;
        }
        break;

        case SYM_OPER_MODE_AES_XTS:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)VECTOR_XtsTestVectorsGet();
            context->testCount = VECTOR_XtsTestVectorCountGet();
            testContext->opMode = CRYPTO_SYM_OPMODE_XTS;
        }
        break;

        default:
        {
            (void) printf(UTILS_RED
                    "\r\n\r\n Unknown symmetric test"
                    " operation mode %d"
                    UTILS_RESET_COLOR, opMode);
            result = TEST_RESULT_INIT_FAILED;
        }
        break;
    }

    if (TEST_RESULT_SUCCESS == result)
    {
        context->testMode = testMode;
        testContext->testMode = context->testMode;

        context->test = 0;
        context->stepCount = sizeof(sym_test_states) / sizeof(sym_test_states[0]);
        context->step = 0;

        /* cppcheck-suppress misra-c2012-11.5
         *
         *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
         *
         *  Reasoning: A generic pointer allows support for various algorithm test context types.
         */
        sym_test_vector_t *tests = (sym_test_vector_t *)context->tests;

        // Start at the first test.
        testContext->test = &tests[context->test];
    }

    return result;
}

test_result_t AESHASH_SymTestAdvance(test_context_t *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    sym_test_context_t *testContext = (sym_test_context_t *)context->testContext;

    if (context->test < context->testCount)
    {
        // Execute the next step in the state machine.
        context->lastResult = context->states[context->step](testContext);
        if (context->lastResult != TEST_RESULT_SUCCESS)
        {
            context->step = SYM_TEST_STEP_NUMBER_OF_STEPS;
        }

        switch (context->step)
        {
            case SYM_TEST_STEP_INIT:
            {
                context->step = SYM_TEST_STEP_FINAL;
            }
            break;

            case SYM_TEST_STEP_FINAL:
            case SYM_TEST_STEP_NUMBER_OF_STEPS:
            default:
            {
                context->test++;
                if (context->test < context->testCount)
                {
                    context->step = SYM_TEST_STEP_INIT;

                    /* cppcheck-suppress misra-c2012-11.5
                     *
                     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
                     *
                     *  Reasoning: A generic pointer allows support for various algorithm test context types.
                     */
                    sym_test_vector_t *tests = (sym_test_vector_t *)context->tests;
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
