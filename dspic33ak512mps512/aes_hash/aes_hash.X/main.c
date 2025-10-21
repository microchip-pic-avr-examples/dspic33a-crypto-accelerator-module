/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This source file starts the application to demonstrate AES encryption.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/* cppcheck-suppress misra-c2012-21.6
*
*  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
*
*  Reasoning: printf is required for printing to the terminal in this code example
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mcc_generated_files/system/system.h"
#include "app_aes_hash.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define RUN_SEPARATE_TESTS (defined(RUN_SYM_TEST) || defined(RUN_MAC_TEST) || defined(RUN_AEAD_TEST) || defined(RUN_HASH_TEST))

int main(void)
{
    SYSTEM_Initialize();

    (void) printf("\r\n\r\n\r\n");
    printSeparator(WHITE, '_');
    (void) printf("\r\n\r\n Starting CRYPTO AES/HASH MULTI-OPERATION program");

#if RUN_SEPARATE_TESTS
    printSeparator(WHITE, '_');
    (void) printf(GREEN "\r\n\r\n Executing in SEPARATE mode" RESET_COLOR);
    printSeparator(WHITE, '_');
    (void) printf("\r\n");

    #ifdef RUN_SYM_TEST
    printSeparator(BLUE, '_');
    aes_test_sym();
    #endif

    #ifdef RUN_MAC_TEST
    printSeparator(BLUE, '_');
    aes_test_mac();
    #endif

    #ifdef RUN_AEAD_TEST
    printSeparator(BLUE, '_');
    aes_test_aead();
    #endif

    #ifdef RUN_HASH_TEST
    printSeparator(BLUE, '_');
    test_hash();
    #endif
#endif

    #ifdef RUN_CONCURRENT_TEST
    printSeparator(WHITE, '_');
    (void) printf(GREEN "\r\n\r\n Executing in CONCURRENT mode" RESET_COLOR);

    test_concurrent();
    #endif

    printSeparator(WHITE, '_');
    (void) printf("\r\n");

    while(1)
    {

    };

    return EXIT_SUCCESS;
}