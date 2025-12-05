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

The included demonstration projects provide examples of the various CAM features. The application project needs to include the Crypto Library in the crypto/common_crypto folder. The Crypto Library API and associated hardware wrappers invoke the pre-compiled CAM Hardware Driver.

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

| Folder     | Description                                         |
| ---        | ---                                                 |
| dsa        | Example application project for ECDSA               |
| hash       | Example application project for Hashing             |
| trng       | Example application project for TRNG                |
| aes        | Example application project for AES                 |
| ecdh       | Example application project for ECDH                |
| aes_hash   | Example application project for concurrent AES+HASH |

# Crypto Accelerator Module APIs

The pre-compiled CAM Hardware Driver can function independently by utilizing the following [APIs](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=CAM&redirect=true).

# MISRA

A MISRA scan has been conducted on the pre-compiled CAM Hardware Driver source files. The source files are MISRA compliant for Required and Mandatory rules.

# Benchmarking

### Performance Benchmarking
The following benchmarking results were obtained while testing the Crypto Library APIs that utilize the pre-compiled Hardware Driver.

Benchmarking parameters: Device clock speed set to 200 MHz

|Algorithm|Performance|Parameters (sizes are in bytes)|
|----|----|----|
|SHA-1 (Digest)|982.99 Mbps|Message Size: 8,192|
|SHA-224 (Digest)|1160.75 Mbps|Message Size: 8,192|
|SHA-256 (Digest)|1160.55 Mbps|Message Size: 8,192|
|SHA-384 (Digest)|1394.09 Mbps|Message Size: 8,192|
|SHA-512 (Digest)|1395.27 Mbps|Message Size: 8,192|
|HMAC SHA-1 (Direct)|139.04 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-224 (Direct)|140.54 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-256 (Direct)|140.51 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-384 (Direct)|230.78 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-512 (Direct)|230.96 Mbps|Key Size: 16, Message Size: 8,192|
|AES-ECB (Encrypt)|435.48 Mbps|Key Size: 16, Plaintext Size: 8,192|
|AES-ECB (Decrypt)|434.36 Mbps|Key Size: 16, Ciphertext Size: 8,192|
|AES-CTR (Encrypt)|435.26 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CTR (Decrypt)|435.35 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CBC (Encrypt)|435.64 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CBC (Decrypt)|434.49 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CFB128 (Encrypt)|435.29 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CFB128 (Decrypt)|435.24 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-OFB (Encrypt)|435.29 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-OFB (Decrypt)|435.24 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-GCM (Encrypt)|420.99 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|
|AES-GCM (Decrypt)|417.59 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|
|AES-XTS (Encrypt)|429.99 Mbps|Key Size: 16, Tweak Size: 16, Plaintext Size: 8,192|
|AES-XTS (Decrypt)|429.20 Mbps|Key Size: 16, Tweak Size: 16, Ciphertext Size: 8,192|
|AES-CMAC (Direct)|26.79 Mbps|Key Size: 16, Plaintext Size: 8,192, MAC Size: 16|
|AES-GMAC (Direct)|154.28 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 8,192, Tag Size: 16|
|ECDSA (Sign)|3.913 ms|Curve: P-192|
|ECDSA (Verify)|5.129 ms|Curve: P-192|
|ECDSA (Sign)|7.396 ms|Curve: P-256|
|ECDSA (Verify)|9.882 ms|Curve: P-256|
|ECDSA (Sign)|19.759 ms|Curve: P-384|
|ECDSA (Verify)|27.006 ms|Curve: P-384|
|ECDSA (Sign)|42.057 ms|Curve: P-521|
|ECDSA (Verify)|58.281 ms|Curve: P-521|
|TRNG (Generate)|122.667 µs|Output Size: 521|
|ECDH (Shared Secret Generate)|3.770 ms|Curve: P-192|
|ECDH (Shared Secret Generate)|7.207 ms|Curve: P-256|
|ECDH (Shared Secret Generate)|19.400 ms|Curve: P-384|
|ECDH (Shared Secret Generate)|41.473 ms|Curve: P-521|

### Memory Size Benchmarking
The following results include usage of single step and multi step APIs. Flash size will vary based on size of the stored data inputs used with the library.

All projects have the following compiler options:
- Optimization for size (-Os)
- Isolate each function in a section - unchecked
- Remove unused sections - unchecked

|Algorithm|RAM (bytes)|FLASH (bytes)|
|----|----|----|
|SHA-1 (Digest)|58|10,708|
|SHA-1 (Init, Update, and Final)|58|10,756|
|SHA-224 (Digest)|58|10,708|
|SHA-224 (Init, Update, and Final)|58|10,756|
|SHA-256 (Digest)|58|10,708|
|SHA-256 (Init, Update, and Final)|58|10,756|
|SHA-384 (Digest)|58|10,708|
|SHA-384 (Init, Update, and Final)|58|10,756|
|SHA-512 (Digest)|58|10,708|
|SHA-512 (Init, Update, and Final)|58|10,756|
|HMAC SHA-1 (Direct)|62|17,112|
|HMAC SHA-1 (Init, Cipher, and Final)|838|17,160|
|HMAC SHA-224 (Direct)|62|17,112|
|HMAC SHA-224 (Init, Cipher, and Final)|838|17,160|
|HMAC SHA-256 (Direct)|62|17,112|
|HMAC SHA-256 (Init, Cipher, and Final)|838|17,160|
|HMAC SHA-384 (Direct)|62|17,112|
|HMAC SHA-384 (Init, Cipher, and Final)|838|17,160|
|HMAC SHA-512 (Direct)|62|17,112|
|HMAC SHA-512 (Init, Cipher, and Final)|838|17,160|
|AES-ECB (Encrypt and Decrypt)|58|11,788|
|AES-ECB (Init and Cipher)|2,006|11,852|
|AES-CTR (Encrypt and Decrypt)|58|11,828|
|AES-CTR (Init and Cipher)|2,006|11,896|
|AES-CBC (Encrypt and Decrypt)|62|12,312|
|AES-CBC (Init and Cipher)|2,010|12,380|
|AES-CFB128 (Encrypt and Decrypt)|62|12,312|
|AES-CFB128 (Init and Cipher)|2,010|12,380|
|AES-OFB (Encrypt and Decrypt)|62|12,312|
|AES-OFB (Init and Cipher)|2,010|12,380|
|AES-GCM (Encrypt and Decrypt)|58|12,204|
|AES-GCM (Init, AddAadData, Cipher, and Final)|598|12,328|
|AES-XTS (Encrypt and Decrypt)|58|11,828|
|AES-XTS (Init and Cipher)|2,006|11,896|
|AES-CMAC (Direct)|62|17,752|
|AES-CMAC (Init, Cipher, and Final)|590|17,800|
|AES-GMAC (Direct)|62|17,196|
|AES-GMAC (Init and Cipher)|590|17,236|
|ECDSA (Sign and Verify (P-192))|4,152|11,360|
|ECDSA (Sign and Verify (P-256))|4,152|11,404|
|ECDSA (Sign and Verify (P-384))|4,152|11,472|
|ECDSA (Sign and Verify (P-521))|4,152|11,552|
|TRNG (Generate)|56|4,612|
|ECDH (Shared Secret (P-192))|4,152|10,744|
|ECDH (Shared Secret (P-256))|4,152|10,788|
|ECDH (Shared Secret (P-384))|4,152|10,856|
|ECDH (Shared Secret (P-521))|4,152|10,928|