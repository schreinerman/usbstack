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
/** \file UsbHostHidMouse.c
 **
 ** Part of Spansion USB Host Driver Module
 **
 ** A detailed description is available at 
 ** @link UsbHostMouseGroup USB Host HID Mouse Module description @endlink
 **
 ** History:
 **   - 2011-03-30    1.0  MSc  First version 
 **   - 2011-08-24    1.1  MSc  Some fixes in X/Y calculation
 **   - 2012-06-05    1.2  MSc  New verison for use with new USB driver for FM3 L3
 **                             Rename HidMouse.c -> UsbHostHidMouse.c
 **   - 2013-10-14    1.3  MSc  PDL support added
 **   - 2014-07-30    1.5  MSc  Report descriptor parsing added
 **   - 2014-09-03    1.6  MSc  Deinit routine fixed
 **                             Switched to dynamic driver registering
 *****************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/


#include "UsbHostHidMouse.h"

#if ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDMOUSE_ENABLED == ON))

#include "usbhostclasssupervisor.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/


/*****************************************************************************/
/* function */
/*****************************************************************************/
static void UsbHostHidMouse_DataReceivedCallback(stc_usbn_t* pstcUsb);

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

static stc_host_endpoint_t *EndpointIN;
static stc_usbdevicemouse_internal_data_t stcInternalData;
static uint8_t        u8EndpointIN;
static uint16_t       u16MaxPackageSizeIN;
static pfn_usbhostmouse_data_change_t MouseEventHandler = NULL;
stc_mousedata_t stcUsbHostMouseData;
static boolean_t bMouseMoved = FALSE;
static stc_usbn_t* pstcUsbHandle = NULL;

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/



/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void ReportDescriptorReturned(stc_usbn_t* pstcUsb, uint8_t* pu8Data, uint32_t u32Size);
static void GetReportDescriptor(stc_usbn_t* pstcUsb, uint8_t u8Index, uint16_t u16Length, usbhost_setupcomplete_t pfnCallback);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/*****************************************************************************/
/* START Driver Setup                                                        */
/*****************************************************************************/

static const stc_usbhostclasssupervisor_usbclassdriver_t stcDriverSetup = {
    UsbHostHidMouse_InitHandler,   //pfnInitClassDriver           deinitialization callback of class driver
    UsbHostHidMouse_DeinitHandler, //pfnDeinitClassDriver         initialization callback of class driver
    0,                             //u16IdVendor                  match vendor ID
    0,                             //u16IdProduct                 match product ID
    0,                             //u8MatchDeviceClass           match device class
    0,                             //u8MatchDeviceSubClass        match device subclass
    0,                             //u8MatchDeviceProtocol        match device protocol
    3,                             //u8MatchInterfaceClass        match interface class
    1,                             //u8MatchInterfaceSubClass     match interface subclass
    2,                             //u8MatchInterfaceProtocol     match interface protocoll
    0,                             //u8Reserved                   reserved for back compatible reasons since UsbClassSupervisor V2.2    
    UsbHostHidMouse_Configured,    //pfnDeviceConfigured          device configured callback of class driver
    NULL,                          //pfnPeriodicTaskMs            periodic ms callback for class driver
    UsbHostHidMouse_IsActive,      //pfnIsActive                  driver is active callback for class driver
};

static stc_linked_list_item_t stcDriverSetupItem;

/**
 ******************************************************************************
 ** \brief Register driver at 
 ** @link UsbHostGroup USB Host Module @endlink
 **
 ******************************************************************************/
void UsbHostHidMouse_RegisterDriver(void)
{
    UsbHostClassSupervisor_RegisterClassDriver((stc_usbhostclasssupervisor_usbclassdriver_t*)&stcDriverSetup,&stcDriverSetupItem);
}

/*****************************************************************************/
/* END Driver Setup                                                          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Called from USB Host Class Supervisor to initialize this driver
 **
 ** \param pstcUsb USB handle
 **
 ** \param pu8Configuration configuration data
 **
 ** \param u32Length data length
 **
 ** \returns TRUE on success
 **
 ******************************************************************************/
boolean_t UsbHostHidMouse_InitHandler(stc_usbn_t* pstcUsb, uint8_t* pu8Configuration, uint32_t u32Length)
{
    uint8_t* pu8Buffer;
    uint8_t u8NumberOfInterfaces;
    uint8_t u8NumberOfEndpoints;
    uint8_t u8EndpointNumber;
    uint8_t u8InterfaceNumber;
    uint8_t u8Interval;
    
    if (pstcUsbHandle != NULL)
    {
        return FALSE;
    }
    pstcUsbHandle = pstcUsb;
    USBDBG("Yeah, called if a HID Device was connected ;-)\n");
    
    u8NumberOfInterfaces=pu8Configuration[4];
    for(u8InterfaceNumber=0;u8InterfaceNumber < u8NumberOfInterfaces;u8InterfaceNumber++)
    {
        USBDBGVAL8("Interface Index: ",u8InterfaceNumber);
        pu8Buffer = UsbHost_GetUsbInterfaceDescriptor(pu8Configuration,u32Length,u8InterfaceNumber);
        if (pu8Buffer == 0)
        {
            USBDBG("Error: Could not Request Interface Descriptor.\n");
            return FALSE;
        }
        u8NumberOfEndpoints = pu8Buffer[4];
        
        if ((pu8Buffer[6] == 0x01) && (pu8Buffer[7] == 0x2)) //SubClass & Protocol
        {
            for(u8EndpointNumber=1;u8EndpointNumber<=u8NumberOfEndpoints;u8EndpointNumber++)
            {
                USBDBGVAL8("Endpoint Index: ",u8EndpointNumber);
                pu8Buffer = UsbHost_GetUsbEndpointDescriptor(pu8Configuration,u32Length,u8InterfaceNumber,u8EndpointNumber);
                if (pu8Buffer[2] & USB_IN_DIRECTION)
                {
                    u8EndpointIN = pu8Buffer[2];
                    u16MaxPackageSizeIN = (uint16_t)(pu8Buffer[4] + (pu8Buffer[5] << 8));
                    u8Interval = pu8Buffer[6]; 
                }
                if (u8EndpointIN != 0)
                {
                    break;
                }
            }
            if (u8EndpointIN == 0)
            {
                USBDBG("Hm, error while resolving endpoints...");
                return FALSE;
            }
            USBDBGVAL8("EP-IN:",u8EndpointIN);

            if (u8EndpointIN != 0)
            {
                UsbHost_AddHostEndpoint(pstcUsbHandle, &EndpointIN,u8EndpointIN,u16MaxPackageSizeIN,u8Interval,UsbHostHidMouse_DataReceivedCallback);
                EndpointIN->pu8Buffer = stcInternalData.au8Data;
                EndpointIN->u32DataSize = u16MaxPackageSizeIN;
                if (EndpointIN->u32DataSize > 10) EndpointIN->u32DataSize = 10;
            } 
            USBDBGVAL8("Endpoint-IN  Addr: ",u8EndpointIN);
            USBDBGVAL8("Endpoint-IN  Size: ",u16MaxPackageSizeIN);
            break;
        }
    }     
    if (u8EndpointIN == 0)
    {
        USBDBG("Hm, error while resolving endpoints...");
        return FALSE;
    }
    return TRUE;                       
}

/**
 ******************************************************************************
 ** \brief Called for report descriptor received data
 **
 ** \param pstcUsb USB handle
 **
 ** \param pu8Data data
 **
 ** \param u32Size size
 **
 ** \returns TRUE on success
 **
 ******************************************************************************/    
static void ReportDescriptorReturned(stc_usbn_t* pstcUsb, uint8_t* pu8Data, uint32_t u32Size)
{
    static uint32_t i,i2 = 0;
    static volatile uint32_t u32Position = 1;
    static volatile uint8_t u8Index = 0;
    static int32_t i32Min = 0;
    static int32_t i32Max = 0;
    static uint8_t u8ReportCount;
    static uint8_t u8ReportSize = 8;
    static uint8_t u8ReportID = 0;
    static stc_usbdevicemouse_internal_data_value_t* pstcHandle;
    u32Position = 1;
    u8Index = 0;
    i32Min = 0;
    i32Max = 0;
    u8ReportCount = 0;
    u8ReportSize = 8;
    u8ReportID = 0;
    
    for(i2 = 0; i2 < 4;i2++)
    {
      pstcHandle = &((stc_usbdevicemouse_internal_data_value_t*)&stcInternalData.stcButtons)[i2];
      pstcHandle->i32Data = 0;
      pstcHandle->i32Min = 0;
      pstcHandle->i32Max = 0;
      pstcHandle->u32Position = 0;
      pstcHandle->u8ReportID = 0;
      pstcHandle->u8Size = 0;
      pstcHandle->u8UsageId = 0;
      pstcHandle->u8Index = 0;
    }
    for(i = 0;i< u32Size;i++)
    {
        
        if (pu8Data[i] == 0x85)
        {
            u8ReportID = pu8Data[i + 1];
            u32Position = 1;
        } else if (pu8Data[i] == 0x05)
        {
            u8Index = 0;
            for(i2 = 0; i2 < 4;i2++)
            {
                pstcHandle = &((stc_usbdevicemouse_internal_data_value_t*)&stcInternalData.stcButtons)[i2];
                pstcHandle->u8Index = 0;
            }
            u8ReportSize = 1; 
            i32Min = 0;
            i32Max = 1;
            u8ReportCount = 1;
            if (pu8Data[i + 1] == 0x01)
            {

            } else if (pu8Data[i + 1] == 0x09)
            {
                u8Index++;
                stcInternalData.stcButtons.u8Index = u8Index;
            }
        }  else if (pu8Data[i] == 0x09)
        {
            {
                if (pu8Data[i + 1] == 0x30)
                {
                   u8Index++;
                   stcInternalData.stcX.u8Index = u8Index;
                } else if (pu8Data[i + 1] == 0x31)
                {
                     u8Index++;
                     stcInternalData.stcY.u8Index = u8Index;
                } else if (pu8Data[i + 1] == 0x38)
                {
                     u8Index++;
                     stcInternalData.stcScroll.u8Index = u8Index;
                }
            }
        } else if (pu8Data[i] == 0x15)
        {
             i32Min = *((int8_t*)&pu8Data[i + 1]);
        } else if (pu8Data[i] == 0x25)
        {
             i32Max = *((int8_t*)&pu8Data[i + 1]);
        } else if (pu8Data[i] == 0x16)
        {
             i32Min = *((int16_t*)&pu8Data[i + 1]);
        } else if (pu8Data[i] == 0x26)
        {
             i32Max = *((int16_t*)&pu8Data[i + 1]);
        } else if (pu8Data[i] == 0x75)     
        {
             u8ReportSize = pu8Data[i + 1];
             
        } /*else if ((pu8Data[i] == 0x0A) && (pu8Data[i + 1] == 0x38)  && (pu8Data[i + 2] == 0x02))      
        {
             u8Index = 0;
             for(i2 = 0; i2 < 4;i2++)
             {
                pstcHandle = &((stc_usbdevicemouse_internal_data_value_t*)&stcInternalData.stcButtons)[i2];
                pstcHandle->u8Index = 0;
             }
             stcInternalData.stcScroll.u8Index = 1;
             u8ReportSize = 8; 
             stcInternalData.stcScroll.i32Max = 1;
             stcInternalData.stcScroll.i32Min = -1;
             stcInternalData.stcScroll.u8Size = 3;
             stcInternalData.stcScroll.u32Position = u32Position + (stcInternalData.stcScroll.u8Index - 1) * u8ReportSize;
             stcInternalData.stcScroll.u8ReportID = u8ReportID;
             if (u8ReportID != 0)
             {
                 stcInternalData.stcScroll.u32Position += 8;
             }
             
        }*/
        else if (pu8Data[i] == 0x95)     
        {
             u8ReportCount = pu8Data[i + 1];
        } else if (pu8Data[i] == 0x81)
        {
             if (pu8Data[i + 1] & 0x4)
             {
                 u32Position = (u32Position + 6) / 8;
                 u32Position = u32Position * 8 + 1;
             }
             for(i2 = 0; i2 < 4;i2++)
             {
                 pstcHandle = &((stc_usbdevicemouse_internal_data_value_t*)&stcInternalData.stcButtons)[i2];
                 if (pstcHandle->u8Index != 0)
                 {
                     if (&stcInternalData.stcButtons == pstcHandle)
                     {
                         pstcHandle->u8Size = u8ReportSize * u8ReportCount;
                     }
                     else
                     {
                         pstcHandle->u8Size = u8ReportSize;
                     }
                     pstcHandle->u32Position = u32Position + (pstcHandle->u8Index - 1) * u8ReportSize;
                     pstcHandle->i32Min = i32Min;
                     pstcHandle->i32Max = i32Max;
                     pstcHandle->u8ReportID = u8ReportID;
                     if (u8ReportID != 0)
                     {
                         pstcHandle->u32Position += 8;
                     }
                 }
             }
             u32Position += u8ReportSize * u8ReportCount;
             u8Index = 0;
             for(i2 = 0; i2 < 4;i2++)
             {
                pstcHandle = &((stc_usbdevicemouse_internal_data_value_t*)&stcInternalData.stcButtons)[i2];
                pstcHandle->u8Index = 0;
             }
             u8ReportSize = 1; 
        }
        i = i + (pu8Data[i] & 0x3);
    }
}

/**
 ******************************************************************************
 ** \brief Called from USB Host Class Supervisor after device is configured
 **
 ** \param pstcUsb USB handle
 **
 ******************************************************************************/
void UsbHostHidMouse_Configured(stc_usbn_t* pstcUsb)
{
    GetReportDescriptor(pstcUsb,0,500,ReportDescriptorReturned);
}

/**
 ******************************************************************************
 ** \brief Called from USB Host Class Supervisor to deinitialize this driver
 **
 ** \param pstcUsb USB handle
 **
 ** \returns TRUE on success
 **
 ******************************************************************************/
boolean_t UsbHostHidMouse_DeinitHandler(stc_usbn_t* pstcUsb)
{
    if (pstcUsb == pstcUsbHandle)
    {
        pstcUsbHandle = NULL;
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** \brief Set callback for mouse data changed
 **
 ** \param pfnCallback Callback
 **
 ******************************************************************************/
void UsbHostHidMouse_SetCallback(pfn_usbhostmouse_data_change_t pfnCallback)
{
    MouseEventHandler = pfnCallback;
}

/**
 ******************************************************************************
 ** \brief Remove callback for mouse data changed
 **
 ******************************************************************************/
void UsbHostHidMouse_RemoveCallback(void)
{
    MouseEventHandler = 0;
}

/**
 ******************************************************************************
 ** \brief Data received callback
 **
 ** \param pstcUsb USB handle
 **
 ******************************************************************************/
static void UsbHostHidMouse_DataReceivedCallback(stc_usbn_t* pstcUsb)
{
    MouseEventType_t tEvent = 0;
    EndpointIN->CompletionHandler = UsbHostHidMouse_DataReceivedCallback;
    USBHOSTMOUSE_CONVERTREPORT(stcInternalData.au8Data,stcInternalData.stcButtons);
    USBHOSTMOUSE_CONVERTREPORT(stcInternalData.au8Data,stcInternalData.stcScroll);
    USBHOSTMOUSE_CONVERTREPORT(stcInternalData.au8Data,stcInternalData.stcX);
    USBHOSTMOUSE_CONVERTREPORT(stcInternalData.au8Data,stcInternalData.stcY);
    
    bMouseMoved = TRUE;
    
    if ((uint8_t)stcInternalData.stcButtons.i32Data != stcUsbHostMouseData.tButtons)
    {
        tEvent |= MOUSEEVENT_BUTTON_CHANGED;
    }
    if (stcInternalData.stcX.i32Data != 0)
    {
        tEvent |= MOUSEEVENT_POSITIONX_CHANGED;
    }
   if (stcInternalData.stcY.i32Data != 0)
    {
        tEvent |= MOUSEEVENT_POSITIONY_CHANGED;
    }
    if (stcInternalData.stcScroll.i32Data != 0)
    {
        tEvent |= MOUSEEVENT_SCROLLING_CHANGED;
    }
    if (((int32_t)stcUsbHostMouseData.stcPosition.u32X + stcInternalData.stcX.i32Data) < 0)
    {
        stcUsbHostMouseData.stcPosition.u32X = 0;
    }
    else 
    {
        stcUsbHostMouseData.stcPosition.u32X += stcInternalData.stcX.i32Data;
    }
    if (((int32_t)stcUsbHostMouseData.stcPosition.u32Y + stcInternalData.stcY.i32Data) < 0)
    {
        stcUsbHostMouseData.stcPosition.u32Y = 0;
    }
    else
    {
        stcUsbHostMouseData.stcPosition.u32Y += stcInternalData.stcY.i32Data;
    }
    stcUsbHostMouseData.i8Scrolling = (int8_t)stcInternalData.stcScroll.i32Data;
    stcUsbHostMouseData.tButtons = (uint8_t)stcInternalData.stcButtons.i32Data;
    
    if (stcUsbHostMouseData.i8Scrolling > 0)
    {
        stcUsbHostMouseData.u32ScrollPosition++;
    }
    if (stcUsbHostMouseData.i8Scrolling < 0)
    {
        stcUsbHostMouseData.u32ScrollPosition--;
    }
    
    if ((tEvent) && (MouseEventHandler))
    {
        MouseEventHandler(tEvent,&stcUsbHostMouseData);
    }
}

/**
 ******************************************************************************
 ** \brief Data received callback
 **
 ** \param pstcUsb USB handle
 **
 ** \param u8Index Report ID
 **
 ** \param u16Length Maximum size
 **
 ** \param pfnCallback Callback
 **
 ******************************************************************************/
static void GetReportDescriptor(stc_usbn_t* pstcUsb, uint8_t u8Index, uint16_t u16Length, usbhost_setupcomplete_t pfnCallback)
{
    stc_usb_request_t stcSetup = {0x81,0x06,0x2200 + u8Index,0x00,u16Length};
    UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t *)&stcSetup,pfnCallback);
}

/**
 ******************************************************************************
 ** \brief Set current cursor position
 **
 ** \param u32X X
 **
 ** \param u32Y Y
 **
 ******************************************************************************/
void UsbHostHidMouse_SetCurrentPosition(uint32_t u32X, uint32_t u32Y)
{
    stcUsbHostMouseData.stcPosition.u32X = u32X;
    stcUsbHostMouseData.stcPosition.u32Y = u32Y;
}

/**
 ******************************************************************************
 ** \brief Get current scroll position
 **
 ** \returns Scroll position
 **
 ******************************************************************************/
uint32_t UsbHostHidMouse_GetCurrentScrollPosition(void)
{
    return stcUsbHostMouseData.u32ScrollPosition;
}

/**
 ******************************************************************************
 ** \brief Set current scroll position
 **
 ** \param u32ScrollPosition New scroll position
 **
 ******************************************************************************/
void UsbHostHidMouse_SetCurrentScrollPosition(uint32_t u32ScrollPosition)
{
    stcUsbHostMouseData.u32ScrollPosition = u32ScrollPosition;
}

/**
 ******************************************************************************
 ** \brief Get mouse buttons bitmap
 **
 ** \returns mouse button bitmap
 **
 ******************************************************************************/
buttons_t UsbHostHidMouse_GetButtons(void)
{
    return stcUsbHostMouseData.tButtons;
}

/**
 ******************************************************************************
 ** \brief Check if mouse was moved
 **
 ** \returns TRUE, if mouse was moved
 **
 ******************************************************************************/
boolean_t UsbHostHidMouse_Moved(void)
{
    if (bMouseMoved)
    {
        bMouseMoved = FALSE;
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** \brief Check if driver is active
 **
 ** \returns TRUE if it is active
 **
 ******************************************************************************/
boolean_t UsbHostHidMouse_DriverActive(void)
{
    return (pstcUsbHandle != NULL);
}

/**
 ******************************************************************************
 ** \brief Check if driver is active for a special USB port
 **        (Used with USB Host Class Supervisor)
 **
 ** \param pstcUsb USB handle
 **
 ** \returns TRUE if it is active
 **
 ******************************************************************************/
boolean_t UsbHostHidMouse_IsActive(stc_usbn_t* pstcUsb)
{
    return (pstcUsbHandle == pstcUsb);
}

#endif /* ((FM_PERIPHERAL_USB_HOST_ENABLED == ON) && (USBHOSTHIDMOUSE_ENABLED == ON)) */
