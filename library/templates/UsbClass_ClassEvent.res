PROTOTYPE[START]
static void ClassSetupRequest%INTERFACENUMBER%(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup);
PROTOTYPE[STOP]

PROTOTYPELOCAL[START]
PROTOTYPELOCAL[STOP]


DEFINE[START]
DEFINE[STOP]

VAR[START]
VAR[STOP]

INIT[START]
    stcUsbClassConfig.u8InterfaceNumber          = %INTERFACENUMBER%;       
    stcUsbClassConfig.u8InterfaceClass           = %INTERFACECLASS%;        
    stcUsbClassConfig.u8InterfaceSubClass        = %INTERFACESUBCLASS%;                    
    stcUsbClassConfig.u8InterfaceProtocoll       = %INTERFACEPROTOCOL%;                    
    stcUsbClassConfig.pfnSetupRequestCallback    = ClassSetupRequest%INTERFACENUMBER%;                    
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                    
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                    
    stcUsbClassConfig.pfnConfCallback            = NULL;  
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
INIT[STOP]

CODE[START]
/**
 ******************************************************************************
 ** Class Setup Request handling
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcSetup Received Setup
 **
 *****************************************************************************/
static void ClassSetupRequest%INTERFACENUMBER%(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup)
{
    
}
CODE[STOP]