/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

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

#ifndef APP_SHA_H
#define	APP_SHA_H

#ifdef	__cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define RESET_COLOR "\x1B""[0m"
#define RED "\x1B""[1;31m"
#define GREEN "\x1B""[1;32m"
#define YELLOW "\x1B""[1;33m"
#define BLUE "\x1B""[1;34m"
#define MAG "\x1B""[1;35m"
#define CYAN "\x1B""[1;36m"

// Uncomment the #define RUN_ALL_VECTORS below to run with all defined test vectors
//#define RUN_ALL_VECTORS
    
// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void sha_1_test(void);
void sha_224_test(void);
void sha_256_test(void);
void sha_384_test(void);
void sha_512_test(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_SHA_H */

