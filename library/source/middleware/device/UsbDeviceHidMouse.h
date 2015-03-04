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
/** \file UsbDeviceHidMouse.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidMouseGroup USB Device Mouse Module description @endlink
 **
 ** History:
 **   - 2013-10-14  1.1  MSc  PDL support added 
 *****************************************************************************/

#ifndef __USBCLASS_H__
#define __USBCLASS_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "usb.h"

#ifndef USBDEVICEHIDMOUSE_ENABLED
    #define USBDEVICEHIDMOUSE_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDMOUSE_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidMouseGroup USB Device Middleware: HID Mouse
 **
 ** Provided functions of USB Device Mouse module:
 ** 
 ** - UsbDeviceHidMouse_Init()
 ** - UsbDeviceHidMouse_Scroll()
 ** - UsbDeviceHidMouse_RightClick()
 ** - UsbDeviceHidMouse_RightDoubleClick()
 ** - UsbDeviceHidMouse_RightDown()
 ** - UsbDeviceHidMouse_RightUp()
 ** - UsbDeviceHidMouse_LeftClick()
 ** - UsbDeviceHidMouse_LeftDoubleClick()
 ** - UsbDeviceHidMouse_LeftDown()
 ** - UsbDeviceHidMouse_LeftUp()
 ** - UsbDeviceHidMouse_Move()
 ** - UsbDeviceHidMouse_IsConnected()
 **
 ** Used to enumerate a mouse device
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

void UsbDeviceHidMouse_Init(stc_usbn_t* pstcUsb);


void UsbDeviceHidMouse_Scroll(int8_t i8Scroll);
void UsbDeviceHidMouse_RightClick(void);
void UsbDeviceHidMouse_RightDoubleClick(void);
void UsbDeviceHidMouse_RightDown(void);
void UsbDeviceHidMouse_RightUp(void);
void UsbDeviceHidMouse_LeftClick(void);
void UsbDeviceHidMouse_LeftDoubleClick(void);
void UsbDeviceHidMouse_LeftDown(void);
void UsbDeviceHidMouse_LeftUp(void);
void UsbDeviceHidMouse_Move(int8_t i8X, int8_t i8Y);
boolean_t UsbDeviceHidMouse_IsConnected(void);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidMouseGroup
#else
    #define UsbDeviceHidMouse_Init(x) ;
#endif
#endif /* __USBCLASS_H__*/
