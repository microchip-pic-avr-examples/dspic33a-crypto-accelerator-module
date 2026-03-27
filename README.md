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

The included demonstration projects provide examples of the various CAM features. The application project needs to include the Crypto Library located under the lib directory. The Crypto Library API and associated hardware wrappers invoke the pre-compiled CAM Hardware Driver.

# Prerequisites

## Software Tool Versions Used
- dsPIC33AK-MP_DFP v1.3.185
- MPLAB® X IDE [v6.30](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.31](https://www.microchip.com/xcdsc)

## Hardware Tools
- dsPIC33AK512MPS512 Curiosity GP DIM ([EV80L65A](https://www.microchip.com/en-us/development-tool/EV80L65A))
- dsPIC33AK256MPS306 Curiosity GP DIM ([EV02E57A](https://www.microchip.com/en-us/development-tool/EV02E57A))
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

# Contents Summary
Example applications are organized by device under the `dspic33ak512mps512` and `dspic33ak256mps306` directories. Each sub-folder contains a README describing the example application in more detail.

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
- UART peripheral and print out statements
- Isolation to use one algorithm at a time
- Store input data in flash memory to benchmark RAM usage for CAM Hardware Driver

The benchmarking projects are configured to use the following:
- Optimization level -Os
- Tools and versions as listed in the Software Tools section above

### Performance Benchmarking
The following benchmarking results were obtained while testing the Crypto Library APIs that utilize the pre-compiled Hardware Driver.

Benchmarking parameters: Device clock speed set to 200 MHz

|Algorithm|Parameters (sizes are in bytes)|dsPIC33AK512MPS512 (s)|dsPIC33AK256MPS306 (s)|
|----|----|----|----|
|SHA-1 (Digest)|Message Size: 8,192|TBD|TBD|
|SHA-224 (Digest)|Message Size: 8,192|TBD|TBD|
|SHA-256 (Digest)|Message Size: 8,192|TBD|TBD|
|SHA-384 (Digest)|Message Size: 8,192|TBD|TBD|
|SHA-512 (Digest)|Message Size: 8,192|TBD|TBD|
|SHA3-224 (Digest)|Message Size: 8,192|Unsupported|TBD|
|SHA3-256 (Digest)|Message Size: 8,192|Unsupported|TBD|
|SHA3-384 (Digest)|Message Size: 8,192|Unsupported|TBD|
|SHA3-512 (Digest)|Message Size: 8,192|Unsupported|TBD|
|SHAKE128 (Digest)|Message Size: 8,192|Unsupported|TBD|
|SHAKE256 (Digest)|Message Size: 8,192|Unsupported|TBD|
|HMAC SHA-1 (Direct)|Key Size: 16, Message Size: 8,192|TBD|TBD|
|HMAC SHA-224 (Direct)|Key Size: 16, Message Size: 8,192|TBD|TBD|
|HMAC SHA-256 (Direct)|Key Size: 16, Message Size: 8,192|TBD|TBD|
|HMAC SHA-384 (Direct)|Key Size: 16, Message Size: 8,192|TBD|TBD|
|HMAC SHA-512 (Direct)|Key Size: 16, Message Size: 8,192|TBD|TBD|
|AES-ECB (Encrypt)|Key Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-ECB (Decrypt)|Key Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-CTR (Encrypt)|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-CTR (Decrypt)|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-CBC (Encrypt)|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-CBC (Decrypt)|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-CFB128 (Encrypt)|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-CFB128 (Decrypt)|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-OFB (Encrypt)|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-OFB (Decrypt)|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-GCM (Encrypt)|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-GCM (Decrypt)|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-CCM (Encrypt)|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-CCM (Decrypt)|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-XTS (Encrypt)|Key Size: 16, Tweak Size: 16, Plaintext Size: 8,192|TBD|TBD|
|AES-XTS (Decrypt)|Key Size: 16, Tweak Size: 16, Ciphertext Size: 8,192|TBD|TBD|
|AES-CMAC (Direct)|Key Size: 16, Plaintext Size: 8,192, MAC Size: 16|TBD|TBD|
|AES-GMAC (Direct)|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 8,192, Tag Size: 16|TBD|TBD|
|ECDSA Sign   (Blocking)|Curve: P-192|TBD|TBD|
|ECDSA Verify (Blocking)|Curve: P-192|TBD|TBD|
|ECDSA Sign   (Blocking)|Curve: P-256|TBD|TBD|
|ECDSA Verify (Blocking)|Curve: P-256|TBD|TBD|
|ECDSA Sign   (Blocking)|Curve: P-384|TBD|TBD|
|ECDSA Verify (Blocking)|Curve: P-384|TBD|TBD|
|ECDSA Sign   (Blocking)|Curve: P-521|TBD|TBD|
|ECDSA Verify (Blocking)|Curve: P-521|TBD|TBD|
|ECDSA Sign   (Non-blocking)|Curve: P-192|TBD|TBD|
|ECDSA Verify (Non-blocking)|Curve: P-192|TBD|TBD|
|ECDSA Sign   (Non-blocking)|Curve: P-256|TBD|TBD|
|ECDSA Verify (Non-blocking)|Curve: P-256|TBD|TBD|
|ECDSA Sign   (Non-blocking)|Curve: P-384|TBD|TBD|
|ECDSA Verify (Non-blocking)|Curve: P-384|TBD|TBD|
|ECDSA Sign   (Non-blocking)|Curve: P-521|TBD|TBD|
|ECDSA Verify (Non-blocking)|Curve: P-521|TBD|TBD|
|TRNG (Generate)|Output Size: 521|TBD|TBD|
|ECDH (Shared Secret Generate)|Curve: P-192|TBD|TBD|
|ECDH (Shared Secret Generate)|Curve: P-256|TBD|TBD|
|ECDH (Shared Secret Generate)|Curve: P-384|TBD|TBD|
|ECDH (Shared Secret Generate)|Curve: P-521|TBD|TBD|

### Memory Size Benchmarking
The following results include usage of single step and multi step APIs. Flash size will vary based on size of the stored data inputs used with the library.

All projects have the following compiler options:
- Optimization for size (-Os)
- Isolate each function in a section - unchecked
- Remove unused sections - unchecked

<table>
  <tr>
    <th rowspan="2">Algorithm</th>
    <th colspan="3">dsPIC33AK512MPS512</th>
    <th colspan="3">dsPIC33AK256MPS306</th>
  </tr>
  <tr>
    <th>FLASH <br>(bytes)</th>
    <th>RAM <br>Static <br>(bytes)</th>
    <th>RAM <br>Stack <br>(bytes)</th>
    <th>FLASH <br>(bytes)</th>
    <th>RAM <br>Static <br>(bytes)</th>
    <th>RAM <br>Stack <br>(bytes)</th>
  </tr>
  <tr>
    <td>SHA-1 (Digest)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-1 (Init, Update, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-224 (Digest)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-224 (Init, Update, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-256 (Digest)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-256 (Init, Update, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-384 (Digest)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-384 (Init, Update, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-512 (Digest)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA-512 (Init, Update, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-224 (Digest)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-224 (Init, Update, and Final)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-256 (Digest)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-256 (Init, Update, and Final)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-384 (Digest)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-384 (Init, Update, and Final)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-512 (Digest)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHA3-512 (Init, Update, and Final)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHAKE128 (Digest)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHAKE128 (Init, Update, and Final)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHAKE256 (Digest)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>SHAKE256 (Init, Update, and Final)</td>
    <td>Unsupported</td><td>Unsupported</td><td>Unsupported</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-1 (Direct)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-1 (Init, Cipher, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-224 (Direct)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-224 (Init, Cipher, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-256 (Direct)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-256 (Init, Cipher, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-384 (Direct)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-384 (Init, Cipher, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-512 (Direct)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>HMAC SHA-512 (Init, Cipher, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-ECB (Encrypt and Decrypt)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-ECB (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CTR (Encrypt and Decrypt)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CTR (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CBC (Encrypt and Decrypt)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CBC (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CFB128 (Encrypt and Decrypt)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CFB128 (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-OFB (Encrypt and Decrypt)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-OFB (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-GCM (Encrypt and Decrypt)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-GCM (Init, AddAadData, Cipher, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CCM (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-XTS (Encrypt and Decrypt)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-XTS (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CMAC (Direct)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-CMAC (Init, Cipher, and Final)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-GMAC (Direct)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>AES-GMAC (Init and Cipher)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-192))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-256))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-384))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-521))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-192))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-256))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-384))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-521))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>TRNG (Generate)</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-192))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-256))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-384))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-521))</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
    <td>TBD</td><td>TBD</td><td>TBD</td>
  </tr>
</table>
