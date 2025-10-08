# dspic33a-crypto-accelerator-module v1.2.0
### Release Highlights
- Support for AES-XTS Encrypt and Decrypt operations
- Added new AES and Hash demo 

### Features Added\Updated
- Crypto Algorithms added in this release:
	- AES
		- AES-XTS Encrypt and Decrypt
- Features added to CAM Hardware Driver:
	- Updates to AES driver: 
		- Context Safe DMA
		- Optimizations
	- The CAM Hardware Driver throws warnings when used with an unintended device
	- Library built to support 64-bit double and short-double types to switch per compiler setting for -fno-short-double

# dspic33a-crypto-accelerator-module v1.1.1
### Release Highlights
- Support for ECDH Shared Secret Generation
- Remove device.h and crypto_config.h files from all projects

### Features Added\Updated
- Crypto Accelerator Module Demos
- Crypto Algorithms added in this release:
	- ECDH
		- ECDH Shared Secret Generation

# dspic33a-crypto-accelerator-module v1.0.2
### Release Highlights
- Updated keywords, readmes and demo title

### Features Added\Updated
- None

# dspic33a-crypto-accelerator-module v1.0.0
### Release Highlights
- Initial Release - Refer to README.md for summary of the demos included

### Features Added\Updated
- Crypto Accelerator Module Demos
- Crypto Algorithms added in this release:
	- AES
		- AES-ECB
		- AES-CTR
		- AES-GCM
		- AES-CMAC
	- DSA
		- ECDSA Sign
		- ECDSA Verify
	- HASH
		- SHA-1
		- SHA-224
		- SHA-256
		- SHA-384
		- SHA-512
	- TRNG