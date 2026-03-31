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
|SHA3-224 (Digest)|Message Size: 8,192|N/A|TBD|
|SHA3-256 (Digest)|Message Size: 8,192|N/A|TBD|
|SHA3-384 (Digest)|Message Size: 8,192|N/A|TBD|
|SHA3-512 (Digest)|Message Size: 8,192|N/A|TBD|
|SHAKE128 (Digest)|Message Size: 8,192|N/A|TBD|
|SHAKE256 (Digest)|Message Size: 8,192|N/A|TBD|
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
