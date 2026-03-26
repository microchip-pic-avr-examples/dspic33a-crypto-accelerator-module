/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecdh.h

  Summary:
    This header file provides prototypes and definitions for ECDH Sign and 
    verify examples.
*******************************************************************************/

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

#ifndef APP_ECDH_H
#define	APP_ECDH_H
#include <xc.h>

#define P28_setLow()    (_LATC8 = 0)
#define P28_setHigh()   (_LATC8 = 1)
#define P28_toggle()    (_LATC8 ^= 1)
#define Enable_P28()    (_TRISC8 = 0)


#define FAIL_setLow()    (_LATD9 = 0)
#define FAIL_setHigh()    (_LATD9 = 1)
#define Enable_FAIL()    (_TRISD9 = 0)

#define PASS_setLow()    (_LATD0 = 0)
#define PASS_setHigh()    (_LATD0 = 1)
#define Enable_PASS()    (_TRISD0 = 0)

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************
void test_ecdh(void);

#endif	/* APP_ECDH_H */