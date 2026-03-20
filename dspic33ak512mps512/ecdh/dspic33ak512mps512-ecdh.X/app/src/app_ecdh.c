/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecdh.c

  Summary:
    This source file handles the running of the ECDH examples.
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
*  (Rule 21.6) REQUIRED: The standard library input/output functions shall not
*  be used
*
*  Reasoning: sprintf is required for formatting the test header string in
*  this code example.
*/
#include <stdio.h>
#include "crypto/common_crypto/crypto_kas.h"
#include "app/app_ecdh.h"
#include "test_vectors/test_vector.h"
#include "utils/app_utils.h"

#define ECDH_MAX_SHARED_SECRET_LENGTH (66U)

static void ECDH_SharedSecretTestRun(ecdh_test_vector_t *vector)
{
    uint8_t sharedSecret[ECDH_MAX_SHARED_SECRET_LENGTH];
    char header[80];

    (void) sprintf(header, " %s ECDH",
            vector->curveInformation);

    UTILS_TestHeaderPrint(header);

    crypto_Kas_Status_E error = Crypto_Kas_Ecdh_SharedSecret(
            CRYPTO_HANDLER_HW_INTERNAL,
            vector->privateKeyData, vector->privateKeyLength,
            vector->publicKeyData, vector->publicKeyLength,
            sharedSecret, vector->expectedSharedSecretLength,
            vector->curveType, UTILS_SESSION_ID);

    UTILS_HexArrayPrint("Shared Secret        ",
            sharedSecret,
            vector->expectedSharedSecretLength);

    if (error == CRYPTO_KAS_SUCCESS)
    {
        UTILS_ArrayEqualityResultPrint(
                "ECDH SHARED SECRET",
                vector->expectedSharedSecret,
                sharedSecret,
                vector->expectedSharedSecretLength);
    }
    else
    {
        UTILS_CryptoErrorPrint("ECDH Shared Secret",
                (int) error);
    }
}

void ECDH_TestRun(void)
{
    ECDH_SharedSecretTestRun(&ecdhP192);
    ECDH_SharedSecretTestRun(&ecdhP256);
    ECDH_SharedSecretTestRun(&ecdhP384);
    ECDH_SharedSecretTestRun(&ecdhP521);
}
