/*******************************************************************************
  Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sha.h

  Summary:
    This header file provides prototypes and definitions for the SHA application.
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

#define BLOCK_SIZE_144     (144UL)
#define BLOCK_SIZE_128     (128UL)
#define BLOCK_SIZE_64      (64UL)
#define BLOCK_SIZE_32      (32UL)
#define BLOCK_SIZE_16      (16UL)
#define BLOCK_SIZE_13      (13UL)
#define BLOCK_SIZE_8       (8UL)
#define BLOCK_SIZE_7       (7UL)

#define SINGLE_STEP        (0U)
#define MULTI_STEP         (1U)
#define TEST_VECTOR_4  (4U)

#define SESSION_ID         (1U)

// Uncomment the #define RUN_ALL_VECTORS below to run with all defined test vectors
#define RUN_ALL_VECTORS

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void SHA_1_Digest(void);

void SHA_224_Digest(void);
void SHA_256_Digest(void);
void SHA_384_Digest(void);
void SHA_512_Digest(void);

void SHA3_224_Digest(void);
void SHA3_256_Digest(void);
void SHA3_384_Digest(void);
void SHA3_512_Digest(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_SHA_H */

