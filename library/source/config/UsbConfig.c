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
/************************************************************************/
/** \file usbconfig.c
 **
 ** Part of FSEU USB Driver Module
 **
 ** History:
 **   - 2012-08-24    2.0  MSc  New Version for use with M3 L3 USB driver
 **   - 2012-10-02    2.1  MSc  use of external interrupts without L3 implemented
 **   - 2012-11-22    2.2  MSc  minor bug fixes
 *****************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "usb.h"
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) || (FM_PERIPHERAL_USB_HOST_ENABLED == ON))
     
#if (FM_PERIPHERAL_USB_DEVICE_ENABLED == ON)
  #include "usbdevice.h"
  #if ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))
      #include "usbdevicehw.h"
  #endif
  #if ((USE_USBDESCRIPTORS_H == 1) || (USB_USE_PDL == 1))
    #include "usbdescriptors.h"
  #endif
#endif
#if (FM_PERIPHERAL_USB_HOST_ENABLED == ON)
  #include "usbhost.h"
  #if ((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))
      #include "usbhosthw.h"
  #endif
#endif

#if (USBDEVICECDCCOM_ENABLED == ON)
    #include "UsbDeviceCdcCom.h"
#endif
#if (USBDEVICEHIDCOM_ENABLED == ON)
    #include "UsbDeviceHidCom.h"
#endif
#if (USBDEVICEHIDJOYSTICK_ENABLED == ON)
    #include "UsbDeviceHidJoystick.h"
#endif
#if (USBDEVICEHIDKEYBOARD_ENABLED == ON)
    #include "UsbDeviceHidKeyboard.h"         
#endif 
#if (USBDEVICEHIDMOUSE_ENABLED == ON)
    #include "UsbDeviceHidMouse.h"
#endif    
#if (USBDEVICELIBUSB_ENABLED == ON)
    #include "UsbDeviceLibUsb.h"
#endif
#if (USBDEVICEPRINTER_ENABLED == ON)
    #include "UsbDevicePrinter.h"
#endif
#if USBHOSTHIDMOUSE_ENABLED == ON
    #include "UsbHostHidMouse.h"
#endif
#if USBHOSTHIDKEYBOARD_ENABLED == ON
    #include "UsbHostHidKeyboard.h"
#endif     
#if (USBDEVICEMASSSTORAGE_ENABLED == ON)
    #include "UsbDeviceMassStorage.h"
#endif 
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

#if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback0(stc_usbn_t* pstcUSB);
#endif

#if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback1(stc_usbn_t* pstcUSB);
#endif

#ifdef __USBDEVICEHW_H__
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))))
        static boolean_t UsbConfig_Device0Vbus(en_usb_extint_param_t enType);
    #endif
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))))
        static boolean_t UsbConfig_Device1Vbus(en_usb_extint_param_t enType);
    #endif
#endif
#ifdef __USBHOSTHW_H__
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
        boolean_t UsbConfig_Host0Overcurrent(en_usb_extint_param_t enType);
        boolean_t UsbConfig_Host0Vbus(en_usb_gpio_param_t enType);
        boolean_t UsbConfig_Host0PulldownHostEnable(en_usb_gpio_param_t enType);
    #endif        
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
        boolean_t UsbConfig_Host1Overcurrent(en_usb_extint_param_t enType);
        boolean_t UsbConfig_Host1Vbus(en_usb_gpio_param_t enType);
        boolean_t UsbConfig_Host1PulldownHostEnable(en_usb_gpio_param_t enType);
    #endif   
#endif

#if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback0(stc_usbn_t* pstcUSB)
{
    #if (USBDEVICECDCCOM_ENABLED == ON)
        UsbDeviceCdcCom_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEHIDCOM_ENABLED == ON)
        UsbDeviceHidCom_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEHIDJOYSTICK_ENABLED == ON)
        UsbDeviceHidJoystick_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEHIDKEYBOARD_ENABLED == ON)
        UsbDeviceHidKeyboard_Init((stc_usbn_t*)&USB0);        
    #endif 
    #if (USBDEVICEHIDMOUSE_ENABLED == ON)
        UsbDeviceHidMouse_Init((stc_usbn_t*)&USB0);
    #endif    
    #if (USBDEVICELIBUSB_ENABLED == ON)
        UsbDeviceLibUsb_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEPRINTER_ENABLED == ON)
        UsbDevicePrinter_Init((stc_usbn_t*)&USB0);
    #endif     
    #if (USBDEVICEMASSSTORAGE_ENABLED == ON)
        UsbDeviceMassStorage_Init((stc_usbn_t*)&USB0);
    #endif  
    /* USB0 WIZARD DEVICECLASSINIT */
}
#endif

#if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback1(stc_usbn_t* pstcUSB)
{
    #if (USBDEVICECDCCOM_ENABLED == ON)
        UsbDeviceCdcCom_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEHIDCOM_ENABLED == ON)
        UsbDeviceHidCom_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEHIDJOYSTICK_ENABLED == ON)
        UsbDeviceHidJoystick_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEHIDKEYBOARD_ENABLED == ON)
        UsbDeviceHidKeyboard_Init((stc_usbn_t*)&USB1);        
    #endif 
    #if (USBDEVICEHIDMOUSE_ENABLED == ON)
        UsbDeviceHidMouse_Init((stc_usbn_t*)&USB1);
    #endif    
    #if (USBDEVICELIBUSB_ENABLED == ON)
        UsbDeviceLibUsb_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEPRINTER_ENABLED == ON)
        UsbDevicePrinter_Init((stc_usbn_t*)&USB1);
    #endif 
    #if (USBDEVICEMASSSTORAGE_ENABLED == ON)
        UsbDeviceMassStorage_Init((stc_usbn_t*)&USB1);
    #endif 
    /* USB1 WIZARD DEVICECLASSINIT */    
}
#endif

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/








/**
 ******************************************************************************
 ** \brief Initialize USB
 **
 ******************************************************************************/
void UsbConfig_UsbInit(void)
{
    stc_usb_config_t       stcUsbConfig;
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    stc_usbdevice_config_t stcUsbDeviceConfig;
    #endif
    
    Usb_Init();
    
    
    /* Setup USB 0 */
    USB_ZERO_STRUCT(stcUsbConfig);
    
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    USB_ZERO_STRUCT(stcUsbDeviceConfig);
    #endif
    
    #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
        stcUsbConfig.enMode = UsbHostDeviceEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == OFF))
        stcUsbConfig.enMode = UsbHostEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB0_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
        stcUsbConfig.enMode = UsbDeviceEnabled;
    #endif
    
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    #if ((USE_USBDESCRIPTORS_H == 1) || (USB_USE_PDL == 1))
    stcUsbDeviceConfig.pu8DeviceDescriptor = (uint8_t*)au8DeviceDescriptor;
    stcUsbDeviceConfig.pu8ConfigDescriptor = (uint8_t*)au8ConfigDescriptor; 
    stcUsbDeviceConfig.astcReportDescriptors = (stc_usbdevice_reportdescriptor_t*)astcReportDescriptors;
    stcUsbDeviceConfig.pstcStringDescriptors = (stc_usbdevice_stringdescriptor_t*)pstcStringDescriptors;
    stcUsbDeviceConfig.u8StringDescriptorCount = USBDESCRIPTORS_STRINGDESCRIPTOR_COUNT;
    #endif
    #endif
    
    stcUsbConfig.bUseInterrupts = TRUE;
    
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1)))
    stcUsbConfig.pfnDeviceVbus = UsbConfig_Device0Vbus;
    #endif
    #endif
    
    #if (!defined(USB_DISBALE_HOST_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && ((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1)))
    stcUsbConfig.pfnHostVbus = UsbConfig_Host0Vbus;
    stcUsbConfig.pfnHostPullDownHostEnable = UsbConfig_Host0PulldownHostEnable;
    stcUsbConfig.pfnHostOvercurrent = UsbConfig_Host0Overcurrent;
    #endif
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
    stcUsbDeviceConfig.pfnInitClassesCallback = UsbConfig_UsbDeviceClassesInitCallback0;
    #endif

    #if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
    UsbDevice_Init((stc_usbn_t*)&USB0,&stcUsbConfig,&stcUsbDeviceConfig);    
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB0 == ON
    #if FM_PERIPHERAL_ENABLE_USB0_HOST == ON
    UsbHost_Init((stc_usbn_t*)&USB0,&stcUsbConfig);
    #endif
    Usb_Configure((stc_usbn_t*)&USB0,&stcUsbConfig);
    #endif
    
    
 
    
    /* Setup USB 1 */
    USB_ZERO_STRUCT(stcUsbConfig);
    
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    USB_ZERO_STRUCT(stcUsbDeviceConfig);
    #endif
    
    #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
        stcUsbConfig.enMode = UsbHostDeviceEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == OFF))
        stcUsbConfig.enMode = UsbHostEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB1_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
        stcUsbConfig.enMode = UsbDeviceEnabled;
    #endif
        
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    #if ((USE_USBDESCRIPTORS_H == 1) || (USB_USE_PDL == 1))
    stcUsbDeviceConfig.pu8DeviceDescriptor = (uint8_t*)au8DeviceDescriptor;
    stcUsbDeviceConfig.pu8ConfigDescriptor = (uint8_t*)au8ConfigDescriptor; 
    stcUsbDeviceConfig.astcReportDescriptors = (stc_usbdevice_reportdescriptor_t*)astcReportDescriptors;
    stcUsbDeviceConfig.pstcStringDescriptors = (stc_usbdevice_stringdescriptor_t*)pstcStringDescriptors;
    stcUsbDeviceConfig.u8StringDescriptorCount = USBDESCRIPTORS_STRINGDESCRIPTOR_COUNT;
    #endif
    #endif
    
    stcUsbConfig.bUseInterrupts = TRUE;
    
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1)))
    stcUsbConfig.pfnDeviceVbus = UsbConfig_Device1Vbus;
    #endif
    #endif
    
    #if (!defined(USB_DISBALE_HOST_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && ((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1)))
    stcUsbConfig.pfnHostVbus = UsbConfig_Host1Vbus;
    stcUsbConfig.pfnHostPullDownHostEnable = UsbConfig_Host1PulldownHostEnable;
    stcUsbConfig.pfnHostOvercurrent = UsbConfig_Host1Overcurrent;
    #endif
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
    stcUsbDeviceConfig.pfnInitClassesCallback = UsbConfig_UsbDeviceClassesInitCallback1;
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
    UsbDevice_Init((stc_usbn_t*)&USB1,&stcUsbConfig,&stcUsbDeviceConfig);    
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB1 == ON
    #if FM_PERIPHERAL_ENABLE_USB1_HOST == ON
    UsbHost_Init((stc_usbn_t*)&USB1,&stcUsbConfig);
    #endif
    Usb_Configure((stc_usbn_t*)&USB1,&stcUsbConfig);
    #endif
}

#ifdef __USBDEVICEHW_H__

#if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))))
/**
 ******************************************************************************
 ** \brief Device 0 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
static boolean_t UsbConfig_Device0Vbus(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        DEVICE0VBUS_DEINIT; 
        break;
      case UsbExtIntInit:
        DEVICE0VBUS_INIT;
        break;  
      case UsbExtIntDisableIsr:
        DEVICE0VBUS_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        DEVICE0VBUS_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        DEVICE0VBUS_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return DEVICE0VBUS_ISRISSET;
      case UsbExtIntSetLowDetect:
        DEVICE0VBUS_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        DEVICE0VBUS_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return DEVICE0VBUS_HIGHDETECT;
    }
    return FALSE;
}
#endif

#if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1)))
/**
 ******************************************************************************
 ** \brief Device 1 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
static boolean_t UsbConfig_Device1Vbus(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        DEVICE1VBUS_DEINIT; 
        break;
      case UsbExtIntInit:
        DEVICE1VBUS_INIT;
        break;  
      case UsbExtIntDisableIsr:
        DEVICE1VBUS_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        DEVICE1VBUS_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        DEVICE1VBUS_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return DEVICE1VBUS_ISRISSET;
        break; 
      case UsbExtIntSetLowDetect:
        DEVICE1VBUS_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        DEVICE1VBUS_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return DEVICE1VBUS_HIGHDETECT;
        break; 
    }
    return FALSE;
}
#endif

#endif

#ifdef __USBHOSTHW_H__

#if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
/**
 ******************************************************************************
 ** \brief Host 0 Overcurrent GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host0Overcurrent(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        HOST0OVERCURRENT_DEINIT; 
        break;
      case UsbExtIntInit:
        HOST0OVERCURRENT_INIT;
        break;  
      case UsbExtIntDisableIsr:
        HOST0OVERCURRENT_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        HOST0OVERCURRENT_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        HOST0OVERCURRENT_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return HOST0OVERCURRENT_ISRISSET;
      case UsbExtIntSetLowDetect:
        HOST0OVERCURRENT_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        HOST0OVERCURRENT_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return HOST0OVERCURRENT_HIGHDETECT;
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 0 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host0Vbus(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST0VBUS_DEINIT; 
        break;
      case UsbGpioInit:
        HOST0VBUS_INIT;
        break;  
      case UsbGpioSet:
        HOST0VBUS_SET;
        break; 
      case UsbGpioClear:
        HOST0VBUS_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 0 pull-down GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host0PulldownHostEnable(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST0OTGPULLDOWN_DEINIT; 
        break;
      case UsbGpioInit:
        HOST0OTGPULLDOWN_INIT;
        break;  
      case UsbGpioSet:
        HOST0OTGPULLDOWN_SET;
        break; 
      case UsbGpioClear:
        HOST0OTGPULLDOWN_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}
#endif

#if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
/**
 ******************************************************************************
 ** \brief Host 1 Overcurrent GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host1Overcurrent(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        HOST1OVERCURRENT_DEINIT; 
        break;
      case UsbExtIntInit:
        HOST1OVERCURRENT_INIT;
        break;  
      case UsbExtIntDisableIsr:
        HOST1OVERCURRENT_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        HOST1OVERCURRENT_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        HOST1OVERCURRENT_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return HOST1OVERCURRENT_ISRISSET;
      case UsbExtIntSetLowDetect:
        HOST1OVERCURRENT_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        HOST1OVERCURRENT_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return HOST1OVERCURRENT_HIGHDETECT;
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 1 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host1Vbus(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST1VBUS_DEINIT; 
        break;
      case UsbGpioInit:
        HOST1VBUS_INIT;
        break;  
      case UsbGpioSet:
        HOST1VBUS_SET;
        break; 
      case UsbGpioClear:
        HOST1VBUS_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 1 pull-down GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host1PulldownHostEnable(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST1OTGPULLDOWN_DEINIT; 
        break;
      case UsbGpioInit:
        HOST1OTGPULLDOWN_INIT;
        break;  
      case UsbGpioSet:
        HOST1OTGPULLDOWN_SET;
        break; 
      case UsbGpioClear:
        HOST1OTGPULLDOWN_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}
#endif

#endif

void UsbConfig_SwitchMode(void)
{
    #if FM_PERIPHERAL_ENABLE_USB0 == ON
        #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == OFF))
            Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchToHost,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchDependingDeviceVbus,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchDependingDeviceVbus,0);
        #endif
    #endif

    #if FM_PERIPHERAL_ENABLE_USB1 == ON
        #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == OFF))

            Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchToHost,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchDependingDeviceVbus,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchDependingDeviceVbus,0);
        #endif
    #endif
}


#if (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON)
void UsbDevice_Usb0ExintCallback(void)
{
    if (Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchDependingDeviceVbus,0) == Ok)
    {
        if (DEVICE0VBUS_HIGHDETECT)
        {
            DEVICE0VBUS_SETLOWDETECT;
        }
        else
        {
            DEVICE0VBUS_SETHIGHDETECT;
        }
    }
    
}
#endif

#if (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON)
void UsbDevice_Usb1ExintCallback(void)
{
    if (Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchDependingDeviceVbus,0) == Ok)
    {
        if (DEVICE1VBUS_HIGHDETECT)
        {
            DEVICE1VBUS_SETLOWDETECT;
        }
        else
        {
            DEVICE1VBUS_SETHIGHDETECT;
        }
    }
}
#endif



#if (USB_USE_L3 == 0)
#if ((USB_USE_L3 == 0) && (USB_USE_EXT_INT == 1))
#if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
void INT8_31_Handler (void)
{
  #if FM_PERIPHERAL_ENABLE_USB0 == ON
  if (DEVICE0VBUS_ISRISSET)
  {
     DEVICE0VBUS_CLEARISRFLAG;
     UsbDevice_Usb0ExintCallback();
  }
  #endif
  #if FM_PERIPHERAL_ENABLE_USB1 == ON
  if (DEVICE1VBUS_ISRISSET)
  {
     DEVICE1VBUS_CLEARISRFLAG;
     UsbDevice_Usb1ExintCallback();
  }
  #endif
}

void INT8_15_Handler (void)
{
  #if FM_PERIPHERAL_ENABLE_USB0 == ON
  if (DEVICE0VBUS_ISRISSET)
  {
     DEVICE0VBUS_CLEARISRFLAG;
     UsbDevice_Usb0ExintCallback();
  }
  #endif
  #if FM_PERIPHERAL_ENABLE_USB1 == ON
  if (DEVICE1VBUS_ISRISSET)
  {
     DEVICE1VBUS_CLEARISRFLAG;
     UsbDevice_Usb1ExintCallback();
  }
  #endif
}

void INT0_7_Handler (void)
{
  #if FM_PERIPHERAL_ENABLE_USB0 == ON
  if (DEVICE0VBUS_ISRISSET)
  {
     DEVICE0VBUS_CLEARISRFLAG;
     UsbDevice_Usb0ExintCallback();
  }
  #endif
  #if FM_PERIPHERAL_ENABLE_USB1 == ON
  if (DEVICE1VBUS_ISRISSET)
  {
     DEVICE1VBUS_CLEARISRFLAG;
     UsbDevice_Usb1ExintCallback();
  }
  #endif
}
#endif
#endif
#endif
#else

/**
 ******************************************************************************
 ** \brief Initialize USB (dummy if USB is disabled)
 **
 ******************************************************************************/
void UsbConfig_UsbInit(void)
{
}

/**
 ******************************************************************************
 ** \brief Switch USB mode (dummy if USB is disabled)
 **
 ******************************************************************************/
void UsbConfig_SwitchMode(void)
{
}

#endif /* ((!defined(USB_DISBALE_DEVICE_FUNCTIONALITY)) || (!defined(USB_DISBALE_HOST_FUNCTIONALITY))) */
