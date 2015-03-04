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
/** \file UsbDeviceHidJoystick.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidJoystickGroup USB Device Joystick Module description @endlink
 **
 ** History:
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "UsbDeviceHidJoystick.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDJOYSTICK_ENABLED == ON))

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

static boolean_t SendData(uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode);

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
static uint8_t pu8JoystickData[4] = {0,0,0,0};
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
static stc_usbn_t* pstcUsbHandle = NULL;
static boolean_t bReady = FALSE;
/*****************************************************************************/
/* Local function prototypes ('static')                                      */
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

void UsbDeviceHidJoystick_Init(stc_usbn_t* pstcUsb)
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
    stcUsbClassConfig.u8InterfaceSubClass        = 0x00;                          // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x00;                          // Custom Protocol
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
 ** Used to change the throttle value
 **
 ** \param iThrottle signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeThrottle(int8_t iThrottle)
{
    pu8JoystickData[0] = (uint8_t)iThrottle;
    SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the X value
 **
 ** \param iX signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeX(int8_t iX)
{
    pu8JoystickData[1] = (uint8_t)iX;
    SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the Y value
 **
 ** \param iY signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeY(int8_t iY)
{
    pu8JoystickData[2] = (uint8_t)iY;
    SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the X and Y value
 **
 ** \param iX signed byte value
 **
 ** \param iY signed byte value
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeXY(int8_t iX, int8_t iY)
{
    pu8JoystickData[1] = (uint8_t)iX;
    pu8JoystickData[2] = (uint8_t)iY;
    SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the button 1 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton1(boolean_t bButtonValue)
{
  if (bButtonValue)
  {
    pu8JoystickData[3] |= (1 << 4);
  }
  else
  {
    pu8JoystickData[3] &= ~(1 << 4);
  }
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the button 2 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton2(boolean_t bButtonValue)
{
  if (bButtonValue)
  {
    pu8JoystickData[3] |= (1 << 5);
  }
  else
  {
    pu8JoystickData[3] &= ~(1 << 5);
  }
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the button 3 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton3(boolean_t bButtonValue)
{
  if (bButtonValue)
  {
    pu8JoystickData[3] |= (1 << 6);
  }
  else
  {
    pu8JoystickData[3] &= ~(1 << 6);
  }
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the button 4 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton4(boolean_t bButtonValue)
{
  if (bButtonValue)
  {
    pu8JoystickData[3] |= (1 << 6);
  }
  else
  {
    pu8JoystickData[3] &= ~(1 << 6);
  }
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the hat direction UP
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatUp(void)
{
  pu8JoystickData[3] = (pu8JoystickData[3] & 0xF0) | 0;
 
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the hat direction RIGHT
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatRight(void)
{
  pu8JoystickData[3] = (pu8JoystickData[3] & 0xF0) | 1;
 
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the hat direction DOWN
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatDown(void)
{
  pu8JoystickData[3] = (pu8JoystickData[3] & 0xF0) | 2;
 
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the hat direction LEFT
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatLeft(void)
{
  pu8JoystickData[3] = (pu8JoystickData[3] & 0xF0) | 3;
 
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Used to change the hat direction no direction
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatNoDirection(void)
{
  pu8JoystickData[3] = (pu8JoystickData[3] & 0xF0) | 7;
 
  SendData((uint8_t*)pu8JoystickData,4,UsbPOLL);
}

/**
 ******************************************************************************
 ** Return connected state
 **
 ** \return TRUE if ready
 **
 *****************************************************************************/
boolean_t UsbDeviceHidJoystick_IsConnected(void)
{
    return bReady;
}
#endif
