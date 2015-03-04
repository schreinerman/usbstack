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
/** \file UsbDeviceHidMouse.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidMouseGroup USB Device Mouse Module description @endlink
 **
 ** History:
 **   - 2013-10-14  1.1  MSc  PDL support added 
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "UsbDeviceHidMouse.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDMOUSE_ENABLED == ON))

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

static uint8_t DataSentFlags = 0; //Status for sent data
static uint8_t pu8MouseData[4];
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
static stc_usbn_t* pstcUsbHandle = NULL;
static boolean_t bReady = FALSE;






/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void ConfCallback(stc_usbn_t* pstcUsb);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/


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

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

void UsbDeviceHidMouse_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                          // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x03;                          // HID Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x01;                          // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x02;                          // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = NULL;                          // setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                          // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                          // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;                  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
        
    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                /* NOT USED */
            }
            else
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            u8NumEndpoints++;
        }
    }
    
}

/**
 ******************************************************************************
 ** Is called from main application to send data via endpoint
 **
 ** \param pu8Buffer Buffer to send
 ** \param u32DataSize Buffersize
 ** \param enMode    polled sending, interrupt sending or DMA
 **
 ** \return 1: if succesful, 0: if usb was not ready
 *****************************************************************************/
static boolean_t SendData(uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsbHandle) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<1);
		
	    UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, u32DataSize, enMode);
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
 ** Move Mouse
 **
 ** \param i8X Move in X direction 
 **
 ** \param i8Y Move in Y direction 
 *****************************************************************************/
void UsbDeviceHidMouse_Move(int8_t i8X, int8_t i8Y)
{
  pu8MouseData[1] = (int8_t)i8X;
  pu8MouseData[2] = (int8_t)i8Y;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Mouse Left Click (Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftClick(void)
{
  UsbDeviceHidMouse_LeftUp();
  UsbDeviceHidMouse_LeftDown();
  UsbDeviceHidMouse_LeftUp();
}


/**
 ******************************************************************************
 ** Mouse Left Click (Up-Down-Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftDoubleClick(void)
{
  UsbDeviceHidMouse_LeftUp();
  UsbDeviceHidMouse_LeftDown();
  UsbDeviceHidMouse_LeftUp();
  UsbDeviceHidMouse_LeftDown();
  UsbDeviceHidMouse_LeftUp();
}

/**
 ******************************************************************************
 ** Mouse Left Down
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftDown(void)
{
  pu8MouseData[0] |= 0x01;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}

/**
 ******************************************************************************
 ** Mouse Left Up
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftUp(void)
{
  pu8MouseData[0] &= ~0x01;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}

/**
 ******************************************************************************
 ** Mouse Right Click (Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightClick(void)
{
  UsbDeviceHidMouse_RightUp();
  UsbDeviceHidMouse_RightDown();
  UsbDeviceHidMouse_RightUp();
}

/**
 ******************************************************************************
 ** Mouse Right Click (Up-Down-Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightDoubleClick(void)
{
  UsbDeviceHidMouse_RightUp();
  UsbDeviceHidMouse_RightDown();
  UsbDeviceHidMouse_RightUp();
  UsbDeviceHidMouse_RightDown();
  UsbDeviceHidMouse_RightUp();
}

/**
 ******************************************************************************
 ** Mouse Right Down
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightDown(void)
{
  pu8MouseData[0] |= 0x02;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}

/**
 ******************************************************************************
 ** Mouse Right Up
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightUp(void)
{
  pu8MouseData[0] &= ~0x02;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);  
}

/**
 ******************************************************************************
 ** Scroll Wheel Movement
 **
 ** \param i8Scroll Move 
 **
 *****************************************************************************/
void UsbDeviceHidMouse_Scroll(int8_t i8Scroll)
{
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = (int8_t)i8Scroll;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);  
}

/**
 ******************************************************************************
 ** Return connected state
 **
 ** \return TRUE if ready
 **
 *****************************************************************************/
boolean_t UsbDeviceHidMouse_IsConnected(void)
{
    return bReady;
}
#endif
