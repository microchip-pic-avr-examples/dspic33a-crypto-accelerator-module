/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sha.c

  Summary:
    This source file handles the running of the Hashing examples.
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
#include "app/app_sha.h"

// Only one test context at a time is supported.
#define TEST_SESSION_ID     (0x1U)

// The SHA hash context.
static st_Crypto_Hash_Sha_Ctx shaCtx __attribute__((aligned(4)));

// *****************************************************************************
// *****************************************************************************
// Section: SHA Hash Test Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Get the length of the SHA digest based on the algorithm.
 * @param opmode The algorithm to use.
 * @return The length of the digest in bytes.
 */
static uint32_t SHA_DigestLengthGet(crypto_Hash_Algo_E algorithm)
{
    uint32_t digestLength = 0U;

    switch (algorithm)
    {
        case CRYPTO_HASH_SHA2_512:
        {
            digestLength = SHA512_DIGEST_LEN_BYTES;
        }
        break;

        default:
        break;
    }

    return digestLength;
}

/**
 * @brief Get the test operation mode string based on the mode.
 * @param algorithm The hash algorithm.
 * @return Pointer to string.
 */
static const char *SHA_TestOpModeGet(crypto_Hash_Algo_E algorithm)
{
    const char *opModeString = "???";

    switch (algorithm)
    {
        case CRYPTO_HASH_SHA2_512:
            opModeString = "SHA-512";
        break;

        default:
        break;
    }

    return opModeString;
}

/**
 * @brief The MAC CMAC test init state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_INIT_FAILED on failure.
 */
static test_result_t SHA_HashTestStepInit(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    hash_test_context_t *testContext = (hash_test_context_t *)context;
    hash_test_vector_t *test = testContext->test;

    crypto_Hash_Status_E status = CRYPTO_HASH_ERROR_NOTSUPPTED;
    test_result_t result;

#ifdef SHOW_TEST_DATA
    (void) printf(BLUE "\r\n\r\n %s Test %lu",
            AESHASH_TestModeGet(testContext->testMode),
            (unsigned long) test->id);

    AESHASH_SeparatorPrint(BLUE, '-');
    AESHASH_HexArrayPrint("Data (Input)", test->inputData,
            test->inputDataLength);
    AESHASH_LabelPrint("Input Data Size", "%d bytes",
            testContext->blockSize);
    AESHASH_SeparatorPrint(BLUE, '-');
#endif

    (void) printf(WHITE);
    AESHASH_OperationStepPrint("INITIALIZING",
            SHA_TestOpModeGet(testContext->algorithm),
            testContext->testMode,
            test->inputDataLength, true);

    status = Crypto_Hash_Sha_Init(&shaCtx, testContext->algorithm, CRYPTO_HANDLER_HW_INTERNAL, 1);
    result = (status == CRYPTO_HASH_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_INIT_FAILED;

    AESHASH_CryptoResultPrint(result, status);

    return result;
}

/**
 * @brief The MAC CMAC test cipher state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_CIPHER_FAILED on failure.
 */
static test_result_t SHA_HashTestHashRun(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    hash_test_context_t *testContext = (hash_test_context_t *)context;
    hash_test_vector_t *test = testContext->test;
    uint32_t bytesLeft = test->inputDataLength - testContext->bytesHashed;
    uint32_t dataLength = (bytesLeft >= testContext->blockSize)
            ? testContext->blockSize : bytesLeft;

    crypto_Hash_Status_E status = CRYPTO_HASH_ERROR_NOTSUPPTED;
    test_result_t result;

    (void) printf(WHITE);
    AESHASH_OperationStepPrint("HASHING",
            SHA_TestOpModeGet(testContext->algorithm),
            testContext->testMode, dataLength, true);

    status = Crypto_Hash_Sha_Update(&shaCtx,
            &test->inputData[testContext->bytesHashed],
            dataLength);
    result = (status == CRYPTO_HASH_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_CIPHER_FAILED;

    AESHASH_CryptoResultPrint(result, status);

    return result;
}

/**
 * @brief The MAC CMAC test finalize/compare state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success,
 *         TEST_RESULT_FINALIZE_FAILED/TEST_RESULT_DATA_COMPARE_FAILED/TEST_RESULT_TAG_COMPARE_FAILED on failure.
 */
static test_result_t SHA_HashTestFinalizeRun(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    hash_test_context_t *testContext = (hash_test_context_t *)context;
    hash_test_vector_t *test = testContext->test;

    crypto_Hash_Status_E status = CRYPTO_HASH_ERROR_NOTSUPPTED;
    test_result_t result = TEST_RESULT_FINALIZE_FAILED;

    (void) printf(WHITE);
    AESHASH_OperationStepPrint("FINALIZING",
            SHA_TestOpModeGet(testContext->algorithm),
            testContext->testMode,
            test->inputDataLength, true);

    status = Crypto_Hash_Sha_Final(&shaCtx,
            testContext->dataResult);
    result = (status == CRYPTO_HASH_SUCCESS)
            ? TEST_RESULT_SUCCESS
            : TEST_RESULT_FINALIZE_FAILED;

    AESHASH_CryptoResultPrint(result, status);

    if (status == CRYPTO_HASH_SUCCESS)
    {
#ifdef SHOW_TEST_DATA
        AESHASH_SeparatorPrint(BLUE, '-');
        AESHASH_LabelPrint("Digest Length", "%d bytes",
                test->expectedDigestLength);
        AESHASH_HexArrayPrint("Data (Result)", testContext->dataResult,
                test->expectedDigestLength);
        AESHASH_SeparatorPrint(BLUE, '-');
#endif

        (void) printf(WHITE);
        AESHASH_OperationStepPrint("VERIFYING",
                SHA_TestOpModeGet(testContext->algorithm),
                testContext->testMode,
                test->inputDataLength, true);

        if (true == AESHASH_ArrayEqualityCheck(testContext->dataResult,
                test->expectedDigest,
                test->expectedDigestLength))
        {
            result = TEST_RESULT_SUCCESS;
            AESHASH_CryptoResultPrint(result, status);
        }
        else
        {
            result = TEST_RESULT_DATA_COMPARE_FAILED;
            AESHASH_CryptoResultPrint(result, status);

            AESHASH_SeparatorPrint(BLUE, '-');
            AESHASH_HexArrayPrint("Expected:", test->expectedDigest, test->expectedDigestLength);
            AESHASH_SeparatorPrint(BLUE, '-');
            AESHASH_HexArrayPrint("Result:", testContext->dataResult, test->expectedDigestLength);
            AESHASH_SeparatorPrint(BLUE, '-');
        }
    }

    return result;
}

/**
 * @brief Defines the states for a HASH test.
 */
enum
{
    HASH_TEST_STEP_INIT = 0,
    HASH_TEST_STEP_HASH = 1,
    HASH_TEST_STEP_FINAL = 2,
    HASH_TEST_STEP_NUMBER_OF_STEPS
};

test_result_t SHA_ContextInit(test_context_t *context,
        test_mode_t mode, crypto_Hash_Algo_E algorithm,
        uint32_t blockSize)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    hash_test_context_t *testContext = (hash_test_context_t *)context->testContext;
    test_result_t result = TEST_RESULT_SUCCESS;

    (void) memset(context, 0, sizeof(test_context_t));
    context->testContext = testContext;

    switch (algorithm)
    {
        case CRYPTO_HASH_SHA2_512:
        {
            context->tests = (uint8_t *)TV_Sha512TestVectorsGet();
            context->testCount = TV_Sha512TestVectorCountGet();
        }
        break;

        default:
        {
            (void) printf(RED "\r\n\r\n Unknown hash test algorithm %d" RESET_COLOR, algorithm);
            result = TEST_RESULT_INIT_FAILED;
        }
        break;
    }

    if (TEST_RESULT_SUCCESS == result)
    {
        static test_state_fn_t hash_test_states[] =
        {
            SHA_HashTestStepInit,
            SHA_HashTestHashRun,
            SHA_HashTestFinalizeRun,
        };

        context->states = hash_test_states;
        context->testMode = mode;

        context->test = 0;
        context->stepCount = sizeof(hash_test_states) / sizeof(hash_test_states[0]);
        context->step = HASH_TEST_STEP_INIT;

        testContext->testMode = context->testMode;
        testContext->algorithm = algorithm;
        testContext->blockSize = blockSize;
        testContext->digestLength = SHA_DigestLengthGet(algorithm);

        /* cppcheck-suppress misra-c2012-11.5
         *
         *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
         *
         *  Reasoning: A generic pointer allows support for various algorithm test context types.
         */
        hash_test_vector_t *tests = (hash_test_vector_t *)context->tests;

        // Start at the first test.
        testContext->test = &tests[context->test];
    }

    return result;
}

test_result_t SHA_HashTestAdvance(test_context_t *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    hash_test_context_t *testContext = (hash_test_context_t *)context->testContext;

    if (context->test < context->testCount)
    {
        // Execute the next step in the state machine.
        context->lastResult = context->states[context->step](testContext);
        if (context->lastResult != TEST_RESULT_SUCCESS)
        {
            context->step = HASH_TEST_STEP_NUMBER_OF_STEPS;
        }

        switch (context->step)
        {
            case HASH_TEST_STEP_INIT:
            {
                context->step = HASH_TEST_STEP_HASH;
            }
            break;

            case HASH_TEST_STEP_HASH:
            {
                testContext->bytesHashed += testContext->blockSize;
                if (testContext->bytesHashed >= testContext->test->inputDataLength)
                {
                    context->step = HASH_TEST_STEP_FINAL;
                }
            }
            break;

            case HASH_TEST_STEP_FINAL:
            case HASH_TEST_STEP_NUMBER_OF_STEPS:
            default:
            {
                context->test++;
                if (context->test < context->testCount)
                {
                    context->step = HASH_TEST_STEP_INIT;

                    testContext->bytesHashed = 0;

                    /* cppcheck-suppress misra-c2012-11.5
                     *
                     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
                     *
                     *  Reasoning: A generic pointer allows support for various algorithm test context types.
                     */
                    hash_test_vector_t *tests = (hash_test_vector_t *)context->tests;
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
