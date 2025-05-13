<picture>
    <source media="(prefers-color-scheme: dark)" srcset="images/microchip_logo_white_red.png">
	<source media="(prefers-color-scheme: light)" srcset="images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="images/microchip_logo_black_red.png">
</picture>

# dsPIC33A Crypto Accelerator Module Cryptography Demos

Board Image if any.

# Description
A collection of MPLAB® X projects to demonstrate the usage of the Crypto V4 Library and the integrated Cryptographic Accelerator Module hardware:
- ECDSA Sign/Verify
- Hashing
- TRNG
- AES

# Prerequisites

## Software Tools
- dsPIC33AK-MP_DFP v1.0.81 ([ATPACK Installation Instructions](https://microchip.my.site.com/s/article/MPLAB-X---Manual-Install-DFP--Device-Family-Pack))
- MPLAB® X IDE [v6.25 or newer](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.21 or newer](https://www.microchip.com/xcdsc)

## Hardware Tools
- dsPIC33AK512MPS512 Curiosity GP DIM (EV80L65A)
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

# Contents Summary
Each folder contains a README describing the example application in more detail.

| Folder     | Description                                             |
| ---        | ---                                                     |
| dsa        | Example application project for ECDSA with Crypto v4    |
| hash       | Example application project for Hashing with Crypto v4  |
| trng       | Example application project for TRNG with Crypto v4     |
| aes        | Example application project for AES with Crypto v4      |

# Crypto v4 APIs

## AES

### AES ECB, CTR APIs
crypto_sym_cipher.h defines Init, Cipher, EncryptDirect, and DecryptDirect APIs.

Inputs to these functions must be **little endian**.

```c
crypto_Sym_Status_E Crypto_Sym_Aes_Init(
    st_Crypto_Sym_BlockCtx *ptr_aesCtx_st, // configuration context for Symmetric AES support
    crypto_HandlerType_E handlerType_en,   // configure Crypto API to use hardware acceleration
    crypto_CipherOper_E cipherOpType_en,   // configure AES to encrypt or decrypt
    crypto_Sym_OpModes_E opMode_en,        // configure type of AES algorithm
    uint8_t *ptr_key,                      // pointer to the key data array to be used
    uint32_t keyLen,                       // length of the key data array
    uint8_t *ptr_initVect,                 // pointer to the initialization vector data array to be used
    uint32_t sessionID                     // session ID for use by Crypto API (maximum of 1)
);
```

```c
crypto_Sym_Status_E Crypto_Sym_Aes_Cipher(
    st_Crypto_Sym_BlockCtx *ptr_aesCtx_st, // configuration context for AES support
    uint8_t *ptr_inputData,                // pointer to the input data array to be encrypted/decrypted
    uint32_t dataLen,                      // length of the input message array
    uint8_t *ptr_outData                   // pointer to the data array for encrypted/decrypted output to be stored
);
```

```c
crypto_Sym_Status_E Crypto_Sym_Aes_EncryptDirect(
    crypto_HandlerType_E handlerType_en,   // configure Crypto API to use hardware acceleration
    crypto_Sym_OpModes_E opMode_en,        // configure type of AES algorithm
    uint8_t *ptr_inputData,                // pointer to the input data array to be encrypted
    uint32_t dataLen,                      // length of the input data array
    uint8_t *ptr_outData,                  // pointer to the data array for encrypted output to be stored
    uint8_t *ptr_key,                      // pointer to the key data array to be used
    uint32_t keyLen,                       // length of the key data array
    uint8_t *ptr_initVect,                 // pointer to the initialization vector data array to be used
    uint32_t sessionID                     // session ID for use by Crypto API (maximum of 1)
);
```

```c
crypto_Sym_Status_E Crypto_Sym_Aes_DecryptDirect(
    crypto_HandlerType_E handlerType_en,   // configure Crypto API to use hardware acceleration
    crypto_Sym_OpModes_E opMode_en,        // configure type of AES algorithm 
    uint8_t *ptr_inputData,                // pointer to the encrypted input data array to be decrypted
    uint32_t dataLen,                      // length of the encrypted input data array
    uint8_t *ptr_outData,                  // pointer to the data array for decrypted output to be stored
    uint8_t *ptr_key,                      // pointer to the key data array to be used
    uint32_t keyLen,                       // length of the key data array
    uint8_t *ptr_initVect,                 // pointer to the initialization vector data array to be used
    uint32_t sessionID                     // session ID for use by Crypto API (maximum of 1)
);      
```

### AES MAC APIs
crypto_mac_cipher.h defines Init, Cipher, Final, and Direct APIs.

```c
crypto_Mac_Status_E Crypto_Mac_AesCmac_Init(
    st_Crypto_Mac_Aes_ctx *ptr_aesCmacCtx_st,   // CMAC context structure used to store values
    crypto_HandlerType_E handlerType_en,        // configure Crypto API to use hardware acceleration
    uint8_t *ptr_key,                           // pointer to the key data array to be used
    uint32_t keyLen,                            // length of the key data array
    uint32_t sessionID                          // session ID for use by the Crypto API (maximum of 1)
);
```    

```c
crypto_Mac_Status_E Crypto_Mac_AesCmac_Cipher(
    st_Crypto_Mac_Aes_ctx *ptr_aesCmacCtx_st,   // CMAC context structure used to store values
    uint8_t *ptr_inputData,                     // pointer to the input message to generate CMAC with
    uint32_t dataLen                            // length of the input message
);
```

```c
crypto_Mac_Status_E Crypto_Mac_AesCmac_Final(
    st_Crypto_Mac_Aes_ctx *ptr_aesCmacCtx_st,   // CMAC context structure used to store values
    uint8_t *ptr_outMac,                        // output pointer to generate the CMAC to
    uint32_t macLen                             // length of the requested CMAC
);
```

```c
crypto_Mac_Status_E Crypto_Mac_AesCmac_Direct(
    crypto_HandlerType_E macHandlerType_en,     // configure Crypto API to use hardware acceleration
    uint8_t *ptr_inputData,                     // pointer to the input message to generate CMAC with
    uint32_t dataLen,                           // length of the input message
    uint8_t *ptr_outMac,                        // output pointer to generate the CMAC to
    uint32_t macLen,                            // length of the requested CMAC
    uint8_t *ptr_key,                           // pointer to the key data array to be used
    uint32_t keyLen,                            // length of the key data array
    uint32_t sessionID                          // session ID for use by the Crypto API (maximum of 1)
);
```

### AES GCM APIs
```c
crypto_Aead_Status_E Crypto_Aead_AesGcm_Init(
    st_Crypto_Aead_AesGcm_ctx *ptr_aesGcmCtx_st,     // configuration context for AES GCM support
    crypto_HandlerType_E handlerType_en,             // configure Crypto API to use hardware acceleration
    crypto_CipherOper_E cipherOper_en,               // configure AES to encrypt or decrypt
    uint8_t *ptr_key,                                // pointer to the key data array to be used
    uint32_t keyLen,                                 // length of the key data array
    uint8_t *ptr_initVect,                           // pointer to the initialization vector data array to be used
    uint32_t initVectLen,                            // length of the initialization vector data array
    uint32_t sessionID                               // session ID for use by the Crypto API (maximum of 1)
    );
```

```c
crypto_Aead_Status_E Crypto_Aead_AesGcm_AddAadData(
    st_Crypto_Aead_AesGcm_ctx *ptr_aesGcmCtx_st,     // configuration context for AES GCM support
    uint8_t *ptr_aad,                                // pointer to the aad data array to be used
    uint32_t aadLen                                  // length of the aad data array
);
```

```c
crypto_Aead_Status_E Crypto_Aead_AesGcm_Cipher(
    st_Crypto_Aead_AesGcm_ctx *ptr_aesGcmCtx_st,     // configuration context for AES GCM support
    uint8_t *ptr_inputData,                          // pointer to the input data array to be used
    uint32_t dataLen,                                // length of the input data array
    uint8_t *ptr_outData                             // pointer to the data array for encrypted/decrypted output to be stored
);
```

```c
crypto_Aead_Status_E Crypto_Aead_AesGcm_Final(
    st_Crypto_Aead_AesGcm_ctx *ptr_aesGcmCtx_st,    // configuration context for AES GCM support
    uint8_t *ptr_authTag,                           // pointer to the authentication tag data array to be used
    uint8_t authTagLen                              // length of the authentication tag data array
);
```

```c
crypto_Aead_Status_E Crypto_Aead_AesGcm_EncryptAuthDirect(
    crypto_HandlerType_E handlerType_en,           // configure Crypto API to use hardware acceleration
    uint8_t *ptr_inputData,                        // pointer to the input data array to be used
    uint32_t dataLen,                              // length of the input data array
    uint8_t *ptr_outData,                          // pointer to the data array for encrypted output to be stored
    uint8_t *ptr_key,                              // pointer to the key data array to be used
    uint32_t keyLen,                               // length of the key data array
    uint8_t *ptr_initVect,                         // pointer to the initialization vector data array to be used
    uint32_t initVectLen,                          // length of the initialization vector data array
    uint8_t *ptr_aad,                              // pointer to the aad data array to be used
    uint32_t aadLen,                               // length of the aad data array
    uint8_t *ptr_authTag,                          // pointer to the authentication tag data array to be used
    uint8_t authTagLen,                            // length of the authentication tag data array
    uint32_t sessionID                             // session ID for use by the Crypto API (maximum of 1)
);
```

```c
crypto_Aead_Status_E Crypto_Aead_AesGcm_DecryptAuthDirect(
    crypto_HandlerType_E handlerType_en,           // configure Crypto API to use hardware acceleration
    uint8_t *ptr_inputData,                        // pointer to the input data array to be used
    uint32_t dataLen,                              // length of the input data array
    uint8_t *ptr_outData,                          // pointer to the data array for decrypted output to be stored
    uint8_t *ptr_key,                              // pointer to the key data array to be used
    uint32_t keyLen,                               // length of the key data array
    uint8_t *ptr_initVect,                         // pointer to the initialization vector data array to be used
    uint32_t initVectLen,                          // length of the initialization vector data array
    uint8_t *ptr_aad,                              // pointer to the aad data array to be used
    uint32_t aadLen,                               // length of the aad data array
    uint8_t *ptr_authTag,                          // pointer to the authentication tag data array to be used
    uint8_t authTagLen,                            // length of the authentication tag data array
    uint32_t sessionID                             // session ID for use by the Crypto API (maximum of 1)
);
```

## DSA

### ECDSA Sign and Verify API Documentation
crypto_digisign.h defines sign and verify functions

- Inputs to these functions must be **big endian**.

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

#### crypto_EccCurveType_E can be any of the following enum values:
- CRYPTO_ECC_CURVE_P192
- CRYPTO_ECC_CURVE_P256
- CRYPTO_ECC_CURVE_P384
- CRYPTO_ECC_CURVE_P521

## Hashing

### Hash API Documentation
crypto_hash.h defines Digest, Init, Update, Final, and GetHashAndHashSize APIs.

```c
crypto_Hash_Status_E Crypto_Hash_Sha_Digest(
    crypto_HandlerType_E shaHandler_en, // configure Crypto API to use hardware acceleration
    uint8_t *ptr_data,                  // pointer to the input data array to be hashed
    uint32_t dataLen,                   // length of the input data array
    uint8_t *ptr_digest,                // pointer to the data array for result hash to be stored
    crypto_Hash_Algo_E shaAlgorithm_en, // configure Crypto API for desired SHA algorithm
    uint32_t shaSessionId               // session ID for use by Crypto API (maximum of 1)
);
```

```c
crypto_Hash_Status_E Crypto_Hash_Sha_Init(
    st_Crypto_Hash_Sha_Ctx *ptr_shaCtx_st, // SHA configuration context
    crypto_Hash_Algo_E shaAlgorithm_en,    // configure Crypto API for desired SHA algorithm
    crypto_HandlerType_E shaHandler_en,    // configure Crypto API to use hardware acceleration
    uint32_t shaSessionId                  // session ID for use by Crypto API (maximum of 1)
);
```

```c
crypto_Hash_Status_E Crypto_Hash_Sha_Update(
    st_Crypto_Hash_Sha_Ctx *ptr_shaCtx_st, // SHA configuration context
    uint8_t *ptr_data,                     // pointer to the input data array to be hashed
    uint32_t dataLen                       // length of the input data array
);
```

```c
crypto_Hash_Status_E Crypto_Hash_Sha_Final(
    st_Crypto_Hash_Sha_Ctx *ptr_shaCtx_st, // SHA configuration context
    uint8_t *ptr_digest                    // pointer to the data array for result digest to be stored
);
```

## TRNG

### TRNG API Documentation
crypto_rng.h defines the Generate API.

```c
crypto_Rng_Status_E Crypto_Rng_Prng_Generate(
    crypto_HandlerType_E rngHandlerType_en, // Determines if hardware or software is used for generation
    uint8_t* ptr_rngData,                   // The location that the TRNG generated data will be stored
    uint32_t rngLen,                        // The length of the generated data
    uint8_t* ptr_nonce,                     // Unused parameter for the CAM implementation, should always be set to NULL
    uint32_t nonceLen,                      // Unused parameter for the CAM implementation, should always be set to 0
    uint32_t sessionID                      // Unused parameter for the CAM implementation, should always be set to 1
);
```

# MISRA

A MISRA scan has been conducted on the source library file. Results can be found [here](./misra_report/).

# Benchmarking

## AES

### Speed Benchmarking
The following benchmarking results were obtained while testing the AES driver with NIST provided test vectors.

Benchmarking parameters: Device clock speed set to 200 MHz

#### AES-ECB
|Key Size (bytes)|Plaintext Size (bytes)|AES-ECB EncryptDirect API (µs)|
|----|----|----|
|8|128|13.36|
|24|128|13.76|
|32|128|14.16|

|Key Size (bytes)|Ciphertext Size (bytes)|AES-ECB DecryptDirect API (µs)|
|----|----|----|
|8|128|13.58|
|24|128|14.06|
|32|128|14.54|

#### AES-CTR
|Key Size (bytes)|Plaintext Size (bytes)|AES-CTR EncryptDirect API (µs)|
|----|----|----|
|8|128|13.76|
|24|128|14.16|
|32|128|14.56|

|Key Size (bytes)|Ciphertext Size (bytes)|AES-CTR DecryptDirect API (µs)|
|----|----|----|
|8|128|13.75|
|24|128|14.15|
|32|128|14.55|

#### AES-CMAC
|Key Size (bytes)|Plaintext Size (bytes)|MAC Size (bytes)|AES-CMAC Direct API (µs)|
|----|----|----|----|
|8|532|16|235.15|
|24|532|16|237.87|
|32|532|16|240.67|

#### AES-GCM
|Key Size (bytes)|Initialization Vector Size (bytes)|Authentication Data Size (bytes)|Tag Size (bytes)|Plaintext Size (bytes)|AES-GCM EncryptAuthDirect API (µs)|
|----|----|----|----|----|----|
|8|12|4|16|16|21.18|
|24|12|4|16|14|21.88|
|32|12|4|16|13|21.13|

|Key Size (bytes)|Initialization vector Size (bytes)|Authentication data Size (bytes)|Tag Size (bytes)|Ciphertext Size (bytes)|AES-GCM EncryptAuthDirect API (µs)|
|----|----|----|----|----|----|
|8|12|4|16|14|23.89|
|24|12|2|16|14|24.07|
|32|12|2|16|13|24.30|

### Size Benchmarking
The following results include usage of ECB, CTR, CMAC, and GCM APIs. Flash size will vary based on size of the stored data inputs used with the library. 

#### Single Step

|AES Mode|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|AES-ECB|240|9,044|
|AES-CTR|240|9,076|
|AES-CMAC|240|8,680|
|AES-GCM|240|10,116|

#### Multi Step

|AES Mode|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|AES-ECB|2,188|9,108|
|AES-CTR|2,188|9,152|
|AES-CMAC|768|8,736|
|AES-GCM|780|10,240|

## DSA

### Speed Benchmarking Results
Benchmarking parameters: Device clock speed set to 200 MHz.

|ECC Curve|Crypto_DigiSign_Ecdsa_Verify (ms)|
|----|----|
|P-192|5.14|
|P-256|9.90|
|P-384|27.07|
|P-521|58.35|

### Size Benchmarking Results:
The following results are using P-521. Flash size will vary based on size of the stored data inputs used with the library. 

|RAM (bytes)|FLASH (bytes)|
|----|----|
|4,108|10,536|

## Hashing

### Speed Benchmarking
The following benchmarking results were obtained while testing the hash driver with NIST provided test vectors.

Benchmarking parameters: Device clock speed set to 200 MHz

|Hash Algorithm|Input Size (bytes)|Crypto_Hash_Sha_Digest (µs)|
|----|----|----|
|SHA-1|4,096|37.26|
|SHA-224|4,096|32.07|
|SHA-256|4,096|31.91|
|SHA-384|4,096|27.39|
|SHA-512|4,096|27.38|

### Size Benchmarking

#### Single Step

|Hash Algorithm|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|SHA-1|240|8,928|
|SHA-224|240|8,928|
|SHA-256|240|8,928|
|SHA-384|240|8,928|
|SHA-512|240|8,928|

#### Multi Step

|Hash Algorithm|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|SHA-1|240|8,976|
|SHA-224|240|8,976|
|SHA-256|240|8,976|
|SHA-384|240|8,976|
|SHA-512|240|8,976|

## TRNG

### Speed Benchmarking

Benchmarking parameters: Device clock speed set to 200 MHz.

|TRNG Size (bytes)|Crypto_Rng_Generate (ms)|
|---|---|
|192|66.334|
|256|77.167|
|384|98.917|
|521|123.084|

### Size Benchmarking

The below sizes are for the process of getting 521 bytes (8-bit words).

|RAM (bytes)|FLASH (bytes)|
|---|---|
|12|3,628|