/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_aes_hash.h

  Summary:
    This header file provides prototypes and definitions for the application.
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

#ifndef APP_AES_HASH_H
#define APP_AES_HASH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// *****************************************************************************
// *****************************************************************************
// Section: Configuration Options
// *****************************************************************************
// *****************************************************************************

#define RUN_SYM_TEST
#define RUN_MAC_TEST
#define RUN_AEAD_TEST
#define RUN_HASH_TEST
#define RUN_CONCURRENT_TEST

/* Uncomment this to see the test input/result data for each test,
 * or define it as a C macro in the project configuration. */
//#define SHOW_TEST_DATA

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define RESET_COLOR "\x1B""[0m"     // Reset the print output to default
#define RED         "\x1B""[1;31m"  // Set output to bold red
#define GREEN       "\x1B""[1;32m"  // Set output to bold green
#define YELLOW      "\x1B""[1;33m"  // Set output to bold yellow
#define BLUE        "\x1B""[1;34m"  // Set output to bold blue
#define MAG         "\x1B""[1;35m"  // set output to bold magenta
#define CYAN        "\x1B""[1;36m"  // Set output to bold cyan
#define WHITE       "\x1B""[1;37m"  // Set output to bold white

/**
 * @brief Defines the test operation mode (encrypt/hash/decrypt).
 */
typedef enum
{
    TEST_MODE_ENCRYPT = 0,
    TEST_MODE_DECRYPT = 1,
    TEST_MODE_HASH = 2,
} test_mode_t;

/**
 * @brief Defines test results.
 */
typedef enum
{
    TEST_RESULT_SUCCESS = 0,
    TEST_RESULT_INIT_FAILED = 1,
    TEST_RESULT_HASH_FAILED = 2,
    TEST_RESULT_ADD_AAD_FAILED = 3,
    TEST_RESULT_CIPHER_FAILED = 4,
    TEST_RESULT_FINALIZE_FAILED = 5,
    TEST_RESULT_DATA_COMPARE_FAILED = 6,
    TEST_RESULT_TAG_COMPARE_FAILED = 7,

    TEST_RESULT_ALL_TESTS_DONE = 255
} test_result_t;

/**
 * @brief Defines a callback for each test step.
 */
typedef test_result_t (*test_state_fn_t)(void *);

/**
 * Defines a test context.
 */
typedef struct test_context
{
    // Pointer to next test context to process.
    struct test_context *next;

    void *tests;                // Pointer to module-specific test contents.
    test_state_fn_t *states;    // Pointer to test states.

    uint8_t testCount;          // Number of tests to execute for the subsystem.
    uint8_t test;               // The current test number.
    uint8_t stepCount;          // Number of steps in the test state machine.
    uint8_t step;               // Current step (set to stepCount on failure).

    test_mode_t testMode;       // Test operation mode (encrypt/decrypt/hash/etc.)

    // Pointer to module specific test context data.
    void *testContext;

    test_result_t lastResult;   // Last test result

} test_context_t;

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Check that two arrays are identical to the specified length.
 * @param a Array 1.
 * @param b Array 2.
 * @param size The length of the data to check.
 * @return TRUE if equal, FALSE if not.
 */
bool AESHASH_ArrayEqualityCheck(const uint8_t *expected,
        const uint8_t *actual, size_t size);

/**
 * @brief Print a separator line.
 * @param color The color string to print first.
 * @param sep The character to print as the separator.
 */
void AESHASH_SeparatorPrint(const char *color, char sep);

/**
 * @brief Print to console with a header label.
 * @param label A label to apply to the output.
 * @param format POinter to format string.
 * @param <variable arguments>
 */
void AESHASH_LabelPrint(const char *label, const char *format, ...);

/**
 * @brief Print an array to the console as hex characters.
 * @param label A label to apply to the output.
 * @param data The array data to print.
 * @param size The length of the data to print.
 */
void AESHASH_HexArrayPrint(const char *label, uint8_t *data, uint32_t size);

/**
 * @brief Get a string representing the test mode.
 * @param testMode The test mode.
 * @return A string representing the test mode.
 */
const char *AESHASH_TestModeGet(test_mode_t testMode);

/**
 * @brief Print the current operation step.
 * @param step String describing the current step.
 * @param op String describing the current operation type.
 * @param testMode The test mode.
 * @param size The data size in bytes
 * @param printSize If TRUE, print the size.
 */
void AESHASH_OperationStepPrint(const char *step, const char *op,
        test_mode_t testMode, uint32_t size, bool printSize);

/**
 * @brief Print out success or error.
 * @param result The test result error code.
 * @param status The status of the operation.
 */
void AESHASH_CryptoResultPrint(test_result_t result, int status);

/**
 * @brief Run the symmetric AES tests.
 */
void AESHASH_SymTestRun(void);

/**
 * @brief Run the AES MAC tests.
 */
void AESHASH_MacTestRun(void);

/**
 * @brief Run the AES AEAD tests.
 */
void AESHASH_AeadTestRun(void);

/**
 * @brief Run the HASH tests.
 */
void AESHASH_HashTestRun(void);

/**
 * @brief Run the concurrent mode tests.
 */
void AESHASH_ConcurrentTestRun(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_AES_HASH_H */
