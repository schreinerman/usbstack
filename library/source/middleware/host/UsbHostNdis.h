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
/** \file usbhostndis.h
 **
 ** Part of Spansion USB Host Driver Module
 **
 ** A detailed description is available at 
 ** @link UsbHostNdisGroup USB Host Ndis Module description @endlink
 **
 ** History:
 **   - 2014-01-31    1.0  MSc  First version
 **   - 2014-09-03    1.1  MSc  Deinit routine fixed
 **                             Switched to dynamic driver registering
 *****************************************************************************/

#ifndef __USBHOSTNDIS_H__
#define __USBHOSTNDIS_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBHOSTNDIS_ENABLED
    #define USBHOSTNDIS_ENABLED ON
#endif
     
#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTNDIS_ENABLED == ON))

#include "UsbHost.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
    ** \defgroup UsbHostNdisGroup USB Host Middleware: NDIS
 **
 ** Provided functions of USB Host NDIS module:
 ** - UsbHostNdis_DriverActive()
 ** - UsbHostNdis_SetMaxRx()
 ** - UsbHostNdis_TxFrame()
 ** - UsbHostNdis_RxFrame()
 **  
 **
 ** Following procedures are used for the UsbHostClassSupervisor:
 ** - UsbHostNdis_InitHandler()
 ** - UsbHostNdis_DeinitHandler()
 ** - UsbHostNdis_IsActive()
 ** - UsbHostNdis_Configured()
 **
 ** Used to connect USB NDIS to the MCU
 **
 ******************************************************************************/
//@{

/**
 ******************************************************************************    
 ** \page usbhostndis_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code   
 ** #include "usb.h"   
 ** #if (USBHOSTNDIS_ENABLED == ON)
 **     #include "UsbHostNdis.h"
 ** #endif  
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ****************************************************************************** 
 ** \page usbhostndis_module_init Example: Initialization
 ** \brief Following initialization is required to register the driver at the 
 **        @link UsbHostGroup USB Host Module@endlink
 ** @code
 ** UsbConfig_UsbInit();   
 ** #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTNDIS_ENABLED == ON))
 **     UsbHostNdis_RegisterDriver();
 ** #endif 
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ******************************************************************************  
 ** \page usbhostndis_example_transfer_polled Example: Sending / Receiving frames
 ** @code 
 ** #include "usb.h"   
 ** #if (USBHOSTNDIS_ENABLED == ON)
 **     #include "UsbHostNdis.h"
 ** #endif  
 **
 ** boolean_t bInitialized = FALSE;
 **   
 ** int main()
 ** {
 **     UsbConfig_UsbInit();   
 **     #if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTNDIS_ENABLED == ON))
 **         UsbHostNdis_RegisterDriver();
 **     #endif 
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode(); 
 **         if (UsbHostNdis_DriverActive()
 **         {
 **             if (!bInitialized)
 **             {
 **                 //init IP stack with global stcUsbHostNdisMacAddress MAC address
 **                 //set UsbHostNdis_SetMaxRx(...) to your frame size
 **                 bInitialized = TRUE;
 **             }
 **             else
 **             {
 **                 //use UsbHostNdis_RxFrame and link it to IP stack RX frame
 **                 //use UsbHostNdis_TxFrame and link it to IP stack TX frame
 **             }
 **         }
 **         else
 **         {
 **              bInitialized = FALSE;
 **         }
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
     
     
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

#define USBHOSTNDIS_VERSION  0110
#define USBHOSTNDIS_DATE     20140903
    
#define USBHOSTNDIS_MAXIMUM_RECEIVE_BUFFER 400
  
#ifndef SET_ETHERNET_MULTICAST_FILTERS
#define SET_ETHERNET_MULTICAST_FILTERS 0x40
#endif
    
#ifndef SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER
#define SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER 0x41
#endif 

#ifndef GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER
#define GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER 0x42
#endif

#ifndef SET_ETHERNET_PACKET_FILTER
#define SET_ETHERNET_PACKET_FILTER 0x43
#endif
  
#ifndef GET_ETHERNET_STATISTIC
#define GET_ETHERNET_STATISTIC 0x44
#endif

   

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

typedef enum en_usbhostndis_type
{
    UsbHostNdisTypeUknown = 0,
    UsbHostNdisTypeEthernetCdc = 1,
    UsbHostNdisTypeRlt8196 = 2,
    UsbHostNdisTypeLan7500 = 3,
    UsbHostNdisTypeZteMf70 = 4,
} en_usbhostndis_type_t;

#ifndef __EN_CDC_NOTIFICATION_CODES__
#define __EN_CDC_NOTIFICATION_CODES__
typedef enum en_cdc_notification_codes
{
    NETWORK_CONNECTION = 0x00,
    RESPONSE_AVAILABLE = 0x01,
    AUX_JACK_HOOK_STATE = 0x08,
    RING_DETECT = 0x09,
    xSERIAL_STATE = 0x20,
    CALL_STATE_CHANGE = 0x28,
    LINE_STATE_CHANGE = 0x29,
    CONNECTION_SPEED_CHANGE = 0x2A
} en_cdc_notification_codes_t;
#endif /* __EN_CDC_NOTIFICATION_CODES__*/
   
#ifndef __EN_ETHERNET_STATISTICS_FEATURE_SELECTOR_CODES_T__
#define __EN_ETHERNET_STATISTICS_FEATURE_SELECTOR_CODES_T__
typedef enum en_ethernet_statistics_feature_selector_codes
{
    XMIT_OK = 0x01,
    RCV_OK = 0x02,
    XMIT_ERROR = 0x03,
    RCV_ERROR = 0x04,
    RCV_NO_BUFFER = 0x05,
    DIRECTED_BYTES_XMIT = 0x06,
    DIRECTED_FRAMES_XMIT = 0x07,
    MULTICAST_BYTES_XMIT = 0x08,
    MULTICAST_FRAMES_XMIT = 0x09,
    BROADCAST_BYTES_XMIT = 0x0A,
    BROADCAST_FRAMES_XMIT = 0x0B,
    DIRECTED_BYTES_RCV = 0x0C,
    DIRECTED_FRAMES_RCV = 0x0D,
    MULTICAST_BYTES_RCV = 0x0E,
    MULTICAST_FRAMES_RCV = 0x0F,
    BROADCAST_BYTES_RCV = 0x10,
    BROADCAST_FRAMES_RCV = 0x11,
    RCV_CRC_ERROR = 0x12,
    TRANSMIT_QUEUE_LENGTH = 0x13,
    RCV_ERROR_ALIGNMENT = 0x14,
    XMIT_ONE_COLLISION = 0x15,
    XMIT_MORE_COLLISIONS = 0x16,
    XMIT_DEFERRED = 0x17,
    XMIT_MAX_COLLISIONS = 0x18,
    RCV_OVERRUN = 0x19,
    XMIT_UNDERRUN = 0x1A,
    XMIT_HEARTBEAT_FAILURE = 0x1B,
    XMIT_TIMES_CRS_LOST = 0x1C,
    XMIT_LATE_COLLISIONS = 0x1D
} en_ethernet_statistics_feature_selector_codes_t;
#endif /* __EN_ETHERNET_STATISTICS_FEATURE_SELECTOR_CODES_T__ */

#ifndef __STC_ETHERNET_PACKET_FILTER_BITMAP_T__
#define __STC_ETHERNET_PACKET_FILTER_BITMAP_T__
typedef struct stc_ethernet_packet_filter_bitmap
{
    uint16_t PACKET_TYPE_PROMISCUOUS:1;
    uint16_t PACKET_TYPE_ALL_MULTICAST:1;
    uint16_t PACKET_TYPE_DIRECTED:1;
    uint16_t PACKET_TYPE_BROADCAST:1;
    uint16_t PACKET_TYPE_MULTICAST:1;
    uint16_t Reserved:11;
} stc_ethernet_packet_filter_bitmap_t;
#endif /* __STC_ETHERNET_PACKET_FILTER_BITMAP__ */

#ifndef __STC_ETHERNET_MULTICAST_ADDRESS_T__
#define __STC_ETHERNET_MULTICAST_ADDRESS_T__
typedef struct stc_ethernet_multicast_address
{
    uint8_t au8MulticastAddress[6];
} stc_ethernet_multicast_address_t;
#endif /* __STC_ETHERNET_MULTICAST_ADDRESS_T__*/

#ifndef __STC_ETHERNET_MAC_ADDRESS_T__
#define __STC_ETHERNET_MAC_ADDRESS_T__
typedef struct stc_ethernet_macadress {
  uint8_t au8Address[6];
} stc_ethernet_macadress_t;
#endif /* __STC_ETHERNET_MAC_ADDRESS_T__*/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/



extern stc_ethernet_macadress_t stcUsbHostNdisMacAddress;

void UsbHostNdis_RegisterDriver(void);
boolean_t UsbHostNdis_InitHandlerLan7500(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t UsbHostNdis_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t UsbHostNdis_InitHandlerZteMf70(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
void UsbHostNdis_SetMaxRx(uint32_t u32MaxRx);
void UsbHostNdis_Configured(stc_usbn_t* pstcUsb);
boolean_t UsbHostNdis_DeinitHandler(stc_usbn_t* pstcUsb);
boolean_t UsbHostNdis_DriverActive(void);
boolean_t UsbHostNdis_IsActive(stc_usbn_t* pstcUsb);

/*
en_result_t UsbHostNdis_SetEthernetMulticastFilters(uint16_t u16Interface, stc_ethernet_multicast_address_t astcMulticastAddresses[], uint16_t u16NumberOfFilters);
en_result_t UsbHostNdis_SetEthernetPowerManagementPatternFilter(uint16_t u16Interface, uint16_t u16FilterNumber, uint8_t* pu8Data, uint16_t u16Size);
en_result_t UsbHostNdis_GetEthernetPowerManagementPatternFilter(uint16_t u16Interface, uint16_t u16FilterNumber, uint16_t* pu16Data);
en_result_t UsbHostNdis_SetEthernetPacketFilter(uint16_t u16Interface, stc_ethernet_packet_filter_bitmap_t* pstcFilter);
en_result_t UsbHostNdis_GetEthernetStatistic(uint16_t u16Interface, en_ethernet_statistics_feature_selector_codes_t enFilterSelector, uint32_t * pu32Data);
*/
en_result_t UsbHostNdis_TxFrame(uint8_t* pu8Data, uint16_t u16DataSize);
en_result_t UsbHostNdis_SendDataInit(uint8_t* pu8Data, uint32_t u32DataSize);

//void UsbHostNdis_SetupReceivedCallback(usbhostndis_data_received_t ReceivedCallback);
uint16_t UsbHostNdis_RxFrame(uint8_t* pu8Data);




#ifdef __cplusplus
}
#endif

//@} // UsbHostNdisGroup

#endif /* (USE_USB_HOST == 1) */

#endif /*  __USBHOSTNDIS_H__ */
