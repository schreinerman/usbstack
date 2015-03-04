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
/** \file UsbDeviceLibUsb.c
 **
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceLibUsbGroup USB Device LibUSB Module description @endlink
 **
 ** History:
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "UsbDeviceLibUsb.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICELIBUSB_ENABLED == ON))

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

static uint8_t DataReceivedFlags = 0; //Status for received data
static uint8_t DataSentFlags = 0; //Status for sent data


static uint32_t u32LastReceivedSize1; // last received size EP1
static stc_usbn_endpoint_buffer_t stcEpBuffer1; //struct for custom buffer EP1
static uint8_t pu8BufferEp1[BUFFER_SIZE_ENDPOINT1]; // custom buffer EP1
static stc_usbn_endpoint_data_t* pstcEpOUT1;

static stc_usbn_endpoint_data_t* pstcEpIN2;
stc_usbn_endpoint_buffer_t stcEpBuffer2; //struct for custom buffer EP2v


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/



static void UsbDeviceLibUsb_RxCallbackEp1(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);

static void UsbDeviceLibUsb_TxCallbackEp2(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** Is called to initialize this class (usally in UsbConfig.c)
 **
 ** \param pstcUsb      USB Handle

 *****************************************************************************/
void UsbDeviceLibUsb_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;

    stcUsbClassConfig.u8InterfaceNumber          = 0;       
    stcUsbClassConfig.u8InterfaceClass           = 0xFF;        
    stcUsbClassConfig.u8InterfaceSubClass        = 0x00;                    
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x00;                    
    stcUsbClassConfig.pfnSetupRequestCallback    = ClassSetupRequest0;                    
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                    
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                    
    stcUsbClassConfig.pfnConfCallback            = NULL;  
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);

    stcEndpointConfig.u8EndpointAddress = 1;
    stcEndpointConfig.pfnRxTxCallback = UsbDeviceLibUsb_RxCallbackEp1;
    stcEndpointConfig.pstcEndpointBuffer = &stcEpBuffer1;
    pstcEpOUT1 = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
    UsbDevice_ReceiveData(pstcUsb, pstcEpOUT1, pu8BufferEp1, BUFFER_SIZE_ENDPOINT1, UsbIRQ);

    stcEndpointConfig.u8EndpointAddress = 0x80 | 2;
    stcEndpointConfig.pfnRxTxCallback = UsbDeviceLibUsb_TxCallbackEp2;
    stcEndpointConfig.pstcEndpointBuffer = &stcEpBuffer2;
    pstcEpIN2 = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);

}

/**
 ******************************************************************************
 ** Class Setup Request handling
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcSetup Received Setup
 **
 *****************************************************************************/
static void ClassSetupRequest0(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup)
{
    
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was received
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint

 *****************************************************************************/
static void UsbDeviceLibUsb_RxCallbackEp1(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    u32LastReceivedSize1 = pstcEndpoint->pstcEndpointBuffer->u32DataSize;;
    DataReceivedFlags |= (1<<1); // setting data received flag

    //Add your code here to process the received buffer
    UsbDevice_ReceiveData(pstcUsb, pstcEpOUT1, pu8BufferEp1, BUFFER_SIZE_ENDPOINT1, UsbIRQ);
}


/**
 ******************************************************************************
 ** Is used to use received data from endpoint
 **
 ** \param ppu8Buffer pointer to buffer
 *****************************************************************************/
uint32_t UsbDeviceLibUsb_GetReceivedDataEndpoint1(uint8_t** ppu8Buffer)
{
    if ((DataReceivedFlags & (1<<1)) == 0)
    {
        return 0; // nothing to receive
    }
    DataReceivedFlags -= (1<<1);
    *ppu8Buffer = pstcEpOUT1->pstcEndpointBuffer->pu8Buffer;
    return u32LastReceivedSize1;
}


/**
 ******************************************************************************
 ** Is called from main application to send data via endpoint
 **
 ** \param pstcUsb      USB Handle
 ** \param pu8Buffer Buffer to send
 ** \param u32DataSize Buffersize
 ** \param enMode    polled sending, interrupt sending or DMA
 **
 ** \return 1: if succesful, 0: if usb was not ready
 *****************************************************************************/
boolean_t UsbDeviceLibUsb_SendDataVia2(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<2);
		
	UsbDevice_SendData(pstcUsb, pstcEpIN2, pu8Buffer, u32DataSize, enMode);
        if (enMode == UsbPOLL)
        {
            DataSentFlags |= (1<<2);
        }
        return 1;
    }
    return 0;
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was sent
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint
 *****************************************************************************/
static void UsbDeviceLibUsb_TxCallbackEp2(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    DataSentFlags |= (1<<2);
}
/**
 ******************************************************************************
 ** Is used to use get the sent status of endpoint 2
 **
 *****************************************************************************/
uint8_t UsbDeviceLibUsb_DataSent2(void)
{
    if ((DataSentFlags & (1<<2)) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
