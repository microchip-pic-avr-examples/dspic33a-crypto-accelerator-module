/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   PLIB Version 1.0.5
 *
 * @skipline  Device : dsPIC33AK256MPS306
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>


// Section: Device Pin Macros
/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA2 GPIO Pin which has a custom name of LED_RED to High
 * @pre      The RA2 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_RED_SetHigh()          (_LATA2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA2 GPIO Pin which has a custom name of LED_RED to Low
 * @pre      The RA2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_RED_SetLow()           (_LATA2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RA2 GPIO Pin which has a custom name of LED_RED
 * @pre      The RA2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_RED_Toggle()           (_LATA2 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RA2 GPIO Pin which has a custom name of LED_RED
 * @param    none
 * @return   none  
 */
#define LED_RED_GetValue()         _RA2

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA2 GPIO Pin which has a custom name of LED_RED as Input
 * @param    none
 * @return   none  
 */
#define LED_RED_SetDigitalInput()  (_TRISA2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA2 GPIO Pin which has a custom name of LED_RED as Output
 * @param    none
 * @return   none  
 */
#define LED_RED_SetDigitalOutput() (_TRISA2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA10 GPIO Pin which has a custom name of LED_GREEN to High
 * @pre      The RA10 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetHigh()          (_LATA10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA10 GPIO Pin which has a custom name of LED_GREEN to Low
 * @pre      The RA10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetLow()           (_LATA10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RA10 GPIO Pin which has a custom name of LED_GREEN
 * @pre      The RA10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_GREEN_Toggle()           (_LATA10 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RA10 GPIO Pin which has a custom name of LED_GREEN
 * @param    none
 * @return   none  
 */
#define LED_GREEN_GetValue()         _RA10

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA10 GPIO Pin which has a custom name of LED_GREEN as Input
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetDigitalInput()  (_TRISA10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA10 GPIO Pin which has a custom name of LED_GREEN as Output
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetDigitalOutput() (_TRISA10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
