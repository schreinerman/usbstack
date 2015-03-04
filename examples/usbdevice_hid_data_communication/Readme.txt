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