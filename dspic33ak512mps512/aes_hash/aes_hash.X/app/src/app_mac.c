
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
#include <string.h>
#include "./app_aes_hash.h"
#include "crypto/common_crypto/crypto_mac_cipher.h"
#include "crypto/common_crypto/crypto_common.h"
#include "test_vectors/mac_test_vector.h"
#include "app/app_mac.h"

// Only one test context at a time is supported.
#define TEST_SESSION_ID         (0x1)

// Maximum size of the CMAC.
#define CMAC_RESULT_SIZE        (16U)


/**
 * @brief test context data for AES CMAC tests.
 */
typedef struct AES_CMAC_TEST_CONTEXT {
    // Pointer to current test vector data.
    TEST_VECTOR *   test;

    // Contains the data result.
    uint8_t         dataResult[CMAC_RESULT_SIZE];

    // The test mode (encrypt/decrypt).
    TEST_MODE       testMode;

    // The CMAC test operation mode.
    MAC_OPER_MODE   opMode;

} AES_CMAC_TEST_CONTEXT;


// Context for CMAC calculations.
static st_Crypto_Mac_Aes_ctx macCtx __attribute__((aligned(4)));


/**
 * @brief Get the test operation mode string based on the mode.
 * @param opMode The MAC operations mode.
 * @return Pointer to string.
 */
static const char *getTestOpMode(MAC_OPER_MODE opMode)
{
    const char* opModeStr = "???";

    switch(opMode)
    {
        case MAC_OPER_MODE_AES_CMAC:
            opModeStr = "AES-CMAC";
            break;

        default:
            break;
    }

    return opModeStr;
}


// *****************************************************************************
// *****************************************************************************
// Section: AES CMAC test state machine Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief The MAC CMAC test init state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_INIT_FAILED on failure.
 */
static TEST_RESULT aes_cmac_test_init(void *context)
{
    AES_CMAC_TEST_CONTEXT *testContext = (AES_CMAC_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;

    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    TEST_RESULT result;

#ifdef SHOW_TEST_DATA
    (void) printf(BLUE "\r\n\r\n %s %s Test %ld", getTestOpMode(testContext->opMode), getTestMode(testContext->testMode), test->id);

    printSeparator(BLUE, '-');
    printHexArray("Key", test->inputKey, test->inputKeyLength);
    printSeparator(BLUE, '-');
    printHexArray("Data (Input)", test->inputData, test->inputDataLength);
    printSeparator(BLUE, '-');
#endif

    status = Crypto_Mac_AesCmac_Init(&macCtx, CRYPTO_HANDLER_HW_INTERNAL, test->inputKey, test->inputKeyLength, TEST_SESSION_ID);
    result = (status == CRYPTO_MAC_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_INIT_FAILED;

    (void) printf(MAG);
    printOperationStep("INITIALIZING", getTestOpMode(testContext->opMode), testContext->testMode, test->inputDataLength, true);
    printCryptoResult(result, status);

    return result;
}

/**
 * @brief The MAC CMAC test cipher state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_CIPHER_FAILED on failure.
 */
static TEST_RESULT aes_cmac_test_cipher(void *context)
{
    AES_CMAC_TEST_CONTEXT *testContext = (AES_CMAC_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;

    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    TEST_RESULT result;

    (void) printf(MAG);
    printOperationStep("CIPHERING", getTestOpMode(testContext->opMode), testContext->testMode, test->inputDataLength, true);

    status = Crypto_Mac_AesCmac_Cipher(&macCtx, test->inputData, test->inputDataLength);
    result = (status == CRYPTO_MAC_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_CIPHER_FAILED;

    printCryptoResult(result, status);

    return result;
}

/**
 * @brief The MAC CMAC test finalize/compare state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success,
 *         TEST_RESULT_FINALIZE_FAILED/TEST_RESULT_DATA_COMPARE_FAILED/TEST_RESULT_TAG_COMPARE_FAILED on failure.
 */
static TEST_RESULT aes_cmac_test_final(void *context)
{
    AES_CMAC_TEST_CONTEXT *testContext = (AES_CMAC_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;

    crypto_Mac_Status_E status = CRYPTO_MAC_ERROR_CIPNOTSUPPTD;
    TEST_RESULT result = TEST_RESULT_FINALIZE_FAILED;

    (void) printf(MAG);
    printOperationStep("FINALIZING", getTestOpMode(testContext->opMode), testContext->testMode, test->inputDataLength, true);

    status = Crypto_Mac_AesCmac_Final(&macCtx, testContext->dataResult, test->expectedMacLength);
    result = (status == CRYPTO_MAC_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_FINALIZE_FAILED;

    printCryptoResult(result, status);

    if (status == CRYPTO_MAC_CIPHER_SUCCESS)
    {
#ifdef SHOW_TEST_DATA
        printSeparator(BLUE, '-');
        printWithLabel("Result Length", "%d bytes", test->expectedMacLength);
        printHexArray("Data (Result)", testContext->dataResult, test->expectedMacLength);
        printSeparator(BLUE, '-');
#endif
        (void) printf(MAG);
        printOperationStep("VERIFYING", getTestOpMode(testContext->opMode), testContext->testMode, test->inputDataLength, true);

        if (true == checkArrayEquality(testContext->dataResult, test->expectedMac, test->expectedMacLength))
        {
            result = TEST_RESULT_SUCCESS;
        }
        else
        {
            result = TEST_RESULT_DATA_COMPARE_FAILED;
        }

        printCryptoResult(result, status);

        if (result != TEST_RESULT_SUCCESS)
        {
            printSeparator(BLUE, '-');
            printHexArray("Expected:", test->expectedMac, test->expectedMacLength);
            printSeparator(BLUE, '-');
            printHexArray("Result:", testContext->dataResult, test->expectedMacLength);
            printSeparator(BLUE, '-');
        }
    }

    return result;
}

/**
 * @brief Defines the states for a AES CMAC test.
 */
typedef enum AES_CMAC_TEST_STEPS {
    AES_CMAC_TEST_STEP_INIT = 0,
    AES_CMAC_TEST_STEP_CIPHER = 1,
    AES_CMAC_TEST_STEP_FINAL = 2,
    AES_CMAC_TEST_STEP_NUMBER_OF_STEPS
} AES_CMAC_TEST_STEPS;

/**
 * @brief Defines the state functions for an AES CMAC test.
 */
static TEST_STATE_FN aes_cmac_test_states[] =
{
    aes_cmac_test_init,
    aes_cmac_test_cipher,
    aes_cmac_test_final,
};


// *****************************************************************************
// *****************************************************************************
// Section: MAC CMAC test Functions
// *****************************************************************************
// *****************************************************************************

TEST_RESULT mac_cmac_test_init(TEST_CONTEXT *context, TEST_MODE testMode, MAC_OPER_MODE opMode)
{
    AES_CMAC_TEST_CONTEXT *testContext = (AES_CMAC_TEST_CONTEXT *)context->testContext;
    TEST_RESULT result = TEST_RESULT_SUCCESS;

    (void) memset(context, 0, sizeof(TEST_CONTEXT));

    switch (opMode)
    {
        case MAC_OPER_MODE_AES_CMAC:
        {
            context->states = aes_cmac_test_states;
            context->tests = (uint8_t *)get_cmac_test_vectors();
            context->testCount = get_cmac_test_vector_count();
            context->stepCount = sizeof(aes_cmac_test_states) / sizeof(aes_cmac_test_states[0]);
        }
        break;

        default:
        {
            (void) printf(RED "\r\n\r\n Unknown MAC test operation mode %d" RESET_COLOR, opMode);
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

        // Start at the first test.
        TEST_VECTOR *tests = (TEST_VECTOR *)context->tests;
        testContext->test = &tests[context->test];
    }

    return result;
}


TEST_RESULT mac_cmac_test_advance(TEST_CONTEXT *context)
{
    AES_CMAC_TEST_CONTEXT *testContext = (AES_CMAC_TEST_CONTEXT *)context->testContext;

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
