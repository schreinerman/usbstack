namespace Spansion_HID_Demo
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.spansionUsbCom1 = new Spansion.UsbLib.SpansionUsbCom();
            this.button1 = new System.Windows.Forms.Button();
            this.logBox = new System.Windows.Forms.RichTextBox();
            this.viewHex = new System.Windows.Forms.CheckBox();
            this.txtSevenSegments = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lblLeft = new System.Windows.Forms.Label();
            this.lblRight = new System.Windows.Forms.Label();
            this.txtToSend = new System.Windows.Forms.RichTextBox();
            this.sendMessage = new System.Windows.Forms.Button();
            this.lblRemain = new System.Windows.Forms.Label();
            this.timerConnectState = new System.Windows.Forms.Timer(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.conState = new System.Windows.Forms.ToolStripStatusLabel();
            this.label4 = new System.Windows.Forms.Label();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.selectDriver = new System.Windows.Forms.ToolStripComboBox();
            this.trackbarLedBlue = new System.Windows.Forms.TrackBar();
            this.trackbarLedGreen = new System.Windows.Forms.TrackBar();
            this.trackbarLedRed = new System.Windows.Forms.TrackBar();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.comboBoxDeviceList = new System.Windows.Forms.ComboBox();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackbarLedBlue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackbarLedGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackbarLedRed)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // spansionUsbCom1
            // 
            this.spansionUsbCom1.AutoOpen = false;
            this.spansionUsbCom1.DeviceProductID = 8200;
            this.spansionUsbCom1.DeviceVendorID = 6762;
            this.spansionUsbCom1.DriverSettings = null;
            this.spansionUsbCom1.DriverType = Spansion.UsbLib.Driver.USB_DRIVER.SPANSION_HID;
            this.spansionUsbCom1.SerialNumber = "";
            this.spansionUsbCom1.OnDataTransferred += new System.EventHandler<Spansion.UsbLib.Events.DataTransferredEvent>(this.spansionUsbCom1_OnDataTransferred);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(6, 121);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(152, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Send Prestored Message";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // logBox
            // 
            this.logBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.logBox.Location = new System.Drawing.Point(6, 167);
            this.logBox.Name = "logBox";
            this.logBox.ReadOnly = true;
            this.logBox.Size = new System.Drawing.Size(257, 82);
            this.logBox.TabIndex = 1;
            this.logBox.Text = "";
            // 
            // viewHex
            // 
            this.viewHex.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.viewHex.AutoSize = true;
            this.viewHex.Location = new System.Drawing.Point(114, 146);
            this.viewHex.Name = "viewHex";
            this.viewHex.Size = new System.Drawing.Size(152, 17);
            this.viewHex.TabIndex = 2;
            this.viewHex.Text = "View incoming data as hex";
            this.viewHex.UseVisualStyleBackColor = true;
            // 
            // txtSevenSegments
            // 
            this.txtSevenSegments.Location = new System.Drawing.Point(74, 21);
            this.txtSevenSegments.MaxLength = 2;
            this.txtSevenSegments.Name = "txtSevenSegments";
            this.txtSevenSegments.Size = new System.Drawing.Size(37, 20);
            this.txtSevenSegments.TabIndex = 3;
            this.txtSevenSegments.Text = "88";
            this.txtSevenSegments.TextChanged += new System.EventHandler(this.txtSevenSegments_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(62, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Left Button:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 38);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Right Button:";
            // 
            // lblLeft
            // 
            this.lblLeft.AutoSize = true;
            this.lblLeft.Location = new System.Drawing.Point(100, 16);
            this.lblLeft.Name = "lblLeft";
            this.lblLeft.Size = new System.Drawing.Size(52, 13);
            this.lblLeft.TabIndex = 6;
            this.lblLeft.Text = "Released";
            // 
            // lblRight
            // 
            this.lblRight.AutoSize = true;
            this.lblRight.Location = new System.Drawing.Point(100, 38);
            this.lblRight.Name = "lblRight";
            this.lblRight.Size = new System.Drawing.Size(52, 13);
            this.lblRight.TabIndex = 6;
            this.lblRight.Text = "Released";
            // 
            // txtToSend
            // 
            this.txtToSend.Location = new System.Drawing.Point(6, 19);
            this.txtToSend.MaxLength = 64;
            this.txtToSend.Name = "txtToSend";
            this.txtToSend.Size = new System.Drawing.Size(257, 71);
            this.txtToSend.TabIndex = 7;
            this.txtToSend.Text = "";
            this.txtToSend.TextChanged += new System.EventHandler(this.txtToSend_TextChanged);
            // 
            // sendMessage
            // 
            this.sendMessage.Location = new System.Drawing.Point(6, 96);
            this.sendMessage.Name = "sendMessage";
            this.sendMessage.Size = new System.Drawing.Size(71, 23);
            this.sendMessage.TabIndex = 0;
            this.sendMessage.Text = "Send";
            this.sendMessage.UseVisualStyleBackColor = true;
            this.sendMessage.Click += new System.EventHandler(this.sendMessage_Click);
            // 
            // lblRemain
            // 
            this.lblRemain.AutoSize = true;
            this.lblRemain.Location = new System.Drawing.Point(159, 93);
            this.lblRemain.Name = "lblRemain";
            this.lblRemain.Size = new System.Drawing.Size(104, 13);
            this.lblRemain.TabIndex = 8;
            this.lblRemain.Text = "Remaining Bytes: 64";
            this.lblRemain.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // timerConnectState
            // 
            this.timerConnectState.Enabled = true;
            this.timerConnectState.Interval = 200;
            this.timerConnectState.Tick += new System.EventHandler(this.timerConnectState_Tick);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.conState});
            this.statusStrip1.Location = new System.Drawing.Point(0, 305);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(489, 22);
            this.statusStrip1.TabIndex = 10;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // conState
            // 
            this.conState.Name = "conState";
            this.conState.Size = new System.Drawing.Size(87, 17);
            this.conState.Text = "connection state";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 147);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(82, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Received Data:";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.selectDriver});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(489, 25);
            this.toolStrip1.TabIndex = 12;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(67, 22);
            this.toolStripLabel1.Text = "Driver Type:";
            // 
            // selectDriver
            // 
            this.selectDriver.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.selectDriver.Items.AddRange(new object[] {
            "LibUsb",
            "HID"});
            this.selectDriver.Name = "selectDriver";
            this.selectDriver.Size = new System.Drawing.Size(121, 25);
            this.selectDriver.SelectedIndexChanged += new System.EventHandler(this.selectDriver_SelectedIndexChanged);
            // 
            // trackbarLedBlue
            // 
            this.trackbarLedBlue.Location = new System.Drawing.Point(59, 74);
            this.trackbarLedBlue.Maximum = 255;
            this.trackbarLedBlue.Name = "trackbarLedBlue";
            this.trackbarLedBlue.Size = new System.Drawing.Size(62, 42);
            this.trackbarLedBlue.TabIndex = 13;
            this.trackbarLedBlue.Scroll += new System.EventHandler(this.trackbarLed_Scroll);
            // 
            // trackbarLedGreen
            // 
            this.trackbarLedGreen.Location = new System.Drawing.Point(59, 44);
            this.trackbarLedGreen.Maximum = 255;
            this.trackbarLedGreen.Name = "trackbarLedGreen";
            this.trackbarLedGreen.Size = new System.Drawing.Size(62, 42);
            this.trackbarLedGreen.TabIndex = 13;
            this.trackbarLedGreen.Scroll += new System.EventHandler(this.trackbarLed_Scroll);
            // 
            // trackbarLedRed
            // 
            this.trackbarLedRed.Location = new System.Drawing.Point(59, 14);
            this.trackbarLedRed.Maximum = 255;
            this.trackbarLedRed.Name = "trackbarLedRed";
            this.trackbarLedRed.Size = new System.Drawing.Size(62, 42);
            this.trackbarLedRed.TabIndex = 13;
            this.trackbarLedRed.Scroll += new System.EventHandler(this.trackbarLed_Scroll);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.trackbarLedBlue);
            this.groupBox1.Controls.Add(this.trackbarLedGreen);
            this.groupBox1.Controls.Add(this.trackbarLedRed);
            this.groupBox1.Location = new System.Drawing.Point(12, 178);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(188, 118);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Dim LEDs (if available)";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 80);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(28, 13);
            this.label8.TabIndex = 14;
            this.label8.Text = "Blue";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 52);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(36, 13);
            this.label7.TabIndex = 14;
            this.label7.Text = "Green";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 24);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(27, 13);
            this.label6.TabIndex = 14;
            this.label6.Text = "Red";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.txtToSend);
            this.groupBox2.Controls.Add(this.sendMessage);
            this.groupBox2.Controls.Add(this.logBox);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.viewHex);
            this.groupBox2.Controls.Add(this.lblRemain);
            this.groupBox2.Controls.Add(this.button1);
            this.groupBox2.Location = new System.Drawing.Point(206, 28);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(272, 268);
            this.groupBox2.TabIndex = 15;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Send / Receive ASCII (Loopback)";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.txtSevenSegments);
            this.groupBox3.Location = new System.Drawing.Point(12, 112);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(188, 51);
            this.groupBox3.TabIndex = 16;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "7 Segment Display (if available)";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.lblLeft);
            this.groupBox4.Controls.Add(this.label2);
            this.groupBox4.Controls.Add(this.label3);
            this.groupBox4.Controls.Add(this.lblRight);
            this.groupBox4.Location = new System.Drawing.Point(12, 28);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(188, 66);
            this.groupBox4.TabIndex = 17;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Buttons (if available)";
            // 
            // comboBoxDeviceList
            // 
            this.comboBoxDeviceList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDeviceList.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.comboBoxDeviceList.FormattingEnabled = true;
            this.comboBoxDeviceList.Location = new System.Drawing.Point(217, 1);
            this.comboBoxDeviceList.Name = "comboBoxDeviceList";
            this.comboBoxDeviceList.Size = new System.Drawing.Size(272, 21);
            this.comboBoxDeviceList.TabIndex = 18;
            this.comboBoxDeviceList.SelectedIndexChanged += new System.EventHandler(this.comboBoxDeviceList_SelectedIndexChanged);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(489, 327);
            this.Controls.Add(this.comboBoxDeviceList);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Name = "frmMain";
            this.Text = "Spansion UsbCom Demo";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackbarLedBlue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackbarLedGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackbarLedRed)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Spansion.UsbLib.SpansionUsbCom spansionUsbCom1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.RichTextBox logBox;
        private System.Windows.Forms.CheckBox viewHex;
        private System.Windows.Forms.TextBox txtSevenSegments;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lblLeft;
        private System.Windows.Forms.Label lblRight;
        private System.Windows.Forms.RichTextBox txtToSend;
        private System.Windows.Forms.Button sendMessage;
        private System.Windows.Forms.Label lblRemain;
        private System.Windows.Forms.Timer timerConnectState;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel conState;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripComboBox selectDriver;
        private System.Windows.Forms.TrackBar trackbarLedBlue;
        private System.Windows.Forms.TrackBar trackbarLedGreen;
        private System.Windows.Forms.TrackBar trackbarLedRed;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ComboBox comboBoxDeviceList;
    }
}

