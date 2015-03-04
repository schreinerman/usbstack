/*******************************************************************************
* Copyright (C) 2013 Spansion LLC. All Rights Reserved. 
*
* This software is owned and published by: 
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with Spansion 
* components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion components. Spansion shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* SPANSION SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.  
*/
/******************************************************************************/
/** \file UsbDeviceHidJoystick.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidJoystickGroup USB Device Joystick Module description @endlink
 **
 ** History:
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEHIDJOYSTICK_H__
#define __USBDEVICEHIDJOYSTICK_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBDEVICEHIDJOYSTICK_ENABLED
    #define USBDEVICEHIDJOYSTICK_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDJOYSTICK_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidJoystickGroup USB Device Middleware: HID Joystick
 **
 ** Provided functions of USB Device Joystick module:
 ** 
 ** - UsbDeviceHidJoystick_Init()
 ** - UsbDeviceHidJoystick_ChangeThrottle()
 ** - UsbDeviceHidJoystick_ChangeX()
 ** - UsbDeviceHidJoystick_ChangeY()
 ** - UsbDeviceHidJoystick_ChangeXY()
 ** - UsbDeviceHidJoystick_ChangeButton1()
 ** - UsbDeviceHidJoystick_ChangeButton2()
 ** - UsbDeviceHidJoystick_ChangeButton3()
 ** - UsbDeviceHidJoystick_ChangeButton4()
 ** - UsbDeviceHidJoystick_ChangeHatUp()
 ** - UsbDeviceHidJoystick_ChangeHatRight()
 ** - UsbDeviceHidJoystick_ChangeHatDown()
 ** - UsbDeviceHidJoystick_ChangeHatLeft()
 ** - UsbDeviceHidJoystick_ChangeHatNoDirection()
 ** - UsbDeviceHidJoystick_IsConnected()
 **
 ** Used to enumerate a joystick device
 **
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/







/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/



/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbDeviceHidJoystick_Init(stc_usbn_t* pstcUsb);



/**
 ******************************************************************************
 ** Used to change the throttle value
 **
 ** \param iThrottle signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeThrottle(int8_t iThrottle);


/**
 ******************************************************************************
 ** Used to change the X value
 **
 ** \param iX signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeX(int8_t iX);


/**
 ******************************************************************************
 ** Used to change the Y value
 **
 ** \param iY signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeY(int8_t iY);


/**
 ******************************************************************************
 ** Used to change the X and Y value
 **
 ** \param iX signed byte value
 **
 ** \param iY signed byte value
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeXY(int8_t iX, int8_t iY);


/**
 ******************************************************************************
 ** Used to change the button 1 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton1(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the button 2 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton2(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the button 3 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton3(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the button 4 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton4(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the hat direction UP
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatUp(void);


/**
 ******************************************************************************
 ** Used to change the hat direction RIGHT
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatRight(void);


/**
 ******************************************************************************
 ** Used to change the hat direction DOWN
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatDown(void);


/**
 ******************************************************************************
 ** Used to change the hat direction LEFT
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatLeft(void);


/**
 ******************************************************************************
 ** Used to change the hat direction no direction
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatNoDirection(void);

/**
 ******************************************************************************
 ** Return connected state
 **
 ** \return TRUE if ready
 **
 *****************************************************************************/
boolean_t UsbDeviceHidJoystick_IsConnected(void);

#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidJoystickGroup
#else
    #define UsbDeviceHidJoystick_Init(x) ;
#endif
#endif /* __UsbDeviceHidJoystickCLASS_H__*/
