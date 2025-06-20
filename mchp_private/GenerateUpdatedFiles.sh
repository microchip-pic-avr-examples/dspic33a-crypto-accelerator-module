#!/bin/bash

: '
This shell script clones the crypto_v4 repo and generates the updated source files for these demos.

Arguments:
    -b Branch/Commit(hash)/Tag name to checkout from crypto_v4 (Defaults to MH3-109828-pegasus-beta2-support)
    -i IP Name folder found in crypto_v4 (Defaults to CAM_05346)

Example Input: ./GenerateUpdatedFiles.sh -b MH3-109828-pegasus-beta2-support -i CAM_05346 (to clone/update the repository)
               ./GenerateUpdatedFiles.sh -b MH3-109828-pegasus-beta2-support -i CAM_05346 -r ../crypto_v4 (to use the repository at ../crypto_v4)
'

# Essential directories
BASE_DIR=`cd "$(dirname "$0")" && pwd` # Absolute path of crypto_apps_dspic33a folder
FMPP_DIR="$BASE_DIR"/fmpp
BUILD_DIR="$FMPP_DIR"/build
DEMO_BASE_DIR="$BASE_DIR"/../dspic33ak512mps512

#Arguments
CHECKOUT_VALUE=MH3-109828-pegasus-beta2-support
IP_NAME=CAM_05346
REPOSITORY_DIR="$BUILD_DIR"/crypto_v4
PREDEFINED_REPO=0

while getopts ":b:i:r:" opt; do
    case $opt in
        r)
            REPOSITORY_DIR=$OPTARG
            PREDEFINED_REPO=1
            ;;
        b)
            CHECKOUT_VALUE=$OPTARG
            ;;
        i)
            IP_NAME=$OPTARG
            ;;
        *)
            echo "Invalid option: -$OPTARG"
            ;;
    esac
done

#Split the IP_NAME to get the DOS number
IP_NAME_SPLIT=(${IP_NAME//"_"/ })
IP_VALUE=${IP_NAME_SPLIT[1]}

#Constants
INPUT_DIR="$BUILD_DIR"/input
OUTPUT_DIR="$BUILD_DIR"/output
IP_NAME_DIR="$INPUT_DIR/$IP_NAME"
COMMON_CRYPTO_DIR="$INPUT_DIR/common_crypto"
TEMPLATE_DIR="$INPUT_DIR/templates"
HASH_CRYPTO_DIR="$DEMO_BASE_DIR"/hash/sha.X/crypto
AES_CRYPTO_DIR="$DEMO_BASE_DIR"/aes/aes.X/crypto
DSA_CRYPTO_DIR="$DEMO_BASE_DIR"/dsa/ecdsa.X/crypto
ECDH_CRYPTO_DIR="$DEMO_BASE_DIR"/ecdh/ecdh.X/crypto
TRNG_CRYPTO_DIR="$DEMO_BASE_DIR"/trng/trng.X/crypto

verifyPreviousCommandExitStatus()
{
    # If command failed, then print parameter string and exit
    if [ $? -ne 0 ]; then
        printf "\n$1\n"
        exit 1
    fi
}

makeTargetDirectories()
{
    mkdir -p "$1/common_crypto"
    mkdir -p "$1/drivers/wrapper/src"
    mkdir -p "$1/drivers/library"
}

copyCommonFiles()
{
    cp -rf "$OUTPUT_DIR/templates/crypto_config.h" "$1/common_crypto"
    cp -rf "$OUTPUT_DIR/common_crypto/crypto_common.h" "$1/common_crypto"

    cp -rf "$OUTPUT_DIR/common_crypto/crypto_$2.h" "$1/common_crypto"
    cp -rf "$OUTPUT_DIR/common_crypto/src/crypto_$2.c" "$1/common_crypto/src"
}

copySpecificFiles()
{
    cp -rf "$OUTPUT_DIR/$IP_NAME/wrapper/crypto_"$2$3"_wrapper.h" "$1/drivers/wrapper"
    cp -rf "$OUTPUT_DIR/$IP_NAME/wrapper/src/crypto_"$2$3"_wrapper.c" "$1/drivers/wrapper/src"
}

mkdir -p $BUILD_DIR

fmpp --version
verifyPreviousCommandExitStatus "Please install prerequisite tools here: https://fmpp.sourceforge.net"

if [ x"$PREDEFINED_REPO" == "x0" ]; then
    if [ ! -d $$REPOSITORY_DIR ]; then
        git clone https://bitbucket.microchip.com/scm/mh3/crypto_v4.git $REPOSITORY_DIR
    fi

    cd $REPOSITORY_DIR
    git fetch
    git checkout $CHECKOUT_VALUE
    git pull
    verifyPreviousCommandExitStatus "Failed to checkout $CHECKOUT_VALUE. Try again with a valid checkout value."
    cd $BASE_DIR
fi

mkdir -p $INPUT_DIR
mkdir -p $IP_NAME_DIR
mkdir -p $COMMON_CRYPTO_DIR
mkdir -p $TEMPLATE_DIR
mkdir -p $OUTPUT_DIR

printf "\nCopying Files from $REPOSITORY_DIR\n"

if [ ! -d "$REPOSITORY_DIR/module_crypto/src/drivers/$IP_NAME" ]; then
    echo "Incorrect IP Name: $IP_NAME. No folder exists in crypto_v4 repo."
    exit 0
fi

cp -rf "$REPOSITORY_DIR/module_crypto/src/drivers/$IP_NAME"/* $IP_NAME_DIR
cp -rf "$REPOSITORY_DIR/module_crypto/src/common_crypto"/* $COMMON_CRYPTO_DIR
cp -rf "$REPOSITORY_DIR/module_crypto/templates"/* $TEMPLATE_DIR

cd $BUILD_DIR

printf "\nGenerating Files\n"

fmpp -C "$FMPP_DIR/config.fmpp" -S $INPUT_DIR -O $OUTPUT_DIR

verifyPreviousCommandExitStatus "FMPP failed to generate. Please check input files and config.fmpp and try again."

if [ -d $HASH_CRYPTO_DIR ]; then
    printf "Copying HASH files\n"
    makeTargetDirectories $HASH_CRYPTO_DIR
    copyCommonFiles $HASH_CRYPTO_DIR hash
    copySpecificFiles $HASH_CRYPTO_DIR hash_cam $IP_VALUE
    copySpecificFiles $HASH_CRYPTO_DIR cam $IP_VALUE
fi

if [ -d $AES_CRYPTO_DIR ]; then
    printf "Copying AES files\n"
    makeTargetDirectories $AES_CRYPTO_DIR
    copyCommonFiles $AES_CRYPTO_DIR sym_cipher
    copySpecificFiles $AES_CRYPTO_DIR sym_cam $IP_VALUE
    copyCommonFiles $AES_CRYPTO_DIR aead_cipher
    copySpecificFiles $AES_CRYPTO_DIR aead_cam $IP_VALUE
    copySpecificFiles $AES_CRYPTO_DIR cam $IP_VALUE
    copyCommonFiles $AES_CRYPTO_DIR mac_cipher
    copySpecificFiles $AES_CRYPTO_DIR mac_cam $IP_VALUE mac_cipher
fi

if [ -d $DSA_CRYPTO_DIR ]; then
    printf "Copying ECDSA files\n"
    makeTargetDirectories $DSA_CRYPTO_DIR
    copyCommonFiles $DSA_CRYPTO_DIR digsign
    copySpecificFiles $DSA_CRYPTO_DIR digisign_cam $IP_VALUE
    copySpecificFiles $DSA_CRYPTO_DIR cam $IP_VALUE
fi

if [ -d $ECDH_CRYPTO_DIR ]; then
    printf "Copying ECDH files\n"
    makeTargetDirectories $ECDH_CRYPTO_DIR
    copyCommonFiles $ECDH_CRYPTO_DIR kas
    copySpecificFiles $ECDH_CRYPTO_DIR kas_cam $IP_VALUE
    copySpecificFiles $ECDH_CRYPTO_DIR cam $IP_VALUE
fi

if [ -d $TRNG_CRYPTO_DIR ]; then
    printf "Copying TRNG files\n"
    makeTargetDirectories $TRNG_CRYPTO_DIR
    copyCommonFiles $TRNG_CRYPTO_DIR rng
    copySpecificFiles $TRNG_CRYPTO_DIR rng_cam $IP_VALUE
    copySpecificFiles $TRNG_CRYPTO_DIR cam $IP_VALUE
fi

sleep 2

exit 0