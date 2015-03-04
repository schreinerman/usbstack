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
/*****************************************************************************/
/** \file UsbDeviceHidCom.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidComGroup USB Device HID Com Module description @endlink
 **
 ** - See README.TXT for project description
 ** - USB Human Interface Device Communication
 **
 ** History:
 **   - 2012-07-20    1.0  MSc  First version for FM3 USB library
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEHIDCOM_H__
#define __USBDEVICEHIDCOM_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "usb.h"

#ifndef USBDEVICEHIDCOM_ENABLED
    #define USBDEVICEHIDCOM_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDCOM_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidComGroup USB Device Middleware: HID Com - Data Communicaton
 **
 ** Provided functions of USB Device HID Communication module:
 ** 
 ** - UsbDeviceHidCom_Init()
 ** - UsbDeviceHidCom_GetReceivedData()
 ** - UsbDeviceHidCom_Send()
 ** - UsbDeviceHidCom_IsSending()
 ** - UsbDeviceHidCom_DataSent()
 **
 ** Used to send or receive 64 byte block data
 **
 ******************************************************************************/
//@{
    
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes ('static')                                      */
/******************************************************************************/

void UsbDeviceHidCom_Init(stc_usbn_t* pstcUsb);
uint32_t UsbDeviceHidCom_GetReceivedData(uint8_t* pu8OutBuffer);
boolean_t UsbDeviceHidCom_Send(uint8_t* pu8Buffer, en_usbsend_mode_t enMode);
boolean_t UsbDeviceHidCom_IsSending(void);
boolean_t UsbDeviceHidCom_DataSent(void);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidComGroup
#else
    #define UsbDeviceHidCom_Init(x) ;
#endif

#endif /*__USBDEVICEHIDCOM_H__ */
