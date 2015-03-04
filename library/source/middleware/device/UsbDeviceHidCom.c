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
/** \file UsbDeviceHidCom.c
 **
 ** CONST USB_ASSISTANT_OFF
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
#define __USBDEVICEHIDCOM_C__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "UsbDeviceHidCom.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDCOM_ENABLED == ON))
/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
static void ConfCallback(stc_usbn_t* pstcUsb);

/******************************************************************************/
/* Local variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

static const uint8_t u8MaxSupportedEndpoints = 2;
static stc_usbn_endpoint_data_t* ppstcUsedEndpoints[2];
static stc_usbn_endpoint_buffer_t pstcEndpointBuffers[2];
static uint8_t ppu8Buffers[2][64];
static const uint32_t pu32BufferSizes[2] = {64,64};
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_endpoint_data_t* pstcEndpointOUT;
static boolean_t bDataReceived = FALSE;
static boolean_t bDataSent = TRUE;
static boolean_t bReady = FALSE;
static stc_usbn_t* pstcUsbHandle = NULL;

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Procedures / Functions                                                     */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Configuration change callback, used to see if configuration is set
 **        or cleared
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return none     
 **
 ******************************************************************************/
static void ConfCallback(stc_usbn_t* pstcUsb)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        bReady = TRUE;
    } else
    {
        bReady = FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief Receive or transmitted data callback
 **
 ** \param pstcUsb         Pointer to USB instance
 **
 ** \param pstcEndpoint    Pointer to endpoint instance
 **
 ** \return none     
 **
 ******************************************************************************/
static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    volatile uint8_t i;
    volatile uint8_t u8EndpointIndex = 0xFF;
    
    if ((pstcEndpoint->u8EndpointAddress) & 0x80)
    {
        bDataSent = TRUE;
    } 
    else
    {
        bDataReceived = TRUE;
    }
}

/**
 ******************************************************************************
 ** \brief Initialisation callback
 **
 ** \param pstcUsb         Pointer to USB instance
 **
 ** \return none     
 **
 ******************************************************************************/
void UsbDeviceHidCom_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                    // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x03;                    // HID Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x00;                    // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x00;                    // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = NULL;                    // No setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                    // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                    // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
    
    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            stcEndpointConfig.pfnRxTxCallback = RxTxCallback;
            stcEndpointConfig.pstcEndpointBuffer = &(pstcEndpointBuffers[u8NumEndpoints]);
            ppstcUsedEndpoints[u8NumEndpoints] = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                pstcEndpointOUT = ppstcUsedEndpoints[u8NumEndpoints];
                UsbDevice_ReceiveData(pstcUsb, ppstcUsedEndpoints[u8NumEndpoints], ppu8Buffers[u8NumEndpoints], pu32BufferSizes[u8NumEndpoints], UsbIRQ);
            }
            else
            {
                pstcEndpointIN = ppstcUsedEndpoints[u8NumEndpoints];
            }
            u8NumEndpoints++;
            if (u8NumEndpoints > u8MaxSupportedEndpoints)
            {
                return;
            }
        }
    }
    
}

/**
 ******************************************************************************
 ** \brief Send data
 **
 ** \param pu8Buffer       Pointer to data buffer
 **
 ** \param enMode          Transfer mode (UsbPOLL, UsbDMA, UsbIRQ)
 **
 ** \return                TRUE on success     
 **
 ******************************************************************************/
boolean_t UsbDeviceHidCom_Send(uint8_t* pu8Buffer, en_usbsend_mode_t enMode)
{
    if ((bReady == FALSE) || (bDataSent == FALSE))
    {
        return FALSE;
    }
    bDataSent = FALSE;
    UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, 64, enMode);
    if (enMode == UsbPOLL)
    {
        bDataSent = TRUE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Check if data transfer is still in process
 **
 ** \return                TRUE on sending     
 **
 ******************************************************************************/
boolean_t UsbDeviceHidCom_IsSending()
{
    return !bDataSent;
}

/**
 ******************************************************************************
 ** \brief Check if data transfer was transmitted
 **
 ** \return                TRUE if data was sent     
 **
 ******************************************************************************/
boolean_t UsbDeviceHidCom_DataSent()
{
    return bDataSent;
}

/**
 ******************************************************************************
 ** \brief Send data
 **
 ** \param pu8OutBuffer    Pointer to data buffer where to write in data
 **
 ** \return                number of bytes read     
 **
 ******************************************************************************/
uint32_t UsbDeviceHidCom_GetReceivedData(uint8_t* pu8OutBuffer)
{
    if (bDataReceived == TRUE)
    {
         bDataReceived = FALSE;
         memcpy(pu8OutBuffer,pstcEndpointOUT->pstcEndpointBuffer->pu8Buffer,64);
         UsbDevice_ReceiveData(pstcUsbHandle, pstcEndpointOUT, pstcEndpointOUT->pstcEndpointBuffer->pu8Buffer, pstcEndpointOUT->pstcEndpointBuffer->u32BufferSize, UsbIRQ);
         return 64;
    }
    return 0;
}
#endif
