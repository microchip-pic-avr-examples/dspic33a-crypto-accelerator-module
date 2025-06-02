<picture>
    <source media="(prefers-color-scheme: dark)" srcset="../../images/microchip_logo_white_red.png">
    <source media="(prefers-color-scheme: light)" srcset="../../images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="../../images/microchip_logo_black_red.png">
</picture>

# Crypto Accelerator Module TRNG Application
- Crypto Accelerator Module will be refered to as CAM throughout the remainder of the document. 

## Description
This example application utilizes the Crypto Library that interacts with the Pre-compiled CAM driver files to perform TRNG.

## Software Tool Versions
- dsPIC33AK-MP_DEV_DFP v1.0.81
- MPLAB® X IDE [v6.25 or newer](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.21 or newer](https://www.microchip.com/xcdsc)

## Setting Up The Hardware
- dsPIC33AK512MPS512 Curiosity GP DIM ([EV80L65A](https://www.microchip.com/en-us/development-tool/EV80L65A))
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

1. Insert the dsPIC33AK512MPS512 DIM into the DIM J1 slot on the Curiosity Platform Development Board.
2. Connect the the board from the J24 USB-C PKoB4 (PICKit™ On-Board 4) to the computer.

<img src="./images/dspic33a_dim_board.jpg" alt="Hardware Setup" width="750"/>

## Running the Application
1. Within MPLAB® X IDE, open the trng.x application project in trng/firmware.
2. Build and Program the application using MPLAB® X IDE.
3. The printed information can be found on the serial COM port reading at a speed of 115200.

### Application Results
The following information will be printed on the COM port:
1. A header showing that the generation has begun.
2. A header for each TRNG result that shows the length in bytes.
3. The TRNG result data.

<img src=".\images\trng_demo.jpg" alt="TRNG Demo Output" height="750">

## Crypto Library APIs
crypto_rng.h defines the Generate API.

```c
crypto_Rng_Status_E Crypto_Rng_Prng_Generate(
    crypto_HandlerType_E rngHandlerType_en, // Determines if hardware or software is used for generation
    uint8_t* ptr_rngData,                   // The location that the TRNG generated data will be stored
    uint32_t rngLen,                        // The length of the generated data
    uint8_t* ptr_nonce,                     // Unused parameter for the CAM implementation, should always be set to NULL
    uint32_t nonceLen,                      // Unused parameter for the CAM implementation, should always be set to 0
    uint32_t sessionID                      // Unused parameter for the CAM implementation, should always be set to 1
);
```

## Benchmarking

### Performance Benchmarking

Benchmarking parameters: Device clock speed set to 200 MHz.

|TRNG Size (bytes)|Crypto_Rng_Generate Performance (ms)|
|---|---|
|192|66.44|
|256|77.25|
|384|98.94|
|521|123.13|

### Memory Size Benchmarking

The below sizes are for the process of getting 521 bytes (8-bit words).

|RAM (bytes)|FLASH (bytes)|
|---|---|
|56|4,612|