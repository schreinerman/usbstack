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
/** \file UsbHostPrinter.c
 **
 ** Part of USB Host Driver Modul
 **
 ** A detailed description is available at 
 ** @link UsbHostPrinterGroup USB Host Printer Module description @endlink
 **
 ** History:
 **   - 2014-09-0s    1.0  MSc  First version 
 *****************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usbhostprinter.h"

#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTPRINTER_ENABLED == ON))
#include "usbhostclasssupervisor.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
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

static stc_host_endpoint_t *EndpointOUT;
static stc_host_endpoint_t *EndpointIN;
static uint8_t        u8EndpointIN, u8EndpointOUT;
static uint16_t       u16MaxPackageSizeIN, u16MaxPackageSizeOUT;
static volatile boolean_t bDataSent = FALSE;
static volatile boolean_t bDataSending = FALSE;
static volatile boolean_t bDataReceived = FALSE;
static stc_usbn_t* pstcUsbHandle = NULL;
volatile static uint8_t        u8Status = 0;
pfn_usbhostprinter_data_sent_t UsbHostPrinterDataSentHandler = NULL;
pfn_usbhostprinter_data_received_t UsbHostPrinterDataReceivedHandler = NULL;

static uint8_t pu8DataReceived[64];


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* START Driver Setup                                                        */
/*****************************************************************************/

static const stc_usbhostclasssupervisor_usbclassdriver_t stcDriverSetup = {
    UsbHostPrinter_InitHandler,   //pfnInitClassDriver           deinitialization callback of class driver
    UsbHostPrinter_DeinitHandler, //pfnDeinitClassDriver         initialization callback of class driver
    0,                            //u16IdVendor                  match vendor ID
    0,                            //u16IdProduct                 match product ID
    0,                            //u8MatchDeviceClass           match device class
    0,                            //u8MatchDeviceSubClass        match device subclass
    0,                            //u8MatchDeviceProtocol        match device protocol
    7,                            //u8MatchInterfaceClass        match interface class
    1,                            //u8MatchInterfaceSubClass     match interface subclass
    0,                            //u8MatchInterfaceProtocol     match interface protocoll
    0,                            //u8Reserved                   reserved for back compatible reasons since UsbClassSupervisor V2.2    
    NULL,                         //pfnDeviceConfigured          device configured callback of class driver
    NULL,                         //pfnPeriodicTaskMs            periodic ms callback for class driver
    NULL,                         //pfnIsActive                  driver is active callback for class driver
};

static stc_linked_list_item_t stcDriverSetupItem;

/**
 ******************************************************************************
 ** \brief Register driver at 
 ** @link UsbHostGroup USB Host Module @endlink
 **
 ******************************************************************************/

void UsbHostPrinter_RegisterDriver(void)
{
    UsbHostClassSupervisor_RegisterClassDriver((stc_usbhostclasssupervisor_usbclassdriver_t*)&stcDriverSetup,&stcDriverSetupItem);
}

/*****************************************************************************/
/* END Driver Setup                                                          */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief RX callback
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHostPrinter_DataReceivedHandler(stc_usbn_t* pstcUsb)
{
    bDataReceived = TRUE;
    if (UsbHostPrinterDataReceivedHandler != 0)
    {
        UsbHostPrinterDataReceivedHandler(pu8DataReceived);
        bDataReceived = FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief TX callback
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHostPrinter_DataSentHandler(stc_usbn_t* pstcUsb)
{
    bDataSent = TRUE;
    bDataSending = FALSE;
    if (UsbHostPrinterDataSentHandler != 0)
    {
        UsbHostPrinterDataSentHandler();
        bDataSent = FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief Register TX callback
 **
 ** \param pfnCallback Callback function
 **
 ******************************************************************************/
void UsbHostPrinter_SetDataSentHandler(pfn_usbhostprinter_data_sent_t pfnCallback)
{
    UsbHostPrinterDataSentHandler = pfnCallback;
}

/**
 ******************************************************************************
 ** \brief Register RX callback
 **
 ** \param pfnCallback Callback function
 **
 ******************************************************************************/
void UsbHostPrinter_SetDataReceivedHandler(pfn_usbhostprinter_data_received_t pfnCallback)
{
    UsbHostPrinterDataReceivedHandler = pfnCallback;
}

/**
 ******************************************************************************
 ** \brief Initiate data sending via IRQs
 **
 ** \param pu8Buffer Data
 **
 ** \param u32DataSize Data size
 **
 ******************************************************************************/
void UsbHostPrinter_InitiateSending(uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    bDataSent = FALSE;
    bDataSending = TRUE;
    UsbHost_TransferData(pstcUsbHandle,EndpointOUT,pu8Buffer, u32DataSize, UsbHostPrinter_DataSentHandler);
}

/**
 ******************************************************************************
 ** \brief Initiate data receiving via IRQs
 **
 ** \param pu8Buffer Data
 **
 ** \param u32DataSize Data size
 **
 ******************************************************************************/
void UsbHostPrinter_InitiateReceiving(uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    bDataReceived = FALSE;
    UsbHost_TransferData(pstcUsbHandle,EndpointIN,pu8Buffer, u32DataSize, UsbHostPrinter_DataReceivedHandler);
}

/**
 ******************************************************************************
 ** \brief Send data polled
 **
 ** \param pu8Buffer Data
 **
 ** \param u32DataSize Data size
 **
 ******************************************************************************/
void UsbHostPrinter_SendPolled(uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    bDataSent = FALSE;
    bDataSending = TRUE;
    UsbHost_TransferData(pstcUsbHandle,EndpointOUT,pu8Buffer, u32DataSize, UsbHostPrinter_DataSentHandler);
    while(bDataSent == FALSE);
}

/**
 ******************************************************************************
 ** \brief Receive data polled
 **
 ** \param pu8Buffer Data
 **
 ** \param u32DataSize Data size
 **
 ** \returns on success pointer to data, else NULL
 **
 ******************************************************************************/
uint8_t* UsbHostPrinter_ReceivePolled(uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    bDataReceived = FALSE;
    UsbHost_TransferData(pstcUsbHandle,EndpointIN,pu8Buffer, u32DataSize, UsbHostPrinter_DataReceivedHandler);
    while(bDataReceived == FALSE);
    return pu8DataReceived;
}

/**
 ******************************************************************************
 ** \brief Get status if data was sent
 **
 ** \returns TRUE, if data was sent
 **
 ******************************************************************************/
boolean_t UsbHostPrinter_IsSent(void)
{
    return bDataSent;
}

/**
 ******************************************************************************
 ** \brief Get status if data is still sending
 **
 ** \returns TRUE, if data is still sending
 **
 ******************************************************************************/
boolean_t UsbHostPrinter_IsSending(void)
{
    return bDataSending;
}

/**
 ******************************************************************************
 ** \brief Get status if data was received
 **
 ** \returns TRUE, if data was received
 **
 ******************************************************************************/
boolean_t UsbHostPrinter_IsReceived(void)
{
    return bDataReceived;
}


/**
 ******************************************************************************
 ** \brief Called from USB Host Class Supervisor to initialize this driver
 **
 ** \param pstcUsb USB handle
 **
 ** \param pu8Configuration configuration data
 **
 ** \param u32Length data length
 **
 ** \returns TRUE on success
 **
 ******************************************************************************/
boolean_t UsbHostPrinter_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length)
{
    uint8_t* pu8Buffer;
    uint8_t u8NumberOfInterfaces;
    uint8_t u8NumberOfEndpoints;
    uint8_t u8EndpointNumber;
    uint8_t u8InterfaceNumber;
    
    if (pstcUsbHandle != NULL)
    {
        return FALSE;
    }
    pstcUsbHandle = pstcUsb;
    bDataSent = FALSE;
    bDataSending = FALSE;
    bDataReceived = FALSE;
    u8NumberOfInterfaces=pu8Configuration[4];
    for(u8InterfaceNumber=0;u8InterfaceNumber < u8NumberOfInterfaces;u8InterfaceNumber++)
    {
        pu8Buffer = UsbHost_GetUsbInterfaceDescriptor(pu8Configuration,u32Length,u8InterfaceNumber);
        if (pu8Buffer == 0)
        {
            return FALSE;
        }
        u8NumberOfEndpoints = pu8Buffer[4];
        u8EndpointIN = 0;
        u8EndpointOUT = 0;
        if ((pu8Buffer[5] == 0x07) && (pu8Buffer[6] == 0x01) && (pu8Buffer[7] == 0x1)) //Class & SubClass & Protocol: Printers, Printers, Unidirectional
        {
            for(u8EndpointNumber=1;u8EndpointNumber<=u8NumberOfEndpoints;u8EndpointNumber++)
            {
                pu8Buffer = UsbHost_GetUsbEndpointDescriptor(pu8Configuration,u32Length,u8InterfaceNumber,u8EndpointNumber);
                if ((pu8Buffer[2] & USB_IN_DIRECTION) == 0)
                {
                    u8EndpointOUT = pu8Buffer[2];
                    u16MaxPackageSizeOUT = (uint16_t)(pu8Buffer[4] + (pu8Buffer[5] << 8));
                    break;
                }
            }
        } else if ((pu8Buffer[5] == 0x07) && (pu8Buffer[6] == 0x01) && (pu8Buffer[7] & 0x3)) //Class & SubClass & Protocol: Printers, Printers, Bidirectional
        {
            for(u8EndpointNumber=1;u8EndpointNumber<=u8NumberOfEndpoints;u8EndpointNumber++)
            {
                pu8Buffer = UsbHost_GetUsbEndpointDescriptor(pu8Configuration,u32Length,u8InterfaceNumber,u8EndpointNumber);
                if (pu8Buffer[2] & USB_IN_DIRECTION)
                {
                    u8EndpointIN = pu8Buffer[2];
                    u16MaxPackageSizeIN = (uint16_t)(pu8Buffer[4] + (pu8Buffer[5] << 8));
                }
                else
                {
                    u8EndpointOUT = pu8Buffer[2];
                    u16MaxPackageSizeOUT = (uint16_t)(pu8Buffer[4] + (pu8Buffer[5] << 8));
                }
                if ((u8EndpointIN != 0) && (u8EndpointOUT != 0))
                {
                    break;
                }
            }
        }
        
        if (u8EndpointOUT == 0)
        {

            return FALSE;
        }
        UsbHost_AddHostEndpoint(pstcUsbHandle,&EndpointOUT,u8EndpointOUT,u16MaxPackageSizeOUT,0,UsbHostPrinter_DataSentHandler);
        UsbHost_AddHostEndpoint(pstcUsbHandle,&EndpointIN,u8EndpointIN,u16MaxPackageSizeIN,0,UsbHostPrinter_DataReceivedHandler);
        EndpointIN->pu8Buffer = pu8DataReceived;
        EndpointIN->u32DataSize = 64; 
        break;
    }
    if ((u8EndpointIN == 0) || (u8EndpointOUT == 0))
    {
        return FALSE;
    }       
    return TRUE;               
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
boolean_t UsbHostPrinter_DeinitHandler(stc_usbn_t* pstcUsb)
{
    pstcUsbHandle = NULL;
    bDataSent = FALSE;
    bDataSending = FALSE;
    bDataReceived = FALSE;
    u8Status = 0;
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Returns if the driver is active
 **
 ** \returns TRUE if it is active
 **
 ******************************************************************************/
boolean_t UsbHostPrinter_DriverActive(void)
{
    return (pstcUsbHandle != NULL);
}

#endif

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
