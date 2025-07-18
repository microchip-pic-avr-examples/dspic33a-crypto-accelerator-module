<picture>
    <source media="(prefers-color-scheme: dark)" srcset="../../images/microchip_logo_white_red.png">
    <source media="(prefers-color-scheme: light)" srcset="../../images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="../../images/microchip_logo_black_red.png">
</picture>

# Crypto Accelerator Module AES/HASH Concurrent Execution Example Application
## Description
This example application utilizes the Crypto Accelerator Module Library to perform concurrent AES and HASH operations to demonstrate context safety of the Crypto Accelerator Module Library.  AES and HASH operations using different sized input vectors are executed in parallel as multi-step (init/cipher/finalize) operations. As this application is not intended to demonstrate complete cryptographic compliance, a limited set of test vectors and algorithms are used.  The following AES and HASH algorithms are used:
- AES-ECB
- AES-CTR
- AES-CMAC
- AES-GCM
- SHA-512

## Software Tool Versions
- dsPIC33AK-MP_DEV_DFP v1.0.81
- MPLAB® X IDE [v6.25 or newer](https://www.microchip.com/mplab-x-ide)
- MPLAB® XC-DSC Compiler [v3.21 or newer](https://www.microchip.com/xcdsc)

## Setting Up The Hardware
- dsPIC33AK512MPS512 Curiosity GP DIM ([EV80L65A](https://www.microchip.com/en-us/development-tool/EV80L65A))
- Curiosity Platform Development Board ([EV74H48A](https://www.microchip.com/EV74H48A))

1. Insert the dsPIC33AK512MPS512 DIM into the DIM J1 slot on the Curiosity Platform Development Board.
2. Connect the board from the J24 USB-C PKoB4 (PICKit™ On-Board 4) to the computer.

<img src="./images/dspic33a_dim_board.jpg" alt="Hardware Setup" width="750"/>

## Running the Application
1. Within MPLAB® X IDE, open the `aes_hash.X` application project in the `aes_hash/` directory.
2. Build and Program the application using MPLAB® X IDE.
3. The printed information can be found on the serial COM port reading at a speed of 115200.
4. The application first executes each algorithm independently to demonstrate basic function is successful, and then runs a concurrent execution test to demonstrate concurrent operations are also successful.

## Application Results

### Individual (Separate Execution) Tests
1. The application will initialize a context for each of the cryptographic operations (AES and HASH).
2. A state machine for each algorithm is defined to execute each step in the algorithm's operation.
3. The main application executes all steps for each algorithm for all test cases without interleaving them.
4. As each operation completes its step, it indicates if the step was executed succesfully.  For the finalization step, a comparison against expected results is performed and the pass/fail message reflects that comparison.

### Concurrent Execution Tests
1. The application will initialize a context for each of the cryptographic operations (AES and HASH).
2. A state machine for each algorithm is defined to execute each step in the algorithm's operation.
3. The main application executes one step from each algorithm at a time, interleaving calls to each operation context.
4. Because each operation has a different number of steps, execution changes which algorithm's operation happens in relation to others.
5. As each operation completes its step, it indicates if the step was executed succesfully.  For the finalization step, a comparison against expected results is performed and the pass/fail message reflects that comparison.
6. Each algorithm executes all defined test cases for that algorithm, one step at a time, until the test data indicates there are no more steps.
7. The application executes a number of iterations of steps 1-6 (defined as `7`, and controlled with the `TEST_ITERATIONS` definition in `app_aes_hash.c`).
