<picture>
    <source media="(prefers-color-scheme: dark)" srcset="images/microchip_logo_white_red.png">
	<source media="(prefers-color-scheme: light)" srcset="images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="images/microchip_logo_black_red.png">
</picture>

# dsPIC33A Crypto Accelerator Module Demos

Board Image if any.

# Description
A collection of MPLAB® X projects to demonstrate the usage of the Cryptographic Accelerator Module APIs:
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

| Folder     | Description                              |
| ---        | ---                                      |
| dsa        | Example application project for ECDSA    |
| hash       | Example application project for Hashing  |
| trng       | Example application project for TRNG     |
| aes        | Example application project for AES      |

# Crypto Accelerator Module APIs

The included demonstration projects provide examples of the various Crypto Accelerator Module features. They display usage of the Crypto Accelerator Module library being consumed by the Common Crypto API and associated hardware wrappers. The Crypto Accelerator Module library can also be used externally by using the following APIs. (API Reference Link)

# MISRA

A MISRA scan has been conducted on the source library file. Results can be found [here](./misra_report/).

# Benchmarking

### Performance Benchmarking
The following benchmarking results were obtained while testing the Crypto Accelerator Module library used with Common Crypto.

Benchmarking parameters: Device clock speed set to 200 MHz

|Algorithm|Performance|Parameters (sizes are in bytes)|
|----|----|----|
|SHA-1 (Digest)|879.44 Mbps|Message Size: 4,096|
|SHA-224 (Digest)|1021.76 Mbps|Message Size: 4,096|
|SHA-256 (Digest)|1026.88 Mbps|Message Size: 4,096|
|SHA-384 (Digest)|1196.32 Mbps|Message Size: 4,096|
|SHA-512 (Digest)|1196.80 Mbps|Message Size: 4,096|
|AES-ECB (Encrypt)|90.40 Mbps|Key Size: 32, Plaintext Size: 128|
|AES-ECB (Decrypt)|88.03 Mbps|Key Size: 32, Ciphertext Size: 128|
|AES-CTR (Encrypt)|96.70 Mbps|Key Size: 32, Initialization Vector Size: 16, Plaintext Size: 128|
|AES-CTR (Decrypt)|96.77 Mbps|Key Size: 32, Initialization Vector Size: 16, Ciphertext Size: 128|
|AES-GCM (Encrypt)|29.15 Mbps|Key Size: 32, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 13|
|AES-GCM (Decrypt)|25.35 Mbps|Key Size: 32, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 13|
|AES-CMAC (Direct)|17.66 Mbps|Key Size: 32, Plaintext Size: 532, MAC Size: 16|
|ECDSA (Verify)|58.35 ms|Curve: P-521|
|TRNG (Generate)|123.084 ms|Output Size: 521|

### Size Benchmarking
The following results include usage of single step and multi step APIs. Flash size will vary based on size of the stored data inputs used with the library. 

|Algorithm|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|SHA-1 (Digest)|240|8,928|
|SHA-1 (Init, Update, and Final)|240|8,976|
|SHA-224 (Digest)|240|8,928|
|SHA-224 (Init, Update, and Final)|240|8,976|
|SHA-256 (Digest)|240|8,928|
|SHA-256 (Init, Update, and Final)|240|8,976|
|SHA-384 (Digest)|240|8,928|
|SHA-384 (Init, Update, and Final)|240|8,976|
|SHA-512 (Digest)|240|8,928|
|SHA-512 (Init, Update, and Final)|240|8,976|
|AES-ECB (Encrypt and Decrypt)|240|9,044|
|AES-ECB (Init and Cipher)|2,188|9,108|
|AES-CTR (Encrypt and Decrypt)|240|9,076|
|AES-CTR (Init and Cipher)|2,188|9,152|
|AES-GCM (Encrypt and Decrypt)|240|10,116|
|AES-GCM (Init, AddAadData, Cipher, and Final)|780|10,240|
|AES-CMAC (Direct)|240|8,680|
|AES-CMAC (Init, Cipher, and Final)|768|8,736|
|ECDSA (Verify)|4,108|10,536|
|TRNG (Generate)|12|3,628|