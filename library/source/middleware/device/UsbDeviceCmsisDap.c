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
/** \file UsbDeviceCmsisDap.c
 **
 ** CMSIS-DAP HID Communication Backend 
 ** for protocol layer visit http://www.arm.com
 **
 ** History:
 **   - 2014-02-28    1.0  MSc  First public version
 *****************************************************************************/
#define __USBDEVICECMSISDAP_C__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "UsbDeviceCmsisDap.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICECMSISDAP_ENABLED == ON))

#include "DAP_config.h"
#include "DAP.h"

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
static void ConfCallback(stc_usbn_t* pstcUsb);
extern uint32_t DAP_ProcessVendorCommand(uint8_t *request, uint8_t *response);

/******************************************************************************/
/* Local variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

#if (64 != DAP_PACKET_SIZE)
#error "USB HID Output Report Size must match DAP Packet Size"
#endif
#if (64 != DAP_PACKET_SIZE)
#error "USB HID Input Report Size must match DAP Packet Size"
#endif

static volatile uint8_t  USB_RequestFlag;       // Request  Buffer Usage Flag
static volatile uint32_t USB_RequestIn;         // Request  Buffer In  Index
static volatile uint32_t USB_RequestOut;        // Request  Buffer Out Index

static volatile uint8_t  USB_ResponseIdle;      // Response Buffer Idle  Flag
static volatile uint8_t  USB_ResponseFlag;      // Response Buffer Usage Flag
static volatile uint32_t USB_ResponseIn;        // Response Buffer In  Index
static volatile uint32_t USB_ResponseOut;       // Response Buffer Out Index


//static const uint8_t u8MaxSupportedEndpoints = 2;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferOUT;

static          uint8_t  USB_Request [DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Request  Buffer
static          uint8_t  USB_Response[DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Response Buffer
uint8_t pu8BufferOUT[64];
uint8_t pu8BufferIN[64];

static stc_usbn_endpoint_data_t* pstcEndpointIN = NULL;
static stc_usbn_endpoint_data_t* pstcEndpointOUT = NULL;

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
        DAP_Setup();                          // DAP Setup
        bDataReceived = FALSE;
        bDataSent = TRUE;
        USB_RequestFlag   = 0;
        USB_RequestIn     = 0;
        USB_RequestOut    = 0;
        USB_ResponseIdle  = 1;
        USB_ResponseFlag  = 0;
        USB_ResponseIn    = 0;
        USB_ResponseOut   = 0;
        bReady = TRUE;
        LED_CONNECTED_OUT(1);                 // Turn on  Debugger Connected LED
        Delayms(50);                         // Wait for 500ms
        LED_RUNNING_OUT(1);                   // Turn on  Target Running LED
        Delayms(50);                         // Wait for 500ms
        LED_CONNECTED_OUT(0);                 // Turn off Debugger Connected LED
        Delayms(50);                         // Wait for 500ms
        LED_RUNNING_OUT(0);                   // Turn off Target Running LED
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
        
        if (pu8BufferOUT[0] == ID_DAP_TransferAbort) {
            DAP_TransferAbort = 1;
            return;
        }
        if (USB_RequestFlag && (USB_RequestIn == USB_RequestOut)) {
            return;  // Discard packet when buffer is full
        }
        // Store data into request packet buffer
        memcpy(USB_Request[USB_RequestIn], pstcEndpoint->pstcEndpointBuffer->pu8Buffer, 64);
        UsbDevice_ReceiveData(pstcUsb, pstcEndpoint, pstcEndpoint->pstcEndpointBuffer->pu8Buffer, 64, UsbIRQ);
        USB_RequestIn++;
        if (USB_RequestIn == DAP_PACKET_COUNT) {
            USB_RequestIn = 0;
        }
        if (USB_RequestIn == USB_RequestOut)
        {
            USB_RequestFlag = 1;
        }
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
void UsbDeviceCmsisDap_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    //uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    pstcEndpointIN = NULL;
    pstcEndpointOUT = NULL;
        
    USB_RequestFlag   = 0;
    USB_RequestIn     = 0;
    USB_RequestOut    = 0;
    USB_ResponseIdle  = 1;
    USB_ResponseFlag  = 0;
    USB_ResponseIn    = 0;
    USB_ResponseOut   = 0;
  
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
            
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferOUT;
                pstcEndpointOUT = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
                UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8BufferOUT, 64, UsbIRQ);
            }
            else
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            if ((pstcEndpointIN != NULL) && (pstcEndpointOUT != NULL))
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
boolean_t UsbDeviceCmsisDap_Send(uint8_t* pu8Buffer, en_usbsend_mode_t enMode)
{
    if ((bReady == FALSE) || (bDataSent == FALSE))
    {
        return FALSE;
    }
    bDataSent = FALSE;
    UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, 64, enMode);
    if ((USB_ResponseOut != USB_ResponseIn) || USB_ResponseFlag) {
        
        USB_ResponseOut++;
        if (USB_ResponseOut == DAP_PACKET_COUNT) {
          USB_ResponseOut = 0;
        }
        if (USB_ResponseOut == USB_ResponseIn) {
          USB_ResponseFlag = 0;
        }
        return TRUE;
    } else {
        USB_ResponseIdle = 1;
    }
    
    if (enMode == UsbPOLL)
    {
        bDataSent = TRUE;
    }
    return TRUE;
}

void UsbDeviceCmsisDap_Tick(void)
{
  uint32_t n;

  // Process pending requests
  if ((USB_RequestOut != USB_RequestIn) || USB_RequestFlag) {

    // Process DAP Command and prepare response
    DAP_ProcessCommand(USB_Request[USB_RequestOut], USB_Response[USB_ResponseIn]);
    DAP_ProcessVendorCommand(USB_Request[USB_RequestOut], USB_Response[USB_ResponseIn]);
    // Update request index and flag
    n = USB_RequestOut + 1;
    if (n == DAP_PACKET_COUNT) {
      n = 0;
    }
    USB_RequestOut = n;
    if (USB_RequestOut == USB_RequestIn) {
      USB_RequestFlag = 0;
    }

    if (USB_ResponseIdle) {
      // Request that data is send back to host
      USB_ResponseIdle = 0;
      #if USB_USES_DMA == ON
      UsbDeviceCmsisDap_Send(USB_Response[USB_ResponseOut],UsbDMA);
      #else
      UsbDeviceCmsisDap_Send(USB_Response[USB_ResponseOut],UsbIRQ);
      #endif
    } else {      
      // Update response index and flag
      n = USB_ResponseIn + 1;
      if (n == DAP_PACKET_COUNT) {
        n = 0;
      }
      USB_ResponseIn = n;
      if (USB_ResponseIn == USB_ResponseOut) {
        USB_ResponseFlag = 1;
      }
    }
  }
}

/**
 ******************************************************************************
 ** \brief Check if data transfer is still in process
 **
 ** \return                TRUE on sending     
 **
 ******************************************************************************/
boolean_t UsbDeviceCmsisDap_IsSending()
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
boolean_t UsbDeviceCmsisDap_DataSent()
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
uint32_t UsbDeviceCmsisDap_GetReceivedData(uint8_t* pu8OutBuffer)
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
