The LibUSB communication example demonstrates how to 
use user defined data transfers via customized protocol. 
There need for LibUSB drivers on Windows side. These drivers
are included in this project in the "forwindows.zip" archive.
 An example in Microsoft Visual C# (Visual Studio Express 2008) is 
included using the Spansion USB Communications library 
to communicate.

Following procedures are used for data transfers on MCU:

UsbClass_SendDataVia2(Buffer,Length,USB_SENDING_MODE_POLLED);
sends a buffer


u32ReceivedLength = UsbClass_GetReceivedDataEndpoint1(&pu8Buffer); 
receive a buffer. Returns length of buffer and pointer to buffer. 
If nothing was received, this returns 0.