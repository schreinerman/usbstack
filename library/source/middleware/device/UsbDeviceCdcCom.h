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
/*****************************************************************************/
/** \file UsbDeviceCdcCom.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceCdcComGroup USB Device Cdc Com Module description @endlink
 **
 ** History:
 **   - 2009-08-31    1.0  MSc  First version  (works with 16FX)
 **   - 2010-04-16    1.1  MSc  new USB library added - API had changed
 **   - 2011-08-30    1.2  MSc  bug fixes while echo data
 **                             added UsbDeviceCdcCom_SetEchoAtSeperatorOnly for
 **                             echo data only after seperator received
 **   - 2012-07-24    1.3  MSc  Version for USB Library >= FM3 
 **   - 2012-11-26    1.4  MSc  Data received routine added 
 **   - 2013-01-30    1.5  MSc  DMA routines added 
 **   - 2013-04-24    1.6  MSc  Data sent handle added
 **   - 2013-10-14    1.7  MSc  PDL support added
 **   - 2014-08-25    1.8  MSc  Polled data changed to IRQ transfer
 **                             USE_DTR_FOR_CONNECT to enable/disable DTR 
 **                             USBDEVICECDCCOM_USE_PRINTF to enable printf
 *****************************************************************************/

#ifndef __USBDEVICECDCCOM_H__
#define __USBDEVICECDCCOM_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBDEVICECDCCOM_ENABLED
    #define USBDEVICECDCCOM_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICECDCCOM_ENABLED == ON))

#include "UsbDevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceCdcComGroup USB Device Middleware: CDC Com - Virtual Com Port
 **
 ** Provided functions of USB module:
 ** 
 ** - UsbDeviceCdcCom_Init()
 ** - UsbDeviceCdcCom_DataSent()
 ** - UsbDeviceCdcCom_IsConnected() 
 ** - UsbDeviceCdcCom_SendByte()
 ** - UsbDeviceCdcCom_SendString()
 ** - UsbDeviceCdcCom_SendBuffer()
 ** - UsbDeviceCdcCom_SetSeparator()
 ** - UsbDeviceCdcCom_SetEchomode()
 ** - UsbDeviceCdcCom_SetEchoAtSeperatorOnly()
 ** - UsbDeviceCdcCom_ReceivedLength()
 ** - UsbDeviceCdcCom_ReceiveBuffer()
 ** - UsbDeviceCdcCom_ReceiveByte()
 ** - UsbDeviceCdcCom_ReceiveLastByte()
 ** - UsbDeviceCdcCom_SendHex()
 ** - UsbDeviceCdcCom_SendDec()
 ** - UsbDeviceCdcCom_SendBinAsci4()
 ** - UsbDeviceCdcCom_SendBinAsci8()
 ** - UsbDeviceCdcCom_SendBin2DotDec99()
 ** - UsbDeviceCdcCom_ReceiveHex()
 ** - UsbDeviceCdcCom_SetReceivedCallback()
 ** - UsbDeviceCdcCom_SetSentCallback()
 ** - UsbDeviceCdcCom_SetCallbackLineCodingChange()
 **
 ** Emulates UART routines for USB virtual com port
 **
 ******************************************************************************/
//@{

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

#define  USBDEVICECDCCOM_EPOUT_BUFFERSIZE   128  //buffer Size by default 512 bytes
#define  USBDEVICECDCCOM_MAX_RECEIVE_BUFFER 512

#define  USBDEVICECDCCOM_USE_UARTCOMMANDS    0
#define  USBDEVICECDCCOM_USE_PRINTF          0
    
#define  USE_DTR_FOR_CONNECT 0

#if (USBDEVICECDCCOM_USE_PRINTF == 1)
    #include <stdio.h>
#endif

/* CDC Management Element Requests */
#define SEND_ENCAPSULATED_COMMAND 0x00
#define GET_ENCAPSULATED_RESPONSE 0x01
#define SET_COMM_FEATURE 0x02
#define GET_COMM_FEATURE 0x03
#define CLEAR_COMM_FEATURE 0x04
#define SET_AUX_LINE_STATE 0x10
#define SET_HOOK_STATE 0x11
#define PULSE_SETUP 0x12
#define SEND_PULSE 0x13
#define SET_PULSE_TIME 0x14
#define RING_AUX_JACK 0x15
#define SET_LINE_CODING 0x20
#define GET_LINE_CODING 0x21
#define SET_CONTROL_LINE_STATE 0x22
#define SEND_BREAK 0x23
#define SET_RINGER_PARMS 0x30
#define GET_RINGER_PARMS 0x31
#define SET_OPERATION_PARMS 0x32
#define GET_OPERATION_PARMS 0x33
#define SET_LINE_PARMS 0x34
#define GET_LINE_PARMS 0x35
#define DIAL_DIGITS 0x36
#define SET_UNIT_PARAMETER 0x37
#define GET_UNIT_PARAMETER 0x38
#define CLEAR_UNIT_PARAMETER 0x39
#define GET_PROFILE 0x3A
#define SET_ETHERNET_MULTICAST_FILTERS 0x40
#define SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER 0x41
#define GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER 0x42
#define SET_ETHERNET_PACKET_FILTER 0x43
#define GET_ETHERNET_STATISTIC 0x44
#define SET_ATM_DATA_FORMAT 0x50
#define GET_ATM_DEVICE_STATISTICS 0x51
#define SET_ATM_DEFAULT_VC 0x52
#define GET_ATM_VC_STATISTICS 0x53


#define DEC 0
#define HEX 1


#if (USBDEVICECDCCOM_USE_UARTCOMMANDS == 1)
   #define puts(x)    UsbDeviceCdcCom_SendString(x)
   #define putch(x)    UsbDeviceCdcCom_SendByte((uint8_t)x)
   #define puthex(x,y)    UsbDeviceCdcCom_SendHex(x,y)
   #define putdec(x)    UsbDeviceCdcCom_SendDec(x)
   #define getch()    ((char_t)UsbDeviceCdcCom_ReceiveByte())
   #define Inputhex(x) UsbDeviceCdcCom_ReceiveHex(x)
   #define binasci8(x,y,z)    UsbDeviceCdcCom_SendBinAsci8(x,y,z);
   #define binasci4(x,y,z)    UsbDeviceCdcCom_SendBinAsci4(x,y,z);
   #define bin2_dot_dec99(x)    UsbDeviceCdcCom_Send_Bin2DotDec99(x);
#endif

#define USBCLASSCDC_CHARFORMAT_1STOPBIT   0
#define USBCLASSCDC_CHARFORMAT_1_5STOPBIT 1
#define USBCLASSCDC_CHARFORMAT_2STOPBIT   2

#define USBCLASSCDC_PARITYTYPE_NONE       0
#define USBCLASSCDC_PARITYTYPE_ODD        1
#define USBCLASSCDC_PARITYTYPE_EVEN       2
#define USBCLASSCDC_PARITYTYPE_MARK       3
#define USBCLASSCDC_PARITYTYPE_SPACE      4

#ifndef SERIAL_STATE
#define SERIAL_STATE 0x20
#endif
typedef void (*cdc_data_received_t)(uint8_t* pu8Data, uint32_t u32ReceviedSize);
typedef void (*cdc_data_sent_t)(void);
typedef void (*cdc_dtr_changed_t)(boolean_t bNewState);
typedef void (*cdc_rts_changed_t)(boolean_t bNewState);

typedef struct stc_cdc_linecoding
{
    volatile uint32_t dwDTERate;
    volatile uint8_t bCharFormat;
    volatile uint8_t bParityType;
    volatile uint8_t bDataBits;
} stc_cdc_linecoding_t ;

typedef struct stc_cdc_control_signal_bitmap
{
  uint16_t DTR:1;
  uint16_t RTS:1;
  uint16_t RESERVED:14;
} stc_cdc_control_signal_bitmap_t;

typedef struct stc_cdc_uart_state_bitmap
{
  uint16_t DCD:1;
  uint16_t DSR:1;
  uint16_t bBreak:1;
  uint16_t RI:1;
  uint16_t bFraming:1;
  uint16_t bParity:1;
  uint16_t bOverRun:1;
  uint16_t RESERVED:7;
} stc_cdc_uart_state_bitmap_t;

typedef struct stc_cdc_uart_state
{
    union
    {
        uint16_t u16UartState;
        stc_cdc_uart_state_bitmap_t u16UartState_f;
    };
} stc_cdc_uart_state_t;
typedef void (*usbdevicecdccom_linecodingchange_func_ptr_t)(stc_cdc_linecoding_t* pstcLineCoding);

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/



/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/


void UsbDeviceCdcCom_SerialStateSet(stc_cdc_uart_state_t* pstcState);
void UsbDeviceCdcCom_SerialStateClear(stc_cdc_uart_state_t* pstcState);
void UsbDeviceCdcCom_SerialStateUpdate(stc_cdc_uart_state_t* pstcState);
void UsbDeviceCdcCom_SerialStateGet(stc_cdc_uart_state_t* pstcState);

void UsbDeviceCdcCom_Init(stc_usbn_t* pstcUsb);
boolean_t UsbDeviceCdcCom_DataSent(void);
boolean_t UsbDeviceCdcCom_IsConnected(void); 
uint8_t UsbDeviceCdcCom_SendByte(char_t c); 
uint8_t UsbDeviceCdcCom_SendString(char_t *pcBuffer); 
uint8_t UsbDeviceCdcCom_SendBuffer(uint8_t *pu8Buffer, uint32_t u32Size); 
void UsbDeviceCdcCom_SetSeparator(const char_t cNewSeperator);
void UsbDeviceCdcCom_SetEchomode(boolean_t bEchoOnOff); 
void UsbDeviceCdcCom_SetEchoAtSeperatorOnly(boolean_t bEchoAtSeperatorOnlyOnOff);
uint32_t UsbDeviceCdcCom_ReceivedLength(void);
uint32_t UsbDeviceCdcCom_ReceiveBuffer(uint8_t *pu8Buffer); 
uint8_t UsbDeviceCdcCom_ReceiveByte(void);
uint8_t UsbDeviceCdcCom_ReceiveLastByte(void); 
uint8_t UsbDeviceCdcCom_SendHex(uint32_t n, uint8_t digits);
uint8_t UsbDeviceCdcCom_SendDec(uint32_t x);
void UsbDeviceCdcCom_SendBinAsci4(uint16_t a, uint8_t base, char_t fill);  
void UsbDeviceCdcCom_SendBinAsci8(uint16_t a, uint8_t base, char_t fill);
void UsbDeviceCdcCom_SendBin2DotDec99(uint8_t a);
uint32_t UsbDeviceCdcCom_ReceiveHex(uint8_t digits);
void UsbDeviceCdcCom_SetDtrChangedCallback(cdc_dtr_changed_t pstnCallback);
void UsbDeviceCdcCom_SetRtsChangedCallback(cdc_rts_changed_t pstnCallback);
void UsbDeviceCdcCom_SetReceivedCallback(cdc_data_received_t pstnCallback);
void UsbDeviceCdcCom_SetSentCallback(cdc_data_sent_t pstnCallback);
void UsbDeviceCdcCom_SetCallbackLineCodingChange(usbdevicecdccom_linecodingchange_func_ptr_t pfnLineCodingChangeCallback);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceCdcComGroup
#else
    #define UsbDeviceCdcCom_Init(x) ;
#endif


#endif /* __USBCLASS_H__*/
