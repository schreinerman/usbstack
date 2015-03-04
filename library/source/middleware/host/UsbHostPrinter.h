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
/** \file UsbHostPrinter.h
 **
 ** Part of FSEU USB Host Driver Module
 **
 **
 ** A detailed description is available at 
 ** @link UsbHostPrinterGroup USB Host Printer Module description @endlink
 **
 ** History:
 **   - 2014-09-03    1.0  MSc  First version 
 *****************************************************************************/

#ifndef __USBHOSTPRINTER_H__
#define __USBHOSTPRINTER_H__

#include "usb.h"

#ifndef USBHOSTPRINTER_ENABLED
    #define USBHOSTPRINTER_ENABLED ON
#endif
     
#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTPRINTER_ENABLED == ON))

#include "UsbHost.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbHostPrinterGroup USB Host Middleware: Printer
 **
 ** Provided functions of USB Host Printer module:
 ** 
 ** - UsbHostPrinter_RegisterDriver()
 ** - UsbHostPrinter_DataReceivedHandler()
 ** - UsbHostPrinter_DataSentHandler()
 ** - UsbHostPrinter_SetDataSentHandler()
 ** - UsbHostPrinter_SetDataReceivedHandler()
 ** - UsbHostPrinter_InitiateSending()
 ** - UsbHostPrinter_InitiateReceiving()
 ** - UsbHostPrinter_SendPolled()
 ** - UsbHostPrinter_ReceivePolled()
 ** - UsbHostPrinter_IsSent()
 ** - UsbHostPrinter_IsSending()
 ** - UsbHostPrinter_IsReceived()
 ** - UsbHostPrinter_DriverActive()
 **
 ** Following procedures are used for the UsbHostClassSupervisor:
 ** - UsbHostPrinter_InitHandler()
 ** - UsbHostPrinter_DeinitHandler()
 **
 **
 ******************************************************************************/
//@{

/**
 ******************************************************************************    
 ** \page usbhostprinter_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code   
 ** #include "usb.h"   
 ** #if (USBHOSTPRINTER_ENABLED == ON)
 **     #include "UsbHostPrinter.h"
 ** #endif  
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ****************************************************************************** 
 ** \page usbhostprinter_module_init Example: Initialization
 ** \brief Following initialization is required to register the driver at the 
 **        @link UsbHostGroup USB Host Module@endlink
 ** @code
 ** UsbConfig_UsbInit();   
 ** #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTPRINTER_ENABLED == ON))
 **     UsbHostPrinter_RegisterDriver();
 ** #endif 
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ******************************************************************************  
 ** \page usbhostprinter_example_transfer_polled Example: Sending data polled 
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTPRINTER_ENABLED == ON)
 **     #include "UsbHostPrinter.h"
 ** #endif  
 **
 ** const char_t pcHelloWorld[] = "Hello World!\r\n";   
 **   
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTPRINTER_ENABLED == ON))
 **         UsbHostPrinter_RegisterDriver();
 **     #endif 
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode(); 
 **         if (UsbHostPrinter_DriverActive()
 **         {
 **             UsbHostPrinter_SendPolled(pcHelloWorld,strlen(pcHelloWorld));
 **         }
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
     
/**
 ****************************************************************************** 
 ** \page usbhostprinter_example_transfer_irq Example: Sending data with IRQs 
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTPRINTER_ENABLED == ON)
 **     #include "UsbHostPrinter.h"
 ** #endif  
 **
 ** const char_t pcHelloWorld[] = "Hello World!\r\n";  
 **   
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTPRINTER_ENABLED == ON))
 **         UsbHostPrinter_RegisterDriver();
 **     #endif 
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode();
 **         if (UsbHostPrinter_DriverActive()
 **         {
 **             UsbHostPrinter_InitiateSending(pcHelloWorld,strlen(pcHelloWorld));
 **             while(UsbHostPrinter_IsSending());
 **         }
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
  
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/    

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

#define USBHOSTMOUSE_VERSION  0100
#define USBHOSTMOUSE_DATE     20140903
    
typedef void (*pfn_usbhostprinter_data_sent_t)(void); 
typedef void (*pfn_usbhostprinter_data_received_t)(uint8_t* pu8Buffer); 

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
    
void UsbHostPrinter_RegisterDriver(void);    
void UsbHostPrinter_DataReceivedHandler(stc_usbn_t* pstcUsb);
void UsbHostPrinter_DataSentHandler(stc_usbn_t* pstcUsb);
void UsbHostPrinter_SetDataSentHandler(pfn_usbhostprinter_data_sent_t pfnCallback);
void UsbHostPrinter_SetDataReceivedHandler(pfn_usbhostprinter_data_received_t pfnCallback);
void UsbHostPrinter_InitiateSending(uint8_t* pu8Buffer, uint32_t u32DataSize);
void UsbHostPrinter_InitiateReceiving(uint8_t* pu8Buffer, uint32_t u32DataSize);
void UsbHostPrinter_SendPolled(uint8_t* pu8Buffer, uint32_t u32DataSize);
uint8_t* UsbHostPrinter_ReceivePolled(uint8_t* pu8Buffer, uint32_t u32DataSize);
boolean_t UsbHostPrinter_IsSent(void);
boolean_t UsbHostPrinter_IsSending(void);
boolean_t UsbHostPrinter_IsReceived(void);
boolean_t UsbHostPrinter_DriverActive(void);

/* USED FOR USBCLASSSUPERVISOR */
boolean_t UsbHostPrinter_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t UsbHostPrinter_DeinitHandler(stc_usbn_t* pstcUsb);

#ifdef __cplusplus
}
#endif

//@} // UsbHostPrinterGroup

#endif
#endif
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
