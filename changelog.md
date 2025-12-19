# dspic33a-crypto-accelerator-module v1.3.0
### Release Highlights
- Support for AES-CBC Encrypt and Decrypt operations
- Support for AES-CFB128 Encrypt and Decrypt operations
- Support for AES-OFB Encrypt and Decrypt operations
- Support for AES-CCM Encrypt and Decrypt operations
- Support for HMAC-SHA
- Support for AES-GMAC
- Added ECDSA non-blocking APIs
- Multi-step hash performance improvements

### Features Added\Updated
- Crypto Algorithms added in this release:
	- AES
		- AES-CBC Encrypt and Decrypt
		- AES-CFB128 Encrypt and Decrypt
		- AES-OFB Encrypt and Decrypt
		- AES-CCM Encrypt and Decrypt
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
		- Optimizations
			- Multi-step: Average ~15% performance improvement

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