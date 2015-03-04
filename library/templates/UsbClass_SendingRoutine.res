PROTOTYPE[START]
boolean_t %FILENAME%_SendDataVia%ENDPOINT%(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode);
uint8_t %FILENAME%_DataSent%ENDPOINT%(void);
PROTOTYPE[STOP]

PROTOTYPELOCAL[START]
static void %FILENAME%_TxCallbackEp%ENDPOINT%(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
PROTOTYPELOCAL[STOP]

DEFINE[START]
DEFINE[STOP]

VAR[START]
static stc_usbn_endpoint_data_t* pstcEpIN%ENDPOINT%;
stc_usbn_endpoint_buffer_t stcEpBuffer%ENDPOINT%; //struct for custom buffer EP%ENDPOINT%v
VAR[STOP]

INIT[START]
    stcEndpointConfig.u8EndpointAddress = 0x80 | %ENDPOINT%;
    stcEndpointConfig.pfnRxTxCallback = %FILENAME%_TxCallbackEp%ENDPOINT%;
    stcEndpointConfig.pstcEndpointBuffer = &stcEpBuffer%ENDPOINT%;
    pstcEpIN%ENDPOINT% = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
INIT[STOP]

CODE[START]
/**
 ******************************************************************************
 ** Is called from main application to send data via endpoint %ENDPOINT%
 **
 ** \param pu8Buffer Buffer to send
 ** \param u32DataSize Buffersize
 ** \param u8PollinMode 1: polled sending; 0: interrupt sending;
 **
 ** \return 1: if succesful, 0: if usb was not ready
 *****************************************************************************/
boolean_t %FILENAME%_SendDataVia%ENDPOINT%(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<%ENDPOINT%);
        
        UsbDevice_SendData(pstcUsb, pstcEpIN%ENDPOINT%, pu8Buffer, u32DataSize, enMode);
        if (enMode == UsbPOLL)
        {
            DataSentFlags |= (1<<%ENDPOINT%);
        }
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** Is called from UsbFunctions.c when a endpoint buffer %ENDPOINT% was sent
 **
 ** \param pstcUsb USB Handle
 ** \param pstcEndpoint Endpoint Handle
 *****************************************************************************/
static void %FILENAME%_TxCallbackEp%ENDPOINT%(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    DataSentFlags |= (1<<%ENDPOINT%);
}
/**
 ******************************************************************************
 ** Is used to use get the sent status of endpoint %ENDPOINT%
 **
 *****************************************************************************/
uint8_t %FILENAME%_DataSent%ENDPOINT%(void)
{
    if ((DataSentFlags & (1<<%ENDPOINT%)) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
CODE[STOP]