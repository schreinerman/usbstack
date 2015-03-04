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
/************************************************************************/
/* History:                                                             */
/*                                                                      */
/* Date       | Version  | Responsible  | Description                   */
/*----------------------------------------------------------------------*/
/* 2011-06-17 | V1.0     | MSc          | First Version                 */
/* 2013-03-04 | V1.1     | MSc          | 64-bit bug fixes              */
/*                                                                      */
/************************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Spansion_HID_Demo
{
    public partial class frmMain : Form
    {
        /// <summary>
        /// Queue used to store received bytes, to share them between different threads
        /// </summary>
        Queue<byte[]> logBoxQueue = new Queue<byte[]>();

        String[] DeviceList;

        /// <summary>
        /// State of the user buttons
        /// </summary>
        Byte ButtonsState = 0;

        /// <summary>
        /// Form constructor
        /// </summary>
        public frmMain()
        {
            InitializeComponent();
        }

        /// <summary>
        /// This procedure is executed after the form was initialized
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void frmMain_Load(object sender, EventArgs e)
        {
            DeviceList = Spansion.UsbLib.Base.Driver.DeviceDetails.ListDeviceIDs(spansionUsbCom1.DeviceVendorID, spansionUsbCom1.DeviceProductID);
            comboBoxDeviceList.DataSource = DeviceList;
            selectDriver.SelectedIndex = 1;
        }

        /// <summary>
        /// This sends command 0x55 to return a predefined message
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            Byte[] test = new Byte[2];
            test[0] = 0x55;
            test[1] = 23;
            spansionUsbCom1.Send(1, test, 1000);
        }

        /// <summary>
        /// If data was transferred, update log-box and check if a command is known
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void spansionUsbCom1_OnDataTransferred(object sender, Spansion.UsbLib.Events.DataTransferredEvent e)
        {
            logBoxQueue.Enqueue(e.Buffer);
            UpdateLogBox();

            switch (e.Buffer[0])
            {
                case 0xBB: // command 0xBB updates the ButtonState
                    ButtonsState = e.Buffer[1];
                    UpdateButtons();
                    break;
            }
        }

        /// <summary>
        /// At form closing, close SpansionUsbCom as well
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            spansionUsbCom1.Close();
        }

        /// <summary>
        /// If segments field changed, send data to eva board
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void txtSevenSegments_TextChanged(object sender, EventArgs e)
        {
            if (txtSevenSegments.Text.Length > 0) // if textfield is not empty
            {
                try // try update segments
                {
                    Byte b = Convert.ToByte(txtSevenSegments.Text);
                    if (b > 99) b = 99;
                    spansionUsbCom1.Send(1, new byte[] { 0x77, b }, 1000);
                }
                catch // else try to write 00 to segments
                {
                    spansionUsbCom1.Send(1, new byte[] { 0x77, 0 }, 1000);
                }
            }
            else // else try to write 00 to segments
            {
                spansionUsbCom1.Send(1, new byte[] { 0x77, 0 }, 1000);
            }
        }


        /// <summary>
        /// Update received data log-box
        /// </summary>
        private void UpdateLogBox()
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(UpdateLogBox));
                return;
            }
            byte[] data = logBoxQueue.Dequeue();
            logBox.AppendText("|-> New Data, Size: " + data.Length.ToString() + ":\r\n");

            if (viewHex.Checked)
            {
                foreach (byte b in data)
                {
                    logBox.AppendText(String.Format("{0:X2} ", b));
                }
            }
            else
            {
                System.Text.Encoding encoding = System.Text.Encoding.UTF8;
                logBox.AppendText(encoding.GetString(data));
            }
            logBox.AppendText("\r\n|-> Data End <\r\n\r\n");
        }

        /// <summary>
        /// Update ButtonState visualisation
        /// </summary>
        private void UpdateButtons()
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(UpdateButtons));
                return;
            }

            if ((ButtonsState & 0x01) != 0)
            {
                lblLeft.Text = "Pressed";
            }
            else
            {
                lblLeft.Text = "Released";
            }

            if ((ButtonsState & 0x02) != 0)
            {
                lblRight.Text = "Pressed";
            }
            else
            {
                lblRight.Text = "Released";
            }
        }

        /// <summary>
        /// Data to send text field, remaining characters view
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void txtToSend_TextChanged(object sender, EventArgs e)
        {
            lblRemain.Text = "Remaining Bytes: " + (64 - txtToSend.Text.Length).ToString();
        }

        /// <summary>
        /// send data of send text field
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void sendMessage_Click(object sender, EventArgs e)
        {
            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
            spansionUsbCom1.Send(1, enc.GetBytes(txtToSend.Text), 1000);
        }

        /// <summary>
        /// poll connection state
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timerConnectState_Tick(object sender, EventArgs e)
        {
            if (spansionUsbCom1.IsOpen())
            {
                conState.Text = "Usb Device: Connected";
            }
            else
            {
                conState.Text = "Usb Device: Disconnected";
            }
        }

        /// <summary>
        /// If selected driver changes...
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void selectDriver_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (selectDriver.Text.Equals("HID"))
            {
                spansionUsbCom1.Close();
                spansionUsbCom1.DeviceProductID = 0x2003; // Update ProductID
                spansionUsbCom1.DriverType = Spansion.UsbLib.Driver.USB_DRIVER.SPANSION_HID;
                spansionUsbCom1.AutoOpen = true;
                spansionUsbCom1.Open();
            }
            if (selectDriver.Text.Equals("LibUsb"))
            {
                spansionUsbCom1.Close();
                spansionUsbCom1.DeviceProductID = 0x2008; // Update ProductID
                spansionUsbCom1.DriverType = Spansion.UsbLib.Driver.USB_DRIVER.LIB_USB_DOT_NET;
                spansionUsbCom1.AutoOpen = true;
                spansionUsbCom1.Open();
            }
            DeviceList = Spansion.UsbLib.Base.Driver.DeviceDetails.ListDeviceIDs(spansionUsbCom1.DeviceVendorID, spansionUsbCom1.DeviceProductID);
            comboBoxDeviceList.DataSource = DeviceList;
        }

        /// <summary>
        /// Update LED dimming
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void trackbarLed_Scroll(object sender, EventArgs e)
        {
            spansionUsbCom1.Send(1, new byte[] { 0x66, (Byte)trackbarLedRed.Value, (Byte)trackbarLedGreen.Value, (Byte)trackbarLedBlue.Value }, 1000);
        }

        private void comboBoxDeviceList_SelectedIndexChanged(object sender, EventArgs e)
        {
            spansionUsbCom1.Close();
            spansionUsbCom1.SerialNumber = Spansion.UsbLib.Base.Driver.DeviceDetails.DeviceSerialNumber((String)(comboBoxDeviceList.SelectedItem));
            spansionUsbCom1.AutoOpen = true;
            spansionUsbCom1.Open();
        }

    }
}
