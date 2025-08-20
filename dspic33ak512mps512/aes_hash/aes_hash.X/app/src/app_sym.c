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

/* cppcheck-suppress misra-c2012-21.6
*
*  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
*
*  Reasoning: printf is required for printing to the terminal in this code example
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "./app_aes_hash.h"

#include "app/app_sym.h"

#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_sym_cipher.h"

#include "test_vectors/sym_test_vector.h"

// Only one test context at a time is supported.
#define TEST_SESSION_ID     (0x1)

// Maximum size of the result data.
#define DATA_RESULT_SIZE    (48U)


/**
 * @brief test context data for AES symmetric tests.
 */
typedef struct SYM_TEST_CONTEXT {
    // Pointer to current test vector data.
    TEST_VECTOR *           test;

    // Contains the data result.
    uint8_t                 dataResult[DATA_RESULT_SIZE];

    // The test mode (encrypt/decrypt).
    TEST_MODE               testMode;

    // The symmetric encryption type.
    crypto_Sym_OpModes_E    opMode;

} SYM_TEST_CONTEXT;


// Context for AES calculations.
static st_Crypto_Sym_BlockCtx aesCtx __attribute__((aligned(4)));


// *****************************************************************************
// *****************************************************************************
// Section: File level Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Compares the result of an encryption operaton.
 * @param expected Pointer to expected data value.
 * @param result Pointer to generated data value.
 * @param length Length of the data.
 * @return TEST_RESULT_SUCCESS if both pass, TEST_RESULT_DATA_COMPARE_FAILED on failure.
 */
static TEST_RESULT checkResult(uint8_t *expected, uint8_t *result, uint32_t length)
{
    TEST_RESULT retval = TEST_RESULT_SUCCESS;

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
static const char *getTestOpMode(crypto_Sym_OpModes_E opMode)
{
    const char* opModeStr = "???";

    switch(opMode)
    {
        case CRYPTO_SYM_OPMODE_ECB:
            opModeStr = "AES-ECB";
            break;

        case CRYPTO_SYM_OPMODE_CTR:
            opModeStr = "AES-CTR";
            break;

        case CRYPTO_SYM_OPMODE_XTS:
            opModeStr = "AES-XTS";
            break;

        default:
            break;
    }

    return opModeStr;
}


// *****************************************************************************
// *****************************************************************************
// Section: Symmetric AES test state machine Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief The AES symmetric test init state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_INIT_FAILED on failure.
 */
static TEST_RESULT aes_sym_test_init(void *context)
{
    SYM_TEST_CONTEXT *testContext = (SYM_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;
    crypto_CipherOper_E testMode = (testContext->testMode == TEST_MODE_ENCRYPT ? CRYPTO_CIOP_ENCRYPT : CRYPTO_CIOP_DECRYPT);

    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPNOTSUPPTD;
    TEST_RESULT result;

#ifdef SHOW_TEST_DATA
    uint8_t *data = (testContext->testMode == TEST_MODE_ENCRYPT ? test->ptData : test->ctData);

    (void) printf(BLUE "\r\n\r\n %s %s Test %ld", getTestOpMode(testContext->opMode), getTestMode(testContext->testMode), test->id);

    printSeparator(BLUE, '-');
    printHexArray("Key", test->key, test->keyLength);
    printSeparator(BLUE, '-');
    printHexArray("Data (Input)", data, test->dataLength);
    printSeparator(BLUE, '-');
#endif

    status = Crypto_Sym_Aes_Init(&aesCtx, CRYPTO_HANDLER_HW_INTERNAL, testMode, testContext->opMode, test->key, test->keyLength,
                                 test->initializationVector, TEST_SESSION_ID);
    result = (status == CRYPTO_SYM_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_INIT_FAILED;

    (void) printf(CYAN);
    printOperationStep("INITIALIZING", getTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);
    printCryptoResult(result, status);

    return result;
}

/**
 * @brief The AES symmetric test finalize/compare state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_CIPHER_FAILED on failure.
 */
static TEST_RESULT aes_sym_test_final(void *context)
{
    SYM_TEST_CONTEXT *testContext = (SYM_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;
    uint8_t *data = (testContext->testMode == TEST_MODE_ENCRYPT ? test->ptData : test->ctData);

    crypto_Sym_Status_E status = CRYPTO_SYM_ERROR_CIPNOTSUPPTD;
    TEST_RESULT result = TEST_RESULT_FINALIZE_FAILED;

    (void) printf(CYAN);
    printOperationStep("FINALIZING", getTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);

    if (test->mode == CRYPTO_SYM_OPMODE_XTS)
    {
        status = Crypto_Sym_AesXts_Cipher(&aesCtx, data, test->dataLength, testContext->dataResult, test->tweak);
    }
    else
    {
        status = Crypto_Sym_Aes_Cipher(&aesCtx, data, test->dataLength, testContext->dataResult);
    }

    result = (status == CRYPTO_SYM_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_FINALIZE_FAILED;

    printCryptoResult(result, status);

    if (status == CRYPTO_SYM_CIPHER_SUCCESS)
    {
        uint8_t *resultData = (testContext->testMode == TEST_MODE_ENCRYPT ? test->ctData : test->ptData);

#ifdef SHOW_TEST_DATA
        printSeparator(BLUE, '-');
        printHexArray("Data (Result)", data, test->dataLength);
        printSeparator(BLUE, '-');
#endif
        (void) printf(CYAN);
        printOperationStep("VERIFYING", getTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);

        result = checkResult(resultData, testContext->dataResult, test->dataLength);

        printCryptoResult(result, status);

        if (result != TEST_RESULT_SUCCESS)
        {
            printSeparator(BLUE, '-');
            printHexArray("Expected:", resultData, test->dataLength);
            printSeparator(BLUE, '-');
            printHexArray("Result: ", testContext->dataResult, test->dataLength);
            printSeparator(BLUE, '-');
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
 * @brief Defines the states for an AES symmetric cipher test.
 */
typedef enum SYM_TEST_STEPS {
    SYM_TEST_STEP_INIT = 0,
    SYM_TEST_STEP_FINAL = 1,
    SYM_TEST_STEP_NUMBER_OF_STEPS
} SYM_TEST_STEPS;

/**
 * @brief Defines the state functions for an AES symmetric cipher test.
 */
static TEST_STATE_FN sym_test_states[] =
{
    aes_sym_test_init,
    aes_sym_test_final,
};


TEST_RESULT sym_test_init(TEST_CONTEXT *context, TEST_MODE testMode, SYM_OPER_MODE opMode)
{
    SYM_TEST_CONTEXT *testContext = (SYM_TEST_CONTEXT *)context->testContext;
    TEST_RESULT result = TEST_RESULT_SUCCESS;

    (void) memset(context, 0, sizeof(TEST_CONTEXT));

    switch (opMode)
    {
        case SYM_OPER_MODE_AES_ECB:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)get_ecb_test_vectors();
            context->testCount = get_ecb_test_vector_count();
            testContext->opMode = CRYPTO_SYM_OPMODE_ECB;
        }
        break;

        case SYM_OPER_MODE_AES_CTR:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)get_ctr_test_vectors();
            context->testCount = get_ctr_test_vector_count();
            testContext->opMode = CRYPTO_SYM_OPMODE_CTR;
        }
        break;

        case SYM_OPER_MODE_AES_XTS:
        {
            context->states = sym_test_states;
            context->tests = (uint8_t *)get_xts_test_vectors();
            context->testCount = get_xts_test_vector_count();
            testContext->opMode = CRYPTO_SYM_OPMODE_XTS;
        }
        break;

        default:
        {
            (void) printf(RED "\r\n\r\n Unknown symmetric test operation mode %d" RESET_COLOR, opMode);
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

        // Start at the first test.
        TEST_VECTOR *tests = (TEST_VECTOR *)context->tests;
        testContext->test = &tests[context->test];
    }

    return result;
}


TEST_RESULT sym_test_advance(TEST_CONTEXT *context)
{
    SYM_TEST_CONTEXT *testContext = (SYM_TEST_CONTEXT *)context->testContext;

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

                    TEST_VECTOR *tests = (TEST_VECTOR *)context->tests;
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
