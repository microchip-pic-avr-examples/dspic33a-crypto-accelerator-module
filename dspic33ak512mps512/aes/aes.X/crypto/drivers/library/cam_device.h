  /**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    cam_device.h

  Summary:
    CAM Hardware Driver Device Support.

  Description:
    This header file defines supported devices by the CAM Hardware Driver.
**************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2025, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

#ifndef CAM_DEVICE_H
#define CAM_DEVICE_H

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/** @cond INTERNAL **/
#if defined(GENERIC_TARGET_CAM_05346) \
    || defined(__dsPIC33AK256MPS505__) || defined(__dsPIC33AK256MPS506__) \
    || defined(__dsPIC33AK256MPS508__) || defined(__dsPIC33AK256MPS510__) \
    || defined(__dsPIC33AK256MPS512__) /* dsPIC33AK256MPSXXX Families with CAN FD and Crypto Accelerator */ \
    || defined(__dsPIC33AK512MPS505__) || defined(__dsPIC33AK512MPS506__) \
    || defined(__dsPIC33AK512MPS508__) || defined(__dsPIC33AK512MPS510__) \
    || defined(__dsPIC33AK512MPS512__) /* dsPIC33AK512MPSXXX Families with CAN FD and Crypto Accelerator */ \
    || defined(__dsPIC33AK256MPS205__) || defined(__dsPIC33AK256MPS206__) \
    || defined(__dsPIC33AK256MPS208__) || defined(__dsPIC33AK256MPS210__) \
    || defined(__dsPIC33AK256MPS212__) /* dsPIC33AK256MPSXXX Families with Crypto Accelerator */ \
    || defined(__dsPIC33AK512MPS205__) || defined(__dsPIC33AK512MPS206__) \
    || defined(__dsPIC33AK512MPS208__) || defined(__dsPIC33AK512MPS210__) \
    || defined(__dsPIC33AK512MPS212__) /* dsPIC33AK512MPSXXX Families with Crypto Accelerator */
        #define CAM_HW_VERSION CAM_05346
#elif __has_include("cam_device_ea.h")
    #include "cam_device_ea.h"
#endif

#define JOIN2(A,B) A##B
#define JOIN(A,B) JOIN2(A,B)

#ifdef CAM_HW_VERSION
    #define MPROTO(x) JOIN(x, JOIN(_, CAM_HW_VERSION))
#else
    #define MPROTO(x) JOIN(x, JOIN(_, no_implementation_available))
    #error "This CAM library version does not support the selected device. Verify that the device has a CAM peripheral and you are using the latest CAM library release."
#endif
/** @endcond **/

#endif /* CAM_DEVICE_H */