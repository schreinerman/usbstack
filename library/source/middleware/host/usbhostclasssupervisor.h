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
/** \file UsbHostClassSupervisor.h
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
#ifndef __USBHOSTCLASSSUPERVISOR_H__
#define __USBHOSTCLASSSUPERVISOR_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#if (FM_PERIPHERAL_USB_HOST_ENABLED == ON)

#include "UsbHost.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbHostClassSupervisor USB Host Class Supervisor
 **
 ** Provided functions of USB Host Class Supervisor module:
 ** 
 ** - UsbHostClassSupervisor_Init()
 ** - UsbHostClassSupervisor_Deinit()
 ** - UsbHostClassSupervisor_RegisterClassDriver()
 ** - UsbHostClassSupervisor_SysTickHandler()
 ** - UsbHostClassSupervisor_UsbClassMatch()
 ** - UsbHostClassSupervisor_SetErrorHandler()
 ** - UsbHostClassSupervisor_GetActiveDriver()
 **
 **
 ** UsbHostClassSupervisor_RegisterClassDriver() can be used to dynamically add
 ** drivers. UsbHostClassSupervisor_Init() is used in the host driver module after
 ** enumeration had finished and defice is addressed. UsbHostClassSupervisor_Deinit() 
 ** is called after the device was disconnected. UsbHostClassSupervisor_SysTickHandler()
 ** is called automatically with SOF interrupt and does some statemachine to request
 ** UsbDeviceDescriptor and UsbConfigurationDescriptor. UsbHostClassSupervisor_UsbClassMatch()
 ** is used to check if a driver matches to a connected device. UsbHostClassSupervisor_SetErrorHandler()
 ** can set an error callback if no driver was found. UsbHostClassSupervisor_GetActiveDriver()
 ** is used to determine if a driver is currently loaded.
 **
 ******************************************************************************/
//@{

/**
 ******************************************************************************
 ** \page example_usbhostclass_csv Example creating USB class named UsbHostClass
 **
 ** usbhostclass.h
 ** @code 
 ** #ifndef __USBHOSTCLASS_H__
 ** #define __USBHOSTCLASS_H__
 ** #include "usb.h"
 ** #include "usbhost.h"
 **
 ** void UsbHostNdis_RegisterDriver(void);
 **
 ** #endif
 ** @endcode
 ** 
 ** usbhostclass.c
 ** @code 
 ** #include "usbhostclass.h"
 **
 ** static stc_usbn_t* pstcUsbHandle = NULL;
 **
 ** static stc_host_endpoint_t *pstcEndpointOUT;
 ** static stc_host_endpoint_t *pstcEndpointIN;
 ** static uint8_t        u8EndpointIN, u8EndpointOUT;
 ** static uint16_t       u16MaxPackageSizeIN, u16MaxPackageSizeOUT;
 **
 ** static boolean_t Init(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
 ** static boolean_t Deinit(stc_usbn_t* pstcUsb);
 ** static void Configured(stc_usbn_t* pstcUsb);
 ** static void MsTask(stc_usbn_t* pstcUsb);
 ** static void IsActive(stc_usbn_t* pstcUsb);
 **
 ** static const stc_usbhostclasssupervisor_usbclassdriver_t stcDriverSetup = {
 **     Init,         //pfnInitClassDriver           deinitialization callback of class driver
 **     Deinit,       //pfnDeinitClassDriver         initialization callback of class driver
 **     0x0BDA,       //u16IdVendor                  match vendor ID
 **     0x8196,       //u16IdProduct                 match product ID
 **     0,            //u8MatchDeviceClass           match device class
 **     0,            //u8MatchDeviceSubClass        match device subclass
 **     0,            //u8MatchDeviceProtocol        match device protocol
 **     0,            //u8MatchInterfaceClass        match interface class
 **     0,            //u8MatchInterfaceSubClass     match interface subclass
 **     0,            //u8MatchInterfaceProtocol     match interface protocoll
 **     0,            //u8Reserved                   reserved for back compatible reasons since UsbClassSupervisor V2.2    
 **     Configured,   //pfnDeviceConfigured          device configured callback of class driver
 **     MsTask,       //pfnPeriodicTaskMs            periodic ms callback for class driver
 **     IsActive,     //pfnIsActive                  driver is active callback for class driver
 ** };
 ** 
 ** static stc_linked_list_item_t stcDriverSetupItem;
 ** 
 ** void UsbHostNdis_RegisterDriver(void)
 ** {
 **     UsbHostClassSupervisor_RegisterClassDriver((stc_usbhostclasssupervisor_usbclassdriver_t*)&stcDriverSetup,&stcDriverSetupItem);
 ** }
 **
 ** static boolean_t Init(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length)
 ** {
 **     uint8_t* pu8Buffer;
 **     uint8_t u8NumberOfInterfaces;
 **     uint8_t u8NumberOfEndpoints;
 **     uint8_t u8EndpointNumber;
 **     uint8_t u8InterfaceNumber;
 **     if (pstcUsbHandle != NULL) // driver already loaded, do not load twice
 **     {
 **         return FALSE;
 **     }
 **     pstcUsbHandle = pstcUsb;
 **
 **     u8NumberOfInterfaces=pu8Configuration[4]; // get number of interfaces in this configuration
 ** 
 **     //for every interface...
 **     for(u8InterfaceNumber=0;u8InterfaceNumber < u8NumberOfInterfaces;u8InterfaceNumber++)
 **     {
 **         //get pointer to interface descriptor
 **         pu8Buffer = UsbHost_GetUsbInterfaceDescriptor(pu8Configuration,u32Length,u8InterfaceNumber);
 **            
 **         //if pointer to interface == 0, data is corrupt
 **         if (pu8Buffer == 0)
 **         {
 **             return FALSE;
 **         }
 **        
 **         //get number of endpoints
 **         u8NumberOfEndpoints = pu8Buffer[4];
 **     
 **         //in this case, search for mass storage interface class
 **         if ((pu8Buffer[6] == 0x06) && (pu8Buffer[7] == 0x50)) //SubClass & Protocol
 **         {
 **             
 **             //for every endpoint in this interface
 **             for(u8EndpointNumber=1;u8EndpointNumber<=u8NumberOfEndpoints;u8EndpointNumber++)
 **             {
 **
 **                 //get pointer to endpoint descriptor
 **                 pu8Buffer = UsbHost_GetUsbEndpointDescriptor(pu8Configuration,u32Length,u8InterfaceNumber,u8EndpointNumber);
 **
 **                 //is endpoint is IN direction?
 **                 if (pu8Buffer[2] & USB_IN_DIRECTION)
 **                 {
 **                     //get endpoint address
 **                     u8EndpointIN = pu8Buffer[2];
 **
 **                     //get endpoint size
 **                     u16MaxPackageSizeIN = (uint16_t)(pu8Buffer[4] + (pu8Buffer[5] << 8));
 **                 }
 **                 else //else endpoint is OUT direction
 **                 {
 **                      //get endpoint address
 **                      u8EndpointOUT = pu8Buffer[2];
 **
 **                      //get endpoint size
 **                      u16MaxPackageSizeOUT = (uint16_t)(pu8Buffer[4] + (pu8Buffer[5] << 8));
 **                 }
 **                 if ((u8EndpointIN != 0) && (u8EndpointOUT != 0)) //all endpoints found?
 **                 {
 **                     break;
 **                 }
 **             }
 **
 **             if ((u8EndpointIN == 0) && (u8EndpointOUT == 0)) //endpoint addresses are not valid?
 **             {
 **                 return FALSE;
 **             }
 **
 **             //registering found endpoints
 **             UsbHost_AddHostEndpoint(pstcUsbHandle,&pstcEndpointOUT,u8EndpointOUT,u16MaxPackageSizeOUT,0,0);
 **             UsbHost_AddHostEndpoint(pstcUsbHandle,&pstcEndpointIN,u8EndpointIN,u16MaxPackageSizeIN,0,0);
 **
 **             return TRUE; //endpoints are ready to use, configuration can be used
 **         }
 **     }
 **
 **     return FALSE; //This configuration is not workable for this driver                      
 ** }
 **
 ** static boolean_t Deinit(stc_usbn_t* pstcUsb)
 ** {
 **     if (pstcUsbHandle == pstcUsb)
 **     {
 **          pstcUsbHandle = NULL;
 **          return TRUE;
 **     }
 **     return FALSE;
 ** }
 **
 ** static void Configured(stc_usbn_t* pstcUsb)
 ** {
 ** }
 **
 ** static void MsTask(stc_usbn_t* pstcUsb)
 ** {
 ** }
 **
 ** static void IsActive(stc_usbn_t* pstcUsb)
 ** {
 **     if (pstcUsbHandle == pstcUsb)
 **     {
 **         return TRUE;
 **     }
 **     return FALSE;
 ** }
 ** @endcode
 **
 ** Example in main.c
 ** @code 
 ** #include "usb.h"
 ** #include "usbhostclass.h"
 **
 ** int main()
 ** {
 **      UsbHostClass_RegisterDriver();
 **      UsbConfig_UsbInit(); 
 **      for(;;)
 **      {
 **          UsbConfig_SwitchMode();
 **          //your code to access UsbHostClass
 **      }
 ** }
 ** @endcode
 **
 ******************************************************************************/
    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

#define USBHOSTCLASSSUPERVISOR_VERSION  0230
#define USBHOSTCLASSSUPERVISOR_DATE     20140903
  
#define USBCLASSSVR_IDLE 0
#define USBCLASSSVR_REQUEST_DESCRIPTORS 1
#define USBCLASSSVR_CLASSCHECK 2
#define USBCLASSSVR_ERROR 3

#define UsbClassHandler_t stc_usbhostclasssupervisor_usbclassdriver_t                                                       // backward compatibility
#define UsbClassSupervisor_Init(pstcUsb) UsbHostClassSupervisor_Init(pstcUsb)                                               // backward compatibility
#define UsbClassSupervisor_Deinit(pstcUsb) UsbHostClassSupervisor_Deinit(pstcUsb)                                           // backward compatibility
#define UsbClassSupervisor_RegisterClassDriver(pstcClassDriver) UsbHostClassSupervisor_RegisterClassDriver(pstcClassDriver) // backward compatibility
#define UsbClassSupervisor_SysTickHandler(pstcUsb) UsbHostClassSupervisor_SysTickHandler(pstcUsb)                           // backward compatibility
#define UsbSupervisor_UsbClassMatch(stcA, stcB) UsbHostClassSupervisor_UsbClassMatch(stcA, stcB)                            // backward compatibility
#define UsbClassSupervisor_UsbClassMatch(stcA, stcB) UsbHostClassSupervisor_UsbClassMatch(stcA, stcB)                       // backward compatibility
#define UsbClassSupervisor_SetErrorHandler(pstcUsb,Handler) UsbHostClassSupervisor_SetErrorHandler(pstcUsb,Handler)         // backward compatibility
#define UsbClassSupervisor_GetActiveDriver(pstcUsb) UsbHostClassSupervisor_GetActiveDriver(pstcUsb)                         // backward compatibility

#define USBHOSTCLASSDRIVERHANDLE(x) ((stc_usbhostclasssupervisor_usbclassdriver_t*)x->pHandle)

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

typedef boolean_t UsbClassDriver_t;

typedef boolean_t (*initialize_class_driver_func_ptr_t)(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);
typedef boolean_t (*deinitialize_class_driver_func_ptr_t)(stc_usbn_t* pstcUsb); 
typedef void (*deviceconfigured_class_driver_func_ptr_t)(stc_usbn_t* pstcUsb);
typedef void (*periodictask_class_driver_func_ptr_t)(stc_usbn_t* pstcUsb);
typedef boolean_t (*isactive_class_driver_func_ptr_t)(stc_usbn_t* pstcUsb);
  
struct stc_usbhostclasssupervisor_usbclassdriver;

typedef struct stc_usbhostclasssupervisor_usbclassdriver
{
    union
    {
        boolean_t (*InitializeClass)(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length);  // <-- back compatible reasons since UsbClassSupervisor V2.2
        initialize_class_driver_func_ptr_t pfnInitClassDriver;                           //<-- initialization callback of class driver
    };
    union
    {
        boolean_t (*DeviceDisconnection)(stc_usbn_t* pstcUsb);                           //<-- back compatible reasons since UsbClassSupervisor V2.2
        deinitialize_class_driver_func_ptr_t pfnDeinitClassDriver;                       //<-- deinitialization callback of class driver
    };
    uint16_t u16IdVendor;                                                                //<-- Match vendor ID
    uint16_t u16IdProduct;                                                               //<-- Match product ID
    uint8_t u8MatchDeviceClass;                                                          //<-- Match device class
    uint8_t u8MatchDeviceSubClass;                                                       //<-- Match device subclass
    uint8_t u8MatchDeviceProtocol;                                                       //<-- Match device protocol
    uint8_t u8MatchInterfaceClass;                                                       //<-- Match interface class
    uint8_t u8MatchInterfaceSubClass;                                                    //<-- Match interface subclass
    uint8_t u8MatchInterfaceProtocol;                                                    //<-- Match interface protocoll
    uint8_t u8Reserved;                                                                  //<-- reserved for back compatible reasons since UsbClassSupervisor V2.2    
    /* added since UsbClassSupervisor V2.2, to be back compatible, added to the end */
    deviceconfigured_class_driver_func_ptr_t pfnDeviceConfigured;                        //<-- device configured callback of class driver
    periodictask_class_driver_func_ptr_t pfnPeriodicTaskMs;                              //<-- periodic ms callback for class driver
    isactive_class_driver_func_ptr_t pfnIsActive;                                        //<-- driver is active callback for class driver
} stc_usbhostclasssupervisor_usbclassdriver_t;

typedef void (*usbclasssupervisor_error_callback_func_ptr_t)(stc_usbn_t* pstcUsb,stc_usbhostclasssupervisor_usbclassdriver_t* pstcMatchClass);

#ifndef __STC_LINKED_LIST_ITEM_T__
#define __STC_LINKED_LIST_ITEM_T__
struct stc_linked_list_item;

typedef struct stc_linked_list_item
{
    struct stc_linked_list_item* pstcPrev;
    struct stc_linked_list_item* pstcNext;
    void* pHandle;
} stc_linked_list_item_t;

#endif

typedef struct stc_usbclasssupervisor_intern_data
{
    UsbClassHandler_t stcMatchClass;
    uint8_t u8SupervisorState;
    uint8_t* pu8Configuration;
    uint32_t u32ConfigurationSize;
    uint8_t u8MaxConfigurations;
    uint8_t u8CurrentConfiguration;
    boolean_t bDriverActive;
    void (*InternalErrorHandler)(stc_usbn_t* pstcUsb,UsbClassHandler_t stcMatchClass);  
} stc_usbclasssupervisor_intern_data_t;

typedef struct stc_usbclasssupervisorn_instance_data
{
    volatile stc_usbn_t*  pstcInstance;  ///< pointer to registers of an instance
    stc_usbclasssupervisor_intern_data_t stcInternData; ///< module internal data of instance
} stc_usbclasssupervisorn_instance_data_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

extern const stc_usb_request_t stcDeviceDescriptorRequest;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
void UsbHostClassSupervisor_Init(stc_usbn_t* pstcUsb);
void UsbHostClassSupervisor_Deinit(stc_usbn_t* pstcUsb);
en_result_t UsbHostClassSupervisor_RegisterClassDriver(stc_usbhostclasssupervisor_usbclassdriver_t* pstcDriver, stc_linked_list_item_t* pstcListItem);
void UsbHostClassSupervisor_SysTickHandler(stc_usbn_t* pstcUsb);
boolean_t UsbHostClassSupervisor_UsbClassMatch(UsbClassHandler_t* stcA, UsbClassHandler_t* stcB);
void UsbHostClassSupervisor_SetErrorHandler(stc_usbn_t* pstcUsb,void (*Handler)(stc_usbn_t* pstcUsb,UsbClassHandler_t stcMatchClass));
boolean_t UsbHostClassSupervisor_GetActiveDriver(stc_usbn_t* pstcUsb);


#ifdef __cplusplus
}
#endif

//@} // UsbHostClassSupervisor

#endif /* (USE_USB_HOST == 1) */
#endif /* __USBHOSTCLASSSUPERVISOR_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/


