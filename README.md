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
- Concurrent AES+HASH (to demonstrate context safety)
- MAC generation 

The included demonstration projects provide examples of the various CAM features. The application project needs to include the Crypto Library in the crypto/common_crypto folder. The Crypto Library API and associated hardware wrappers invoke the pre-compiled CAM Hardware Driver.

# Prerequisites

## Software Tool Versions Used
- dsPIC33AK-MP_DFP v1.3.185
- MPLAB® X IDE [v6.30](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.31](https://www.microchip.com/xcdsc)

## Hardware Tools
- dsPIC33AK512MPS512 Curiosity GP DIM ([EV80L65A](https://www.microchip.com/en-us/development-tool/EV80L65A))
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

# Contents Summary
Each folder contains a README describing the example application in more detail.

| Folder     | Description                                         |
| ---        | ---                                                 |
| dsa        | Example application project for ECDSA               |
| hash       | Example application project for Hashing             |
| trng       | Example application project for TRNG                |
| aes        | Example application project for AES                 |
| ecdh       | Example application project for ECDH                |
| aes_hash   | Example application project for concurrent AES+HASH |
| mac        | Example application project for CMAC, HMAC and GMAC |

# Crypto Accelerator Module APIs

The pre-compiled CAM Hardware Driver can function independently by utilizing the following [APIs](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=CAM&redirect=true).

# MISRA

A MISRA scan has been conducted on the pre-compiled CAM Hardware Driver source files. The source files are MISRA compliant for Required and Mandatory rules.

# Benchmarking
## Benchmarking Setup
The benchmarking results are obtained by modifying the demo projects to remove overheads such as:
- UART peripheral and print out statements.
- Isolation to use one algorithm at a time.
- Store input data in flash memory to benchmark RAM usage for CAM Hardware Driver

The benchmarking projects are configured to use the following:
- Optimization level -Os
- Tools and versions as listed in the Software Tools section above


### Performance Benchmarking
The following benchmarking results were obtained while testing the Crypto Library APIs that utilize the pre-compiled Hardware Driver.

Benchmarking parameters: Device clock speed set to 200 MHz

|Algorithm|Performance|Parameters (sizes are in bytes)|
|----|----|----|
|SHA-1 (Digest)|TBD|Message Size: 8,192|
|SHA-224 (Digest)|TBD|Message Size: 8,192|
|SHA-256 (Digest)|TBD|Message Size: 8,192|
|SHA-384 (Digest)|TBD|Message Size: 8,192|
|SHA-512 (Digest)|TBD|Message Size: 8,192|
|HMAC SHA-1 (Direct)|TBD|Key Size: 16, Message Size: 8,192|
|HMAC SHA-224 (Direct)|TBD|Key Size: 16, Message Size: 8,192|
|HMAC SHA-256 (Direct)|TBD|Key Size: 16, Message Size: 8,192|
|HMAC SHA-384 (Direct)|TBD|Key Size: 16, Message Size: 8,192|
|HMAC SHA-512 (Direct)|TBD|Key Size: 16, Message Size: 8,192|
|AES-ECB (Encrypt)|TBD|Key Size: 16, Plaintext Size: 8,192|
|AES-ECB (Decrypt)|TBD|Key Size: 16, Ciphertext Size: 8,192|
|AES-CTR (Encrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CTR (Decrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CBC (Encrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CBC (Decrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CFB128 (Encrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CFB128 (Decrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-OFB (Encrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-OFB (Decrypt)|TBD|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-GCM (Encrypt)|TBD|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|
|AES-GCM (Decrypt)|TBD|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|
|AES-CCM (Encrypt)|TBD|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|
|AES-CCM (Decrypt)|TBD|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|
|AES-XTS (Encrypt)|TBD|Key Size: 16, Tweak Size: 16, Plaintext Size: 8,192|
|AES-XTS (Decrypt)|TBD|Key Size: 16, Tweak Size: 16, Ciphertext Size: 8,192|
|AES-CMAC (Direct)|TBD|Key Size: 16, Plaintext Size: 8,192, MAC Size: 16|
|AES-GMAC (Direct)|TBD|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 8,192, Tag Size: 16|
|ECDSA Sign   (Blocking)|TBD|Curve: P-192|
|ECDSA Verify (Blocking)|TBD|Curve: P-192|
|ECDSA Sign   (Blocking)|TBD|Curve: P-256|
|ECDSA Verify (Blocking)|TBD|Curve: P-256|
|ECDSA Sign   (Blocking)|TBD|Curve: P-384|
|ECDSA Verify (Blocking)|TBD|Curve: P-384|
|ECDSA Sign   (Blocking)|TBD|Curve: P-521|
|ECDSA Verify (Blocking)|TBD|Curve: P-521|
|ECDSA Sign   (Non-blocking)|TBD|Curve: P-192|
|ECDSA Verify (Non-blocking)|TBD|Curve: P-192|
|ECDSA Sign   (Non-blocking)|TBD|Curve: P-256|
|ECDSA Verify (Non-blocking)|TBD|Curve: P-256|
|ECDSA Sign   (Non-blocking)|TBD|Curve: P-384|
|ECDSA Verify (Non-blocking)|TBD|Curve: P-384|
|ECDSA Sign   (Non-blocking)|TBD|Curve: P-521|
|ECDSA Verify (Non-blocking)|TBD|Curve: P-521|
|TRNG (Generate)|TBD|Output Size: 521|
|ECDH (Shared Secret Generate)|TBD|Curve: P-192|
|ECDH (Shared Secret Generate)|TBD|Curve: P-256|
|ECDH (Shared Secret Generate)|TBD|Curve: P-384|
|ECDH (Shared Secret Generate)|TBD|Curve: P-521|

### Memory Size Benchmarking
The following results include usage of single step and multi step APIs. Flash size will vary based on size of the stored data inputs used with the library.

All projects have the following compiler options:
- Optimization for size (-Os)
- Isolate each function in a section - unchecked
- Remove unused sections - unchecked

| Algorithm | FLASH (bytes) | RAM Static (bytes) | RAM Stack (bytes)|
|----|----|----|----|
|SHA-1 (Digest)|TBD|TBD|TBD|
|SHA-1 (Init, Update, and Final)|TBD|TBD|TBD|
|SHA-224 (Digest)|TBD|TBD|TBD|
|SHA-224 (Init, Update, and Final)|TBD|TBD|TBD|
|SHA-256 (Digest)|TBD|TBD|TBD|
|SHA-256 (Init, Update, and Final)|TBD|TBD|TBD|
|SHA-384 (Digest)|TBD|TBD|TBD|
|SHA-384 (Init, Update, and Final)|TBD|TBD|TBD|
|SHA-512 (Digest)|TBD|TBD|TBD|
|SHA-512 (Init, Update, and Final)|TBD|TBD|TBD|
|HMAC SHA-1 (Direct)|TBD|TBD|TBD|
|HMAC SHA-1 (Init, Cipher, and Final)|TBD|TBD|TBD|
|HMAC SHA-224 (Direct)|TBD|TBD|TBD|
|HMAC SHA-224 (Init, Cipher, and Final)|TBD|TBD|TBD|
|HMAC SHA-256 (Direct)|TBD|TBD|TBD|
|HMAC SHA-256 (Init, Cipher, and Final)|TBD|TBD|TBD|
|HMAC SHA-384 (Direct)|TBD|TBD|TBD|
|HMAC SHA-384 (Init, Cipher, and Final)|TBD|TBD|TBD|
|HMAC SHA-512 (Direct)|TBD|TBD|TBD|
|HMAC SHA-512 (Init, Cipher, and Final)|TBD|TBD|TBD|
|AES-ECB (Encrypt and Decrypt)|TBD|TBD|TBD|
|AES-ECB (Init and Cipher)|TBD|TBD|TBD|
|AES-CTR (Encrypt and Decrypt)|TBD|TBD|TBD|
|AES-CTR (Init and Cipher)|TBD|TBD|TBD|
|AES-CBC (Encrypt and Decrypt)|TBD|TBD|TBD|
|AES-CBC (Init and Cipher)|TBD|TBD|TBD|
|AES-CFB128 (Encrypt and Decrypt)|TBD|TBD|TBD|
|AES-CFB128 (Init and Cipher)|TBD|TBD|TBD|
|AES-OFB (Encrypt and Decrypt)|TBD|TBD|TBD|
|AES-OFB (Init and Cipher)|TBD|TBD|TBD|
|AES-GCM (Encrypt and Decrypt)|TBD|TBD|TBD|
|AES-GCM (Init, AddAadData, Cipher, and Final)|TBD|TBD|TBD|
|AES-CCM (Init and Cipher)|TBD|TBD|TBD|
|AES-XTS (Encrypt and Decrypt)|TBD|TBD|TBD|
|AES-XTS (Init and Cipher)|TBD|TBD|TBD|
|AES-CMAC (Direct)|TBD|TBD|TBD|
|AES-CMAC (Init, Cipher, and Final)|TBD|TBD|TBD|
|AES-GMAC (Direct)|TBD|TBD|TBD|
|AES-GMAC (Init and Cipher)|TBD|TBD|TBD|
|ECDSA Blocking (Sign and Verify (P-192))|TBD|TBD|TBD|
|ECDSA Blocking (Sign and Verify (P-256))|TBD|TBD|TBD|
|ECDSA Blocking (Sign and Verify (P-384))|TBD|TBD|TBD|
|ECDSA Blocking (Sign and Verify (P-521))|TBD|TBD|TBD|
|ECDSA Non-blocking (Sign and Verify (P-192))|TBD|TBD|TBD|
|ECDSA Non-blocking (Sign and Verify (P-256))|TBD|TBD|TBD|
|ECDSA Non-blocking (Sign and Verify (P-384))|TBD|TBD|TBD|
|ECDSA Non-blocking (Sign and Verify (P-521))|TBD|TBD|TBD|
|TRNG (Generate)|TBD|TBD|TBD|
|ECDH (Shared Secret (P-192))|TBD|TBD|TBD|
|ECDH (Shared Secret (P-256))|TBD|TBD|TBD|
|ECDH (Shared Secret (P-384))|TBD|TBD|TBD|
|ECDH (Shared Secret (P-521))|TBD|TBD|TBD|
