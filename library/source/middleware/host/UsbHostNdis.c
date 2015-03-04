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
/** \file usbhostndis.c
 **
 ** Part of Spansion USB Host Driver Module
 **
 ** A detailed description is available at 
 ** @link UsbHostNdisGroup USB Host NDIS Module description @endlink
 **
 ** History:
 **   - 2014-01-31    1.0  MSc  First version
 **   - 2014-09-03    1.1  MSc  Deinit routine fixed
 **                             Switched to dynamic driver registering
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#define __USBHOSTNDIS_C__

#include "UsbHostNdis.h"

#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTNDIS_ENABLED == ON))

#include "usbhostclasssupervisor.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/


/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/


/*****************************************************************************/
/* function */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/


/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/


static en_usbhostndis_type_t enSelectedDriver = UsbHostNdisTypeUknown;
static uint8_t au8ManagementElementNotifications[8];
static stc_host_endpoint_t *ControlEndpointIN;
static stc_host_endpoint_t *DataEndpointIN;
static stc_host_endpoint_t *DataEndpointOUT;

static volatile boolean_t bInitComplete = FALSE;
static stc_usbn_t* pstcUsbHandle = NULL;

static volatile boolean_t bDataReceived = FALSE;
static volatile boolean_t bDataReceiving = TRUE;
static volatile boolean_t bIsSending = FALSE;
static volatile boolean_t bSetupFinished = TRUE;
static volatile boolean_t bInterfaceSetFinished = FALSE;
static uint8_t u8SelectedInterface;
static uint8_t u8SelectedAlternateInterface;
static uint8_t u8MacAddressIndex;
static boolean_t bMacAddressReturned = FALSE;

static uint32_t u32MaxRxSize = USBHOSTNDIS_MAXIMUM_RECEIVE_BUFFER;
stc_ethernet_macadress_t stcUsbHostNdisMacAddress = {0,0,0,0,0,0};

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void ReturnedMacAddress(stc_usbn_t* pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataSize);
static uint8_t HexUnicodeToByte(uint8_t* pu8Data);
static void DataReceivedCallback(stc_usbn_t* pstcUsb);
static void ReceiveManagementElementNotifications(stc_usbn_t* pstcUsb);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/*****************************************************************************/
/* START Driver Setup                                                        */
/*****************************************************************************/

static const stc_usbhostclasssupervisor_usbclassdriver_t stcDriverSetup = {
    UsbHostNdis_InitHandler,   //pfnInitClassDriver           deinitialization callback of class driver
    UsbHostNdis_DeinitHandler, //pfnDeinitClassDriver         initialization callback of class driver
    0x0BDA,                    //u16IdVendor                  match vendor ID
    0x8196,                    //u16IdProduct                 match product ID
    0,                         //u8MatchDeviceClass           match device class
    0,                         //u8MatchDeviceSubClass        match device subclass
    0,                         //u8MatchDeviceProtocol        match device protocol
    0,                         //u8MatchInterfaceClass        match interface class
    0,                         //u8MatchInterfaceSubClass     match interface subclass
    0,                         //u8MatchInterfaceProtocol     match interface protocoll
    0,                         //u8Reserved                   reserved for back compatible reasons since UsbClassSupervisor V2.2    
    UsbHostNdis_Configured,    //pfnDeviceConfigured          device configured callback of class driver
    NULL,                      //pfnPeriodicTaskMs            periodic ms callback for class driver
    UsbHostNdis_IsActive,      //pfnIsActive                  driver is active callback for class driver
};

static stc_linked_list_item_t stcDriverSetupItem;

/**
 ******************************************************************************
 ** \brief Register driver at 
 ** @link UsbHostGroup USB Host Module @endlink
 **
 ******************************************************************************/

void UsbHostNdis_RegisterDriver(void)
{
    UsbHostClassSupervisor_RegisterClassDriver((stc_usbhostclasssupervisor_usbclassdriver_t*)&stcDriverSetup,&stcDriverSetupItem);
}

/*****************************************************************************/
/* END Driver Setup                                                          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Convert Hex to Byte
 **
 ** \param pu8Data Unicode 2 digit hex value
 **
 ** \returns Byte
 **
 ******************************************************************************/
static uint8_t HexUnicodeToByte(uint8_t* pu8Data)
{
    uint8_t u8Temp;
    if ((pu8Data[0] >= (uint8_t)'a') && (pu8Data[0] <= (uint8_t)'f'))
    {
        u8Temp = pu8Data[0] - (uint8_t)'a' + 10;
    }
    if ((pu8Data[0] >= (uint8_t)'A') && (pu8Data[0] <= (uint8_t)'F'))
    {
        u8Temp = pu8Data[0] - (uint8_t)'A' + 10;
    }
    if ((pu8Data[0] >= (uint8_t)'0') && (pu8Data[0] <= (uint8_t)'9'))
    {
        u8Temp = pu8Data[0] - (uint8_t)'0';
    }
    u8Temp = u8Temp * 16;
    if ((pu8Data[2] >= (uint8_t)'a') && (pu8Data[2] <= (uint8_t)'f'))
    {
        u8Temp += pu8Data[2] - (uint8_t)'a' + 10;
    }
    if ((pu8Data[2] >= (uint8_t)'A') && (pu8Data[2] <= (uint8_t)'F'))
    {
        u8Temp += pu8Data[2] - (uint8_t)'A' + 10;
    }
    if ((pu8Data[2] >= (uint8_t)'0') && (pu8Data[2] <= (uint8_t)'9'))
    {
        u8Temp += pu8Data[2] - (uint8_t)'0';
    }
    return u8Temp;
}

/**
 ******************************************************************************
 ** \brief Callback with returned MAC address
 **
 ** \param pstcUsb USB handle
 **
 ** \param pu8Buffer Data
 **
 ** \param u32DataSize Data size
 **
 ******************************************************************************/
static void ReturnedMacAddress(stc_usbn_t* pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    stcUsbHostNdisMacAddress.au8Address[5] = HexUnicodeToByte(&pu8Buffer[2]);
    stcUsbHostNdisMacAddress.au8Address[4] = HexUnicodeToByte(&pu8Buffer[6]);
    stcUsbHostNdisMacAddress.au8Address[3] = HexUnicodeToByte(&pu8Buffer[10]);
    stcUsbHostNdisMacAddress.au8Address[2] = HexUnicodeToByte(&pu8Buffer[14]);
    stcUsbHostNdisMacAddress.au8Address[1] = HexUnicodeToByte(&pu8Buffer[18]);
    stcUsbHostNdisMacAddress.au8Address[0] = HexUnicodeToByte(&pu8Buffer[22]);
    bMacAddressReturned = TRUE;
    bInitComplete = TRUE;
}

/**
 ******************************************************************************
 ** \brief Request Mac Address via Setup Request
 **
 ** \param u8IndexStringDescriptor Index of string descriptor containing MAC address
 **
 ******************************************************************************/
void UsbHostNdis_RequestMacAddress(uint8_t u8IndexStringDescriptor)
{
    stc_usb_request_t stcSetup = {0x80,0x06,0x0300,0x00,25};
    stcSetup.wValue |= u8IndexStringDescriptor;
    bMacAddressReturned = FALSE;
    UsbHost_SetupRequest(pstcUsbHandle,(stc_usb_request_t *)&stcSetup,ReturnedMacAddress);
}

/**
 ******************************************************************************
 ** \brief Request Mac Address via Setup Request (Polled)
 **
 ** \param u8IndexStringDescriptor Index of string descriptor containing MAC address
 **
 ******************************************************************************/
void UsbHostNdis_RequestMacAddressPolled(uint8_t u8IndexStringDescriptor)
{
    volatile uint32_t u32Timeout = 1000000;
    UsbHostNdis_RequestMacAddress(u8IndexStringDescriptor);
    while((u32Timeout > 0) && (bMacAddressReturned == FALSE)) u32Timeout--;
}

/**
 ******************************************************************************
 ** \brief Called from UsbHostClassSupervisor to initialize this module
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Configuration Configurationdata
 **
 ** \param u32Length Length of data
 **
 ** \return TRUE if driver was successfully loaded     
 **
 ******************************************************************************/
boolean_t UsbHostNdis_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length)
{
    uint32_t i = 0;
    uint8_t u8InterfaceType = 0;
    uint8_t u8ControlEpAddress = 0;
    uint32_t u32ControlEpSize = 0;
    uint8_t u8ControlEpInterval = 0;
    uint8_t u8DataEpAddressIn = 0;
    uint32_t u32DataSizeEpIn = 0;
    uint8_t u8DataEpAddressOut = 0;
    uint32_t u32DataSizeEpOut = 0;
    uint8_t u8InterfaceNumber = 0;
    uint8_t u8InterfaceAlternateSetting = 0;
    
    if (pstcUsbHandle != NULL)
    {
        return FALSE;
    }
    
    if (pu8Configuration[5] != 2) return FALSE;
    
    bInitComplete = FALSE;
    pstcUsbHandle = pstcUsb;
    enSelectedDriver = UsbHostNdisTypeRlt8196;
    ControlEndpointIN->pu8Buffer = au8ManagementElementNotifications;
    ControlEndpointIN->u32BufferSize = sizeof(au8ManagementElementNotifications);
    ControlEndpointIN->u32DataSize = 0;
    while((i < u32Length) && ((DataEndpointIN == NULL) || (DataEndpointOUT == NULL)))
    {
        switch(pu8Configuration[i + 1])
        {
            case 0x04:
              u8InterfaceNumber = pu8Configuration[i + 2];
              u8InterfaceAlternateSetting = pu8Configuration[i + 3];
              if ((pu8Configuration[i + 5] == 0x02) && (pu8Configuration[i + 6] == 0x06))
              {
                u8InterfaceType = 1;
              }
              if ((pu8Configuration[i + 5] == 0x0A) && (pu8Configuration[i + 6] == 0x00))
              {
                u8InterfaceType = 2;
              }
              break;
            case 0x24:
              if ((pu8Configuration[i + 2] == 0x0f) && (u8InterfaceType == 1))
              {
                  u8MacAddressIndex = pu8Configuration[i + 3];
              }
              break;
            case 0x05:
              if (u8InterfaceType == 1)
              {
                  if (pu8Configuration[i + 2] & 0x80)
                  {
                      u8ControlEpAddress = pu8Configuration[i + 2];
                      u32ControlEpSize = pu8Configuration[i + 4] + pu8Configuration[i + 5] * 256;
                      u8ControlEpInterval = pu8Configuration[i + 6];
                  }
              }
              else if (u8InterfaceType == 2)
              {
                  if (pu8Configuration[i + 2] & 0x80)
                  {
                     if (u8DataEpAddressIn == 0)
                     {
                        u8DataEpAddressIn = pu8Configuration[i + 2];
                        u32DataSizeEpIn = pu8Configuration[i + 4] + pu8Configuration[i + 5] * 256;
                     }
                  }
                  else
                  {
                     if (u8DataEpAddressOut == 0)
                     {
                         u8DataEpAddressOut = pu8Configuration[i + 2];
                         u32DataSizeEpOut = pu8Configuration[i + 4] + pu8Configuration[i + 5] * 256;
                     }
                  }
                  if ((u32DataSizeEpOut > 64) || (u32DataSizeEpIn > 64))
                  {
                      u8DataEpAddressIn = 0;
                      u8DataEpAddressOut = 0;
                  }
                  if ((u8DataEpAddressIn != 0) && (u8DataEpAddressOut != 0))
                  {
                      u8SelectedInterface = u8InterfaceNumber;
                      u8SelectedAlternateInterface = u8InterfaceAlternateSetting;
                  }
              }
              break;
        }
        i +=pu8Configuration[i];
    }
    if ((u8DataEpAddressIn != 0) && (u8DataEpAddressOut != 0))
    {
        UsbHost_AddHostEndpoint(pstcUsbHandle, &DataEndpointIN,u8DataEpAddressIn,u32DataSizeEpIn,0,NULL);
        UsbHost_AddHostEndpoint(pstcUsbHandle, &DataEndpointOUT,u8DataEpAddressOut,u32DataSizeEpOut,0,NULL);
    }
    if (u8ControlEpAddress != 0)
    {
        UsbHost_AddHostEndpoint(pstcUsbHandle, &ControlEndpointIN,u8ControlEpAddress,u32ControlEpSize,u8ControlEpInterval,ReceiveManagementElementNotifications);
    }
    UsbHost_TransferData(pstcUsbHandle,ControlEndpointIN,au8ManagementElementNotifications, sizeof(au8ManagementElementNotifications), ReceiveManagementElementNotifications);
    UsbHostNdis_RequestMacAddress(u8MacAddressIndex);
    
    //UsbHost_SetConfigurationDescriptor(pstcUsb,2);
    if ((u8DataEpAddressIn != 0) && (u8DataEpAddressOut != 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief Called from UsbHostClassSupervisor to initialize this module
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Configuration Configurationdata
 **
 ** \param u32Length Length of data
 **
 ** \return TRUE if driver was successfully loaded     
 **
 ******************************************************************************/
boolean_t UsbHostNdis_InitHandlerLan7500(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length)
{
    if (pstcUsbHandle != NULL)
    {
        return FALSE;
    }
    //if (pu8Configuration[5] != 2) return FALSE;
    bInitComplete = FALSE;
    pstcUsbHandle = pstcUsb;
    enSelectedDriver = UsbHostNdisTypeLan7500;
    ControlEndpointIN->pu8Buffer = au8ManagementElementNotifications;
    ControlEndpointIN->u32BufferSize = sizeof(au8ManagementElementNotifications);
    ControlEndpointIN->u32DataSize = 0;
    
    UsbHost_AddHostEndpoint(pstcUsbHandle, &DataEndpointIN,0x81,64,0,NULL);
    UsbHost_AddHostEndpoint(pstcUsbHandle, &DataEndpointOUT,0x02,64,0,NULL);
    UsbHost_AddHostEndpoint(pstcUsbHandle, &ControlEndpointIN,0x83,8,1,ReceiveManagementElementNotifications);
    
    UsbHost_TransferData(pstcUsbHandle,ControlEndpointIN,au8ManagementElementNotifications, sizeof(au8ManagementElementNotifications), ReceiveManagementElementNotifications);

    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Called from UsbHostClassSupervisor to initialize this module
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Configuration Configurationdata
 **
 ** \param u32Length Length of data
 **
 ** \return TRUE if driver was successfully loaded     
 **
 ******************************************************************************/
boolean_t UsbHostNdis_InitHandlerZteMf70(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length)
{
    if (pstcUsbHandle != NULL)
    {
        return FALSE;
    }
    //if (pu8Configuration[5] != 2) return FALSE;
    bInitComplete = FALSE;
    pstcUsbHandle = pstcUsb;
    enSelectedDriver = UsbHostNdisTypeZteMf70;
    ControlEndpointIN->pu8Buffer = au8ManagementElementNotifications;
    ControlEndpointIN->u32BufferSize = sizeof(au8ManagementElementNotifications);
    ControlEndpointIN->u32DataSize = 0;
    
    UsbHost_AddHostEndpoint(pstcUsbHandle, &DataEndpointIN,0x82,64,0,NULL);
    UsbHost_AddHostEndpoint(pstcUsbHandle, &DataEndpointOUT,0x01,64,0,NULL);
    UsbHost_AddHostEndpoint(pstcUsbHandle, &ControlEndpointIN,0x81,8,5,ReceiveManagementElementNotifications);
    
    UsbHost_TransferData(pstcUsbHandle,ControlEndpointIN,au8ManagementElementNotifications, sizeof(au8ManagementElementNotifications), ReceiveManagementElementNotifications);

    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Setup Rx Data Size
 **
 ** \param u32MaxRx Data size
 **
 ******************************************************************************/
void UsbHostNdis_SetMaxRx(uint32_t u32MaxRx)
{
    u32MaxRxSize = u32MaxRx;
}

/**
 ******************************************************************************
 ** \brief Interface set callback
 **
 ** \param pstcUsb    Usb Handle
 **
 ** \param pu8Buffer  Data Buffer
 **
 ** \param DataSize   Data Size 
 **
 ******************************************************************************/
static void InterfaceSet_Callback(stc_usbn_t* pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    bInterfaceSetFinished = TRUE;
    UsbHostNdis_RequestMacAddress(u8MacAddressIndex);
}

/**
 ******************************************************************************
 ** \brief Set network interface   
 **
 ******************************************************************************/
static void SetInterface(void)
{
    stc_usb_request_t stcSetup = {0x01,0x0B,0x0001,0x01,0x00};
    stcSetup.wValue = u8SelectedAlternateInterface;
    stcSetup.wIndex = u8SelectedInterface;
    UsbHost_SetupRequest(pstcUsbHandle,(stc_usb_request_t *)&stcSetup,InterfaceSet_Callback);
    bInterfaceSetFinished = FALSE;
}

/**
 ******************************************************************************
 ** \brief Called from USB Host Class Supervisor after device is configured
 **
 ** \param pstcUsb USB handle
 **
 ******************************************************************************/
void UsbHostNdis_Configured(stc_usbn_t* pstcUsb)
{
    if ((u8SelectedAlternateInterface != 0) || (u8SelectedInterface != 0))
    {
        SetInterface();
    } else
    {
        UsbHostNdis_RequestMacAddress(u8MacAddressIndex);
    }
}


/**
 ******************************************************************************
 ** \brief Called from USB Host Class Supervisor to deinitialize this driver
 **
 ** \param pstcUsb USB handle
 **
 ** \returns TRUE on success
 **
 ******************************************************************************/
boolean_t UsbHostNdis_DeinitHandler(stc_usbn_t* pstcUsb)
{
    if (pstcUsb == pstcUsbHandle)
    {
        pstcUsbHandle = NULL;
        bInitComplete = FALSE;
        bDataReceived = FALSE;
        bDataReceiving = TRUE;
        bIsSending = FALSE;
        bSetupFinished = TRUE;
        bMacAddressReturned = FALSE;
        u8SelectedInterface = 0;
        u8SelectedAlternateInterface = 0;
        u8MacAddressIndex = 0;
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** \brief Returns if the driver is active
 **
 ** \returns TRUE if it is active
 **
 ******************************************************************************/
boolean_t UsbHostNdis_DriverActive(void)
{
    return bInitComplete; //(pstcUsbHandle != NULL);
}

/**
 ******************************************************************************
 ** \brief Check if driver is active for a special USB port
 **        (Used with USB Host Class Supervisor)
 **
 ** \param pstcUsb USB handle
 **
 ** \returns TRUE if it is active
 **
 ******************************************************************************/
boolean_t UsbHostNdis_IsActive(stc_usbn_t* pstcUsb)
{
    return (pstcUsbHandle == pstcUsb);
}

/*
en_result_t UsbHostNdis_SetEthernetMulticastFilters(uint16_t u16Interface, stc_ethernet_multicast_address_t astcMulticastAddresses[], uint16_t u16NumberOfFilters)
{
    return Ok;
}

en_result_t UsbHostNdis_SetEthernetPowerManagementPatternFilter(uint16_t u16Interface, uint16_t u16FilterNumber, uint8_t* pu8Data, uint16_t u16Size)
{
    return Ok;
}

en_result_t UsbHostNdis_GetEthernetPowerManagementPatternFilter(uint16_t u16Interface, uint16_t u16FilterNumber, uint16_t* pu16Data)
{
    return Ok;
}

en_result_t UsbHostNdis_SetEthernetPacketFilter(uint16_t u16Interface, stc_ethernet_packet_filter_bitmap_t* pstcFilter)
{
    return Ok;
}

en_result_t UsbHostNdis_GetEthernetStatistic(uint16_t u16Interface, en_ethernet_statistics_feature_selector_codes_t enFilterSelector, uint32_t * pu32Data)
{
    return Ok;
}
*/

/**
 ******************************************************************************
 ** \brief Management Element Notifications Received Callback
 **
 ** \param pstcUsb USB handle
 **
 ******************************************************************************/
static void ReceiveManagementElementNotifications(stc_usbn_t* pstcUsb)
{
    switch(au8ManagementElementNotifications[1])
    {
        case NETWORK_CONNECTION:
            break;
        case RESPONSE_AVAILABLE:
            break;
        case CONNECTION_SPEED_CHANGE:
            break;
    }
    UsbHost_TransferData(pstcUsbHandle,ControlEndpointIN,au8ManagementElementNotifications, sizeof(au8ManagementElementNotifications), ReceiveManagementElementNotifications);
}

/**
 ******************************************************************************
 ** \brief Data sent callback
 **
 ** \param pstcUsb USB handle
 **
 ******************************************************************************/
static void DataSentCallback(stc_usbn_t* pstcUsbHandle)
{
    bIsSending = FALSE;
}

/**
 ******************************************************************************
 ** \brief Initiate data sending
 **
 ** \param pu8Data Data
 **
 ** \param u32DataSize Data size
 **
 ******************************************************************************/
en_result_t UsbHostNdis_SendDataInit(uint8_t* pu8Data, uint32_t u32DataSize)
{
    if (bIsSending)
    {
        return OperationInProgress;
    }
    if ((pstcUsbHandle == NULL) || (DataEndpointOUT == NULL) || (bInitComplete == FALSE))
    {
        return ErrorUninitialized;
    }
    bIsSending = TRUE;
    DataEndpointOUT->pu8Buffer = pu8Data;
    DataEndpointOUT->u32BufferSize = u32DataSize;
    DataEndpointOUT->u32DataSize = 0;
    UsbHost_TransferData(pstcUsbHandle,DataEndpointOUT,pu8Data, u32DataSize, DataSentCallback);
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Send frame
 **
 ** \param pu8Data Data
 **
 ** \param u32DataSize Data size
 **
 ******************************************************************************/
en_result_t UsbHostNdis_TxFrame(uint8_t* pu8Data, uint16_t u16DataSize)
{
    en_result_t res;
    volatile uint32_t u32Timeout;
    u32Timeout = 10000000;
    while((bIsSending == TRUE) && (u32Timeout > 0)) 
    {
      u32Timeout--;
     //UsbHost_Tick();
    }
    if (u32Timeout == 0) return ErrorNotReady;
    
    res = UsbHostNdis_SendDataInit(pu8Data, u16DataSize);

    return res;
}

/**
 ******************************************************************************
 ** \brief Receive frame
 **
 ** \param pu8Data Data
 **
 ** \returns Data size
 **
 ******************************************************************************/
uint16_t UsbHostNdis_RxFrame(uint8_t* pu8Data)
{
    static volatile uint32_t u32Timeout;
    if ((pstcUsbHandle == NULL) || (DataEndpointIN == NULL) || (bInitComplete == FALSE))
    {
        return 0;
    }
    
    if ((DataEndpointIN->u8Status & 7) > 0)
    {
        u32Timeout--;
        if (u32Timeout == 0)
        {
            
            DataEndpointIN->bAbortTransfer = TRUE;
            u32Timeout = 1000000;
            while((u32Timeout > 0) && (DataEndpointIN->bAbortTransfer == TRUE))
            {
              //UsbHost_Tick();
              u32Timeout--;
            }
              
            u32Timeout = 100000;
            DataEndpointIN->pu8Buffer = pu8Data;
            DataEndpointIN->u32BufferSize = u32MaxRxSize;
            DataEndpointIN->u32DataSize = 0;
            
            UsbHost_TransferData(pstcUsbHandle,DataEndpointIN,pu8Data, u32MaxRxSize, DataReceivedCallback);
        }
        return 0;
    }
    else
    {
        u32Timeout = 100000;
        if (bDataReceived == FALSE)
        {
            DataEndpointIN->pu8Buffer = pu8Data;
            DataEndpointIN->u32BufferSize = u32MaxRxSize;
            DataEndpointIN->u32DataSize = 0;
            
            UsbHost_TransferData(pstcUsbHandle,DataEndpointIN,pu8Data, u32MaxRxSize, DataReceivedCallback);
            return 0;
        }
        else
        {
            bDataReceived = FALSE;
            return DataEndpointIN->u32DataSize;
        }
    }
}

/**
 ******************************************************************************
 ** \brief Data received callback
 **
 ** \param pstcUsb USB handle
 **
 ******************************************************************************/
static void DataReceivedCallback(stc_usbn_t* pstcUsb)
{
    bDataReceived = TRUE;
    /*if (pfnDataReceived != NULL)
    {
        pfnDataReceived(&pu8ReceiveBuffer[2],Size);
        UsbHost_TransferData(pstcUsbHandle,DataEndpointIN,pu8ReceiveBuffer, 512, DataReceivedCallback);
    }*/
}
#endif

      