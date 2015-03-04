USB Example for USB CDC class with HID Com. Data received will be looped after linefeed was received.

HID Data Communication
----------------------
The HID communication example demonstrates how to 
use user defined data transfers via HID protocol. 
Datasizes will be always 64 byte. There is no need for
drivers on Windows side. An example in Microsoft Visual
C# (Visual Studio Express 2008) is included using the
Spansion USB Communications library to communicate.

Following procedures are used for data transfers on MCU:

void UsbHidCom_SendPolled(puint8_t pu8Buffer);
sends a 64 byte buffer

puint8_t UsbHidCom_ReceivePolled(void);
receive a 64 byte buffer (waits for data was received), 
returns pointer to buffer

puint8_t UsbHidCom_Receive(void);
receive a 64 byte buffer. Returns pointer to buffer. 
If nothing was received, this returns 0.

Virtual Com Port
----------------
This is a project demonstrates the virtual com port usage under windows 32-bit.
Drivers can be found under directory "Windows Drivers". After a successful 
driver installation, start a serial terminal like "Hyper Terminal".
Try to connect with 9600,N,8 without hardware flow control. The connection speed
has no effect to the transfer speed and will be all the time the maximum speed
which will be reached by the usb library. After connect the 7 segments
should display "OK" and a welcome message will be displayed on the terminal.
Messages can be typed in and every char will be displayed as hex in the 7 segment 
display, too. After a return, the terminal answers. 

Notes:
  The virtual comm port does not work in every serial terminal configuration.
  For example it does not work with SK Wizard. For own applications, set the timeout delay
  in the serial component in your program / serial terminal to "0". (not -1)
  This should work.

For more information see also application note mcu-an-300122-e-usb_virtual_com_port.


The Virtual Comm Port was tested on following platforms:
- Windows 2000
- Windows XP
- Windows Vista 32/64 Bit
- Linux
- Mac OS X

CDC API:
  void UsbClassCdc_Init(void);                                           /* Initialisation */
  void UsbClassCdc_SetSeparator(const char_t cNewSeperator);             /* defines buffer end character nad splits buffer here */
  void UsbClassCdc_SetEchomode(boolean_t bEchoOnOff);                    /* echo on/off */
  uint8_t UsbClassCdc_IsConnected(void);                                 /* get vcomm connection state */
  uint8_t UsbClassCdc_SendByte(char_t c);                                /* send a byte or char */
  uint8_t UsbClassCdc_SendString(char_t *pcBuffer);                      /* send a string */
  uint8_t UsbClassCdc_SendBuffer(uint8_t *pu8Buffer, uint32_t u32Size);  /* send a buffer */
  uint8_t UsbClassCdc_SendHex(uint32_t n, uint8_t digits);               /* send a hex value in ASCII */
  uint8_t UsbClassCdc_SendDec(uint32_t x);                               /* send a decimal value in ASCII */
  void UsbClassCdc_SendBinAsci4(uint16_t a, uint8_t base, char_t fill);  /* bin to ascii functions */
  void UsbClassCdc_SendBinAsci8(uint16_t a, uint8_t base, char_t fill);  /* bin to ascii functions */
  void UsbClassCdc_SendBin2DotDec99(uint8_t a);                          /* bin to ascii functions */
  uint32_t UsbClassCdc_ReceivedLength(void);                             /* get received length */
  uint32_t UsbClassCdc_ReceiveBuffer(uint8_t *pu8Buffer);                /* receive a buffer */
  uint8_t UsbClassCdc_ReceiveByte(void);                                 /* receive a byte or char (first in the buffer) */
  uint8_t UsbClassCdc_ReceiveLastByte(void);                             /* receive the last received byte (last one in the receive buffer */
  uint8_t UsbClassCdc_SendHex(uint32_t n, uint8_t digits);               /* receive a hex value */

To use the standard uart functions (like in uart.c), set in UsbClassCdc.h
VCOMM_USE_UARTCOMMANDS to 1. Available functions are:
  puts(...)
  putch(...)
  puthex(...,...)
  putdec(...)
  binasci8(...,...,...)
  binasci4(...,...,...)
  bin2_dot_dec99(...)
  getch()
  Inputhex(...)