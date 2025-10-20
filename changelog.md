# dspic33a-crypto-accelerator-module v1.2.0
### Release Highlights
- Support for AES-XTS Encrypt and Decrypt operations
- Added new AES and Hash demo 

### Features Added\Updated
- Crypto Algorithms added in this release:
	- AES
		- AES-XTS Encrypt and Decrypt
- Features added to the CAM Hardware Driver:
	- Updates to AES driver:
		- Context safety
		- Optimizations
			- Average ~80% RAM usage reduction and ~14% FLASH usage increase 
	- Updates to Hash driver:
		- Optimizations
			- Average ~80% RAM usage reduction and ~8% FLASH usage increase 
	- A build-time check has been added to verify usage of CAM library with supported device
	- Support for 64-bit double has been added and can be enabled per XC-DSC compiler setting
- Benchmarking improvements
	- Larger vectors for SHA and AES have been used to better demonstrate CAM performance

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