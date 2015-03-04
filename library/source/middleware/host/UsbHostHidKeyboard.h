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
/** \file UsbHostHidKeyboard.h
 **
 ** Part of Spansion USB Host Driver Module
 **
 ** A detailed description is available at 
 ** @link UsbHostKeyboardGroup USB Host HID Keyboard Module description @endlink
 **
 ** History:
 **   - 2010-12-13    1.0  MSc  First version  (works with 16FX)
 **   - 2010-01-05    1.1  MSc  API Updates
 **   - 2011-03-30    1.2  MSc  Public Release
 **   - 2011-08-24    1.3  MSc  Bug fixes
 **   - 2012-06-05    1.4  MSc  New verison for use with new USB driver for FM3 L3
 **                             Rename HidKeyboard.h -> UsbHostHidKeyboard.h
 **   - 2013-10-14    1.5  MSc  PDL support added
 **   - 2014-09-03    1.6  MSc  Deinit routine fixed
 **                             Switched to dynamic driver registering
 *****************************************************************************/

#ifndef __USBHOSTHIDKEYBOARD_H__
#define __USBHOSTHIDKEYBOARD_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBHOSTHIDKEYBOARD_ENABLED
    #define USBHOSTHIDKEYBOARD_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDKEYBOARD_ENABLED == ON))

#include "UsbHost.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbHostKeyboardGroup USB Host Middleware: HID Keyboard
 **
 ** Provided functions of USB Host HID Keyboard module:
 ** 
 ** - UsbHostHidKeyboard_RegisterDriver()
 ** - UsbHostHidKeyboard_DriverActive()
 ** - UsbHostHidKeyboard_SetCallback()
 ** - UsbHostHidKeyboard_RemoveCallback()
 ** - UsbHostHidKeyboard_GetCh()
 ** - HidKeyboard_SetLeds()
 **
 ** Following procedures are used for the UsbHostClassSupervisor:
 ** - UsbHostHidKeyboard_InitHandler()
 ** - UsbHostHidKeyboard_DeinitHandler()
 **
 ** Used to connect USB keyboards to the MCU
 ******************************************************************************/
//@{

/**
 ******************************************************************************    
 ** \page usbhostkeyboard_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code   
 ** #include "usb.h"   
 ** #if (USBHOSTHIDKEYBOARD_ENABLED == ON)
 **     #include "UsbHostHidKeyboard.h"
 ** #endif  
 ** @endcode
 **
 ******************************************************************************/
     
/**
 ****************************************************************************** 
 ** \page usbhostkeyboard_module_init Example: Initialization
 ** \brief Following initialization is required to register the driver at the 
 **        @link UsbHostGroup USB Host Module@endlink
 ** @code
 ** UsbConfig_UsbInit();   
 ** #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDKEYBOARD_ENABLED == ON))
 **     UsbHostHidKeyboard_RegisterDriver();
 ** #endif 
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ******************************************************************************    
    ** \page usbhostkeyboard_example_receive Example: Receiving data  
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTHIDKEYBOARD_ENABLED == ON)
 **     #include "UsbHostHidKeyboard.h"
 ** #endif  
 **
 ** char_t c;
 **   
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDKEYBOARD_ENABLED == ON))
 **         UsbHostHidKeyboard_RegisterDriver();
 **     #endif 
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode();
 **         c = UsbHostHidKeyboard_GetCh(1000); //Timeout in ticks
 **         // if c == 0, no data was received, if c != 0, c = received char
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
     
/**
 ******************************************************************************    
 ** \page usbhostkeyboard_example_callbacks Example: Using callbacks
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTHIDKEYBOARD_ENABLED == ON)
 **     #include "UsbHostHidKeyboard.h"
 ** #endif  
 **   
 ** void KeyboardCallback(stc_usbhostkeyboard_data_t *pstcData)
 ** {
 **    //data received stored in pstcData
 ** }
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDKEYBOARD_ENABLED == ON))
 **         UsbHostHidKeyboard_RegisterDriver();
 **     #endif 
 **     UsbHostHidKeyboard_SetCallback(KeyboardCallback);
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode();
 **     }
 ** }
 ** @endcode
 ******************************************************************************/

    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
     
#define USBHOSTHIDKEYBOARD_VERSION  0160
#define USBHOSTHIDKEYBOARD_DATE     20140903
     
#define KEYCODE_ASCII(keycode)  u8Keycodes[keycode]

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

typedef struct stc_usbhostkeyboard_data KeyboardData_t;

typedef struct stc_usbhostkeyboard_data
{
    uint8_t u8ModifierKeys;
    uint8_t u8Reserved;
    uint8_t u8KeyCode1;
    uint8_t u8KeyCode2;
    uint8_t u8KeyCode3;
    uint8_t u8KeyCode4;
    uint8_t u8KeyCode5;
    uint8_t u8KeyCode6;
} stc_usbhostkeyboard_data_t;

typedef void (*pfn_usbhostkeyboard_datareceived_t)(stc_usbhostkeyboard_data_t *pstcData);

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

#ifndef __HIDKEYBOARD_C__
    extern const uint8_t u8Keycodes[];
#endif

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
void UsbHostHidKeyboard_RegisterDriver(void);
boolean_t UsbHostHidKeyboard_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t UsbHostHidKeyboard_DeinitHandler(stc_usbn_t* pstcUsb);
boolean_t UsbHostHidKeyboard_DriverActive(void);
void UsbHostHidKeyboard_SetCallback(pfn_usbhostkeyboard_datareceived_t pfnCallback);
void UsbHostHidKeyboard_RemoveCallback(void);
char_t UsbHostHidKeyboard_GetCh(volatile uint32_t u32TimeOut);
void HidKeyboard_SetLeds(uint8_t u8Data);

#ifdef __cplusplus
}
#endif

//@} // UsbHostKeyboardGroup

#endif /* (USE_USB_HOST == 1) */
#endif /* __HidKeyboard_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
