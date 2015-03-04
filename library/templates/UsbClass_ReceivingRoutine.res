PROTOTYPE[START]
uint32_t %FILENAME%_GetReceivedDataEndpoint%ENDPOINT%(uint8_t** ppu8Buffer);
PROTOTYPE[STOP]

PROTOTYPELOCAL[START]
static void %FILENAME%_RxCallbackEp%ENDPOINT%(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
PROTOTYPELOCAL[STOP]

DEFINE[START]
#define BUFFER_SIZE_ENDPOINT%ENDPOINT% %BUFFERSIZE%
DEFINE[STOP]

VAR[START]
static uint32_t u32LastReceivedSize%ENDPOINT%; // last received size EP%ENDPOINT%
static stc_usbn_endpoint_buffer_t stcEpBuffer%ENDPOINT%; //struct for custom buffer EP%ENDPOINT%
static uint8_t pu8BufferEp%ENDPOINT%[BUFFER_SIZE_ENDPOINT%ENDPOINT%]; // custom buffer EP%ENDPOINT%
static stc_usbn_endpoint_data_t* pstcEpOUT%ENDPOINT%;
VAR[STOP]

INIT[START]
    stcEndpointConfig.u8EndpointAddress = %ENDPOINT%;
    stcEndpointConfig.pfnRxTxCallback = %FILENAME%_RxCallbackEp%ENDPOINT%;
    stcEndpointConfig.pstcEndpointBuffer = &stcEpBuffer%ENDPOINT%;
    pstcEpOUT%ENDPOINT% = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
    UsbDevice_ReceiveData(pstcUsb, pstcEpOUT%ENDPOINT%, pu8BufferEp%ENDPOINT%, BUFFER_SIZE_ENDPOINT%ENDPOINT%, UsbIRQ);
INIT[STOP]

CODE[START]
/**
 ******************************************************************************
 ** Is called from UsbFunctions.c when a endpoint buffer %ENDPOINT% was received
 **
 ** \param pstcUsb USB Handle
 ** \param pstcEndpoint Endpoint handle
 *****************************************************************************/
static void %FILENAME%_RxCallbackEp%ENDPOINT%(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    uint8_t* pu8Buffer = pstcEndpoint->pstcEndpointBuffer->pu8Buffer;
    uint32_t u32DataSize = pstcEndpoint->pstcEndpointBuffer->u32DataSize;
    u32LastReceivedSize%ENDPOINT% = u32DataSize;
    DataReceivedFlags |= (1<<%ENDPOINT%); // setting data received flag

    //Add your code here to process the received buffer
    UsbDevice_ReceiveData(pstcUsb, pstcEpOUT%ENDPOINT%, pu8BufferEp%ENDPOINT%, BUFFER_SIZE_ENDPOINT%ENDPOINT%, UsbIRQ);
}


/**
 ******************************************************************************
 ** Is used to use received data from endpoint %ENDPOINT%
 **
 ** \param ppu8Buffer pointer to buffer
 *****************************************************************************/
uint32_t %FILENAME%_GetReceivedDataEndpoint%ENDPOINT%(uint8_t** ppu8Buffer)
{
    if ((DataReceivedFlags & (1<<%ENDPOINT%)) == 0)
    {
        return 0; // nothing to receive
    }
    DataReceivedFlags -= (1<<%ENDPOINT%);
    *ppu8Buffer = pstcEpOUT%ENDPOINT%->pstcEndpointBuffer->pu8Buffer;
    return u32LastReceivedSize%ENDPOINT%;
}
CODE[STOP]