/************************************************************************/
/*               (C) Spansion Semiconductor Europe GmbH (Spansion)           */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user's obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* Spansion does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law Spansion disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, Spansion's liability  */
/* is restricted to intentional misconduct and gross negligence.        */
/* Spansion is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.5)                                                               */
/************************************************************************/
/** \file usbconfig.c
 **
 ** Part of Spansion USB Driver Module
 **
 ** History:
 **   - 2012-08-24    2.0  MSc  New Version for use with M3 L3 USB driver
 **   - 2012-10-02    2.1  MSc  use of external interrupts without L3 implemented
 **   - 2012-11-22    2.2  MSc  minor bug fixes
 *****************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "usbconfig.h"
#include "usb.h"
#if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
  #include "usbdevice.h"
  #include "UsbDeviceHidCom.h"
  #include "usbdevicehw.h"
  #if (USE_USBDESCRIPTORS_H == 1)
    #include "usbdescriptors.h"
  #endif
#endif
#if (!defined(USB_DISBALE_HOST_FUNCTIONALITY))
  #include "usbhost.h"
  #include "usbhosthw.h"
#endif

#if ((!defined(USB_DISBALE_DEVICE_FUNCTIONALITY)) || (!defined(USB_DISBALE_HOST_FUNCTIONALITY)))
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
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (USE_USBDEVICEHW_H == 1))
        static boolean_t UsbConfig_Device0Vbus(en_usb_extint_param_t enType);
    #endif
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (USE_USBDEVICEHW_H == 1))
        static boolean_t UsbConfig_Device1Vbus(en_usb_extint_param_t enType);
    #endif
#endif
#ifdef __USBHOSTHW_H__
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (USE_USBHOSTHW_H == 1))
        boolean_t UsbConfig_Host0Overcurrent(en_usb_extint_param_t enType);
        boolean_t UsbConfig_Host0Vbus(en_usb_gpio_param_t enType);
        boolean_t UsbConfig_Host0PulldownHostEnable(en_usb_gpio_param_t enType);
    #endif        
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (USE_USBHOSTHW_H == 1))
        boolean_t UsbConfig_Host1Overcurrent(en_usb_extint_param_t enType);
        boolean_t UsbConfig_Host1Vbus(en_usb_gpio_param_t enType);
        boolean_t UsbConfig_Host1PulldownHostEnable(en_usb_gpio_param_t enType);
    #endif   
#endif

#if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback0(stc_usbn_t* pstcUSB)
{
    USB0DEVICE_CLASS_INIT;
}
#endif

#if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback1(stc_usbn_t* pstcUSB)
{
    USB1DEVICE_CLASS_INIT;
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
    #if (USE_USBDESCRIPTORS_H == 1)
    stcUsbDeviceConfig.pu8DeviceDescriptor = (uint8_t*)au8DeviceDescriptor;
    stcUsbDeviceConfig.pu8ConfigDescriptor = (uint8_t*)au8ConfigDescriptor; 
    stcUsbDeviceConfig.pu8ReportDescriptor = (uint8_t*)au8ReportDescriptor;
    stcUsbDeviceConfig.u16ReportDescriptorSize = USBDESCRIPTORS_REPORTDESCRIPTOR_LENGTH;
    stcUsbDeviceConfig.pstcStringDescriptors = (stc_usbdevice_stringdescriptor_t*)pstcStringDescriptors;
    stcUsbDeviceConfig.u8StringDescriptorCount = USBDESCRIPTORS_STRINGDESCRIPTOR_COUNT;
    #endif
    #endif
    
    stcUsbConfig.bUseInterrupts = TRUE;
    
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (USE_USBDEVICEHW_H == 1))
    stcUsbConfig.pfnDeviceVbus = UsbConfig_Device0Vbus;
    #endif
    #endif
    
    #if (!defined(USB_DISBALE_HOST_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (USE_USBHOSTHW_H == 1))
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
    #if (USE_USBDESCRIPTORS_H == 1)
    stcUsbDeviceConfig.pu8DeviceDescriptor = (uint8_t*)au8DeviceDescriptor;
    stcUsbDeviceConfig.pu8ConfigDescriptor = (uint8_t*)au8ConfigDescriptor; 
    stcUsbDeviceConfig.pu8ReportDescriptor = (uint8_t*)au8ReportDescriptor;
    stcUsbDeviceConfig.pstcStringDescriptors = (stc_usbdevice_stringdescriptor_t*)pstcStringDescriptors;
    stcUsbDeviceConfig.u8StringDescriptorCount = USBDESCRIPTORS_STRINGDESCRIPTOR_COUNT;
    #endif
    #endif
    
    stcUsbConfig.bUseInterrupts = TRUE;
    
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (USE_USBDEVICEHW_H == 1))
    stcUsbConfig.pfnDeviceVbus = UsbConfig_Device1Vbus;
    #endif
    #endif
    
    #if (!defined(USB_DISBALE_HOST_FUNCTIONALITY))
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (USE_USBHOSTHW_H == 1))
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

#if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (USE_USBDEVICEHW_H == 1))
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

#if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (USE_USBDEVICEHW_H == 1))
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

#if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (USE_USBHOSTHW_H == 1))
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

#if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (USE_USBHOSTHW_H == 1))
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
