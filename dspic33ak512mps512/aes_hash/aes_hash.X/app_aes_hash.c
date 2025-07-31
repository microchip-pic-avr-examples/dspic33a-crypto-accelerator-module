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

/* cppcheck-suppress misra-c2012-17.1
*
*  (Rule 17.1) REQUIRED: The features of the standard argument header, stdarg.h, file shall not be used
*
*  Reasoning: printf is required for printing to the terminal in this code example
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "app_aes_hash.h"

#include "app/app_sym.h"
#include "app/app_mac.h"
#include "app/app_aead.h"
#include "app/app_sha.h"

#include "crypto/common_crypto/crypto_common.h"


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/* Test multiple iterations of incremental hashing with different block sizes
 * to show that arbitrary sizes are accepted. */
#define BLOCK_SIZE_128              128UL
#define BLOCK_SIZE_64               64UL
#define BLOCK_SIZE_32               32UL
#define BLOCK_SIZE_16               16UL
#define BLOCK_SIZE_13               13UL
#define BLOCK_SIZE_8                8UL
#define BLOCK_SIZE_7                7UL
#define BLOCK_SIZE_ITERATION_COUNT  7


// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

bool checkArrayEquality(const uint8_t *a, const uint8_t *b, size_t size)
{
	uint8_t result = 0U;
    bool success = false;

    if (size > 0U)
    {
        const uint8_t *tempa = a;
        const uint8_t *tempb = b;

        for (unsigned int i = 0; i < size; i++)
        {
            result |= tempa[i] ^ tempb[i];
        }
    }

    success = (result == 0U);

    return success;
}


const char *getTestMode(TEST_MODE testMode)
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


typedef struct TEST_MODE_ERROR_STRING {
    TEST_RESULT value;
    const char *string;
} TEST_MODE_ERROR_STRING;

static TEST_MODE_ERROR_STRING testModeErrorStrings[] = {

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

const char * getTestError(TEST_RESULT result)
{
    const char *testErrorString = "UNKNOWN";
    TEST_MODE_ERROR_STRING *err = testModeErrorStrings;

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


#define HEADER_STR_SZ (105)
void printSeparator(const char *color, char sep)
{
    char header[HEADER_STR_SZ + 1];
    (void) snprintf(header, HEADER_STR_SZ, "%0*d", (HEADER_STR_SZ - 1), 0);
    for (int i = 0; i < HEADER_STR_SZ; i++)
    {
        if (header[i] == '0')
        {
            header[i] = sep;
        }
    }

    (void) printf("\r\n %s%s" RESET_COLOR, color, header);
}


void printWithLabel(const char* label, const char* format, ...)
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


void printHexArray(const char* label, void* data, uint32_t size)
{
    (void) printf("\r\n %-22s :", label);

    if (size > 0U)
    {
        const uint8_t *ldata = (uint8_t *) data;

        for (uint32_t index = 0U; index < size; index++)
        {
            if (((index % 16U) == 0U) && (index != 0U))
            {
                (void) printf("\r\n%-25s", "");
            }

            (void) printf(" 0x%02x", ldata[index]);
        }
    }
    else
    {
        (void) printf(" None");
    }
}


void printOperationStep(const char *step, const char *op, TEST_MODE testMode, uint32_t size, bool printSize)
{
    if (true == printSize)
    {
        (void) printf("\r\n %-12s %-8s %-8s OPERATION (%-3lu BYTES)... ", step, op, getTestMode(testMode), size);
    }
    else
    {
        (void) printf("\r\n %-12s %-8s %-8s OPERATION... ", step, op, getTestMode(testMode));
    }
}


void printCryptoResult(TEST_RESULT result, int status)
{
    if (result == TEST_RESULT_SUCCESS)
    {
        (void) printf(GREEN " PASS" RESET_COLOR);
    }
    else
    {
        (void) printf(RED " %s: operation status %d" RESET_COLOR, getTestError(result), status);
    }
}


/*******************************************************************************
 * Symmetric tests
 *******************************************************************************/
#ifdef RUN_SYM_TEST

#ifdef RUN_SYM_ECB_TEST
void aes_test_ecb(void)
{
    TEST_CONTEXT context;
    TEST_RESULT result;

    (void) printf(BLUE "\r\n\r\n Starting SYMMETRIC AES-ECB mode test\r\n" RESET_COLOR);

    // Run encryption operations.
    (void) sym_test_init(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_ECB);
    do
    {
        do
        {
            result = sym_test_advance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) sym_test_init(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_ECB);
    do
    {
        do
        {
            result = sym_test_advance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

#ifdef RUN_SYM_CTR_TEST
void aes_test_ctr(void)
{
    TEST_CONTEXT context;
    TEST_RESULT result;

    (void) printf(BLUE "\r\n\r\n Starting SYMMETRIC AES-CTR mode test\r\n" RESET_COLOR);

    // Run encryption operations.
    (void) sym_test_init(&context, TEST_MODE_ENCRYPT, SYM_OPER_MODE_AES_CTR);
    do
    {
        do
        {
            result = sym_test_advance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) sym_test_init(&context, TEST_MODE_DECRYPT, SYM_OPER_MODE_AES_CTR);
    do
    {
        do
        {
            result = sym_test_advance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);
}
#endif

void aes_test_sym(void)
{
#ifdef RUN_SYM_ECB_TEST
    aes_test_ecb();
#endif

#ifdef RUN_SYM_CTR_TEST
    aes_test_ctr();
#endif
}

#endif // RUN_SYM_TEST


/*******************************************************************************
 * MAC tests
 *******************************************************************************/
#ifdef RUN_MAC_TEST

void aes_test_mac(void)
{
#ifdef RUN_MAC_CMAC_TEST
    TEST_CONTEXT context;
    TEST_RESULT result;

    (void) printf(BLUE "\r\n\r\n Starting MAC AES-CMAC mode test\r\n" RESET_COLOR);

    // Run CMAC operations.
    (void) mac_cmac_test_init(&context, TEST_MODE_ENCRYPT, MAC_OPER_MODE_AES_CMAC);
    do
    {
        do
        {
            result = mac_cmac_test_advance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

#endif
}

#endif // RUN_MAC_TEST


/*******************************************************************************
 * AEAD tests
 *******************************************************************************/
#ifdef RUN_AEAD_TEST

void aes_test_aead(void)
{
#ifdef RUN_AEAD_GCM_TEST
    TEST_CONTEXT context;
    TEST_RESULT result;

    (void) printf(BLUE "\r\n\r\n Starting AEAD AES-GCM mode test\r\n" RESET_COLOR);

    // Run encryption operations.
    (void) aead_test_init(&context, TEST_MODE_ENCRYPT, AEAD_OPER_MODE_AES_GCM);
    do
    {
        do
        {
            result = aead_test_advance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

    // Run decryption operations.
    (void) aead_test_init(&context, TEST_MODE_DECRYPT, AEAD_OPER_MODE_AES_GCM);
    do
    {
        do
        {
            result = aead_test_advance(&context);
        } while (result == TEST_RESULT_SUCCESS);

    } while (result != TEST_RESULT_ALL_TESTS_DONE);

#endif
}

#endif // RUN_AEAD_TEST


/*******************************************************************************
 * HASH tests
 *******************************************************************************/
#ifdef RUN_HASH_TEST

void test_hash(void)
{
    TEST_CONTEXT context;
    TEST_RESULT result;

    uint32_t blockSizes[] = {BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32, BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8, BLOCK_SIZE_7};
    int iterCount = (sizeof(blockSizes) / sizeof(blockSizes[0]));

    (void) printf(BLUE "\r\n\r\n Starting HASH test\r\n" RESET_COLOR);

    for (int iter = 0; iter < iterCount; iter++)
    {
        uint32_t hashBlockSize = blockSizes[iter];

        printSeparator(BLUE, '_');
        (void) printf(BLUE "\r\n\r\n Test Iteration %d: Hash Block Size %lu" RESET_COLOR, (iter + 1), hashBlockSize);

        // Run hash operations.
        (void) hash_test_init(&context, TEST_MODE_HASH, CRYPTO_HASH_SHA2_512, hashBlockSize);
        do
        {
            do
            {
                result = hash_test_advance(&context);
            } while (result == TEST_RESULT_SUCCESS);

        } while (result != TEST_RESULT_ALL_TESTS_DONE);
    }
}

#endif // RUN_HASH_TEST

#ifdef RUN_CONCURRENT_TEST

// The number of iterations of the test.  This cannot be more than the number of hash block sizes!
#define TEST_ITERATIONS 7

#if TEST_ITERATIONS > BLOCK_SIZE_ITERATION_COUNT
#error TEST_ITERATIONS CANNOT EXCEED BLOCK_SIZE_ITERATION_COUNT.
#endif

void test_concurrent(void)
{
    TEST_CONTEXT symContext;
    TEST_CONTEXT aeadContext;
    TEST_CONTEXT macContext;
    TEST_CONTEXT hashContext;

    TEST_MODE macTestMode = TEST_MODE_ENCRYPT;
    TEST_MODE aeadTestMode = TEST_MODE_ENCRYPT;
    TEST_MODE symTestMode = TEST_MODE_ENCRYPT;
    SYM_OPER_MODE symOperMode = SYM_OPER_MODE_AES_ECB;

    // Hash block sizes for iterative testing of arbitrary input sizes.
    const uint32_t blockSizes[] = {BLOCK_SIZE_128, BLOCK_SIZE_64, BLOCK_SIZE_32, BLOCK_SIZE_16, BLOCK_SIZE_13, BLOCK_SIZE_8, BLOCK_SIZE_7};

    for (int iter = 0; iter < TEST_ITERATIONS; iter++)
    {
        TEST_RESULT symResult;
        TEST_RESULT aeadResult;
        TEST_RESULT macResult;
        TEST_RESULT hashResult;

        uint32_t hashBlockSize = blockSizes[iter];

        printSeparator(BLUE, '_');
        (void) printf(BLUE "\r\n\r\n Test Iteration %d: Hash Block Size %lu\r\n", (iter + 1), hashBlockSize);

        // Initialize one of each context.
        symResult = sym_test_init(&symContext, TEST_MODE_ENCRYPT, symOperMode);
        aeadResult = aead_test_init(&aeadContext, TEST_MODE_ENCRYPT, AEAD_OPER_MODE_AES_GCM);
        macResult = mac_cmac_test_init(&macContext, TEST_MODE_ENCRYPT, MAC_OPER_MODE_AES_CMAC);
        hashResult = hash_test_init(&hashContext, TEST_MODE_HASH, CRYPTO_HASH_SHA2_512, hashBlockSize);

        // If any initializations fail, stop.
        if ((symResult || aeadResult || macResult || hashResult))
        {
            (void) printf(RED "\r\n\r\nFailed to initialize tests!");
            break;
        }

        // Cycle through the contexts to interleave executing steps of each operation.
        do
        {
            symResult = sym_test_advance(&symContext);
            aeadResult = aead_test_advance(&aeadContext);
            macResult = mac_cmac_test_advance(&macContext);
            hashResult = hash_test_advance(&hashContext);

            // If any but the hash tests finish, restart them.  The hash test has the most iterations.
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
                    symOperMode = (symOperMode == SYM_OPER_MODE_AES_ECB) ? SYM_OPER_MODE_AES_CTR : SYM_OPER_MODE_AES_ECB;
                }

                (void)sym_test_init(&symContext, symTestMode, symOperMode);
            }

            if (aeadResult == TEST_RESULT_ALL_TESTS_DONE)
            {
                // Flip AEAD test mode.
                aeadTestMode = (aeadTestMode == TEST_MODE_ENCRYPT) ? TEST_MODE_DECRYPT : TEST_MODE_ENCRYPT;
                (void)aead_test_init(&aeadContext, aeadTestMode, AEAD_OPER_MODE_AES_GCM);
            }

            if (macResult == TEST_RESULT_ALL_TESTS_DONE)
            {
                (void)mac_cmac_test_init(&macContext, macTestMode, MAC_OPER_MODE_AES_CMAC);
            }

        } while (hashResult != TEST_RESULT_ALL_TESTS_DONE);

        // Finish any resmining tests.
        while ((symResult != TEST_RESULT_ALL_TESTS_DONE) ||
               (aeadResult != TEST_RESULT_ALL_TESTS_DONE) ||
               (macResult != TEST_RESULT_ALL_TESTS_DONE))
        {
            symResult = sym_test_advance(&symContext);
            aeadResult = aead_test_advance(&aeadContext);
            macResult = mac_cmac_test_advance(&macContext);
        }
    }
}
#endif
