namespace PhotoViewer
{
    partial class Form1
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
            this.serialPort_Uart = new System.IO.Ports.SerialPort(this.components);
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.richTextBox_Uart_Transmission = new System.Windows.Forms.RichTextBox();
            this.button_take_photo = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.button_Open = new System.Windows.Forms.Button();
            this.pictureBox_ColourTest = new System.Windows.Forms.PictureBox();
            this.textBox_Red = new System.Windows.Forms.TextBox();
            this.label_Colour = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_ColourTest)).BeginInit();
            this.SuspendLayout();
            // 
            // serialPort_Uart
            // 
            this.serialPort_Uart.BaudRate = 57600;
            this.serialPort_Uart.PortName = "COM13";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(13, 13);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(147, 20);
            this.textBox1.TabIndex = 1;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(13, 40);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(385, 276);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // richTextBox_Uart_Transmission
            // 
            this.richTextBox_Uart_Transmission.Location = new System.Drawing.Point(405, 40);
            this.richTextBox_Uart_Transmission.Name = "richTextBox_Uart_Transmission";
            this.richTextBox_Uart_Transmission.ReadOnly = true;
            this.richTextBox_Uart_Transmission.Size = new System.Drawing.Size(259, 276);
            this.richTextBox_Uart_Transmission.TabIndex = 4;
            this.richTextBox_Uart_Transmission.Text = "";
            // 
            // button_take_photo
            // 
            this.button_take_photo.Location = new System.Drawing.Point(166, 11);
            this.button_take_photo.Name = "button_take_photo";
            this.button_take_photo.Size = new System.Drawing.Size(75, 23);
            this.button_take_photo.TabIndex = 5;
            this.button_take_photo.Text = "Take Photo";
            this.button_take_photo.UseVisualStyleBackColor = true;
            this.button_take_photo.Click += new System.EventHandler(this.button_take_photo_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // button_Open
            // 
            this.button_Open.Location = new System.Drawing.Point(247, 11);
            this.button_Open.Name = "button_Open";
            this.button_Open.Size = new System.Drawing.Size(75, 23);
            this.button_Open.TabIndex = 6;
            this.button_Open.Text = "Open";
            this.button_Open.UseVisualStyleBackColor = true;
            this.button_Open.Click += new System.EventHandler(this.button_Open_Click);
            // 
            // pictureBox_ColourTest
            // 
            this.pictureBox_ColourTest.Location = new System.Drawing.Point(999, 40);
            this.pictureBox_ColourTest.Name = "pictureBox_ColourTest";
            this.pictureBox_ColourTest.Size = new System.Drawing.Size(100, 100);
            this.pictureBox_ColourTest.TabIndex = 7;
            this.pictureBox_ColourTest.TabStop = false;
            // 
            // textBox_Red
            // 
            this.textBox_Red.Location = new System.Drawing.Point(999, 14);
            this.textBox_Red.Name = "textBox_Red";
            this.textBox_Red.Size = new System.Drawing.Size(100, 20);
            this.textBox_Red.TabIndex = 8;
            this.textBox_Red.TextChanged += new System.EventHandler(this.textBox_Colour_TextChanged);
            // 
            // label_Colour
            // 
            this.label_Colour.AutoSize = true;
            this.label_Colour.Location = new System.Drawing.Point(339, 14);
            this.label_Colour.Name = "label_Colour";
            this.label_Colour.Size = new System.Drawing.Size(35, 13);
            this.label_Colour.TabIndex = 9;
            this.label_Colour.Text = "label1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1176, 336);
            this.Controls.Add(this.label_Colour);
            this.Controls.Add(this.textBox_Red);
            this.Controls.Add(this.pictureBox_ColourTest);
            this.Controls.Add(this.button_Open);
            this.Controls.Add(this.button_take_photo);
            this.Controls.Add(this.richTextBox_Uart_Transmission);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.textBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_ColourTest)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort_Uart;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.RichTextBox richTextBox_Uart_Transmission;
        private System.Windows.Forms.Button button_take_photo;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button button_Open;
        private System.Windows.Forms.PictureBox pictureBox_ColourTest;
        private System.Windows.Forms.TextBox textBox_Red;
        private System.Windows.Forms.Label label_Colour;
    }
}

