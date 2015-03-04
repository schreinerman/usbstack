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
/** \file UsbDevicePrinter.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 **
 ** A detailed description is available at 
 ** @link UsbDevicePrinterGroup USB Device Printer Module description @endlink
 **
 ** History:
 **   - 2013-08-12    1.0  MSc  First version
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEPRINTER_H__
#define __USBDEVICEPRINTER_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "usb.h"

#ifndef USBDEVICEPRINTER_ENABLED
    #define USBDEVICEPRINTER_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEPRINTER_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDevicePrinterGroup USB Device Middleware: Printer Class
 **
 ** Provided functions of USB Device Printer module:
 ** 
 ** - void UsbDevicePrinter_Init()
 ** - UsbDevicePrinter_GetReceivedData()
 ** - UsbDevicePrinter_SendData()
 ** - UsbDevicePrinter_DataSent(void)
 ** - UsbDevicePrinter_SetReceivedCallback()
 ** - UsbDevicePrinter_SetSentCallback()
 **
 ** Used to enumerate printe class
 **
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/



#define BUFFER_SIZE_ENDPOINTOUT 512



/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/


typedef void (*usbdeviceprinter_datareceived_func_ptr_t)(uint8_t* pu8DataBuffer, uint32_t u32DataLength);
typedef void (*usbdeviceprinter_datasent_func_ptr_t)(void);


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbDevicePrinter_Init(stc_usbn_t* pstcUsb);



uint32_t UsbDevicePrinter_GetReceivedData(uint8_t** ppu8Buffer);

boolean_t UsbDevicePrinter_SendData(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode);
uint8_t UsbDevicePrinter_DataSent(void);
void UsbDevicePrinter_SetReceivedCallback(usbdeviceprinter_datareceived_func_ptr_t pstnCallback);
void UsbDevicePrinter_SetSentCallback(usbdeviceprinter_datasent_func_ptr_t pstnCallback);
#ifdef __cplusplus
}
#endif

//@} // UsbDevicePrinterGroup

#else
    #define UsbDevicePrinter_Init(x) ;
#endif

#endif /* __USBCLASS_H__*/
