/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_aead.c

  Summary:
    This source file handles the running of the AEAD AES examples.
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

#include "app/app_aead.h"
#include "app/app_aes_hash.h"

#include "crypto/common_crypto/crypto_common.h"
#include "crypto/common_crypto/crypto_aead_cipher.h"

// Only one test context at a time is supported.
#define TEST_SESSION_ID         (0x1)

// Context for AES calculations.
static st_Crypto_Aead_AesGcm_ctx aeadGcmCtx __attribute__((aligned(4)));
static st_Crypto_Aead_AesCcm_ctx aeadCcmCtx __attribute__((aligned(4)));

// *****************************************************************************
// *****************************************************************************
// Section: File level Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Compares the result of an encryption operaton and the generated authTag.
 * @param expected Pointer to expected data value.
 * @param result Pointer to generated data value.
 * @param length Length of the data.
 * @param authTag Pointer to expected authauthTag.
 * @param authTagResult Pointer to generated authTag.
 * @return TEST_RESULT_SUCCESS if both pass, TEST_RESULT_DATA_COMPARE_FAILED/TEST_RESULT_TAG_COMPARE_FAILED on failure.
 * @note This function is only called if the caller's operation result was a success.
 */
static TEST_RESULT checkEncryptionAndTag(uint8_t *expected, uint8_t *result, uint32_t length, uint8_t *authTag, uint8_t *authTagResult, uint32_t authTagLength)
{
    TEST_RESULT dataRetval = TEST_RESULT_SUCCESS;
    TEST_RESULT tagRetval = TEST_RESULT_SUCCESS;

    (void)(void) printf(RESET_COLOR " DATA");

    if (length > 0UL)
    {
        if (0 != memcmp(result, expected, length))
        {
            dataRetval = TEST_RESULT_DATA_COMPARE_FAILED;
        }
    }

    printCryptoResult(dataRetval, CRYPTO_AEAD_CIPHER_SUCCESS);

    (void)(void) printf(RESET_COLOR " AUTHTAG");

    if (0 != memcmp(authTagResult, authTag, authTagLength))
    {
        tagRetval = TEST_RESULT_TAG_COMPARE_FAILED;
    }

    printCryptoResult(tagRetval, CRYPTO_AEAD_CIPHER_SUCCESS);

    if (dataRetval != TEST_RESULT_SUCCESS)
    {
        printSeparator(BLUE, '-');
        printHexArray("Expected Data: ", expected, length);
        printSeparator(BLUE, '-');
        printHexArray("Result Data: ", result, length);
        printSeparator(BLUE, '-');
    }

    if (tagRetval != TEST_RESULT_SUCCESS)
    {
        printSeparator(BLUE, '-');
        printHexArray("Expected Tag: ", authTag, authTagLength);
        printSeparator(BLUE, '-');
        printHexArray("Result Tag: ", authTagResult, authTagLength);
        printSeparator(BLUE, '-');
    }


    return dataRetval | tagRetval;
}

/**
 * @brief Compares the result of an encryption operaton and the generated authTag.
 * @param expected Pointer to expected data value.
 * @param result Pointer to generated data value.
 * @param length Length of the data.
 * @return TEST_RESULT_SUCCESS if both pass, TEST_RESULT_DATA_COMPARE_FAILED/TEST_RESULT_TAG_COMPARE_FAILED on failure.
 * @note This function is only called if the caller's operation result was a success.
 */
static TEST_RESULT checkDecryption(uint8_t *expected, uint8_t *result, uint32_t length)
{
    TEST_RESULT retval = TEST_RESULT_SUCCESS;

    if (length > 0UL)
    {
        if (0 != memcmp(result, expected, length))
        {
            retval = TEST_RESULT_DATA_COMPARE_FAILED;
        }
    }

    printCryptoResult(retval, CRYPTO_AEAD_CIPHER_SUCCESS);

    if (retval != TEST_RESULT_SUCCESS)
    {
        printSeparator(BLUE, '-');
        printHexArray("Expected: ", expected, length);
        printSeparator(BLUE, '-');
        printHexArray("Result: ", result, length);
        printSeparator(BLUE, '-');
    }

    return retval;
}

/**
 * @brief Get the test operation mode string based on the mode.
 * @param opMode The AES AEAD operations mode.
 * @return Pointer to string.
 */
static const char *getAeadTestOpMode(AEAD_OPER_MODE opMode)
{
    const char* opModeStr = "???";

    switch(opMode)
    {
        case AEAD_OPER_MODE_AES_GCM:
            opModeStr = "AES-GCM";
            break;

        case AEAD_OPER_MODE_AES_CCM:
            opModeStr = "AES-CCM";
            break;

        default:
            break;
    }

    return opModeStr;
}


// *****************************************************************************
// *****************************************************************************
// Section: AES AEAD GCM test state machine Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief The AES AEAD test init state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_INIT_FAILED on failure.
 */
static TEST_RESULT aes_aead_test_init(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    AEAD_TEST_CONTEXT *testContext = (AEAD_TEST_CONTEXT *)context;
    AEAD_TEST_VECTOR *test = testContext->test;

    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    TEST_RESULT result;

#ifdef SHOW_TEST_DATA
    uint8_t *data = (testContext->testMode == TEST_MODE_ENCRYPT) ? test->plaintextData : test->ciphertextData;

    (void) printf(BLUE "\r\n\r\n %s %s Test %lu", getAeadTestOpMode(testContext->opMode), getTestMode(testContext->testMode), (unsigned long) test->id);

    printSeparator(BLUE, '-');
    printHexArray("Key", test->inputKey, test->inputKeyLength);
    printSeparator(BLUE, '-');
    printHexArray("Initialization Vector", test->initializationVector, test->initializationVectorLength);
    printSeparator(BLUE, '-');
    printHexArray("Authentication Data", test->aad, test->aadLength);
    printSeparator(BLUE, '-');
    printHexArray("Data (Input)", data, test->dataLength);

    if (testContext->testMode == TEST_MODE_DECRYPT)
    {
        printSeparator(BLUE, '-');
        printHexArray("Authentication Tag", test->authTag, test->authTagLength);
    }

    printSeparator(BLUE, '-');
#endif

    (void) printf(YELLOW);
    printOperationStep("INITIALIZING", getAeadTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);

    switch(testContext->opMode)
    {
        case AEAD_OPER_MODE_AES_GCM:
        {
            crypto_CipherOper_E oper = (testContext->testMode == TEST_MODE_ENCRYPT) ? CRYPTO_CIOP_ENCRYPT : CRYPTO_CIOP_DECRYPT;
            status = Crypto_Aead_AesGcm_Init(&aeadGcmCtx, CRYPTO_HANDLER_HW_INTERNAL, oper,
                                            test->inputKey, test->inputKeyLength, test->initializationVector, test->initializationVectorLength, TEST_SESSION_ID);
        }
        break;

        case AEAD_OPER_MODE_AES_CCM:
        {
            status = Crypto_Aead_AesCcm_Init(&aeadCcmCtx, CRYPTO_HANDLER_HW_INTERNAL,
                                            test->inputKey, test->inputKeyLength, TEST_SESSION_ID);
        }
        break;

        default:
            break;
    }

    result = (status == CRYPTO_AEAD_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_INIT_FAILED;

    printCryptoResult(result, status);

    return result;
}


/**
 * @brief The AES AEAD add AAD state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_ADD_AAD_FAILED on failure.
 */
static TEST_RESULT aes_aead_test_add_aad(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    AEAD_TEST_CONTEXT *testContext = (AEAD_TEST_CONTEXT *)context;
    AEAD_TEST_VECTOR *test = testContext->test;
    uint8_t *aad = (test->aadLength > 0UL) ? test->aad : NULL;

    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    TEST_RESULT result;

    (void) printf(YELLOW);
    printOperationStep("ADDING AAD", getAeadTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);

    status = Crypto_Aead_AesGcm_AddAadData(&aeadGcmCtx, aad, test->aadLength);
    result = (status == CRYPTO_AEAD_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_ADD_AAD_FAILED;

    printCryptoResult(result, status);

    return result;
}


/**
 * @brief The AES AEAD test cipher state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success, TEST_RESULT_CIPHER_FAILED on failure.
 */
static TEST_RESULT aes_aead_test_cipher(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    AEAD_TEST_CONTEXT *testContext = (AEAD_TEST_CONTEXT *)context;
    AEAD_TEST_VECTOR *test = testContext->test;

    uint8_t *data = (testContext->testMode == TEST_MODE_ENCRYPT) ? test->plaintextData : test->ciphertextData;
    uint8_t *resultData = (test->dataLength > 0U) ? testContext->dataResult : NULL;

    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    TEST_RESULT result;

    (void) printf(YELLOW);
    printOperationStep("CIPHERING", getAeadTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);

    status = Crypto_Aead_AesGcm_Cipher(&aeadGcmCtx, data, test->dataLength, resultData);
    result = (status == CRYPTO_AEAD_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_CIPHER_FAILED;

    printCryptoResult(result, status);

    return result;
}


/**
 * @brief The AES AEAD test finalize/compare state function.
 * @param context Pointer to context block.
 * @return TEST_RESULT_SUCCESS on success,
 *         TEST_RESULT_FINALIZE_FAILED/TEST_RESULT_DATA_COMPARE_FAILED/TEST_RESULT_TAG_COMPARE_FAILED on failure.
 */
static TEST_RESULT aes_aead_test_final(void *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    AEAD_TEST_CONTEXT *testContext = (AEAD_TEST_CONTEXT *)context;
    AEAD_TEST_VECTOR *test = testContext->test;

    crypto_Aead_Status_E status = CRYPTO_AEAD_ERROR_CIPFAIL;
    TEST_RESULT result = TEST_RESULT_FINALIZE_FAILED;

    (void) printf(YELLOW);
    printOperationStep("FINALIZING", getAeadTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);

    switch(testContext->opMode)
    {
        case AEAD_OPER_MODE_AES_GCM:
        {
            status = Crypto_Aead_AesGcm_Final(&aeadGcmCtx, testContext->authTagResult, test->authTagLength);
        }
        break;

        case AEAD_OPER_MODE_AES_CCM:
        {
            crypto_CipherOper_E oper = (testContext->testMode == TEST_MODE_ENCRYPT) ? CRYPTO_CIOP_ENCRYPT : CRYPTO_CIOP_DECRYPT;
            uint8_t *data = (testContext->testMode == TEST_MODE_ENCRYPT) ? test->plaintextData : test->ciphertextData;
            uint8_t *resultData = (test->dataLength > 0U) ? testContext->dataResult : NULL;

            status = Crypto_Aead_AesCcm_Cipher(&aeadCcmCtx, oper,
                                                data, test->dataLength, resultData,
                                                test->initializationVector, test->initializationVectorLength,
                                                testContext->authTagResult, test->authTagLength,
                                                test->aad, test->aadLength);
        }
        break;

        default:
            break;
    }

    result = (status == CRYPTO_AEAD_CIPHER_SUCCESS) ? TEST_RESULT_SUCCESS : TEST_RESULT_FINALIZE_FAILED;

    printCryptoResult(result, status);

    if (status == CRYPTO_AEAD_CIPHER_SUCCESS)
    {
#ifdef SHOW_TEST_DATA
        printSeparator(BLUE, '-');
        printHexArray("Data (Result)", testContext->dataResult, test->dataLength);

        if (testContext->testMode == TEST_MODE_ENCRYPT)
        {
            printSeparator(BLUE, '-');
            printHexArray("Authentication Tag", testContext->authTagResult, test->authTagLength);
        }

        printSeparator(BLUE, '-');
#endif

        (void) printf(YELLOW);
        printOperationStep("VERIFYING", getAeadTestOpMode(testContext->opMode), testContext->testMode, test->dataLength, true);

        // These calls also print out the test result...
        if (testContext->testMode == TEST_MODE_ENCRYPT)
        {
            result = checkEncryptionAndTag(test->ciphertextData, testContext->dataResult, test->dataLength, test->authTag, testContext->authTagResult, test->authTagLength);
        }
        else
        {
            result = checkDecryption(test->plaintextData, testContext->dataResult, test->dataLength);
        }
    }

    return result;
}

/**
 * @brief Defines the states for a AEAD test.
 */
enum {
    AEAD_TEST_STEP_INIT = 0,
    AEAD_TEST_STEP_ADD_AAD = 1,
    AEAD_TEST_STEP_CIPHER = 2,
    AEAD_TEST_STEP_FINAL = 3,
    AEAD_TEST_STEP_NUMBER_OF_STEPS
};

// *****************************************************************************
// *****************************************************************************
// Section: AES AEAD Test Functions
// *****************************************************************************
// *****************************************************************************

TEST_RESULT aead_test_init(TEST_CONTEXT *context, TEST_MODE testMode, AEAD_OPER_MODE opMode)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    AEAD_TEST_CONTEXT *testContext = (AEAD_TEST_CONTEXT *)context->testContext;
    TEST_RESULT result = TEST_RESULT_SUCCESS;

    (void) memset(context, 0, sizeof(TEST_CONTEXT));
    context->testContext = testContext;

    static TEST_STATE_FN aead_test_states[] =
    {
        aes_aead_test_init,
        aes_aead_test_add_aad,
        aes_aead_test_cipher,
        aes_aead_test_final,
    };

    // Select the AES AEAD operation mode.
    switch (opMode)
    {
        case AEAD_OPER_MODE_AES_GCM:
        {
            context->tests = (uint8_t *)get_gcm_test_vectors();
            context->states = aead_test_states;

            context->testCount = get_gcm_test_vector_count();
            context->stepCount = sizeof(aead_test_states) / sizeof(aead_test_states[0]);
        }
        break;

        case AEAD_OPER_MODE_AES_CCM:
        {
            context->tests = (uint8_t *)get_ccm_test_vectors();
            context->states = aead_test_states;

            context->testCount = get_ccm_test_vector_count();
            context->stepCount = sizeof(aead_test_states) / sizeof(aead_test_states[0]);
        }
        break;

        default:
        {
            (void) printf(RED "\r\n\r\n Unknown AEAD test operation mode %d" RESET_COLOR, opMode);
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
         *  Reasoning: A generic pointer allows support for various test vector types.
         */
        AEAD_TEST_VECTOR *tests = (AEAD_TEST_VECTOR *)context->tests;

        // Start at the first test.
        testContext->test = &tests[context->test];
    }

    return result;
}


TEST_RESULT aead_test_advance(TEST_CONTEXT *context)
{
    /* cppcheck-suppress misra-c2012-11.5
     *
     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
     *
     *  Reasoning: A generic pointer allows support for various algorithm test context types.
     */
    AEAD_TEST_CONTEXT *testContext = (AEAD_TEST_CONTEXT *)context->testContext;

    if (context->test < context->testCount)
    {
        // Execute the next step in the state machine.
        context->lastResult = context->states[context->step](testContext);
        if (context->lastResult != TEST_RESULT_SUCCESS)
        {
            context->step = AEAD_TEST_STEP_NUMBER_OF_STEPS;
        }

        switch (context->step)
        {
            case AEAD_TEST_STEP_INIT:
            {
                if (testContext->opMode == AEAD_OPER_MODE_AES_GCM)
                {
                    context->step = AEAD_TEST_STEP_ADD_AAD;
                }
                else if (testContext->opMode == AEAD_OPER_MODE_AES_CCM)
                {
                    context->step = AEAD_TEST_STEP_FINAL;
                }
                else
                {
                    context->step = AEAD_TEST_STEP_CIPHER;
                }
            }
            break;

            case AEAD_TEST_STEP_ADD_AAD:
            {
                context->step = AEAD_TEST_STEP_CIPHER;
            }
            break;

            case AEAD_TEST_STEP_CIPHER:
            {
                context->step = AEAD_TEST_STEP_FINAL;
            }
            break;

            case AEAD_TEST_STEP_FINAL:
            case AEAD_TEST_STEP_NUMBER_OF_STEPS:
            default:
            {
                context->test++;
                if (context->test < context->testCount)
                {
                    context->step = AEAD_TEST_STEP_INIT;

                    /* cppcheck-suppress misra-c2012-11.5
                     *
                     *  (Rule 11.5) Advisory: A conversion should not be performed from pointer to void into pointer to object
                     *
                     *  Reasoning: A generic pointer allows support for various algorithm test context types.
                     */
                    AEAD_TEST_VECTOR *tests = (AEAD_TEST_VECTOR *)context->tests;
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
