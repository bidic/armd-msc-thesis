using System;
using System.Drawing;
using System.IO.Ports;
using System.Drawing.Imaging;
using System.Collections.Generic;
using System.Reflection;

namespace DarkExplorerSDK
{

	public enum PacketRequestType
	{
		PowerLevel,
		CameraData
	}

    [AttributeUsage(AttributeTargets.Field, AllowMultiple = false, Inherited = true)]
    public class EnumAttribute : Attribute
    {
        public EnumAttribute()
        {
        }
    }

    public static class EnumExtension
    {
        public static ImageQualityAttribute GetImageQualityAttributes(this ImageQuality value)
        {
            Type type = value.GetType();
            FieldInfo fieldInfo = type.GetField(value.ToString());
            var atts = (ImageQualityAttribute[])fieldInfo.GetCustomAttributes(typeof(ImageQualityAttribute), false);
            return atts.Length > 0 ? atts[0] : null;
        }
    }

    public class ImageQualityAttribute : EnumAttribute
    {
        public char Prefix { get; set; }
        public int TotalPacketCount { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
    } 

    public enum ImageQuality
    {
        [ImageQualityAttribute(Prefix = 'p', TotalPacketCount = 5, Width = 160, Height = 100)]
        LowBW,
        [ImageQualityAttribute(Prefix = 'x', TotalPacketCount = 10, Width = 160, Height = 100)]
        LowColor,
        [ImageQualityAttribute(Prefix = 'f', TotalPacketCount = 20, Width = 320, Height = 200)]
        HighBW
    }

	public delegate void RequestSender (DataRequestCommand cmd);

	public class DataRequestCommand
	{
		public DataRequestCommand (PacketRequestType PacketType, int PacketBytes, int PacketCount)
		{
			this.PacketBytes = PacketBytes;
			this.PacketCount = PacketCount;
			this.PacketType = PacketType;
		}

		public RequestSender Request { get; set; }
		public int PacketBytes { get; set; }
		public int PacketCount { get; set; }
		public PacketRequestType PacketType { get; set; }

		private List<byte> receivedData;
		public byte[] DataBuffer {
			get { return receivedData != null ? receivedData.ToArray () : null; }
			set { receivedData = new List<byte> (value); }
		}

		public void AppendData (byte[] DataToAppend)
		{
            if (receivedData == null)
            {
                this.DataBuffer = DataToAppend;
            }
            else
            {
                receivedData.AddRange(DataToAppend);
            }
		}
	}

    public class CameraDataRequestCommand : DataRequestCommand
    {
        public CameraDataRequestCommand(ImageQuality Quality)
            : base(PacketRequestType.CameraData, 3200, Quality.GetImageQualityAttributes().TotalPacketCount)
        {
            this.Quality = Quality;
        }
        public ImageQuality Quality { get; set; }
    }

	public class CameraDataArgs : EventArgs
	{
		public byte[] RawInputData { get; private set; }
		public Bitmap BmpImage { get; private set; }
        public ImageQuality Quality { get; private set; }

		public CameraDataArgs (byte[] RawInputData, ImageQuality Quality)
		{
			this.RawInputData = RawInputData;
            this.Quality = Quality;
			readByteArray ();
		}

		private void readByteArray ()
		{
            int width = Quality.GetImageQualityAttributes().Width;
            int height = Quality.GetImageQualityAttributes().Height;
            this.BmpImage = new Bitmap(width, height, PixelFormat.Format32bppRgb);
			
			int count = 0;
			double cb = 0;
			double cr = 0;
			bool cbcr = false;

            for (int y = 0; y < height; y++)
            {
				for (int x = 0; x < width; x++) {
                    if (this.Quality == ImageQuality.LowColor)
                    {
                        int color = RawInputData[count];
                        int color2 = RawInputData[count + 1];

                        cbcr = !cbcr;
                        if (cbcr)
                        {
                            cb = (color / 1.15) + 15;
                        }
                        else
                        {
                            cr = (color / 1.15) + 15;
                        }

                        double Y = (color2 / 1.15) + 15;
                        int r = Math.Min(255, Math.Max(0, (int)((1.403 * cr) - 179.5 + Y)));
                        int g = Math.Min(255, Math.Max(0, (int)((-0.3443 * cb) + 135.5 + Y - (0.7144 * cr))));
                        int b = Math.Min(255, Math.Max(0, (int)(-227.0 + Y + (1.773 * cb))));

                        this.BmpImage.SetPixel(width - x - 1, height - y - 1, Color.FromArgb(r, g, b));
                        count += 2;
                    }
                    else
                    {
                        this.BmpImage.SetPixel(width - x - 1, height - y - 1, Color.FromArgb(RawInputData[count], RawInputData[count], RawInputData[count]));
                        count++;
                    }
				}
			}
		}
	}

	public class PowerLevelArgs : EventArgs
	{
		public double Voltage { get; private set; }
		public double PowerLevel { get; private set; }

		public PowerLevelArgs (byte[] input)
		{
			int val_rx = (input[0] << 8) + input[1];
			this.Voltage = 3.3f / 1023;
			this.Voltage *= val_rx * 3f;
			
			this.PowerLevel = (this.Voltage - 6.0) / 2.5;
		}
	}

	public class ConnectionOpenedEventArgs : EventArgs
	{
		public SerialPort Port { get; private set; }

		public ConnectionOpenedEventArgs (SerialPort port)
		{
			this.Port = Port;
		}
	}

	public class ConnectionClosedEventArgs : EventArgs
	{
		public SerialPort Port { get; private set; }

		public ConnectionClosedEventArgs (SerialPort port)
		{
			this.Port = Port;
		}
	}

	public class DataPacketReceivedEventArgs : EventArgs
	{
		public int BytesReceived { get; private set; }
		public byte[] PacketData { get; private set; }

		public DataPacketReceivedEventArgs (int BytesReceived, byte[] PacketData)
		{
			this.BytesReceived = BytesReceived;
			this.PacketData = PacketData;
		}
	}
}
