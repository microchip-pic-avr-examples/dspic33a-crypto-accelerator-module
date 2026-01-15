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

## Software Tools
- dsPIC33AK-MP_DFP v1.2.135
- MPLAB® X IDE [v6.25 or newer](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.30 or newer](https://www.microchip.com/xcdsc)

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

### Performance Benchmarking
The following benchmarking results were obtained while testing the Crypto Library APIs that utilize the pre-compiled Hardware Driver.

Benchmarking parameters: Device clock speed set to 200 MHz

|Algorithm|Performance|Parameters (sizes are in bytes)|
|----|----|----|
|SHA-1 (Digest)|1056.69 Mbps|Message Size: 8,192|
|SHA-224 (Digest)|1264.44 Mbps|Message Size: 8,192|
|SHA-256 (Digest)|1264.69 Mbps|Message Size: 8,192|
|SHA-384 (Digest)|1524.80 Mbps|Message Size: 8,192|
|SHA-512 (Digest)|1525.16 Mbps|Message Size: 8,192|
|HMAC SHA-1 (Direct)|139.77 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-224 (Direct)|140.49 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-256 (Direct)|140.50 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-384 (Direct)|230.63 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-512 (Direct)|230.70 Mbps|Key Size: 16, Message Size: 8,192|
|AES-ECB (Encrypt)|436.88 Mbps|Key Size: 16, Plaintext Size: 8,192|
|AES-ECB (Decrypt)|436.24 Mbps|Key Size: 16, Ciphertext Size: 8,192|
|AES-CTR (Encrypt)|436.94 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CTR (Decrypt)|437.09 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CBC (Encrypt)|436.94 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CBC (Decrypt)|436.16 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CFB128 (Encrypt)|436.74 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CFB128 (Decrypt)|436.89 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-OFB (Encrypt)|437.00 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-OFB (Decrypt)|437.21 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-GCM (Encrypt)|423.29 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|
|AES-GCM (Decrypt)|421.18 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|
|AES-CCM (Encrypt)|303.17 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|
|AES-CCM (Decrypt)|304.32 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|
|AES-XTS (Encrypt)|431.77 Mbps|Key Size: 16, Tweak Size: 16, Plaintext Size: 8,192|
|AES-XTS (Decrypt)|430.98 Mbps|Key Size: 16, Tweak Size: 16, Ciphertext Size: 8,192|
|AES-CMAC (Direct)|26.76 Mbps|Key Size: 16, Plaintext Size: 8,192, MAC Size: 16|
|AES-GMAC (Direct)|424.89 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 8,192, Tag Size: 16|
|ECDSA Sign   (Blocking)|3.88 ms|Curve: P-192|
|ECDSA Verify (Blocking)|5.12 ms|Curve: P-192|
|ECDSA Sign   (Blocking)|7.36 ms|Curve: P-256|
|ECDSA Verify (Blocking)|9.84 ms|Curve: P-256|
|ECDSA Sign   (Blocking)|19.64 ms|Curve: P-384|
|ECDSA Verify (Blocking)|26.92 ms|Curve: P-384|
|ECDSA Sign   (Blocking)|41.88 ms|Curve: P-521|
|ECDSA Verify (Blocking)|58.08 ms|Curve: P-521|
|ECDSA Sign   (Non-blocking)|3.88 ms|Curve: P-192|
|ECDSA Verify (Non-blocking)|5.00 ms|Curve: P-192|
|ECDSA Sign   (Non-blocking)|7.24 ms|Curve: P-256|
|ECDSA Verify (Non-blocking)|9.76 ms|Curve: P-256|
|ECDSA Sign   (Non-blocking)|19.56 ms|Curve: P-384|
|ECDSA Verify (Non-blocking)|26.80 ms|Curve: P-384|
|ECDSA Sign   (Non-blocking)|41.88 ms|Curve: P-521|
|ECDSA Verify (Non-blocking)|57.72 ms|Curve: P-521|
|TRNG (Generate)|122.667 µs|Output Size: 521|
|ECDH (Shared Secret Generate)|3.760 ms|Curve: P-192|
|ECDH (Shared Secret Generate)|7.160 ms|Curve: P-256|
|ECDH (Shared Secret Generate)|19.360 ms|Curve: P-384|
|ECDH (Shared Secret Generate)|41.400 ms|Curve: P-521|

### Memory Size Benchmarking
The following results include usage of single step and multi step APIs. Flash size will vary based on size of the stored data inputs used with the library.

All projects have the following compiler options:
- Optimization for size (-Os)
- Isolate each function in a section - unchecked
- Remove unused sections - unchecked

|Algorithm|RAM (bytes)|FLASH (bytes)|Stack Usage (bytes)|Heap Usage (bytes)|
|----|----|----|----|----|
|SHA-1 (Digest)|74|10,476|308|0|
|SHA-1 (Init, Update, and Final)|74|10,520|700|0|
|SHA-224 (Digest)|74|10,476|316|0|
|SHA-224 (Init, Update, and Final)|74|10,524|708|0|
|SHA-256 (Digest)|74|10,476|320|0|
|SHA-256 (Init, Update, and Final)|74|10,524|712|0|
|SHA-384 (Digest)|74|10,476|336|0|
|SHA-384 (Init, Update, and Final)|74|10,524|728|0|
|SHA-512 (Digest)|74|10,476|352|0|
|SHA-512 (Init, Update, and Final)|74|10,524|744|0|
|HMAC SHA-1 (Direct)|74|18,716|884|0|
|HMAC SHA-1 (Init, Cipher, and Final)|850|18,764|156|0|
|HMAC SHA-224 (Direct)|74|18,716|892|0|
|HMAC SHA-224 (Init, Cipher, and Final)|850|18,764|164|0|
|HMAC SHA-256 (Direct)|74|18,716|896|0|
|HMAC SHA-256 (Init, Cipher, and Final)|850|18,764|168|0|
|HMAC SHA-384 (Direct)|74|18,716|912|0|
|HMAC SHA-384 (Init, Cipher, and Final)|850|18,764|184|0|
|HMAC SHA-512 (Direct)|74|18,716|928|0|
|HMAC SHA-512 (Init, Cipher, and Final)|850|18,764|200|0|
|AES-ECB (Encrypt and Decrypt)|74|11,840|408|0|
|AES-ECB (Init and Cipher)|2,022|11,904|140|0|
|AES-CTR (Encrypt and Decrypt)|74|11,880|424|0|
|AES-CTR (Init and Cipher)|2,022|11,948|156|0|
|AES-CBC (Encrypt and Decrypt)|74|12,208|424|0|
|AES-CBC (Init and Cipher)|2,022|12,276|156|0|
|AES-CFB128 (Encrypt and Decrypt)|74|12,208|424|0|
|AES-CFB128 (Init and Cipher)|2,022|12,276|156|0|
|AES-OFB (Encrypt and Decrypt)|74|12,208|424|0|
|AES-OFB (Init and Cipher)|2,022|12,276|156|0|
|AES-GCM (Encrypt and Decrypt)|74|13,324|468|0|
|AES-GCM (Init, AddAadData, Cipher, and Final)|614|13,448|188|0|
|AES-CCM (Init and Cipher)|590|13,272|308|0|
|AES-XTS (Encrypt and Decrypt)|62|11,684|424|0|
|AES-XTS (Init and Cipher)|2,010|11,752|156|0|
|AES-CMAC (Direct)|74|18,764|340|0|
|AES-CMAC (Init, Cipher, and Final)|602|18,812|104|0|
|AES-GMAC (Direct)|74|18,800|380|0|
|AES-GMAC (Init and Cipher)|602|18,840|112|0|
|ECDSA Blocking (Sign and Verify (P-192))|13,440|4,172|324|
|ECDSA Blocking (Sign and Verify (P-256))|13,492|4,172|324|
|ECDSA Blocking (Sign and Verify (P-384))|13,588|4,172|324|
|ECDSA Blocking (Sign and Verify (P-521))|13,696|4,172|324|
|ECDSA Non-blocking (Sign and Verify (P-192))|13,480|4,480|316|
|ECDSA Non-blocking (Sign and Verify (P-256))|13,520|4,172|316|
|ECDSA Non-blocking (Sign and Verify (P-384))|13,616|4,172|316|
|ECDSA Non-blocking (Sign and Verify (P-521))|13,724|4,172|316|
|TRNG (Generate)|56|4,612|356|0|
|ECDH (Shared Secret (P-192))|11,588|4,168|308|
|ECDH (Shared Secret (P-256))|11,604|4,168|308|
|ECDH (Shared Secret (P-384))|11,684|4,168|308|
|ECDH (Shared Secret (P-521))|11,780|4,168|308|
