![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# dsPIC33A Application Demos File Generation

# Description
This README describes how to generate the required Commmon Crypto API and wrapper files and install the required Crypto Accelerator Module (CAM) library files.

## Generating the CryptoV4 wrapper files
To generate the files for the CryptoV4 module's source code, the following must be installed:
* fmpp
* Python (3.10 or later)

### Installing for Windows
The packages can be found at these links:
* [fmpp (through Cygwin installer)](https://www.cygwin.com/install.html)
* [Python 3.10 or later](https://www.python.org)

### Installing for Linux
For Linux, the fmpp and Python installers are installed with the relevant package manager.

### Generating the Files
Once the prerequisites are installed, open a Command prompt or Unix terminal and navigate to the `mchp_private` directory of the repository.<br>
Then, launch the `generateFiles.py` script with the desired arguments:

    usage: generateFiles.py [-h] [-r REPOSITORY] [-c] [-g GIT] [-b BRANCH] -i IPNAME

    options:
      -h, --help            show this help message and exit
      -r REPOSITORY, --repository REPOSITORY
                            use this CryptoV4 repository
      -c, --clone           attempt to clone/update the repository
      -g GIT, --git GIT     fetch the CryptoV4 repository from this Git link
      -b BRANCH, --branch BRANCH
                            if cloning/updating, branch name for checkout
      -i IPNAME, --ipname IPNAME
                            CAM IP to use

* If specified, the repository must already exist.
* If specified, the script may clone the repository and use the specified branch.  An existing repository will be updated.
* The script uses the repository as the source location for the CryptoV4 module source files.

The script uses `fmpp` to generate the CryptoV4 module source files from their Freemarker Template Library (FTL) template files, and places them in the appropriate locations in each demonstration MPLAB® X project.

### Installing the Crypto Accelerator Module Library

The CAM library must be built before installation can proceed.

Open a Command prompt or Unix terminal and navigate to the `mchp_private` directory of the repository.<br>
Then, launch the `fetchLibrary.py` script with the desired arguments:

    usage: fetchLibrary.py [-h] [-r REPOSITORY] [-c] [-g GIT] [-b BRANCH] [-i INSTALL]

    options:
      -h, --help            show this help message and exit
      -r REPOSITORY, --repository REPOSITORY
                            use this pre-existing CAM library repository
      -c, --clone           attempt to clone/update the CAM library repository
      -g GIT, --git GIT     fetch the CAM library repository from this Git link
      -b BRANCH, --branch BRANCH
                            if cloning/updating, branch name for checkout
      -i INSTALL, --install INSTALL
                            install libraries from pre-existing library directory

The script will copy the CAM header and library files to the appropriate locations in each demonstration MPLAB® X project.

## Example Parameters
### Generating with Cloning the CryptoV4 Repository, With Branch 'dev'
    python generateFiles.py -c -b dev -i CAM_05346

### Generating Using an Existing Repository
    python generateFiles.py -r ..\..\crypto_v4 -i CAM_05346

### Fetching the CAM Library Files From an Existing Repository
    python fetchLibrary.py -i ..\..\crypto_cam_src

### Fetching the CAM Library Files with Cloning the crypto_cam_src Repository, With Branch 'dev'
    python fetchLibrary.py -c -b dev
