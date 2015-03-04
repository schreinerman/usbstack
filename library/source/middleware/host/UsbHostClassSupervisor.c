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
/** \file UsbHostClassSupervisor.c
 **
 ** Part of USB Host Driver Module
 **
 ** A detailed description is available at 
 ** @link UsbHostClassSupervisor USB Host Class Supervisor @endlink
 **
 ** History:
 **   - 2011-03-30    1.0  MSc  First version
 **   - 2012-06-01    2.0  MSc  New Version for use with M3 L3 USB driver
 **   - 2013-10-14    2.1  MSc  PDL support added
 **   - 2014-07-30    2.2  MSc  Updated documentation, added dynamic driver list,
 **                             added support for more configurations,
 **                             added three step initialization:
 **                             (1) add driver to driver list
 **                             (2) called from UsbHostClassSupervisor init device endpoints
 **                             (3) called from UsbHostClassSupervisor device is configured
 **  - 2014-09-03    2.3  MSc   Example Documentation added
 **  - 2014-10-31    2.4  MSc   Fix for __STC_LINKED_LIST_ITEM_T__ while use with 
 **                             USB device mass storage middleware
 *****************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#define __USBHOSTCLASSSUPERVISOR_C__
#include "UsbHostClassSupervisor.h"

#if (FM_PERIPHERAL_USB_HOST_ENABLED == ON)

#if USE_USBHOSTCLASSDRIVERTABLE_H == 1
#include "UsbHostClassDriverTable.h"
#endif

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

static stc_linked_list_item_t* pstcClassDriverRoot = NULL;

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

void ReturnedDeviceDescriptor(stc_usbn_t* pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataSize);
void ReturnedConfigurationDescriptor(stc_usbn_t* pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataSize);
void MsTick(stc_usbn_t* pstcUsb);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/// Macro to return the number of enabled MFS instances
#define USBCLASSSUPERVISOR_INSTANCE_COUNT (uint32_t)(sizeof(m_astcUsbHostClassSupervisorInstanceDataLut) / sizeof(m_astcUsbHostClassSupervisorInstanceDataLut[0]))

/// Look-up table for all enabled MFS instances and their internal data
stc_usbclasssupervisorn_instance_data_t m_astcUsbHostClassSupervisorInstanceDataLut[] =
{
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (defined(USB0)) && (FM_PERIPHERAL_ENABLE_USB0_HOST == ON))
    { &USB0,  // pstcInstance
      NULL    // stcInternData (not initialized yet)
    },
    #endif
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (defined(USB1)) && (FM_PERIPHERAL_ENABLE_USB1_HOST == ON))
    { &USB1,  // pstcInstance
      NULL    // stcInternData (not initialized yet)
    }
    #endif
};

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain USB instance.
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_usbclasssupervisor_intern_data_t* UsbHostClassSupervisorGetInternDataPtr(stc_usbn_t* pstcUsb) 
{
    volatile uint32_t u32Instance;
   
    for (u32Instance = 0; u32Instance < USBCLASSSUPERVISOR_INSTANCE_COUNT; u32Instance++)
    {
        if ((uint32_t)pstcUsb == (uint32_t)(m_astcUsbHostClassSupervisorInstanceDataLut[u32Instance].pstcInstance))
        {
            return &m_astcUsbHostClassSupervisorInstanceDataLut[u32Instance].stcInternData;
        }
    }
    return NULL;
}

/**
 ******************************************************************************
 ** \brief Init the UsbHostClassSupervisor from UsbHost driver
 **        after device was addressed
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHostClassSupervisor_Init(stc_usbn_t* pstcUsb)
{
    stc_usbclasssupervisor_intern_data_t* pstcUsbHostClassSupervisorIntern;
    pstcUsbHostClassSupervisorIntern = UsbHostClassSupervisorGetInternDataPtr(pstcUsb);
    pstcUsbHostClassSupervisorIntern->u8SupervisorState = USBCLASSSVR_IDLE;
    pstcUsbHostClassSupervisorIntern->bDriverActive = FALSE;
    pstcUsbHostClassSupervisorIntern->InternalErrorHandler = 0;
    pstcUsbHostClassSupervisorIntern->u8MaxConfigurations = 0;
    pstcUsbHostClassSupervisorIntern->u8CurrentConfiguration = 1;
    Usb_RemoveTimeOut(pstcUsb, UsbHostClassSupervisor_SysTickHandler);
    Usb_AddTimeOut(pstcUsb, UsbHostClassSupervisor_SysTickHandler,200);
}

/**
 ******************************************************************************
 ** \brief Deinit the UsbHostClassSupervisor from UsbHost driver
 **        after device was disconnected
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHostClassSupervisor_Deinit(stc_usbn_t* pstcUsb)
{
    volatile uint8_t i;
    stc_usbclasssupervisor_intern_data_t* pstcUsbHostClassSupervisorIntern;
    stc_linked_list_item_t* pstcCurrent = pstcClassDriverRoot;
    pstcUsbHostClassSupervisorIntern = UsbHostClassSupervisorGetInternDataPtr(pstcUsb);
    pstcUsbHostClassSupervisorIntern->bDriverActive = FALSE;
    #if USE_USBHOSTCLASSDRIVERTABLE_H == 1
        for(i=0;i<MAX_CLASSHANDLERS;i++)
        {
            if (UsbSupervisor_UsbClassMatch((UsbClassHandler_t*)&UsbClassHandlers[i],&pstcUsbHostClassSupervisorIntern->stcMatchClass))
            {
                if (UsbClassHandlers[i].DeviceDisconnection != NULL)
                {
                    UsbClassHandlers[i].DeviceDisconnection(pstcUsb);
                    break;
                }
            }
        }
    #endif
    while(pstcCurrent != NULL)
    {
        if (UsbSupervisor_UsbClassMatch(USBHOSTCLASSDRIVERHANDLE(pstcCurrent),&pstcUsbHostClassSupervisorIntern->stcMatchClass))
        {
          if (USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnDeinitClassDriver != NULL)
          {
            USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnDeinitClassDriver(pstcUsb);
          }
        }
        pstcCurrent = pstcCurrent->pstcNext;
    }
}

/**
 ******************************************************************************
 ** \brief Register new usb class driver
 **
 ** \param pstcDriver driver handle
 **
 ** \param pstcClassDriver linked list item
 **
 ** \return Ok on success, else Error
 **
 ******************************************************************************/
en_result_t UsbHostClassSupervisor_RegisterClassDriver(stc_usbhostclasssupervisor_usbclassdriver_t* pstcDriver, stc_linked_list_item_t* pstcListItem)
{
    stc_linked_list_item_t* pstcCurrent = pstcClassDriverRoot;
    if ((pstcListItem == NULL) || (pstcDriver == NULL))
    {
        return Error;
    }
    pstcListItem->pHandle = pstcDriver;
    if (pstcCurrent == NULL)
    {
        pstcClassDriverRoot = pstcListItem;
        pstcClassDriverRoot->pstcPrev = NULL;
        pstcClassDriverRoot->pstcNext = NULL;
        return Ok;
    }
    while(pstcCurrent->pstcNext != NULL)
    {
        if (pstcCurrent == pstcListItem)
        {
            return Error;
        }
        pstcCurrent = pstcCurrent->pstcNext;
    }
    pstcCurrent->pstcNext = pstcListItem;
    pstcListItem->pstcPrev = pstcCurrent;
    pstcListItem->pstcNext = NULL;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Periodical ms event, automatically initialized with SOF by
 **        the UsbHostClassSupervisor itself
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void MsTick(stc_usbn_t* pstcUsb)
{
    boolean_t bMsTickRequired = FALSE;
    volatile uint8_t i;
    stc_linked_list_item_t* pstcCurrent = pstcClassDriverRoot;
    #if USE_USBHOSTCLASSDRIVERTABLE_H == 1
        for(i=0;i<MAX_CLASSHANDLERS;i++)
        {
            if ((UsbClassHandlers[i].pfnPeriodicTaskMs != NULL) && (UsbClassHandlers[i].pfnIsActive != NULL))
            {
                 if (UsbClassHandlers[i].pfnIsActive(pstcUsb))
                 {
                     UsbClassHandlers[i].pfnPeriodicTaskMs(pstcUsb);
                     bMsTickRequired = TRUE;
                 }
            }
        }
    #endif
    if (pstcCurrent != NULL)
    {
      while(pstcCurrent != NULL)
      {
        if ((USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnPeriodicTaskMs != NULL) && (USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnIsActive != NULL))
        {
            if (USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnIsActive(pstcUsb))
            {
                USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnPeriodicTaskMs(pstcUsb);
                bMsTickRequired = TRUE;
            }
        }
        pstcCurrent = pstcCurrent->pstcNext;
      }
    }
    if (bMsTickRequired == TRUE)
    {
        Usb_AddTimeOut(pstcUsb,MsTick,1);
    }
}
/**
 ******************************************************************************
 ** \brief Periodical event, automatically initialized with SOF by
 **        the UsbHostClassSupervisor itself
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHostClassSupervisor_SysTickHandler(stc_usbn_t* pstcUsb)
{
  volatile uint8_t i;
  stc_usb_request_t stcSetup = {0x80,0x06,0x0200,0x00,0x09};
  stc_usbclasssupervisor_intern_data_t* pstcUsbHostClassSupervisorIntern;
  pstcUsbHostClassSupervisorIntern = UsbHostClassSupervisorGetInternDataPtr(pstcUsb);
  stc_linked_list_item_t* pstcCurrent = pstcClassDriverRoot;
  
  if ((UsbHost_GetDeviceStatus(pstcUsb) == USBHOST_DEVICE_ADDRESSED) && (UsbHost_GetDeviceStatus(pstcUsb) != USBHOST_DEVICE_CONFIGURED))
  {
    switch(pstcUsbHostClassSupervisorIntern->u8SupervisorState)
    {
    case USBCLASSSVR_IDLE:
      USBDBG("Requesting Class Informations\n");
      pstcUsbHostClassSupervisorIntern->u8SupervisorState = USBCLASSSVR_REQUEST_DESCRIPTORS;
      UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t *)&stcDeviceDescriptorRequest,ReturnedDeviceDescriptor);
      break;
    case USBCLASSSVR_CLASSCHECK:
      pstcUsbHostClassSupervisorIntern->u8SupervisorState = USBCLASSSVR_IDLE;
      #if USE_USBHOSTCLASSDRIVERTABLE_H == 1
          for(i=0;i<MAX_CLASSHANDLERS;i++)
          {
            if (UsbSupervisor_UsbClassMatch((UsbClassHandler_t*)&UsbClassHandlers[i],&pstcUsbHostClassSupervisorIntern->stcMatchClass))
            {
              if (UsbClassHandlers[i].InitializeClass != NULL)
              {
                if (UsbClassHandlers[i].InitializeClass(pstcUsb,pstcUsbHostClassSupervisorIntern->pu8Configuration,pstcUsbHostClassSupervisorIntern->u32ConfigurationSize) == TRUE)
                {
                  USBDBG("Setting Configuration (1)\n");
                  UsbHost_SetConfigurationDescriptor(pstcUsb,pstcUsbHostClassSupervisorIntern->u8CurrentConfiguration);
                  pstcUsbHostClassSupervisorIntern->bDriverActive = TRUE;
                  break;
                }
              }
            }
          }
      #endif
      if ((pstcUsbHostClassSupervisorIntern->bDriverActive == FALSE) && (pstcCurrent != NULL))
      {
        while(pstcCurrent != NULL)
        {
          if (UsbSupervisor_UsbClassMatch(USBHOSTCLASSDRIVERHANDLE(pstcCurrent),&pstcUsbHostClassSupervisorIntern->stcMatchClass))
          {
            if (USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->InitializeClass != NULL)
            {
              if (USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->InitializeClass(pstcUsb,pstcUsbHostClassSupervisorIntern->pu8Configuration,pstcUsbHostClassSupervisorIntern->u32ConfigurationSize) == TRUE)
              {
                USBDBG("Setting Configuration (1)\n");
                UsbHost_SetConfigurationDescriptor(pstcUsb,pstcUsbHostClassSupervisorIntern->u8CurrentConfiguration);
                pstcUsbHostClassSupervisorIntern->bDriverActive = TRUE;
                break;
              }
            }
          }
          pstcCurrent = pstcCurrent->pstcNext;
        }
      }
      if (pstcUsbHostClassSupervisorIntern->bDriverActive == FALSE)
      {
        if (pstcUsbHostClassSupervisorIntern->u8CurrentConfiguration < pstcUsbHostClassSupervisorIntern->u8MaxConfigurations)
        {
          pstcUsbHostClassSupervisorIntern->u8CurrentConfiguration++;
          stcSetup.wValue |= (pstcUsbHostClassSupervisorIntern->u8CurrentConfiguration - 1) & 0xFF;
          UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t *)&stcSetup,ReturnedConfigurationDescriptor);
        }
        else
        {
          pstcUsbHostClassSupervisorIntern->u8SupervisorState = USBCLASSSVR_ERROR;
          if (pstcUsbHostClassSupervisorIntern->InternalErrorHandler != 0)
          {
            pstcUsbHostClassSupervisorIntern->InternalErrorHandler(pstcUsb,pstcUsbHostClassSupervisorIntern->stcMatchClass);
          }
        }
      }
      break;
    default:
      break;
    }
    
  }
  if (UsbHost_GetDeviceStatus(pstcUsb) == USBHOST_DEVICE_CONFIGURED)
  {
    Usb_AddTimeOut(pstcUsb,MsTick,1);
    #if USE_USBHOSTCLASSDRIVERTABLE_H == 1
        for(i=0;i<MAX_CLASSHANDLERS;i++)
        {
          if ((UsbClassHandlers[i].pfnIsActive != NULL) && (UsbClassHandlers[i].pfnDeviceConfigured != NULL))
          {
            if (UsbClassHandlers[i].pfnIsActive(pstcUsb) == TRUE)
            {
              UsbClassHandlers[i].pfnDeviceConfigured(pstcUsb);
            }
          }
        }
    #endif
    if (pstcCurrent != NULL)
    {
      while(pstcCurrent != NULL)
      {
        if ((USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnIsActive != NULL) && (USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnDeviceConfigured != NULL))
        {
          if (USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnIsActive(pstcUsb) == TRUE)
          {
            USBHOSTCLASSDRIVERHANDLE(pstcCurrent)->pfnDeviceConfigured(pstcUsb);
          }
        }
        pstcCurrent = pstcCurrent->pstcNext;
      }
    }
  }
  else
  { 
    Usb_AddTimeOut(pstcUsb,UsbHostClassSupervisor_SysTickHandler,100);
  } 
}

/**
 ******************************************************************************
 ** \brief Register error callback if no matched driver was found
 **
 ** \param pstcUsb UsbHandle
 **
 ** \param Handle Callback handle
 **
 ******************************************************************************/
void UsbHostClassSupervisor_SetErrorHandler(stc_usbn_t* pstcUsb,void (*Handler)(stc_usbn_t* pstcUsb,UsbClassHandler_t stcMatchClass))
{
    stc_usbclasssupervisor_intern_data_t* pstcUsbHostClassSupervisorIntern;
    pstcUsbHostClassSupervisorIntern = UsbHostClassSupervisorGetInternDataPtr(pstcUsb);
    pstcUsbHostClassSupervisorIntern->InternalErrorHandler = Handler;
}

/**
 ******************************************************************************
 ** \brief Check if driver is loaded
 **
 ** \param pstcUsb UsbHandle
 **
 ** \return TRUE if driver is loaded
 **
 ******************************************************************************/
boolean_t UsbHostClassSupervisor_GetActiveDriver(stc_usbn_t* pstcUsb)
{
    stc_usbclasssupervisor_intern_data_t* pstcUsbHostClassSupervisorIntern;
    pstcUsbHostClassSupervisorIntern = UsbHostClassSupervisorGetInternDataPtr(pstcUsb);
    return pstcUsbHostClassSupervisorIntern->bDriverActive;
}

/**
 ******************************************************************************
 ** \brief Callback after requesting device descriptor
 **
 ** \param pstcUsb UsbHandle
 **
 ** \param pu8Buffer data
 **
 ** \param u32DataSize data size
 **
 ******************************************************************************/
void ReturnedDeviceDescriptor(stc_usbn_t* pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    stc_usb_request_t stcSetup = {0x80,0x06,0x0200,0x00,0x09};
    stc_usbclasssupervisor_intern_data_t* pstcUsbHostClassSupervisorIntern;
    pstcUsbHostClassSupervisorIntern = UsbHostClassSupervisorGetInternDataPtr(pstcUsb);
    Usb_RemoveTimeOut(pstcUsb,UsbHostClassSupervisor_SysTickHandler);
    Usb_AddTimeOut(pstcUsb,UsbHostClassSupervisor_SysTickHandler,200);
    pstcUsbHostClassSupervisorIntern->u8SupervisorState = USBCLASSSVR_IDLE;
    pstcUsbHostClassSupervisorIntern->stcMatchClass.u16IdVendor = (uint16_t)((uint16_t)pu8Buffer[8] + ((uint16_t)pu8Buffer[9] << 8));
    pstcUsbHostClassSupervisorIntern->stcMatchClass.u16IdProduct = (uint16_t)((uint16_t)pu8Buffer[10] + ((uint16_t)pu8Buffer[11] << 8));
    USBDBG("Device Descriptor Returned\n");
    if (pu8Buffer[4] != 0)
    {
        pstcUsbHostClassSupervisorIntern->stcMatchClass.u8MatchDeviceClass    = pu8Buffer[4];
        pstcUsbHostClassSupervisorIntern->stcMatchClass.u8MatchDeviceSubClass = pu8Buffer[5];
        pstcUsbHostClassSupervisorIntern->stcMatchClass.u8MatchDeviceProtocol = pu8Buffer[6];
    }
    pstcUsbHostClassSupervisorIntern->u8MaxConfigurations = pu8Buffer[11];
    //stcSetup.wValue |= 1;
    USBDBG("Requesting 9 byte of Configuration\n");
    UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t *)&stcSetup,ReturnedConfigurationDescriptor);
}

/**
 ******************************************************************************
 ** \brief Callback after requesting configuratiom descriptor
 **
 ** \param pstcUsb UsbHandle
 **
 ** \param pu8Buffer data
 **
 ** \param u32DataSize data size
 **
 ******************************************************************************/
void ReturnedConfigurationDescriptor(stc_usbn_t* pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    uint8_t i;
    uint8_t u8NumberOfInterfaces;
    uint8_t* pu8Pointer;
    stc_usb_request_t stcSetup = {0x80,0x06,0x0200,0x00,9};
    stc_usbclasssupervisor_intern_data_t* pstcUsbHostClassSupervisorIntern;
    pstcUsbHostClassSupervisorIntern = UsbHostClassSupervisorGetInternDataPtr(pstcUsb);
    Usb_RemoveTimeOut(pstcUsb,UsbHostClassSupervisor_SysTickHandler);
    Usb_AddTimeOut(pstcUsb,UsbHostClassSupervisor_SysTickHandler,200);
    pstcUsbHostClassSupervisorIntern->u8SupervisorState = USBCLASSSVR_IDLE;
    stcSetup.wValue |= (pstcUsbHostClassSupervisorIntern->u8CurrentConfiguration - 1) & 0xFF;
    if (pu8Buffer[1] != USB_CONFIGURATION_DESCRIPTOR_TYPE) 
    {
        USBDBG("Error while receiving configuration!\n");
        USBDBG("Requesting 9 byte of Configuration\n");
        UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t *)&stcSetup,ReturnedConfigurationDescriptor);
        return;
    }
    if (u32DataSize == 9)
    {
        USBDBG("Requesting 9 byte of Configuration received\n");
        //stcSetup.wValue |= 1;
        stcSetup.wLength = ((uint8_t)pu8Buffer[2]) | (((uint8_t)pu8Buffer[3]) << 8);
        USBDBG("Requesting full Configuration\n");
        UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t *)&stcSetup,ReturnedConfigurationDescriptor);
        return;
    }
   
    USBDBG("Configuration received.\n");
    pstcUsbHostClassSupervisorIntern->pu8Configuration = pu8Buffer;
    pstcUsbHostClassSupervisorIntern->u32ConfigurationSize = u32DataSize;
    u8NumberOfInterfaces = pu8Buffer[4];
    for(i = 0;i<u8NumberOfInterfaces;i++)
    {
        pu8Pointer = UsbHost_GetUsbInterfaceDescriptor(pu8Buffer,(uint16_t)u32DataSize,i);
        if (pu8Pointer != 0)
        {
            if (pu8Pointer[5] != 0)
            {
                pstcUsbHostClassSupervisorIntern->stcMatchClass.u8MatchInterfaceClass = pu8Pointer[5];
                pstcUsbHostClassSupervisorIntern->stcMatchClass.u8MatchInterfaceSubClass = pu8Pointer[6];
                pstcUsbHostClassSupervisorIntern->stcMatchClass.u8MatchInterfaceProtocol = pu8Pointer[7];
                pstcUsbHostClassSupervisorIntern->u8SupervisorState = USBCLASSSVR_CLASSCHECK;
                return;
            }
        }
    }
}

/**
 ******************************************************************************
 ** \brief Check if driver is matching
 **
 ** \param pstcA Driver handle A
 **
 ** \param pstcA Driver handle B
 **
 ** \return TRUE if driver is matching
 **
 ******************************************************************************/
boolean_t UsbHostClassSupervisor_UsbClassMatch(UsbClassHandler_t* pstcA, UsbClassHandler_t* pstcB)
{
    if ((pstcA->u16IdVendor != 0) && (pstcB->u16IdVendor != 0))
    {
       if (pstcA->u16IdVendor != pstcB->u16IdVendor)
       {
          return FALSE;
       }
    }
    
    if ((pstcA->u16IdProduct != 0) && (pstcB->u16IdProduct != 0))
    {
       if (pstcA->u16IdProduct != pstcB->u16IdProduct)
       {
          return FALSE;
       }
    }
    
    if ((pstcA->u8MatchDeviceClass != 0) && (pstcB->u8MatchDeviceClass != 0))
    {
       if (pstcA->u8MatchDeviceClass != pstcB->u8MatchDeviceClass)
       {
          return FALSE;
       }
    }
    

    if ((pstcA->u8MatchDeviceSubClass != 0) && (pstcB->u8MatchDeviceSubClass != 0))
    {
       if (pstcA->u8MatchDeviceSubClass != pstcB->u8MatchDeviceSubClass)
       {
          return FALSE;
       }
    }
    
    
    if ((pstcA->u8MatchDeviceProtocol != 0) && (pstcB->u8MatchDeviceProtocol != 0))
    {
       if (pstcA->u8MatchDeviceProtocol != pstcB->u8MatchDeviceProtocol)
       {
          return FALSE;
       }
    }


    if ((pstcA->u8MatchInterfaceClass != 0) && (pstcB->u8MatchInterfaceClass != 0))
    {
       if (pstcA->u8MatchInterfaceClass != pstcB->u8MatchInterfaceClass)
       {
          return FALSE;
       }
    }
    

    if ((pstcA->u8MatchInterfaceSubClass != 0) && (pstcB->u8MatchInterfaceSubClass != 0))
    {
       if (pstcA->u8MatchInterfaceSubClass != pstcB->u8MatchInterfaceSubClass)
       {
          return FALSE;
       }
    }
    

    if ((pstcA->u8MatchInterfaceProtocol != 0) && (pstcB->u8MatchInterfaceProtocol != 0))
    {
       if (pstcA->u8MatchInterfaceProtocol != pstcB->u8MatchInterfaceProtocol)
       {
          return FALSE;
       }
    }
    return TRUE;
}

#endif /* (FM_PERIPHERAL_USB_HOST_ENABLED == ON) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

