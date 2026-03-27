<picture>
    <source media="(prefers-color-scheme: dark)" srcset="../../images/microchip_logo_white_red.png">
    <source media="(prefers-color-scheme: light)" srcset="../../images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="../../images/microchip_logo_black_red.png">
</picture>

# Crypto Accelerator Module ECDSA Sign and Verify Example Application
- Crypto Accelerator Module will be referred to as CAM throughout the remainder of the document. 

## Description
This example application utilizes Crypto Library that interacts with the Pre-compiled CAM Hardware Driver to perform ECDSA Sign and Verify operations.

The ECDSA Sign and Verify operations use NIST defined public and private keys for the following ECC curves:
- P-192
- P-256
- P-384
- P-521

## Software Tool Versions Used
- dsPIC33AK-MP_DFP v1.3.185
- MPLAB® X IDE [v6.30](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.31](https://www.microchip.com/xcdsc)

## Setting Up The Hardware
- dsPIC33AK512MPS512 Curiosity GP DIM ([EV80L65A](https://www.microchip.com/en-us/development-tool/EV80L65A))
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

1. Insert the dsPIC33AK512MPS512 DIM into the DIM J1 slot on the Curiosity Platform Development Board.
2. Connect the the board from the J24 USB-C PKoB4 (PICKit™ On-Board 4) to the computer.

<img src="../../images/hardware/dspic33ak512mps512_dim_board.jpg" alt="Hardware Setup" width="750"/>

## Running The Application
1. Within MPLAB® X IDE, open the `dspic33ak512mps512-ecdsa.X` application project.
2. Build and Program the application using MPLAB® X IDE.
3. The printed information can be found on the serial COM port reading at a speed of 115200.

## Application Results
The following information will be printed on the COM port for all supported ECC Curve types using the blocking then non-blocking API calls:
1. A header Section with the ECC curve type name is printed.
2. The signature is generated. (A status of the Crypto Device will be returned and printed)
3. Generated signature is printed out.
4. This signature is then verified and the status is printed out.

## Crypto Library APIs
crypto_digsign.h defines sign and verify functions

- Inputs to these functions must be **big endian**.

### Blocking API implementations
```c
crypto_DigiSign_Status_E Crypto_DigiSign_Ecdsa_Sign(
    crypto_HandlerType_E ecdsaHandlerType_en,   // The type of crypto implementation to use 
    uint8_t *ptr_inputHash,                     // A pointer to a Hash output generated with SHA
    uint32_t hashLen,                           // Length of the generated Hash
    uint8_t *ptr_outSig,                        // A pointer to store the output Signature
    uint32_t sigLen,                            // Length of the expected Signature
    uint8_t *ptr_privKey,                       // Private Key to sign the message with
    uint32_t privKeyLen,                        // Length of the Private Key
    crypto_EccCurveType_E eccCurveType_En,      // Type of curve being used (described below)
    uint32_t ecdsaSessionId                     // Session ID (This is set to 1)
);
```

```c
crypto_DigiSign_Status_E Crypto_DigiSign_Ecdsa_Verify(
    crypto_HandlerType_E ecdsaHandlerType_en,   // The type of crypto implementation to use 
    uint8_t *ptr_inputHash,                     // A pointer to a Hash output generated with SHA
    uint32_t hashLen,                           // Length of the generated Hash
    uint8_t *ptr_inputSig,                      // A pointer given Signature
    uint32_t sigLen,                            // Length of the Signature
    uint8_t *ptr_pubKey,                        //  Public Key to verify the signature
    uint32_t pubKeyLen,                         // Length of the Public Key
    int8_t *ptr_sigVerifyStat,                  // Status bit to indicate a successful verification
    crypto_EccCurveType_E eccCurveType_En,      // Type of curve being used(described below)
    uint32_t ecdsaSessionId                     // Session ID (This is set to 1)
);
```

### Non-Blocking API implementations

```c
crypto_DigiSign_Status_E Crypto_DigiSign_Ecdsa_Sign_Start(
    crypto_HandlerType_E ecdsaHandlerType_en,   // The type of crypto implementation to use  
    uint8_t *ptr_inputHash,                     // A pointer to a Hash output generated with SHA
    uint32_t hashLen,                           // Length of the generated Hash 
    uint8_t *ptr_privKey,                       // Private Key to sign the message with 
    uint32_t privKeyLen,                        // Length of the Private Key 
    crypto_EccCurveType_E eccCurveType_En,      // Type of curve being used (described below) 
    uint32_t ecdsaSessionId                     // Session ID (This is set to 1)
);
```

```c
crypto_DigiSign_Status_E Crypto_DigiSign_Ecdsa_Verify_Start(
    crypto_HandlerType_E ecdsaHandlerType_en,   // The type of crypto implementation to use
    uint8_t *ptr_inputHash,                     // A pointer to a Hash output generated with SHA 
    uint32_t hashLen,                           // Length of the generated Hash 
    uint8_t *ptr_inputSig,                      // A pointer given Signature 
    uint32_t sigLen,                            // Length of the Signature 
    uint8_t *ptr_pubKey,                        // Public Key to verify the signature 
    uint32_t pubKeyLen,                         // Length of the Public Key 
    crypto_EccCurveType_E eccCurveType_En,      // Type of curve being used(described below) 
    uint32_t ecdsaSessionId                     // Session ID (This is set to 1)
);
```

```c
crypto_DigiSign_Status_E  Crypto_DigiSign_Ecdsa_Sign_GetStatus(void);
```

```c
crypto_DigiSign_Status_E  Crypto_DigiSign_Ecdsa_Verify_GetStatus(void);
```

```c
crypto_DigiSign_Status_E Crypto_DigiSign_Ecdsa_Sign_GetResult(
    uint8_t *ptr_outputSig,                     // A pointer to store the output Signature 
    uint32_t sigLen                             // Length of the expected Signature
);
```

```c
crypto_DigiSign_Status_E Crypto_DigiSign_Ecdsa_Verify_GetResult(void);
```

```c
void Crypto_DigiSign_Ecdsa_SignComplete_CallbackRegister(
    void (*handler)(void)                      // A pointer to a function call 
);
```

```c
void Crypto_DigiSign_Ecdsa_VerifyComplete_CallbackRegister(
    void (*handler)(void)                      // A pointer to a function call 
);
```

### crypto_EccCurveType_E can be any of the following enum values
- CRYPTO_ECC_CURVE_P192
- CRYPTO_ECC_CURVE_P256
- CRYPTO_ECC_CURVE_P384
- CRYPTO_ECC_CURVE_P521

## Non-blocking API usages guide
1. Call the Start function (ex: Crypto_DigiSign_Ecdsa_Sign_Start(CRYPTO_HANDLER_HW_INTERNAL, &inputHash, sizeof(inputHash), &privKey, sizeof(privKeyLen), CRYPTO_ECC_CURVE_P256, 1))
2. Wait for the operation to complete. There are two options to do so:
    - Poll Crypto_DigiSign_Ecdsa_Sign_GetStatus()  
    - Use SignOperationCompleteCallbackRegister(handler) within wrapper file.
3. When the operation is complete call the GetResult function (ex: Crypto_DigiSign_Ecdsa_Sign_GetResult(&outputSig, {expected size of the signature})
  
**Disclaimers**: 
- If result is not fetched after the completion of an operation, Crypto_DigiSign_Ecdsa_Hw_ClearMemory(void) function within the digisign wrapper will need to be called.
    - Crypto_DigiSign_Ecdsa_Hw_ClearMemory_GetStatus(void) can be used to check the status of the clear memory operation. 
- If the start APIs return CRYPTO_DIGISIGN_PKE_BUSY This means there is an operation in progress.

## NIST Vectors
The input test vectors used in this demo are from NIST Publication: [FIPS 186-4 ECDSA Algorithm Test Vectors](https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/digital-signatures)

Hash values used in this demo are from NIST [Cryptographic Standards and Guidelines](https://csrc.nist.gov/projects/cryptographic-standards-and-guidelines/example-values) and can be found under the Secure Hashing section for all Digest Sizes.

## Benchmarking

### Performance Benchmarking
Benchmarking parameters: Device clock speed set to 200 MHz.

### BMX RAM access and performance
The Crypto Accelerator Module has a lower priority on the BMX RAM access. 

If an empty polling loop is used with the Non-blocking API's there will be an increase in time.
- The empty polling loop causes the CPU to over-utilize the BMX thereby blocking the Crypto Accelerator Module from accessing RAM.

A possible workaround if an empty polling loop is required will be to change the Crypto BMX access to have priority over the CPU. 
- **Disclaimer: This is not recommended as the crypto operations may restrict critical system functions and cause unexpected behavior.**

Example for the dsPIC33AK512MPS512: `BMXINITPRbits.CRYPTPR = 1`

For use of the non-blocking ECDSA API's it is recommended to utilize the callback register functions to check if the operation is complete rather than use a polling loop.

Otherwise it may be beneficial to use the blocking API calls if non-blocking is not required. 

#### Blocking API Performance:
|ECC Curve|Crypto_DigiSign_Ecdsa_Sign Performance (ms)|
|----|----|
|P-192|TBD|
|P-256|TBD|
|P-384|TBD|
|P-521|TBD|

|ECC Curve|Crypto_DigiSign_Ecdsa_Verify Performance (ms)|
|----|----|
|P-192|TBD|
|P-256|TBD|
|P-384|TBD|
|P-521|TBD|

#### Non-blocking API Performance
|ECC Curve|Crypto_DigiSign_Ecdsa_Sign Performance (ms)|
|----|----|
|P-192|TBD|
|P-256|TBD|
|P-384|TBD|
|P-521|TBD|

|ECC Curve|Crypto_DigiSign_Ecdsa_Verify Performance (ms)|
|----|----|
|P-192|TBD|
|P-256|TBD|
|P-384|TBD|
|P-521|TBD|

### Memory Size Benchmarking
Flash size will vary based on size of the stored data inputs used with the library. 

#### Blocking Memory Usage
|ECC Curve|FLASH (bytes)|RAM Static (bytes)|RAM Stack (bytes)|
|----|----|----|----|
|P-192|TBD|TBD|TBD|
|P-256|TBD|TBD|TBD|
|P-384|TBD|TBD|TBD|
|P-521|TBD|TBD|TBD|

#### Non-blocking Memory Usage
|ECC Curve|FLASH (bytes)|RAM Static (bytes)|RAM Stack (bytes)|
|----|----|----|----|
|P-192|TBD|TBD|TBD|
|P-256|TBD|TBD|TBD|
|P-384|TBD|TBD|TBD|
|P-521|TBD|TBD|TBD|

## ACVP
The ECDSA Verify API has been self-tested according to the NIST ACVP specification and generated test vectors. More information can be found [here](https://pages.nist.gov/ACVP/).