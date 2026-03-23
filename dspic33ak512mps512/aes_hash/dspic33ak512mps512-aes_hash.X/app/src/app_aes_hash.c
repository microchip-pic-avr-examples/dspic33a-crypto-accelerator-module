/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_aes_hash.c

  Summary:
    This source file handles the running of a multi-operation example to intersperse
    AES and HASH operations to show context safety of the CAM library.
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

/* cppcheck-suppress misra-c2012-17.1
*
*  (Rule 17.1) REQUIRED: The features of the standard argument header, stdarg.h, file shall not be used
*
*  Reasoning: printf is required for printing to the terminal in this code example
*/

#include <stdio.h>
#include <stdarg.h>

#include "app/app_aes_hash.h"

#include "app/app_sym.h"
#include "app/app_mac.h"
#include "app/app_aead.h"
#include "app/app_sha.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/* Test multiple iterations of incremental hashing with different block sizes
 * to show that arbitrary sizes are accepted. */
#define BLOCK_SIZE_128              (128UL)
#define BLOCK_SIZE_64               (64UL)
#define BLOCK_SIZE_32               (32UL)
#define BLOCK_SIZE_16               (16UL)
#define BLOCK_SIZE_13               (13UL)
#define BLOCK_SIZE_8                (8UL)
#define BLOCK_SIZE_7                (7UL)
#define BLOCK_SIZE_ITERATION_COUNT  (7)

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

const char *AESHASH_TestModeGet(test_mode_t testMode)
{
    const char *testModeString = "???";

    switch (testMode)
    {
        case TEST_MODE_ENCRYPT:
            testModeString = "ENCRYPT";
            break;

        case TEST_MODE_DECRYPT:
            testModeString = "DECRYPT";
            break;

        case TEST_MODE_HASH:
            testModeString = "HASH";
            break;

        default:
            break;
    }

    return testModeString;
}

typedef struct
{
    test_result_t value;
    const char *string;
} test_mode_error_string_t;

/**
 * @brief Get the error description string for a test result code.
 * @param result The test result code.
 * @return Pointer to the error description string.
 */
static const char *AESHASH_TestErrorGet(test_result_t result)
{
    static test_mode_error_string_t testModeErrorStrings[] =
    {
        {TEST_RESULT_SUCCESS, "SUCCESS"},
        {TEST_RESULT_INIT_FAILED, "INITIALIZE FAILED"},
        {TEST_RESULT_HASH_FAILED, "HASH FAILED"},
        {TEST_RESULT_ADD_AAD_FAILED, "ADD AAD FAILED"},
        {TEST_RESULT_CIPHER_FAILED, "CIPHER FAILED"},
        {TEST_RESULT_FINALIZE_FAILED, "FINALIZE FAILED"},
        {TEST_RESULT_DATA_COMPARE_FAILED, "DATA COMPARE FAILED"},
        {TEST_RESULT_TAG_COMPARE_FAILED, "TAG COMPARE FAILED"},
        {TEST_RESULT_ALL_TESTS_DONE, "ALL TESTS DONE"},
        {-1, NULL}
    };

    const char *testErrorString = "UNKNOWN";
    test_mode_error_string_t *err = testModeErrorStrings;

    do
    {
        if (result == err->value)
        {
            testErrorString = err->string;
            break;
        }

        err++;
    } while (NULL != err->string);

    return testErrorString;
}

void AESHASH_LabelPrint(const char *label, const char *format, ...)
{
    // cppcheck-suppress misra-c2012-17.1
    va_list args;

    // cppcheck-suppress misra-c2012-17.1
    va_start(args, format);

    (void) printf("\r\n %-22s : ", label);
    (void) vfprintf(stdout, format, args);

    // cppcheck-suppress misra-c2012-17.1
    va_end(args);
}

void AESHASH_OperationStepPrint(const char *step, const char *op,
        test_mode_t testMode, uint32_t size, bool printSize)
{
    if (true == printSize)
    {
        (void) printf("\r\n %-12s %-10s %-8s OPERATION (%-3lu BYTES)... ",
                step, op, AESHASH_TestModeGet(testMode),
                (unsigned long) size);
    }
    else
    {
        (void) printf("\r\n %-12s %-10s %-8s OPERATION... ",
                step, op, AESHASH_TestModeGet(testMode));
    }
}

void AESHASH_CryptoResultPrint(test_result_t result, int status)
{
    if (result == TEST_RESULT_SUCCESS)
    {
        (void) printf(UTILS_GREEN " PASS" UTILS_RESET_COLOR);
    }
    else
    {
        (void) printf(UTILS_RED " %s: operation status %d"
                UTILS_RESET_COLOR,
                AESHASH_TestErrorGet(result), status);
    }
}

/*******************************************************************************
 * Symmetric tests
 *******************************************************************************/
#ifdef RUN_SYM_TEST

#ifdef RUN_SYM_ECB_TEST
/**
 * @brief Run AES-ECB symmetric encrypt and decrypt tests.
 */
static void AESHASH_EcbTestRun(void)
{
    test_result_t result;
    sym_test_context_t ecbTestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &ecbTestContext
    };
    (void) printf(UTILS_BLUE "\r\n\r\n Starting SYMMETRIC AES-ECB mode test\r\n" UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_ECB);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_ECB);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

#ifdef RUN_SYM_CTR_TEST
/**
 * @brief Run AES-CTR symmetric encrypt and decrypt tests.
 */
static void AESHASH_CtrTestRun(void)
{
    test_result_t result;
    sym_test_context_t ctrTestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &ctrTestContext
    };
    (void) printf(UTILS_BLUE "\r\n\r\n Starting SYMMETRIC AES-CTR mode test\r\n" UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_CTR);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_CTR);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

#ifdef RUN_SYM_CBC_TEST
/**
 * @brief Run AES-CBC symmetric encrypt and decrypt tests.
 */
static void AESHASH_CbcTestRun(void)
{
    test_result_t result;
    sym_test_context_t cbcTestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &cbcTestContext
    };
    (void) printf(UTILS_BLUE "\r\n\r\n Starting SYMMETRIC AES-CBC mode test\r\n" UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_CBC);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_CBC);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

#ifdef RUN_SYM_CFB128_TEST
/**
 * @brief Run AES-CFB128 symmetric encrypt and decrypt tests.
 */
static void AESHASH_Cfb128TestRun(void)
{
    test_result_t result;
    sym_test_context_t cfb128TestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &cfb128TestContext
    };

    (void) printf(UTILS_BLUE
            "\r\n\r\n Starting SYMMETRIC AES-CFB128 mode test\r\n"
            UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_CFB128);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_CFB128);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

#ifdef RUN_SYM_OFB_TEST
/**
 * @brief Run AES-OFB symmetric encrypt and decrypt tests.
 */
static void AESHASH_OfbTestRun(void)
{
    test_result_t result;
    sym_test_context_t ofbTestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &ofbTestContext
    };

    (void) printf(UTILS_BLUE "\r\n\r\n Starting SYMMETRIC AES-OFB mode test\r\n" UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_OFB);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_OFB);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

#ifdef RUN_SYM_XTS_TEST
/**
 * @brief Run AES-XTS symmetric encrypt and decrypt tests.
 */
static void AESHASH_XtsTestRun(void)
{
    test_result_t result;
    sym_test_context_t xtsTestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &xtsTestContext
    };

    (void) printf(UTILS_BLUE "\r\n\r\n Starting SYMMETRIC AES-XTS mode test\r\n" UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_XTS);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) SYM_ContextInit(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_XTS);
    do
    {
        do
        {
            result = SYM_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

void AESHASH_SymTestRun(void)
{
#ifdef RUN_SYM_ECB_TEST
    AESHASH_EcbTestRun();
#endif

#ifdef RUN_SYM_CTR_TEST
    AESHASH_CtrTestRun();
#endif

#ifdef RUN_SYM_CBC_TEST
    AESHASH_CbcTestRun();
#endif

#ifdef RUN_SYM_CFB128_TEST
    AESHASH_Cfb128TestRun();
#endif

#ifdef RUN_SYM_OFB_TEST
    AESHASH_OfbTestRun();
#endif

#ifdef RUN_SYM_XTS_TEST
    AESHASH_XtsTestRun();
#endif
}

#endif // RUN_SYM_TEST

/*******************************************************************************
 * MAC tests
 *******************************************************************************/
#ifdef RUN_MAC_TEST

void AESHASH_MacTestRun(void)
{
#ifdef RUN_MAC_CMAC_TEST
    test_result_t result;
    aes_cmac_test_context_t macTestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &macTestContext
    };

    (void) printf(UTILS_BLUE "\r\n\r\n Starting MAC AES-CMAC mode test\r\n" UTILS_RESET_COLOR);

    // Run CMAC operations.
    (void) MAC_CmacContextInit(&context, TEST_MODE_ENCRYPT, MAC_OPER_MODE_AES_CMAC);
    do
    {
        do
        {
            result = MAC_CmacTestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

#endif
}

#endif // RUN_MAC_TEST

/*******************************************************************************
 * AEAD tests
 *******************************************************************************/
#ifdef RUN_AEAD_TEST

void AESHASH_AeadTestRun(void)
{
#if defined(RUN_AEAD_GCM_TEST) || defined(RUN_AEAD_CCM_TEST)
    test_result_t result;
    aead_test_context_t aeadTestContext = {0};
    test_context_t context =
    {
        .testContext = (void *) &aeadTestContext
    };
#endif

#ifdef RUN_AEAD_GCM_TEST
    (void) printf(UTILS_BLUE "\r\n\r\n Starting AEAD AES-GCM mode test\r\n" UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) AEAD_ContextInit(&context, TEST_MODE_ENCRYPT, AEAD_OPER_MODE_AES_GCM);
    do
    {
        do
        {
            result = AEAD_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) AEAD_ContextInit(&context, TEST_MODE_DECRYPT, AEAD_OPER_MODE_AES_GCM);
    do
    {
        do
        {
            result = AEAD_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
#endif

#ifdef RUN_AEAD_CCM_TEST
    (void) printf(UTILS_BLUE "\r\n\r\n Starting AEAD AES-CCM mode test\r\n" UTILS_RESET_COLOR);

    // Run encryption operations.
    (void) AEAD_ContextInit(&context, TEST_MODE_ENCRYPT, AEAD_OPER_MODE_AES_CCM);
    do
    {
        do
        {
            result = AEAD_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) AEAD_ContextInit(&context, TEST_MODE_DECRYPT, AEAD_OPER_MODE_AES_CCM);
    do
    {
        do
        {
            result = AEAD_TestAdvance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
#endif
}

#endif // RUN_AEAD_TEST

/*******************************************************************************
 * HASH tests
 *******************************************************************************/
#ifdef RUN_HASH_TEST

void AESHASH_HashTestRun(void)
{
    test_result_t result;
    test_context_t context;

    uint32_t blockSizes[] =
    {
        BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32,
        BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8,
        BLOCK_SIZE_7
    };
    int iterationCount = (sizeof(blockSizes) / sizeof(blockSizes[0]));

    (void) printf(UTILS_BLUE
            "\r\n\r\n Starting HASH test\r\n" UTILS_RESET_COLOR);

    for (int iteration = 0; iteration < iterationCount; iteration++)
    {
        uint32_t hashBlockSize = blockSizes[iteration];

        UTILS_SeparatorPrint(UTILS_BLUE, '_');
        (void) printf(UTILS_BLUE
                "\r\n\r\n Test Iteration %d: Hash Block Size %lu"
                UTILS_RESET_COLOR, (iteration + 1),
                (unsigned long) hashBlockSize);

        hash_test_context_t hashTestContext = {0};
        context.testContext = (void *) &hashTestContext;

        // Run hash operations.
        (void) SHA_ContextInit(&context, TEST_MODE_HASH,
                CRYPTO_HASH_SHA2_512, hashBlockSize);
        do
        {
            do
            {
                result = SHA_TestAdvance(&context);
            } while (result == TEST_RESULT_SUCCESS);

        } while (result != TEST_RESULT_ALL_TESTS_DONE);
    }
}

#endif // RUN_HASH_TEST

#ifdef RUN_CONCURRENT_TEST

// The number of iterations of the test.  This cannot be more than the number of hash block sizes!
#define TEST_ITERATIONS (7)

#if TEST_ITERATIONS > BLOCK_SIZE_ITERATION_COUNT
#error TEST_ITERATIONS CANNOT EXCEED BLOCK_SIZE_ITERATION_COUNT.
#endif

void AESHASH_ConcurrentTestRun(void)
{

    sym_test_context_t symTestContext = {0};
    test_context_t symContext =
    {
        .testContext = (void *) &symTestContext
    };

    aead_test_context_t aeadTestContext = {0};
    test_context_t aeadContext =
    {
        .testContext = (void *) &aeadTestContext
    };

    aes_cmac_test_context_t macTestContext = {0};
    test_context_t macContext =
    {
        .testContext = (void *) &macTestContext
    };

    test_context_t hashContext = {0};

    test_mode_t macTestMode = TEST_MODE_ENCRYPT;
    test_mode_t aeadTestMode = TEST_MODE_ENCRYPT;
    test_mode_t symTestMode = TEST_MODE_ENCRYPT;
    sym_oper_mode_t symOperMode = SYM_OPER_MODE_AES_ECB;
    aead_oper_mode_t aeadOperMode = AEAD_OPER_MODE_AES_GCM;

    // Hash block sizes for iterative testing of arbitrary input sizes.
    const uint32_t blockSizes[] =
    {
        BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32,
        BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8,
        BLOCK_SIZE_7
    };

    for (int iteration = 0; iteration < TEST_ITERATIONS; iteration++)
    {
        test_result_t symResult;
        test_result_t aeadResult;
        test_result_t macResult;
        test_result_t hashResult;

        uint32_t hashBlockSize = blockSizes[iteration];

        UTILS_SeparatorPrint(UTILS_BLUE, '_');
        (void) printf(UTILS_BLUE
                "\r\n\r\n Test Iteration %d: Hash Block Size %lu\r\n",
                (iteration + 1), (unsigned long) hashBlockSize);

        // Initialize one of each context.
        symResult = SYM_ContextInit(&symContext, TEST_MODE_ENCRYPT, symOperMode);
        aeadResult = AEAD_ContextInit(&aeadContext, TEST_MODE_ENCRYPT, aeadOperMode);
        macResult = MAC_CmacContextInit(&macContext,
                TEST_MODE_ENCRYPT, MAC_OPER_MODE_AES_CMAC);

        hash_test_context_t hashTestContext = {0};
        hashContext.testContext = (void *) &hashTestContext;

        hashResult = SHA_ContextInit(&hashContext, TEST_MODE_HASH,
                CRYPTO_HASH_SHA2_512, hashBlockSize);

        // If any initializations fail, stop.
        if ((symResult || aeadResult || macResult || hashResult))
        {
            (void) printf(UTILS_RED "\r\n\r\nFailed to initialize tests!");
            break;
        }

        // Cycle through the contexts to interleave executing steps of each operation.
        do
        {
            symResult = SYM_TestAdvance(&symContext);
            aeadResult = AEAD_TestAdvance(&aeadContext);
            macResult = MAC_CmacTestAdvance(&macContext);
            hashResult = SHA_TestAdvance(&hashContext);

            /*
             * If any but the hash tests finish, restart them.
             * The hash test has the most iterations.
             */
            if (symResult == TEST_RESULT_ALL_TESTS_DONE)
            {
                if (symTestMode == TEST_MODE_ENCRYPT)
                {
                    // Flip symmetric test mode.
                    symTestMode = TEST_MODE_DECRYPT;
                }
                else
                {
                    // Flip symmetric test mode and operations mode.
                    symTestMode = TEST_MODE_ENCRYPT;
                    int nextSymMode = (int)symOperMode + 1;
                    symOperMode = (sym_oper_mode_t)nextSymMode;
                    if (symOperMode == SYM_OPER_MODE_COUNT)
                    {
                        symOperMode = SYM_OPER_MODE_AES_ECB;
                    }
                }

                (void) SYM_ContextInit(&symContext, symTestMode, symOperMode);
            }

            if (aeadResult == TEST_RESULT_ALL_TESTS_DONE)
            {
                // Flip AEAD test mode.
                if (aeadTestMode == TEST_MODE_ENCRYPT)
                {
                    // Flip symmetric test mode.
                    aeadTestMode = TEST_MODE_DECRYPT;
                }
                else
                {
                    // Flip AEAD test mode and operations mode.
                    aeadTestMode = TEST_MODE_ENCRYPT;
                    int nextAeadMode = (int)aeadOperMode + 1;
                    aeadOperMode = (aead_oper_mode_t)nextAeadMode;
                    if (aeadOperMode == AEAD_OPER_MODE_COUNT)
                    {
                        aeadOperMode = AEAD_OPER_MODE_AES_GCM;
                    }
                }

                (void) AEAD_ContextInit(&aeadContext, aeadTestMode, aeadOperMode);
            }

            if (macResult == TEST_RESULT_ALL_TESTS_DONE)
            {
                (void) MAC_CmacContextInit(&macContext, macTestMode, MAC_OPER_MODE_AES_CMAC);
            }

        } while (hashResult != TEST_RESULT_ALL_TESTS_DONE);

        // Finish any resmining tests.
        while ((symResult != TEST_RESULT_ALL_TESTS_DONE) ||
               (aeadResult != TEST_RESULT_ALL_TESTS_DONE) ||
               (macResult != TEST_RESULT_ALL_TESTS_DONE))
        {
            symResult = SYM_TestAdvance(&symContext);
            aeadResult = AEAD_TestAdvance(&aeadContext);
            macResult = MAC_CmacTestAdvance(&macContext);
        }
    }
}
#endif
