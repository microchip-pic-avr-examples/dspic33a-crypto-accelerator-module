<picture>
    <source media="(prefers-color-scheme: dark)" srcset="../../images/microchip_logo_white_red.png">
    <source media="(prefers-color-scheme: light)" srcset="../../images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="../../images/microchip_logo_black_red.png">
</picture>

# Crypto Accelerator Module AES Example Application
- Crypto Accelerator Module will be referred to as CAM throughout the remainder of the document.

## Description
This example application utilizes the Crypto Library that interacts with the Pre-compiled CAM Hardware Driver to perform AES encryption operations using different sized input vectors. The following AES algorithms are supported:
- ECB
- CTR
- CBC
- CFB128
- OFB
- CMAC
- GCM
- CCM
- XTS

## Software Tool Versions Used
- dsPIC33AK-MP_DFP v1.3.185
- MPLAB® X IDE [v6.30](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.31](https://www.microchip.com/xcdsc)

## Setting Up The Hardware
- dsPIC33AK256MPS306 Curiosity GP DIM ([EV02E57A](https://www.microchip.com/en-us/development-tool/EV02E57A))
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

1. Insert the dsPIC33AK256MPS306 DIM into the DIM J1 slot on the Curiosity Platform Development Board.
2. Connect the board from the J24 USB-C PKoB4 (PICKit™ On-Board 4) to the computer.

<img src="../../images/hardware/dspic33ak256mps306_dim_board.jpg" alt="Hardware Setup" width="750"/>

## Running The Application
1. Within MPLAB® X IDE, open the `dspic33ak256mps306-aes.X` application project.
    - (Optional) To run all added test vectors uncomment the RUN_ALL_VECTORS definition in dspic33ak256mps306-aes.X/app/app_aes.h
2. Build and Program the application using MPLAB® X IDE.
3. The printed information can be found on the serial COM port reading at a speed of 115200.

## Application Results
### AES ECB, CTR, CBC, CFB128, OFB Results
The following information will be printed on the COM port for symmetric AES algorithms:
1. A header is displayed showing the used AES algorithm and input data length.
2. The key data is printed out.
3. The initialization vector data is printed out.
4. The data is encrypted using the single-step API.
    1. The input data is printed out
    2. The encrypted data is printed out.
5. The data is decrypted using the single-step API.
    1. The encrypted input data is printed out.
    2. The decrypted data is printed out.
    3. The decrypted data and original input data are compared.
    4. Pass or Fail is printed out depending on the comparison result.
6. The data is encrypted using the multi-step API.
    1. The input data is printed out.
    2. The encrypted data is printed out.
7. The data is decrypted using the multi-step API.
    1. The encrypted input data is printed out.
    2. The decrypted data is printed out.
    3. The decrypted data and original input data are compared.
    4. Pass or Fail is printed out depending on the comparison result.
8. The single-step and multi-step encrypted data results are compared.
    1. Pass or Fail is printed out depending on the comparison result.

### AES GCM, CCM Results
The following information will be printed on the COM port for the AES AEAD algorithms:
1. A header is displayed showing the used AES algorithm and input data length.
2. The key data is printed out.
3. The initialization vector data is printed out.
4. The data is encrypted using the single-step API.
    1. The input data is printed out
    2. The encrypted data is printed out.
    3. The authentication tag is printed out.
    4. The encrypted data is compared to an expected value.
    5. The authentication tag is compared to an expected value.
5. The data is decrypted using the single-step API.
    1. The encrypted input data is printed out.
    2. The decrypted data is printed out.
    3. The decrypted data and original input data are compared.
    4. Pass or Fail is printed out depending on the comparison result.

**FOR GCM ONLY**

6. The data is encrypted using the multi-step API.
    1. The input data is printed out.
    2. The encrypted data is printed out.
    3. The authentication tag is printed out.
    4. The encrypted data is compared to an expected value.
    5. The authentication tag is compared to an expected value.
7. The data is decrypted using the multi-step API.
    1. The encrypted input data is printed out.
    2. The authentication tag is printed out.
    3. The decrypted data is printed out.
    4. The authentication tag is printed out.
    5. The decrypted data and original input data are compared.
    6. Pass or Fail is printed out depending on the comparison result.
8. The single-step and multi-step encrypted data results are compared.
    1. Pass or Fail is printed out depending on the comparison result.

### AES XTS Results
The following information will be printed on the COM port for symmetric AES XTS algorithm:
1. A header is displayed showing the used AES algorithm and input data length.
2. The key data is printed out.
3. The key 2 data is printed out.
4. The tweak data is printed out.
4. The data is encrypted using the single-step API.
    1. The input data is printed out
    2. The encrypted data is printed out.
    3. The encrypted data is compared to an expected value.
5. The data is decrypted using the single-step API.
    1. The encrypted input data is printed out.
    2. The decrypted data is printed out.
    3. The decrypted data and original input data are compared.
    4. Pass or Fail is printed out depending on the comparison result.
6. The data is encrypted using the multi-step API.
    1. The input data is printed out.
    2. The encrypted data is printed out.
    4. The encrypted data is compared to an expected value.
7. The data is decrypted using the multi-step API.
    1. The encrypted input data is printed out.
    3. The decrypted data is printed out.
    5. The decrypted data and original input data are compared.
    6. Pass or Fail is printed out depending on the comparison result.
8. The single-step and multi-step encrypted data results are compared.
    1. Pass or Fail is printed out depending on the comparison result.

## Crypto Library APIs

### AES ECB, CTR, XTS APIs
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
crypto_Sym_Status_E Crypto_Sym_AesXts_Cipher(
    st_Crypto_Sym_BlockCtx *ptr_aesCtx_st, // configuration context for AES support
    uint8_t *ptr_inputData,                // pointer to the input data array to be encrypted/decrypted
    uint32_t dataLen,                      // length of the input message array
    uint8_t *ptr_outData,                  // pointer to the data array for encrypted/decrypted output to be stored
    uint8_t *ptr_tweak                     // pointer to the tweak array used to adjust the result ciphertext
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

### AES AEAD APIs

```c
crypto_Aead_Status_E Crypto_Aead_AesCcm_Init(
    st_Crypto_Aead_AesCcm_ctx *ptr_aesCcmCtx_st,     // configuration context for AES CCM support
    crypto_HandlerType_E handlerType_en,             // configure Crypto API to use hardware acceleration
    uint8_t *ptr_key,                                // pointer to the key data array to be used
    uint32_t keyLen,                                 // length of the key data array
    uint32_t sessionID                               // session ID for use by the Crypto API (maximum of 1)
);
```

```c
crypto_Aead_Status_E Crypto_Aead_AesCcm_Cipher(
    st_Crypto_Aead_AesCcm_ctx *ptr_aesCcmCtx_st,     // configuration context for AES CCM support 
    crypto_CipherOper_E cipherOper_en,               // configure AES to encrypt or decrypt 
    uint8_t *ptr_inputData,                          // pointer to the input data array to be used
    uint32_t dataLen,                                // length of the input data array
    uint8_t *ptr_outData,                            // pointer to the data array for encrypted output to be stored
    uint8_t *ptr_nonce,                              // pointer to the nonce array to be used
    uint32_t nonceLen,                               // length of the nonce array 
    uint8_t *ptr_authTag,                            // pointer to the authentication tag data array to be used
    uint32_t authTagLen,                             // length of the authentication tag data array 
    uint8_t *ptr_aad,                                // pointer to the aad data array to be used 
    uint32_t aadLen                                  // length of the aad data array
);
```

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
    st_Crypto_Aead_AesGcm_ctx *ptr_aesGcmCtx_st,     // configuration context for AES GCM support
    uint8_t *ptr_authTag,                            // pointer to the authentication tag data array to be used
    uint8_t authTagLen                               // length of the authentication tag data array
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

## Benchmarking

### Performance Benchmarking
The following benchmarking results were obtained with the device clock speed set to 200MHz.

#### AES-ECB
|Key Size (bytes)|Plaintext Size (bytes)|AES-ECB Crypto_Sym_Aes_EncryptDirect Performance (Mbps)|
|----|----|----|
|16|8,192|TBD|
|24|8,192|TBD|
|32|8,192|TBD|

|Key Size (bytes)|Ciphertext Size (bytes)|AES-ECB Crypto_Sym_Aes_DecryptDirect Performance (Mbps)|
|----|----|----|
|16|8,192|TBD|
|24|8,192|TBD|
|32|8,192|TBD|

#### AES-CTR
|Key Size (bytes)|Initialization Vector Size (bytes)|Plaintext Size (bytes)|AES-CTR Crypto_Sym_Aes_EncryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

|Key Size (bytes)|Initialization Vector Size (bytes)|Ciphertext Size (bytes)|AES-CTR Crypto_Sym_Aes_DecryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

#### AES-CBC
|Key Size (bytes)|Initialization Vector Size (bytes)|Plaintext Size (bytes)|AES-CBC Crypto_Sym_Aes_EncryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

|Key Size (bytes)|Initialization Vector Size (bytes)|Ciphertext Size (bytes)|AES-CBC Crypto_Sym_Aes_DecryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

#### AES-CFB128
|Key Size (bytes)|Initialization Vector Size (bytes)|Plaintext Size (bytes)|AES-CFB128 Crypto_Sym_Aes_EncryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

|Key Size (bytes)|Initialization Vector Size (bytes)|Ciphertext Size (bytes)|AES-CFB128 Crypto_Sym_Aes_DecryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

#### AES-OFB
|Key Size (bytes)|Initialization Vector Size (bytes)|Plaintext Size (bytes)|AES-OFB Crypto_Sym_Aes_EncryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

|Key Size (bytes)|Initialization Vector Size (bytes)|Ciphertext Size (bytes)|AES-OFB Crypto_Sym_Aes_DecryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|24|16|8,192|TBD|
|32|16|8,192|TBD|

#### AES-CCM
|Key Size (bytes)|Initialization Vector Size (bytes)|Authentication Data Size (bytes)|Tag Size (bytes)|Plaintext Size (bytes)|AES-CCM Crypto_Aead_AesCcm_EncryptAuthDirect Performance (Mbps)|
|----|----|----|----|----|----|
|16|12|4|16|8,192|TBD|
|24|12|4|16|8,192|TBD|
|32|12|4|16|8,192|TBD|

|Key Size (bytes)|Initialization Vector Size (bytes)|Authentication Data Size (bytes)|Tag Size (bytes)|Ciphertext Size (bytes)|AES-CCM Crypto_Aead_AesCcm_DecryptAuthDirect Performance (Mbps)|
|----|----|----|----|----|----|
|16|12|4|16|8,192|TBD|
|24|12|2|16|8,192|TBD|
|32|12|2|16|8,192|TBD|

#### AES-GCM
|Key Size (bytes)|Initialization Vector Size (bytes)|Authentication Data Size (bytes)|Tag Size (bytes)|Plaintext Size (bytes)|AES-GCM Crypto_Aead_AesGcm_EncryptAuthDirect Performance (Mbps)|
|----|----|----|----|----|----|
|16|12|4|16|8,192|TBD|
|24|12|4|16|8,192|TBD|
|32|12|4|16|8,192|TBD|

|Key Size (bytes)|Initialization Vector Size (bytes)|Authentication Data Size (bytes)|Tag Size (bytes)|Ciphertext Size (bytes)|AES-GCM Crypto_Aead_AesGcm_DecryptAuthDirect Performance (Mbps)|
|----|----|----|----|----|----|
|16|12|4|16|8,192|TBD|
|24|12|2|16|8,192|TBD|
|32|12|2|16|8,192|TBD|

#### AES-XTS
|Key Size (bytes)|Tweak Size (bytes)|Plaintext Size (bytes)|AES-XTS Crypto_Sym_Aes_EncryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|32|16|8,192|TBD|

|Key Size (bytes)|Tweak Size (bytes)|Ciphertext Size (bytes)|AES-XTS Crypto_Sym_Aes_DecryptDirect Performance (Mbps)|
|----|----|----|----|
|16|16|8,192|TBD|
|32|16|8,192|TBD|

### Memory Size Benchmarking
The following results include usage of ECB, CTR, GCM, XTS, and CMAC APIs. Flash size will vary based on size of the stored data inputs used with the library.

#### Single Step

|AES Mode|FLASH (bytes)|RAM Static (bytes)|RAM Stack (bytes)|
|----|----|----|----|
|AES-ECB|TBD|TBD|TBD|
|AES-CTR|TBD|TBD|TBD|
|AES-CBC|TBD|TBD|TBD|
|AES-CFB128|TBD|TBD|TBD|
|AES-OFB|TBD|TBD|TBD|
|AES-GCM|TBD|TBD|TBD|
|AES-CCM|TBD|TBD|TBD|
|AES-XTS|TBD|TBD|TBD|

#### Multi Step

|AES Mode|FLASH (bytes)|RAM Static (bytes)|RAM Stack (bytes)|
|----|----|----|----|
|AES-ECB|TBD|TBD|TBD|
|AES-CTR|TBD|TBD|TBD|
|AES-CBC|TBD|TBD|TBD|
|AES-CFB128|TBD|TBD|TBD|
|AES-OFB|TBD|TBD|TBD|
|AES-GCM|TBD|TBD|TBD|
|AES-XTS|TBD|TBD|TBD|

## ACVP
The APIs have been self-tested according to the NIST ACVP specification and generated test vectors. More information can be found [here](https://pages.nist.gov/ACVP/).
