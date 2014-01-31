using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Diagnostics;

namespace PhotoViewer
{
    public partial class Form1 : Form
    {
        int PhotoWidth = 320;
        int PhotoHeight = 240;

        //string Data;
        byte[] imageData;
        int ptr;
        Timer tick;
        Bitmap Image;
        Boolean Photo;
        public Form1()
        {
            Photo = false;
            InitializeComponent();
            //Data = "";
            try
            {
                serialPort_Uart.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort_Uart_DataReceived);
                serialPort_Uart.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                
            }

            tick = new Timer();
            tick.Interval = 100;
            tick.Tick += new EventHandler(tick_Tick);
            tick.Start();

            this.FormClosing += new FormClosingEventHandler(Form1_FormClosing);

            imageData = new byte[PhotoHeight * PhotoWidth * 2];
            ptr = 0;

        }

        void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(serialPort_Uart.IsOpen)//close the serial port
                serialPort_Uart.Close();
        }

        //Ticker - updates the count and when complete, makes the image.
        void tick_Tick(object sender, EventArgs e) 
        {
            textBox1.Text = ptr.ToString();
            //richTextBox_Uart_Transmission.Text = Data;
            if ((ptr == (PhotoHeight * PhotoWidth * 2)) && Photo)
            {
                ImageReceived();
            }
        }

        void serialPort_Uart_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            try
            {
                SerialPort sp = (SerialPort)sender;
                int DataIn = sp.ReadByte();
                while (DataIn != -1)//read all bytes- N.B. sp.ReadExisting() causes problems with any byte value over 0x7F
                {
                    if (Photo) //if we're taking a photo
                    {
                        imageData[ptr++] = (byte)DataIn;
                    }
                    else
                    {
                        //Data += (char)DataIn;
                    }
                    DataIn = sp.ReadByte();
                }
            }
            catch { }//ignore any exceptions
        }

        private void ImageReceived()
        {
            Photo = false;
            try
            {
                //Make a new Image
                Image = new Bitmap(PhotoWidth, PhotoHeight);

                for (int i = 0; i < imageData.Length; i += 2)
                {
                    byte high = imageData[i+1];
                    byte low = imageData[i];
                    ushort rgb565 = (ushort)(high << 8 | low);

                    Color c = RGBtoColor(rgb565);
                    

                    Image.SetPixel((i / 2) % 320, (i / 2) / 320, c);
                }
                ImageFormatConverter ifc = new ImageFormatConverter();
                
                pictureBox1.Image = Image;
                Image.Save(@"./Image_" + DateTime.Now.ToString("_ddMMyy_HHmmss") + ".bmp");
                //Get ready for another image
                
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private static Color RGBtoColor(ushort rgb565)
        {

            ushort[] RGBMask = new ushort[3];
            RGBMask[0] = 0xF800; //Red mask
            RGBMask[1] = 0x07E0; //green mask
            RGBMask[2] = 0x001F; //Blue mask

            //apply colour masks and shift
            byte RED = (byte)((rgb565 & RGBMask[0]) >> 11);
            byte GREEN = (byte)((rgb565 & RGBMask[1]) >> 5);
            byte BLUE = (byte)(rgb565 & RGBMask[2]);

            //rationalise all data to a byte. 
            RED = (byte)(RED * 8);
            GREEN = (byte)((GREEN * 255) / 63);
            BLUE = (byte)(BLUE * 8);

            //set colour
            Color c = Color.FromArgb(RED, GREEN, BLUE);
            return c;
        }

        private void button_take_photo_Click(object sender, EventArgs e)
        {
            try
            {
                
                ptr = 0;
                imageData = new byte[PhotoHeight * PhotoWidth * 2];
                serialPort_Uart.Write("P\r");
                Photo = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button_Open_Click(object sender, EventArgs e)
        {
            try
            {
                DialogResult dr = openFileDialog1.ShowDialog();

                if (dr == DialogResult.OK)
                {
                    byte[] data = File.ReadAllBytes(openFileDialog1.FileName);
                    

                    for (int i = 0; i < data.Length; i++)
                    {
                        imageData[i] = data[i];
                    }
                    ImageReceived();
                }
                else
                    throw new IOException("File Error");
            }
            catch
            {
                Debug.Write("File Error");
                MessageBox.Show("Error Loading File");
            }
        }

        private void textBox_Colour_TextChanged(object sender, EventArgs e)
        {
            TestImage();

        }

        private void TestImage()
        {
            Bitmap test = new Bitmap(100, 100);
            ushort colour = Convert.ToUInt16(textBox_Red.Text);
            for (int i = 0; i < pictureBox_ColourTest.Width; i++)
            {
                for (int j = 0; j < pictureBox_ColourTest.Height; j++)
                {
                    test.SetPixel(i, j, RGBtoColor(colour));
                }
            }
            pictureBox_ColourTest.Image = test;
        }


        

    }
}
