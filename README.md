<picture>
    <source media="(prefers-color-scheme: dark)" srcset="images/microchip_logo_white_red.png">
	<source media="(prefers-color-scheme: light)" srcset="images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="images/microchip_logo_black_red.png">
</picture>

# dsPIC33A Crypto Accelerator Module Library and Demos
- Crypto Accelerator Module will be refered to as CAM throughout the remainder of the document. 
    
# Description
A collection of MPLAB® X IDE projects to demonstrate the CAM operations:
- ECDSA Sign/Verify
- Hashing
- TRNG
- AES

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

# CAM APIs

The included demonstration projects provide examples of the various CAM features. The application project needs to include the Crypto Library in the crypto/common_crypto folder. The Crypto Library API and associated hardware wrappers invoke the pre-compiled CAM driver files. 

The pre-compiled CAM driver files can function independently by utilizing the following [APIs](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=CAM&redirect=true).

# MISRA

A MISRA scan has been conducted on the pre-compiled CAM driver source files. The source files are Misra compliant for Required and Mandatory rules.

# Benchmarking

### Performance Benchmarking
The following benchmarking results were obtained while testing the Crypto Library APIs that utilize the pre-compiled CAM driver files.

Benchmarking parameters: Device clock speed set to 200 MHz

|Algorithm|Performance|Parameters (sizes are in bytes)|
|----|----|----|
|SHA-1 (Digest)|928.80 Mbps|Message Size: 4,096|
|SHA-224 (Digest)|1084.67 Mbps|Message Size: 4,096|
|SHA-256 (Digest)|1091.54 Mbps|Message Size: 4,096|
|SHA-384 (Digest)|1294.15 Mbps|Message Size: 4,096|
|SHA-512 (Digest)|1293.64 Mbps|Message Size: 4,096|
|AES-ECB (Encrypt)|113.17 Mbps|Key Size: 32, Plaintext Size: 128|
|AES-ECB (Decrypt)|109.12 Mbps|Key Size: 32, Ciphertext Size: 128|
|AES-CTR (Encrypt)|115.07 Mbps|Key Size: 32, Initialization Vector Size: 16, Plaintext Size: 128|
|AES-CTR (Decrypt)|115.07 Mbps|Key Size: 32, Initialization Vector Size: 16, Ciphertext Size: 128|
|AES-GCM (Encrypt)|39.82 Mbps|Key Size: 32, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 13|
|AES-GCM (Decrypt)|35.13 Mbps|Key Size: 32, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 13|
|AES-CMAC (Direct)|30.75 Mbps|Key Size: 32, Plaintext Size: 532, MAC Size: 16|
|ECDSA (Sign)|3.92 ms|Curve: P-192|
|ECDSA (Verify)|5.15 ms|Curve: P-192|
|ECDSA (Sign)|7.42 ms|Curve: P-256|
|ECDSA (Verify)|9.92 ms|Curve: P-256|
|ECDSA (Sign)|19.79 ms|Curve: P-384|
|ECDSA (Verify)|27.07 ms|Curve: P-384|
|ECDSA (Sign)|42.16 ms|Curve: P-521|
|ECDSA (Verify)|58.10 ms|Curve: P-521|
|TRNG (Generate)|123.13 ms|Output Size: 521|

### Memory Size Benchmarking
The following results include usage of single step and multi step APIs. Flash size will vary based on size of the stored data inputs used with the library. 

All projects have the following compiler options:
- Optimization for size (-Os)
- Isolate each function in a section - unchecked
- Remove unused sections - unchecked

|Algorithm|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|SHA-1 (Digest)|284|9,892|
|SHA-1 (Init, Update, and Final)|284|9,936|
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
|ECDSA (Sign and Verify (P-192))|4,152|11,328|
|ECDSA (Sign and Verify (P-256))|4,152|11,372|
|ECDSA (Sign and Verify (P-384))|4,152|11,440|
|ECDSA (Sign and Verify (P-521))|4,152|11,520|
|TRNG (Generate)|56|4,612|