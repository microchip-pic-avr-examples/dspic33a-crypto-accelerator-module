# dspic33a-crypto-accelerator-module v1.4.0
### Release Highlights
- Added support for dsPIC33AK256MPS306 device across all demo projects
	- HASH demo updated to showcase SHA-3 APIs
	- Added new SHAKE demo project
- Consolidated crypto library files into a single shared `lib` folder at the repository root
- Consolidated application utility functions and macros into a single shared `utils` folder at the repository root
- Aligned demo API naming and formatting

### Features Added\Updated
- Crypto Algorithms added in this release:
	- HASH (dsPIC33AK256MPS306 only)
		- SHA3-224
		- SHA3-256
		- SHA3-384
		- SHA3-512
		- SHAKE128
		- SHAKE256
- Features added to the CAM Hardware Driver (v1.5.0):
	- Updates to HASH driver:
		- Support for SHA-3 and SHAKE

# dspic33a-crypto-accelerator-module v1.3.0
### Release Highlights
- AES demo updated to showcase AES-CBC, AES-CFB128, AES-OFB, and AES-CCM Encrypt/Decrypt APIs
- Added new MAC demo to showcase HMAC-SHA, AES-GMAC, and AES-CMAC APIs
- ECDSA demo updated to showcase non-blocking APIs
- Multi-step hash performance improvements

### Features Added\Updated
- Crypto Algorithms added in this release:
	- AES
		- AES-CBC
		- AES-CFB128
		- AES-OFB
		- AES-CCM
	- MAC
		- HMAC-SHA-1
		- HMAC-SHA2-224
		- HMAC-SHA2-256
		- HMAC-SHA2-384
		- HMAC-SHA2-512
		- AES-GMAC
- Features added to the CAM Hardware Driver:
	- Updates to ECDSA driver:
		- Support for non-blocking implementations
	- Updates to Hash driver:
		- Optimizations:
			- Multi-step: Average ~15% performance improvement
	- Updates to AES driver:
		- Support for AES-CBC, AES-CFB128, AES-OFB, and AES-CCM Encrypt/Decrypt operations

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
			- Single-step: Average ~80% RAM usage reduction and ~14% FLASH usage increase 
			- Multi-step: Average ~19% RAM usage reduction and ~14% FLASH usage increase
	- Updates to Hash driver:
		- Optimizations
			- Single-step: Average ~80% RAM usage reduction and ~8% FLASH usage increase
			- Multi-step: Average ~80% RAM usage reduction and ~8% FLASH usage increase
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