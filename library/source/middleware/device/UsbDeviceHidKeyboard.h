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
/** \file UsbDeviceHidKeyboard.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidKeyboardGroup USB Device Keyboard Module description @endlink
 **
 ** History:
 **   - 2013-05-08  1.0  MSc  First Version
 **   - 2013-10-14  1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEHIDKEYBOARD_H__
#define __USBDEVICEHIDKEYBOARD_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBDEVICEHIDKEYBOARD_ENABLED
    #define USBDEVICEHIDKEYBOARD_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDKEYBOARD_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidKeyboardGroup USB Device Middleware: HID Keyboard
 **
 ** Provided functions of USB Device Keyboard module:
 ** 
 ** - UsbDeviceHidKeyboard_Init()
 ** - UsbDeviceHidKeyboard_SetModifierKey()
 ** - UsbDeviceHidKeyboard_ClearModifierKey()
 ** - UsbDeviceHidKeyboard_KeyPress()
 ** - UsbDeviceHidKeyboard_KeyDown()
 ** - UsbDeviceHidKeyboard_KeyUp()
 ** - UsbDeviceHidKeyboard_IsConnected()
 **
 ** Used to enumerate a keyboard device
 **
 ******************************************************************************/
//@{
    
extern const uint8_t u8AsciiToKeyCode[];

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/



#ifndef MODIFIERKEY_LEFT_CTRL
#define MODIFIERKEY_LEFT_CTRL   (1 << 0)
#endif
#ifndef MODIFIERKEY_LEFT_SHIFT
#define MODIFIERKEY_LEFT_SHIFT  (1 << 1)
#endif
#ifndef MODIFIERKEY_LEFT_ALT
#define MODIFIERKEY_LEFT_ALT    (1 << 2)
#endif
#ifndef MODIFIERKEY_LEFT_GUI
#define MODIFIERKEY_LEFT_GUI    (1 << 3)
#endif

#ifndef MODIFIERKEY_RIGHT_CTRL
#define MODIFIERKEY_RIGHT_CTRL   (1 << 4)
#endif
#ifndef MODIFIERKEY_RIGHT_SHIFT
#define MODIFIERKEY_RIGHT_SHIFT  (1 << 5)
#endif
#ifndef MODIFIERKEY_RIGHT_ALT
#define MODIFIERKEY_RIGHT_ALT    (1 << 6)
#endif
#ifndef MODIFIERKEY_RIGHT_GUI
#define MODIFIERKEY_RIGHT_GUI    (1 << 7)
#endif

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

typedef struct stc_usbdevice_keyboard
{
    uint8_t u8ModifierKeys;
    uint8_t u8Reserved;
    uint8_t u8KeyCode1;
    uint8_t u8KeyCode2;
    uint8_t u8KeyCode3;
    uint8_t u8KeyCode4;
    uint8_t u8KeyCode5;
    uint8_t u8KeyCode6;
} stc_usbdevice_keyboard_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbDeviceHidKeyboard_Init(stc_usbn_t* pstcUsb);
void UsbDeviceHidKeyboard_SetModifierKey(uint8_t u8ModifierKey);
void UsbDeviceHidKeyboard_ClearModifierKey(uint8_t u8ModifierKey);
void UsbDeviceHidKeyboard_KeyPress(uint8_t u8Index, uint8_t u8KeyCode);
void UsbDeviceHidKeyboard_KeyDown(uint8_t u8Index, uint8_t u8KeyCode);
void UsbDeviceHidKeyboard_KeyUp(uint8_t u8Index, uint8_t u8KeyCode);

boolean_t UsbDeviceHidKeyboard_IsConnected(void);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidKeyboardGroup
#else
    #define UsbDeviceHidKeyboard_Init(x) ;
#endif

#endif /* __UsbDeviceHidKeyboard__*/

