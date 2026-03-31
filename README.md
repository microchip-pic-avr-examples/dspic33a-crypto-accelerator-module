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

|Algorithm|dsPIC33AK512MPS512 Benchmarking|dsPIC33AK256MPS306 Benchmarking|Parameters (sizes are in bytes)|
|----|----|----|----|
|SHA-1 (Digest)|1,025.44 Mbps|1,115.70 Mbps|Message Size: 8,192|
|SHA-224 (Digest)|1,219.05 Mbps|1,351.26 Mbps|Message Size: 8,192|
|SHA-256 (Digest)|1,219.27 Mbps|1,349.87 Mbps|Message Size: 8,192|
|SHA-384 (Digest)|1,491.83 Mbps|1,731.92 Mbps|Message Size: 8,192|
|SHA-512 (Digest)|1,491.15 Mbps|1,728.27 Mbps|Message Size: 8,192|
|SHA3-224 (Digest)|N/A|2,714.83 Mbps|Message Size: 8,192|
|SHA3-256 (Digest)|N/A|2,687.00 Mbps|Message Size: 8,192|
|SHA3-384 (Digest)|N/A|2,701.40 Mbps|Message Size: 8,192|
|SHA3-512 (Digest)|N/A|2,705.86 Mbps|Message Size: 8,192|
|SHAKE128 (Digest)|N/A|2,743.24 Mbps|Message Size: 8,192|
|SHAKE256 (Digest)|N/A|2,713.71 Mbps|Message Size: 8,192|
|HMAC SHA-1 (Direct)|135.77 Mbps|135.81 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-224 (Direct)|138.69 Mbps|137.33 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-256 (Direct)|138.69 Mbps|137.29 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-384 (Direct)|225.89 Mbps|224.85 Mbps|Key Size: 16, Message Size: 8,192|
|HMAC SHA-512 (Direct)|226.01 Mbps|224.74 Mbps|Key Size: 16, Message Size: 8,192|
|AES-ECB (Encrypt)|437.20 Mbps|437.34 Mbps|Key Size: 16, Plaintext Size: 8,192|
|AES-ECB (Decrypt)|436.32 Mbps|436.15 Mbps|Key Size: 16, Ciphertext Size: 8,192|
|AES-CTR (Encrypt)|436.35 Mbps|436.21 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CTR (Decrypt)|436.35 Mbps|436.21 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CBC (Encrypt)|436.38 Mbps|436.24 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CBC (Decrypt)|435.57 Mbps|435.40 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-CFB128 (Encrypt)|436.18 Mbps|436.03 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-CFB128 (Decrypt)|436.15 Mbps|435.95 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-OFB (Encrypt)|436.44 Mbps|436.30 Mbps|Key Size: 16, Initialization Vector Size: 16, Plaintext Size: 8,192|
|AES-OFB (Decrypt)|436.44 Mbps|436.21 Mbps|Key Size: 16, Initialization Vector Size: 16, Ciphertext Size: 8,192|
|AES-GCM (Encrypt)|421.45 Mbps|421.97 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|
|AES-GCM (Decrypt)|419.35 Mbps|419.67 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|
|AES-CCM (Encrypt)|216.22 Mbps|216.40 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Plaintext Size: 8,192|
|AES-CCM (Decrypt)|215.30 Mbps|215.57 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 4, Tag Size: 16, Ciphertext Size: 8,192|
|AES-XTS (Encrypt)|431.13 Mbps|431.04 Mbps|Key Size: 16, Tweak Size: 16, Plaintext Size: 8,192|
|AES-XTS (Decrypt)|430.28 Mbps|429.97 Mbps|Key Size: 16, Tweak Size: 16, Ciphertext Size: 8,192|
|AES-CMAC (Direct)|25.61 Mbps|28.20 Mbps|Key Size: 16, Plaintext Size: 8,192, MAC Size: 16|
|AES-GMAC (Direct)|424.32 Mbps|424.84 Mbps|Key Size: 16, Initialization Vector Size: 12, Authentication Data Size: 8,192, Tag Size: 16|
|ECDSA Sign   (Blocking)|3.90 ms|3.99 ms|Curve: P-192|
|ECDSA Verify (Blocking)|5.16 ms|5.11 ms|Curve: P-192|
|ECDSA Sign   (Blocking)|7.39 ms|7.49 ms|Curve: P-256|
|ECDSA Verify (Blocking)|9.95 ms|9.88 ms|Curve: P-256|
|ECDSA Sign   (Blocking)|19.76 ms|19.90 ms|Curve: P-384|
|ECDSA Verify (Blocking)|27.15 ms|27.07 ms|Curve: P-384|
|ECDSA Sign   (Blocking)|42.06 ms|42.31 ms|Curve: P-521|
|ECDSA Verify (Blocking)|58.22 ms|15.18 ms|Curve: P-521|
|ECDSA Sign   (Non-blocking)|3.95 ms|4.05 ms|Curve: P-192|
|ECDSA Verify (Non-blocking)|5.11 ms|5.05 ms|Curve: P-192|
|ECDSA Sign   (Non-blocking)|7.57 ms|7.70 ms|Curve: P-256|
|ECDSA Verify (Non-blocking)|9.90 ms|9.82 ms|Curve: P-256|
|ECDSA Sign   (Non-blocking)|20.37 ms|20.56 ms|Curve: P-384|
|ECDSA Verify (Non-blocking)|27.12 ms|27.01 ms|Curve: P-384|
|ECDSA Sign   (Non-blocking)|43.33 ms|43.63 ms|Curve: P-521|
|ECDSA Verify (Non-blocking)|58.20 ms|15.12 ms|Curve: P-521|
|TRNG (Generate)|114.00 µs|223.00 µs|Output Size: 521|
|ECDH (Shared Secret Generate)|3.79 ms|3.74 ms|Curve: P-192|
|ECDH (Shared Secret Generate)|7.25 ms|7.19 ms|Curve: P-256|
|ECDH (Shared Secret Generate)|19.50 ms|19.42 ms|Curve: P-384|
|ECDH (Shared Secret Generate)|41.68 ms|41.58 ms|Curve: P-521|

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
    <td>11,024</td><td>74</td><td>280</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA-1 (Init, Update, and Final)</td>
    <td>11,864</td><td>74</td><td>44</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA-224 (Digest)</td>
    <td>11,024</td><td>74</td><td>280</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA-224 (Init, Update, and Final)</td>
    <td>11,864</td><td>74</td><td>44</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA-256 (Digest)</td>
    <td>11,024</td><td>74</td><td>280</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA-256 (Init, Update, and Final)</td>
    <td>11,864</td><td>74</td><td>44</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA-384 (Digest)</td>
    <td>11,024</td><td>74</td><td>280</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA-384 (Init, Update, and Final)</td>
    <td>11,864</td><td>74</td><td>44</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA-512 (Digest)</td>
    <td>11,024</td><td>74</td><td>280</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA-512 (Init, Update, and Final)</td>
    <td>11,864</td><td>74</td><td>44</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA3-224 (Digest)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA3-224 (Init, Update, and Final)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA3-256 (Digest)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA3-256 (Init, Update, and Final)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA3-384 (Digest)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA3-384 (Init, Update, and Final)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHA3-512 (Digest)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,344</td><td>74</td><td>480</td>
  </tr>
  <tr>
    <td>SHA3-512 (Init, Update, and Final)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>13,184</td><td>74</td><td>48</td>
  </tr>
  <tr>
    <td>SHAKE128 (Digest)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,100</td><td>74</td><td>476</td>
  </tr>
  <tr>
    <td>SHAKE128 (Init, Update, and Final)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,944</td><td>74</td><td>44</td>
  </tr>
  <tr>
    <td>SHAKE256 (Digest)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,100</td><td>74</td><td>476</td>
  </tr>
  <tr>
    <td>SHAKE256 (Init, Update, and Final)</td>
    <td>N/A</td><td>N/A</td><td>N/A</td>
    <td>12,944</td><td>74</td><td>44</td>
  </tr>
  <tr>
    <td>HMAC SHA-1 (Direct)</td>
    <td>18,948</td><td>74</td><td>848</td>
    <td>19,976</td><td>74</td><td>1,056</td>
  </tr>
  <tr>
    <td>HMAC SHA-1 (Init, Cipher, and Final)</td>
    <td>19,920</td><td>74</td><td>120</td>
    <td>20,948</td><td>74</td><td>124</td>
  </tr>
  <tr>
    <td>HMAC SHA-224 (Direct)</td>
    <td>18,948</td><td>74</td><td>848</td>
    <td>19,976</td><td>74</td><td>1,056</td>
  </tr>
  <tr>
    <td>HMAC SHA-224 (Init, Cipher, and Final)</td>
    <td>19,920</td><td>74</td><td>120</td>
    <td>20,948</td><td>74</td><td>124</td>
  </tr>
  <tr>
    <td>HMAC SHA-256 (Direct)</td>
    <td>18,948</td><td>74</td><td>848</td>
    <td>19,976</td><td>74</td><td>1,056</td>
  </tr>
  <tr>
    <td>HMAC SHA-256 (Init, Cipher, and Final)</td>
    <td>19,920</td><td>74</td><td>120</td>
    <td>20,948</td><td>74</td><td>124</td>
  </tr>
  <tr>
    <td>HMAC SHA-384 (Direct)</td>
    <td>18,948</td><td>74</td><td>848</td>
    <td>19,976</td><td>74</td><td>1,056</td>
  </tr>
  <tr>
    <td>HMAC SHA-384 (Init, Cipher, and Final)</td>
    <td>19,920</td><td>74</td><td>120</td>
    <td>20,948</td><td>74</td><td>124</td>
  </tr>
  <tr>
    <td>HMAC SHA-512 (Direct)</td>
    <td>18,948</td><td>74</td><td>848</td>
    <td>19,976</td><td>74</td><td>1,056</td>
  </tr>
  <tr>
    <td>HMAC SHA-512 (Init, Cipher, and Final)</td>
    <td>19,920</td><td>74</td><td>120</td>
    <td>20,948</td><td>74</td><td>124</td>
  </tr>
  <tr>
    <td>AES-ECB (Encrypt and Decrypt)</td>
    <td>12,004</td><td>74</td><td>324</td>
    <td>12,148</td><td>74</td><td>324</td>
  </tr>
  <tr>
    <td>AES-ECB (Init and Cipher)</td>
    <td>12,060</td><td>2,022</td><td>56</td>
    <td>12,204</td><td>2,022</td><td>56</td>
  </tr>
  <tr>
    <td>AES-CTR (Encrypt and Decrypt)</td>
    <td>12,028</td><td>74</td><td>324</td>
    <td>12,172</td><td>74</td><td>324</td>
  </tr>
  <tr>
    <td>AES-CTR (Init and Cipher)</td>
    <td>12,080</td><td>2,022</td><td>56</td>
    <td>12,224</td><td>2,022</td><td>56</td>
  </tr>
  <tr>
    <td>AES-CBC (Encrypt and Decrypt)</td>
    <td>12,168</td><td>74</td><td>324</td>
    <td>12,172</td><td>74</td><td>324</td>
  </tr>
  <tr>
    <td>AES-CBC (Init and Cipher)</td>
    <td>12,220</td><td>2,022</td><td>56</td>
    <td>12,224</td><td>2,022</td><td>56</td>
  </tr>
  <tr>
    <td>AES-CFB128 (Encrypt and Decrypt)</td>
    <td>12,168</td><td>74</td><td>324</td>
    <td>12,172</td><td>74</td><td>324</td>
  </tr>
  <tr>
    <td>AES-CFB128 (Init and Cipher)</td>
    <td>12,220</td><td>2,022</td><td>56</td>
    <td>12,224</td><td>2,022</td><td>56</td>
  </tr>
  <tr>
    <td>AES-OFB (Encrypt and Decrypt)</td>
    <td>12,168</td><td>74</td><td>324</td>
    <td>12,172</td><td>74</td><td>324</td>
  </tr>
  <tr>
    <td>AES-OFB (Init and Cipher)</td>
    <td>12,220</td><td>2,022</td><td>56</td>
    <td>12,224</td><td>2,022</td><td>56</td>
  </tr>
  <tr>
    <td>AES-GCM (Encrypt and Decrypt)</td>
    <td>13,448</td><td>106</td><td>340</td>
    <td>13,592</td><td>106</td><td>340</td>
  </tr>
  <tr>
    <td>AES-GCM (Init, AddAadData, Cipher, and Final)</td>
    <td>13,540</td><td>646</td><td>56</td>
    <td>13,684</td><td>646</td><td>56</td>
  </tr>
  <tr>
    <td>AES-CCM (Init and Cipher)</td>
    <td>13,544</td><td>602</td><td>116</td>
    <td>13,688</td><td>602</td><td>116</td>
  </tr>
  <tr>
    <td>AES-XTS (Encrypt and Decrypt)</td>
    <td>12,028</td><td>74</td><td>324</td>
    <td>12,172</td><td>74</td><td>324</td>
  </tr>
  <tr>
    <td>AES-XTS (Init and Cipher)</td>
    <td>12,088</td><td>2,022</td><td>56</td>
    <td>12,232</td><td>2,022</td><td>56</td>
  </tr>
  <tr>
    <td>AES-CMAC (Direct)</td>
    <td>18,920</td><td>74</td><td>292</td>
    <td>19,948</td><td>74</td><td>292</td>
  </tr>
  <tr>
    <td>AES-CMAC (Init, Cipher, and Final)</td>
    <td>19,484</td><td>74</td><td>56</td>
    <td>20,512</td><td>74</td><td>56</td>
  </tr>
  <tr>
    <td>AES-GMAC (Direct)</td>
    <td>18,948</td><td>74</td><td>312</td>
    <td>19,976</td><td>74</td><td>312</td>
  </tr>
  <tr>
    <td>AES-GMAC (Init and Cipher)</td>
    <td>19,496</td><td>74</td><td>44</td>
    <td>20,524</td><td>74</td><td>44</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-192))</td>
    <td>14,360</td><td>4,252</td><td>240</td>
    <td>15,284</td><td>4,252</td><td>248</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-256))</td>
    <td>14,396</td><td>4,268</td><td>240</td>
    <td>15,320</td><td>4,268</td><td>248</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-384))</td>
    <td>14,460</td><td>4,300</td><td>240</td>
    <td>15,384</td><td>4,300</td><td>248</td>
  </tr>
  <tr>
    <td>ECDSA Blocking (Sign and Verify (P-521))</td>
    <td>14,532</td><td>4,336</td><td>240</td>
    <td>15,456</td><td>4,336</td><td>248</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-192))</td>
    <td>14,400</td><td>4,252</td><td>232</td>
    <td>15,324</td><td>4,252</td><td>240</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-256))</td>
    <td>14,424</td><td>4,268</td><td>232</td>
    <td>15,348</td><td>4,268</td><td>240</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-384))</td>
    <td>14,488</td><td>4,300</td><td>232</td>
    <td>15,412</td><td>4,300</td><td>240</td>
  </tr>
  <tr>
    <td>ECDSA Non-blocking (Sign and Verify (P-521))</td>
    <td>14,560</td><td>4,336</td><td>232</td>
    <td>15,484</td><td>4,336</td><td>240</td>
  </tr>
  <tr>
    <td>TRNG (Generate)</td>
    <td>5,740</td><td>626</td><td>76</td>
    <td>6,020</td><td>626</td><td>84</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-192))</td>
    <td>12,084</td><td>4,200</td><td>224</td>
    <td>13,008</td><td>4,200</td><td>232</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-256))</td>
    <td>12,100</td><td>4,200</td><td>224</td>
    <td>13,024</td><td>4,200</td><td>232</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-384))</td>
    <td>12,180</td><td>4,200</td><td>224</td>
    <td>13,104</td><td>4,200</td><td>232</td>
  </tr>
  <tr>
    <td>ECDH (Shared Secret (P-521))</td>
    <td>12,276</td><td>4,200</td><td>224</td>
    <td>13,200</td><td>4,200</td><td>232</td>
  </tr>
</table>
