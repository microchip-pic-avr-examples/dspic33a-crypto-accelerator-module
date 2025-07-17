/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This source file handles the running of the Hashing examples.
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

#include "app_aes_hash.h"
#include "app/app_sha.h"

#include "test_vectors/test_vectors_sha512.h"

// Only one test context at a time is supported.
#define TEST_SESSION_ID     (0x1)


/**
 * @brief test context data for AES symmetric tests.
 */
typedef struct SYM_TEST_CONTEXT {
    // Pointer to current test vector data.
    TEST_VECTOR *           test;

    // Contains the data result.
    uint8_t                 dataResult[SHA512_DIGEST_LEN_BYTES];

    // The test mode (hash).
    TEST_MODE               testMode;

    // The hash algorithm type.
    crypto_Hash_Algo_E      algorithm;

    // The size of the hash data to use each time the hash is executed.
    uint16_t                blockSize;

    // The number of bytes hashed.
    uint16_t                bytesHashed;

    // The length of the resulting digest.
    uint32_t                digestLen;

} HASH_TEST_CONTEXT;

// The SHA hash context.
static st_Crypto_Hash_Sha_Ctx shaCtx __attribute__((aligned(4)));


// *****************************************************************************
// *****************************************************************************
// Section: HASH Test Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Get the length of the SHA digest based on the algorithm.
 * @param opmode The algorithm to use.
 * @return The length of the digest in bytes.
 */
static uint32_t getDigestLength(crypto_Hash_Algo_E algorithm)
{
    uint32_t digestLength = 0;

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
static const char *getTestOpMode(crypto_Hash_Algo_E algorithm)
{
    const char* opModeStr = "???";

    switch(algorithm)
    {
        case CRYPTO_HASH_SHA2_512:
            opModeStr = "SHA-512";
        break;

        default:
        break;
    }

    return opModeStr;
}


/**
 * @brief The MAC CMAC test init state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_INIT_FAILED on failure.
 */
static TEST_RESULT sha_hash_test_init(void *context)
{
    HASH_TEST_CONTEXT *testContext = (HASH_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;

    crypto_Hash_Status_E status = CRYPTO_HASH_ERROR_NOTSUPPTED;
    TEST_RESULT result;

#ifdef SHOW_TEST_DATA
    (void) printf(BLUE "\r\n\r\n %s Test %ld", getTestMode(testContext->testMode), test->id);

    printSeparator(BLUE, '-');
    printHexArray("Data (Input)", test->inputData, test->inputDataLength);
    printWithLabel("Input Data Size", "%d bytes", testContext->blockSize);
    printSeparator(BLUE, '-');
#endif

    (void) printf(WHITE);
    printOperationStep("INITIALIZING", getTestOpMode(testContext->algorithm), testContext->testMode, test->inputDataLength, true);

    status = Crypto_Hash_Sha_Init(&shaCtx, testContext->algorithm, CRYPTO_HANDLER_HW_INTERNAL, 1);
    result = (status == CRYPTO_HASH_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_INIT_FAILED;

    printCryptoResult(result, status);

    return result;
}

/**
 * @brief The MAC CMAC test cipher state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_CIPHER_FAILED on failure.
 */
static TEST_RESULT sha_hash_test_hash(void *context)
{
    HASH_TEST_CONTEXT *testContext = (HASH_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;
    uint32_t bytesLeft = test->inputDataLength - testContext->bytesHashed;
    uint32_t dataLength = (bytesLeft >= testContext->blockSize) ? testContext->blockSize : bytesLeft;

    crypto_Hash_Status_E status = CRYPTO_HASH_ERROR_NOTSUPPTED;
    TEST_RESULT result;

    (void) printf(WHITE);
    printOperationStep("HASHING", getTestOpMode(testContext->algorithm), testContext->testMode, dataLength, true);

    status = Crypto_Hash_Sha_Update(&shaCtx, &test->inputData[testContext->bytesHashed], dataLength);
    result = (status == CRYPTO_HASH_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_CIPHER_FAILED;

    printCryptoResult(result, status);

    return result;
}


/**
 * @brief The MAC CMAC test finalize/compare state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success,
 *         TEST_RESULT_FINALIZE_FAILED/TEST_RESULT_DATA_COMPARE_FAILED/TEST_RESULT_TAG_COMPARE_FAILED on failure.
 */
static TEST_RESULT sha_hash_test_final(void *context)
{
    HASH_TEST_CONTEXT *testContext = (HASH_TEST_CONTEXT *)context;
    TEST_VECTOR *test = testContext->test;

    crypto_Hash_Status_E status = CRYPTO_HASH_ERROR_NOTSUPPTED;
    TEST_RESULT result = TEST_RESULT_FINALIZE_FAILED;

    (void) printf(WHITE);
    printOperationStep("FINALIZING", getTestOpMode(testContext->algorithm), testContext->testMode, test->inputDataLength, true);

    status = Crypto_Hash_Sha_Final(&shaCtx, testContext->dataResult);
    result = (status == CRYPTO_HASH_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_FINALIZE_FAILED;

    printCryptoResult(result, status);

    if (status == CRYPTO_HASH_SUCCESS)
    {
#ifdef SHOW_TEST_DATA
        printSeparator(BLUE, '-');
        printWithLabel("Digest Length", "%d bytes", test->expectedDigestLength);
        printHexArray("Data (Result)", testContext->dataResult, test->expectedDigestLength);
        printSeparator(BLUE, '-');
#endif

        (void) printf(WHITE);
        printOperationStep("VERIFYING", getTestOpMode(testContext->algorithm), testContext->testMode, test->inputDataLength, true);

        if (true == checkArrayEquality(testContext->dataResult, test->expectedDigest, test->expectedDigestLength))
        {
            result = TEST_RESULT_SUCCESS;
            printCryptoResult(result, status);
        }
        else
        {
            result = TEST_RESULT_DATA_COMPARE_FAILED;
            printCryptoResult(result, status);

            printSeparator(BLUE, '-');
            printHexArray("Expected:", test->expectedDigest, test->expectedDigestLength);
            printSeparator(BLUE, '-');
            printHexArray("Result:", testContext->dataResult, test->expectedDigestLength);
            printSeparator(BLUE, '-');
        }
    }

    return result;
}

/**
 * @brief Defines the states for a HASH test.
 */
typedef enum HASH_TEST_STEPS {
    HASH_TEST_STEP_INIT = 0,
    HASH_TEST_STEP_HASH = 1,
    HASH_TEST_STEP_FINAL = 2,
    HASH_TEST_STEP_NUMBER_OF_STEPS
} HASH_TEST_STEPS;

/**
 * @brief Defines the state functions for a HASH test.
 */
TEST_STATE_FN hash_test_states[] =
{
    sha_hash_test_init,
    sha_hash_test_hash,
    sha_hash_test_final,
};


TEST_RESULT hash_test_init(TEST_CONTEXT *context, TEST_MODE mode, crypto_Hash_Algo_E algorithm, uint32_t blockSize)
{
    TEST_RESULT result = TEST_RESULT_SUCCESS;

    (void) memset(context, 0, sizeof(TEST_CONTEXT));

    switch (algorithm)
    {
        case CRYPTO_HASH_SHA2_512:
        {
            context->tests = (uint8_t *)get_sha_512_test_vectors();
            context->testCount = get_sha_512_test_vector_count();
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
        context->states = hash_test_states;
        context->testMode = mode;

        context->test = 0;
        context->stepCount = sizeof(hash_test_states) / sizeof(hash_test_states[0]);
        context->step = HASH_TEST_STEP_INIT;

        HASH_TEST_CONTEXT *testContext = (HASH_TEST_CONTEXT *)context->testContext;
        testContext->testMode = context->testMode;
        testContext->algorithm = algorithm;
        testContext->blockSize = blockSize;
        testContext->digestLen = getDigestLength(algorithm);

        // Start at the first test.
        TEST_VECTOR *tests = (TEST_VECTOR *)context->tests;
        testContext->test = &tests[context->test];
    }

    return result;
}


TEST_RESULT hash_test_advance(TEST_CONTEXT *context)
{
    HASH_TEST_CONTEXT *testContext = (HASH_TEST_CONTEXT *)context->testContext;

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
