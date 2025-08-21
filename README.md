<picture>
    <source media="(prefers-color-scheme: dark)" srcset="images/microchip_logo_white_red.png">
	<source media="(prefers-color-scheme: light)" srcset="images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="images/microchip_logo_black_red.png">
</picture>

# dsPIC33A Crypto Accelerator Module Hardware Driver and Demos
- Crypto Accelerator Module will be referred to as CAM throughout the remainder of the document.

# Description
dsPIC33A CAM Hardware Driver and Demos are a collection of MPLAB® X IDE projects to demonstrate the following CAM operations:
- ECDSA Sign/Verify
- Hashing
- TRNG
- AES
- ECDH Shared Secret

The included demonstration projects provide examples of the various CAM features. The application project needs to include the Crypto Library in the crypto/common_crypto folder. The Crypto Library API and associated hardware wrappers invoke the Pre-compiled CAM Hardware Driver.

# Prerequisites

## Software Tools
- dsPIC33AK-MP_DFP v1.0.81
- MPLAB® X IDE [v6.25 or newer](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.21 or newer](https://www.microchip.com/xcdsc)

## Hardware Tools
- dsPIC33AK512MPS512 Curiosity GP DIM ([EV80L65A](https://www.microchip.com/en-us/development-tool/EV80L65A))
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

# Contents Summary
Each folder contains a README describing the example application in more detail.

| Folder     | Description                              |
| ---        | ---                                      |
| dsa        | Example application project for ECDSA    |
| hash       | Example application project for Hashing  |
| trng       | Example application project for TRNG     |
| aes        | Example application project for AES      |
| ecdh       | Example application project for ECDH     |
| aes_hash   | Example concurrent AES+HASH project      |

# Crypto Accelerator Module APIs

The Pre-compiled CAM Hardware Driver can function independently by utilizing the following [APIs](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=CAM&redirect=true).

# MISRA

A MISRA scan has been conducted on the Pre-compiled CAM Hardware Driver source files. The source files are Misra compliant for Required and Mandatory rules.

# Benchmarking

### Performance Benchmarking
The following benchmarking results were obtained while testing the Crypto Library APIs that utilize the Pre-compiled Hardware Driver.

Benchmarking parameters: Device clock speed set to 200 MHz

|Algorithm|Performance|Parameters (sizes are in bytes)|
|----|----|----|
|SHA-1 (Digest)|934.09 Mbps|Message Size: 4,096|
|SHA-224 (Digest)|1090.45 Mbps|Message Size: 4,096|
|SHA-256 (Digest)|1094.46 Mbps|Message Size: 4,096|
|SHA-384 (Digest)|1302.90 Mbps|Message Size: 4,096|
|SHA-512 (Digest)|1299.29 Mbps|Message Size: 4,096|
|AES-ECB (Encrypt)|104.25 Mbps|Key Size: 128, Plaintext Size: 128|
|AES-ECB (Decrypt)|101.59 Mbps|Key Size: 128, Ciphertext Size: 128|
|AES-CTR (Encrypt)|112.78 Mbps|Key Size: 128, Initialization Vector Size: 16, Plaintext Size: 128|
|AES-CTR (Decrypt)|112.48 Mbps|Key Size: 128, Initialization Vector Size: 16, Ciphertext Size: 128|
|AES-GCM (Encrypt)|33.71 Mbps|Key Size: 128, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 16|
|AES-GCM (Decrypt)|28.87 Mbps|Key Size: 128, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 16|
|AES-CMAC (Direct)|28.07 Mbps|Key Size: 128, Plaintext Size: 532, MAC Size: 16|
|ECDSA (Sign)|3.92 ms|Curve: P-192|
|ECDSA (Verify)|5.14 ms|Curve: P-192|
|ECDSA (Sign)|7.42 ms|Curve: P-256|
|ECDSA (Verify)|9.91 ms|Curve: P-256|
|ECDSA (Sign)|19.80 ms|Curve: P-384|
|ECDSA (Verify)|27.06 ms|Curve: P-384|
|ECDSA (Sign)|42.15 ms|Curve: P-521|
|ECDSA (Verify)|58.41 ms|Curve: P-521|
|TRNG (Generate)|122.75 ms|Output Size: 521|
|ECDH (Shared Secret Generate)|3.78 ms|Curve: P-192|
|ECDH (Shared Secret Generate)|7.22 ms|Curve: P-256|
|ECDH (Shared Secret Generate)|19.44 ms|Curve: P-384|
|ECDH (Shared Secret Generate)|41.55 ms|Curve: P-521|

### Memory Size Benchmarking
The following results include usage of single step and multi step APIs. Flash size will vary based on size of the stored data inputs used with the library.

All projects have the following compiler options:
- Optimization for size (-Os)
- Isolate each function in a section - unchecked
- Remove unused sections - unchecked

|Algorithm|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|SHA-1 (Digest)|284|9,892|
|SHA-1 (Init, Update, and Final)|284|9,940|
|SHA-224 (Digest)|284|9,892|
|SHA-224 (Init, Update, and Final)|284|9,940|
|SHA-256 (Digest)|284|9,892|
|SHA-256 (Init, Update, and Final)|284|9,940|
|SHA-384 (Digest)|284|9,892|
|SHA-384 (Init, Update, and Final)|284|9,940|
|SHA-512 (Digest)|284|9,892|
|SHA-512 (Init, Update, and Final)|284|9,940|
|AES-ECB (Encrypt and Decrypt)|284|10,008|
|AES-ECB (Init and Cipher)|2,232|10,072|
|AES-CTR (Encrypt and Decrypt)|284|10,040|
|AES-CTR (Init and Cipher)|2,232|10,116|
|AES-GCM (Encrypt and Decrypt)|284|11,032|
|AES-GCM (Init, AddAadData, Cipher, and Final)|824|11,156|
|AES-CMAC (Direct)|284|9,644|
|AES-CMAC (Init, Cipher, and Final)|812|9,700|
|ECDSA (Sign and Verify (P-192))|4,152|11,360|
|ECDSA (Sign and Verify (P-256))|4,152|11,404|
|ECDSA (Sign and Verify (P-384))|4,152|11,472|
|ECDSA (Sign and Verify (P-521))|4,152|11,552|
|TRNG (Generate)|56|4,612|
|ECDH (Shared Secret (P-192))|4,152|10,744|
|ECDH (Shared Secret (P-256))|4,152|10,788|
|ECDH (Shared Secret (P-384))|4,152|10,856|
|ECDH (Shared Secret (P-521))|4,152|10,928|