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
/** \file UsbDevicePrinter.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDevicePrinterGroup USB Device Printer Module description @endlink
 **
 ** History:
 **   - 2013-08-12    1.0  MSc  First version
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "UsbDevicePrinter.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEPRINTER_ENABLED == ON))


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


static uint32_t u32LastReceivedSize; // last received size
static stc_usbn_endpoint_buffer_t stcEndpointBufferOUT; //struct for custom buffer 
static uint8_t pu8IngoingBuffer[BUFFER_SIZE_ENDPOINTOUT]; // custom buffer 
static stc_usbn_endpoint_data_t* pstcEndpointOUT;

static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
stc_usbn_endpoint_buffer_t stcEpBufferIN; //struct for custom buffer
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_t* pstcUsbHandle = NULL;

static usbdeviceprinter_datareceived_func_ptr_t pstnReceivedData;
static usbdeviceprinter_datasent_func_ptr_t pstnSentData;


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void ClassSetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup);

static void UsbDevicePrinter_RxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);

static void UsbDevicePrinter_TxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);

 
/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** Is called to initialize this class (usally in UsbConfig.c)
 **
 ** \param pstcUsb      USB Handle

 *****************************************************************************/
void UsbDevicePrinter_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;

    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;       
    stcUsbClassConfig.u8InterfaceClass           = 0x07;        
    stcUsbClassConfig.u8InterfaceSubClass        = 0x01;                    
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x02;                    
    stcUsbClassConfig.pfnSetupRequestCallback    = ClassSetupRequest;                    
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                    
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                    
    stcUsbClassConfig.pfnConfCallback            = NULL;  
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);

    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                stcEndpointConfig.pfnRxTxCallback = UsbDevicePrinter_RxCallback;
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferOUT;
                pstcEndpointOUT = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);

                UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8IngoingBuffer, BUFFER_SIZE_ENDPOINTOUT, UsbIRQ);
            }
            else
            {
                stcEndpointConfig.pfnRxTxCallback = UsbDevicePrinter_TxCallback;
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            u8NumEndpoints++;
        }
    }
}

/**
 ******************************************************************************
 ** Set received callback
 **
 ** \param pstnCallback callback handle
 **
 *****************************************************************************/
void UsbDevicePrinter_SetReceivedCallback(usbdeviceprinter_datareceived_func_ptr_t pstnCallback)
{
    pstnReceivedData = pstnCallback;
}

/**
 ******************************************************************************
 ** Set sent callback
 **
 ** \param pstnCallback callback handle
 **
 *****************************************************************************/
void UsbDevicePrinter_SetSentCallback(usbdeviceprinter_datasent_func_ptr_t pstnCallback)
{
    pstnSentData = pstnCallback;
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
static void ClassSetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup)
{
    
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was received
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint

 *****************************************************************************/
static void UsbDevicePrinter_RxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    uint8_t* pu8Buffer = pstcEndpoint->pstcEndpointBuffer->pu8Buffer;
    uint32_t u32DataSize = pstcEndpoint->pstcEndpointBuffer->u32DataSize;
    u32LastReceivedSize = u32DataSize;
    DataReceivedFlags |= (1<<2); // setting data received flag
    if (pstnReceivedData != NULL)
    {
        pstnReceivedData(pu8Buffer,u32DataSize);
    }
    //Add your code here to process the received buffer
    UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8IngoingBuffer, BUFFER_SIZE_ENDPOINTOUT, UsbIRQ);
}


/**
 ******************************************************************************
 ** Is used to use received data from endpoint
 **
 ** \param ppu8Buffer pointer to buffer
 *****************************************************************************/
uint32_t UsbDevicePrinter_GetReceivedData(uint8_t** ppu8Buffer)
{
    if ((DataReceivedFlags & (1<<2)) == 0)
    {
        return 0; // nothing to receive
    }
    DataReceivedFlags -= (1<<2);
    *ppu8Buffer = pstcEndpointOUT->pstcEndpointBuffer->pu8Buffer;
    return u32LastReceivedSize;
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
boolean_t UsbDevicePrinter_SendData(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<1);
        
        UsbDevice_SendData(pstcUsb, pstcEndpointIN, pu8Buffer, u32DataSize, enMode);
        if (enMode == UsbPOLL)
        {
            DataSentFlags |= (1<<1);
        }
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was sent
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint
 *****************************************************************************/
static void UsbDevicePrinter_TxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    DataSentFlags |= (1<<1);
    if (pstnSentData != NULL)
    {
        pstnSentData();
    }
}
/**
 ******************************************************************************
 ** Is used to use get the sent status of endpoint
 **
 *****************************************************************************/
uint8_t UsbDevicePrinter_DataSent(void)
{
    if ((DataSentFlags & (1<<1)) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#endif
