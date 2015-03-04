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
/** \file UsbHostHidCom.h
 **
 ** Part of FSEU USB Host Driver Module for use with Fujitsu HID Com Device
 **
 **
 ** A detailed description is available at 
 ** @link UsbHostHidComGroup USB Host HID Com Module description @endlink
 **
 ** History:
 **   - 2012-07-18    1.0  MSc  First version 
 **   - 2013-10-14    1.1  MSc  PDL support added
 **   - 2014-09-03    1.2  MSc  Deinit routine fixed
 **                             Switched to dynamic driver registering
 *****************************************************************************/

#ifndef __USBHOSTHIDCOM_H__
#define __USBHOSTHIDCOM_H__

#include "usb.h"

#ifndef USBHOSTHIDCOM_ENABLED
    #define USBHOSTHIDCOM_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDCOM_ENABLED == ON))

#include "UsbHost.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbHostHidComGroup USB Host Middleware: HID Com - Data Communicaton
 **
 ** Provided functions of USB Host HID Communication module:
 ** 
 ** - UsbHostHidCom_RegisterDriver()
 ** - UsbHostHidCom_SetDataSentHandler()
 ** - UsbHostHidCom_SetDataReceivedHandler()
 ** - UsbHostHidCom_InitiateSending()
 ** - UsbHostHidCom_SendPolled()
 ** - UsbHostHidCom_ReceivePolled()
 ** - UsbHostHidCom_IsSent()
 ** - UsbHostHidCom_IsSending()
 ** - UsbHostHidCom_IsReceived()
 ** - UsbHostHidCom_DriverActive()
 **
 ** Following procedures are used for the UsbHostClassSupervisor:
 ** - UsbHostHidCom_InitHandler()
 ** - UsbHostHidCom_DeinitHandler()
 **
 ** Used to send or receive 64 byte block data
 **   
 ******************************************************************************/
//@{
    
/**
 ******************************************************************************    
 ** \page usbhosthidcom_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code   
 ** #include "usb.h"   
 ** #if (USBHOSTHIDCOM_ENABLED == ON)
 **     #include "UsbHostHidCom.h"
 ** #endif  
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ****************************************************************************** 
 ** \page usbhosthidcom_module_init Example: Initialization
 ** \brief Following initialization is required to register the driver at the 
 **        @link UsbHostGroup USB Host Module@endlink
 ** @code
 ** UsbConfig_UsbInit();   
 ** #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDCOM_ENABLED == ON))
 **     UsbHostHidCom_RegisterDriver();
 ** #endif 
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ******************************************************************************  
 ** \page usbhosthidcom_example_transfer_polled Example: Sending / receiving data polled 
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTHIDCOM_ENABLED == ON)
 **     #include "UsbHostHidCom.h"
 ** #endif  
 **
 ** uint8_t au8Data[64]; // data always is 64 bytes
 ** uint8_t pu8Data;
 **   
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDCOM_ENABLED == ON))
 **         UsbHostHidCom_RegisterDriver();
 **     #endif 
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode();
 **         pu8Data = UsbHostHidCom_ReceivePolled(void);
 **         if (pu8Data != NULL)
 **         {
 **             memcpy(au8Data,pu8Data,64);   
 **             UsbHostHidCom_SendPolled(au8Data);
 **         }
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
     
/**
 ****************************************************************************** 
 ** \page usbhosthidcom_example_transfer_irq Example: Sending / receiving data with IRQs 
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTHIDCOM_ENABLED == ON)
 **     #include "UsbHostHidCom.h"
 ** #endif  
 **
 ** uint8_t au8Data[64]; // data always is 64 bytes
 ** uint8_t pu8Data;
 **   
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDCOM_ENABLED == ON))
 **         UsbHostHidCom_RegisterDriver();
 **     #endif 
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode();
 **         if (UsbHostHidCom_IsReceived())
 **         {   
 **             pu8Data = UsbHostHidCom_ReceivePolled(void);
 **             if (pu8Data != NULL)
 **             {
 **                 memcpy(au8Data,pu8Data,64);   
 **                 UsbHostHidCom_InitiateSending(au8Data);
 **                 while(UsbHostHidCom_IsSending());
 **             }
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
     
/**
 ******************************************************************************    
 ** \page usbhosthidcom_example_transfer_callback Example: Using callbacks:  
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTHIDCOM_ENABLED == ON)
 **     #include "UsbHostHidCom.h"
 ** #endif  
 **   
 ** void CallbackRx(uint8_t* pu8Data)
 ** {
 **     //data was received, process data here
 ** }
 **
 ** void CallbackTx(void)
 ** {
 **     //data was sent
 ** }
 **
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDCOM_ENABLED == ON))
 **         UsbHostHidCom_RegisterDriver();
 **     #endif 
 **     UsbHostHidCom_SetDataReceivedHandler(CallbackRx);
 **     UsbHostHidCom_SetDataSentHandler(CallbackTx);
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode();
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/


    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/    

#define USBHOSTHIDCOM_VERSION  0120
#define USBHOSTHIDCOM_DATE     20140903
     
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
    
typedef void (*pfn_usbhosthidcom_data_sent_t)(void); 
typedef void (*pfn_usbhosthidcom_data_received_t)(uint8_t* pu8Buffer); 

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbHostHidCom_RegisterDriver(void);  
void UsbHostHidCom_SetDataSentHandler(pfn_usbhosthidcom_data_sent_t pfnDataSent);
void UsbHostHidCom_SetDataReceivedHandler(pfn_usbhosthidcom_data_received_t pfnDataReceived);
void UsbHostHidCom_InitiateSending(uint8_t* pu8Buffer);
void UsbHostHidCom_SendPolled(uint8_t* pu8Buffer);
uint8_t* UsbHostHidCom_ReceivePolled(void);
boolean_t UsbHostHidCom_IsSent(void);
boolean_t UsbHostHidCom_IsSending(void);
boolean_t UsbHostHidCom_IsReceived(void);
boolean_t UsbHostHidCom_DriverActive(void);

/* USED FOR USBCLASSSUPERVISOR */
boolean_t UsbHostHidCom_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t UsbHostHidCom_DeinitHandler(stc_usbn_t* pstcUsb);

#ifdef __cplusplus
}
#endif

//@} // UsbHostHidComGroup

#endif
#endif

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
