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
/** \file UsbHostHidMouse.h
 **
 ** Part of Spansion USB Host Driver Module
 **
 ** A detailed description is available at 
 ** @link UsbHostMouseGroup USB Host HID Mouse Module description @endlink
 **
 ** History:
 **   - 2011-03-30    1.0  MSc  First version 
 **   - 2011-08-24    1.1  MSc  Some fixes in X/Y calculation
 **   - 2012-06-05    1.2  MSc  New verison for use with new USB driver for FM3 L3
 **                             Rename HidMouse.h -> UsbHostHidMouse.h
 **   - 2013-10-14    1.3  MSc  PDL support added
 **   - 2014-07-30    1.5  MSc  Report descriptor parsing added
 **   - 2014-09-03    1.6  MSc  Deinit routine fixed
 **                             Switched to dynamic driver registering
 *****************************************************************************/


#ifndef __USBHOSTHIDMOUSE_H__
#define __USBHOSTHIDMOUSE_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBHOSTHIDMOUSE_ENABLED
    #define USBHOSTHIDMOUSE_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDMOUSE_ENABLED == ON))

#include "UsbHost.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbHostMouseGroup USB Host Middleware: HID Mouse
 **
 ** Provided functions of USB Host HID Mouse module:
 ** 
 ** - UsbHostHidMouse_SetCallback()
 ** - UsbHostHidMouse_RemoveCallback()
 ** - UsbHostHidMouse_GetCurrentPosition()
 ** - UsbHostHidMouse_SetCurrentPosition()
 ** - UsbHostHidMouse_GetCurrentScrollPosition()
 ** - UsbHostHidMouse_SetCurrentScrollPosition()
 ** - UsbHostHidMouse_GetButtons()
 ** - UsbHostHidMouse_Moved()
 ** - UsbHostHidMouse_DriverActive()
 **
 ** Following procedures are used for the UsbHostClassSupervisor:
 ** - UsbHostHidMouse_InitHandler()
 ** - UsbHostHidMouse_DeinitHandler()
 ** - UsbHostHidMouse_Configured()
 ** - UsbHostHidMouse_IsActive()   
 **
 ** Used to connect USB mouse to the MCU
 **
 ******************************************************************************/
//@{

/**
 ******************************************************************************    
 ** \page usbhostmouse_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code   
 ** #include "usb.h"   
 ** #if (USBHOSTHIDMOUSE_ENABLED == ON)
 **     #include "UsbHostHidMouse.h"
 ** #endif  
 ** @endcode
 **
 ******************************************************************************/
     
/**
 ****************************************************************************** 
 ** \page usbhostmouse_module_init Example: Initialization
 ** \brief Following initialization is required to register the driver at the 
 **        @link UsbHostGroup USB Host Module@endlink
 ** @code
 ** UsbConfig_UsbInit();   
 ** #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDMOUSE_ENABLED == ON))
 **     UsbHostHidMouse_RegisterDriver();
 ** #endif 
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ******************************************************************************    
 ** \page usbhostmouse_example_receive Example: Receiving data  
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTHIDMOUSE_ENABLED == ON)
 **     #include "UsbHostHidMouse.h"
 ** #endif  
 **   
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDMOUSE_ENABLED == ON))
 **         UsbHostHidMouse_RegisterDriver();
 **     #endif 
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode();
 **         if (UsbHostHidMouse_Moved())
 **         {
 **             printf("\r\nMouse Position %d,%d\r\n",
 **                     UsbHostHidMouse_GetCurrentPosition().u32X,    
 **                     UsbHostHidMouse_GetCurrentPosition().u32Y);
 **             printf("Scroll Position %d\r\n",
 **                     UsbHostHidMouse_GetCurrentScrollPosition());
 **             printf("Button Bitmap 0x%02X\r\n",
 **                     UsbHostHidMouse_GetButtons());
 **         }
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
     
/**
 ******************************************************************************    
 ** \page usbhostmouse_example_callbacks Example: Using callbacks
 ** @code  
 ** #include "usb.h"   
 ** #if (USBHOSTHIDMOUSE_ENABLED == ON)
 **     #include "UsbHostHidMouse.h"
 ** #endif  
 **   
 ** void MouseCallback(MouseEventType_t u8EventType, stc_mousedata_t* pstcInternalMouseData);
 ** {
 **    if (MOUSEEVENT_POSITIONX_CHANGED & u8EventType)
 **    {
 **        //your code...
 **    }
 **    if (MOUSEEVENT_POSITIONY_CHANGED & u8EventType)
 **    {
 **        //your code...
 **    }
 **    if (MOUSEEVENT_BUTTON_CHANGED & u8EventType)
 **    {
 **        //your code...
 **    }
 **    if (MOUSEEVENT_SCROLLING_CHANGED & u8EventType)
 **    {
 **        //your code...
 **    }
 ** }
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDMOUSE_ENABLED == ON))
 **         UsbHostHidMouse_RegisterDriver();
 **     #endif 
 **     UsbHostHidMouse_SetCallback(MouseCallback);
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

#define USBHOSTHIDMOUSE_VERSION  0160
#define USBHOSTHIDMOUSE_DATE     20140903

/**
 ******************************************************************************
 ** \brief Get current cursor position
 **
 ** \returns stc_point_t
 **
 ******************************************************************************/     
#define UsbHostHidMouse_GetCurrentPosition()    (stcUsbHostMouseData.stcPosition)
#define USBHOSTMOUSE_CONVERTREPORT(data,value) \
  if ((value.u8ReportID == 0) || (value.u8ReportID == data[0]))\
  {\
     *((uint32_t*)&value.i32Data) = *((uint32_t*)&data[((value.u32Position - 1) / 8)]);\
     *((uint32_t*)&value.i32Data) = *((uint32_t*)&value.i32Data) >> ((value.u32Position - 1) % 8);\
     *((uint32_t*)&value.i32Data) = *((uint32_t*)&value.i32Data) & (0xFFFFFFFF >> (32 - value.u8Size));\
     if ((*((uint32_t*)&value.i32Data) & (1 << (value.u8Size - 1))) && (value.i32Min < 0))\
     {\
         *((uint32_t*)&value.i32Data) |= (0xFFFFFFFF << (value.u8Size -1));\
     }\
  }
  
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef uint8_t buttons_t;

typedef struct stc_usbdevicemouse_internal_data_value
{
    int32_t i32Data;
    uint8_t u8Size;
    int32_t i32Min;
    int32_t i32Max;
    uint32_t u32Position;
    uint8_t u8ReportID;
    uint8_t u8Index;
    uint8_t u8UsageId;
} stc_usbdevicemouse_internal_data_value_t;

typedef struct stc_usbdevicemouse_internal_data
{
    uint8_t au8Data[10];
    stc_usbdevicemouse_internal_data_value_t stcButtons;
    stc_usbdevicemouse_internal_data_value_t stcX;
    stc_usbdevicemouse_internal_data_value_t stcY;
    stc_usbdevicemouse_internal_data_value_t stcScroll;
} stc_usbdevicemouse_internal_data_t;

typedef struct stc_point
{
    uint32_t u32X;
    uint32_t u32Y;
} stc_point_t;

typedef struct stc_pointdifference
{
    int8_t i8X;
    int8_t i8Y;
} stc_pointdifference_t;

typedef struct stc_mousedata
{
    stc_point_t   stcPosition;
    buttons_t tButtons;
    stc_pointdifference_t    stcDiffXY;
    uint32_t  u32ScrollPosition;
    int8_t    i8Scrolling;    
} stc_mousedata_t;


typedef uint8_t MouseEventType_t;

typedef void (*pfn_usbhostmouse_data_change_t)(MouseEventType_t u8EventType, stc_mousedata_t* pstcInternalMouseData);

#define MOUSEEVENT_POSITIONX_CHANGED 1
#define MOUSEEVENT_POSITIONY_CHANGED 2
#define MOUSEEVENT_BUTTON_CHANGED    4
#define MOUSEEVENT_SCROLLING_CHANGED  8
#define MOUSEEVENT_ANY_CHANGE        0xFF
/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

extern stc_mousedata_t stcUsbHostMouseData;


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbHostHidMouse_RegisterDriver(void);
boolean_t UsbHostHidMouse_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t UsbHostHidMouse_DeinitHandler(stc_usbn_t* pstcUsb);
void UsbHostHidMouse_Configured(stc_usbn_t* pstcUsb);
void UsbHostHidMouse_SetCallback(pfn_usbhostmouse_data_change_t pfnCallback);
void UsbHostHidMouse_RemoveCallback(void);

void UsbHostHidMouse_SetCurrentPosition(uint32_t u32X, uint32_t u32Y);
uint32_t UsbHostHidMouse_GetCurrentScrollPosition(void);
void UsbHostHidMouse_SetCurrentScrollPosition(uint32_t u32ScrollPosition);
buttons_t UsbHostHidMouse_GetButtons(void);
boolean_t UsbHostHidMouse_Moved(void);
boolean_t UsbHostHidMouse_DriverActive(void);
boolean_t UsbHostHidMouse_IsActive(stc_usbn_t* pstcUsb);

#ifdef __cplusplus
}
#endif

//@} // UsbHostMouseGroup

#endif /* ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDMOUSE_ENABLED == ON)) */
#endif /* __USBHOSTHIDMOUSE_H__ */
