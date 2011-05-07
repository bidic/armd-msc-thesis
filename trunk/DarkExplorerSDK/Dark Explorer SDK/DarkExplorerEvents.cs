using System;
using System.Drawing;
using System.IO.Ports;
using System.Drawing.Imaging;
using System.Collections.Generic;


namespace DarkExplorerSDK
{
    /// <summary>
    /// Zdarzenie otrzymania danych prostokąta ograniczającego wyznaczonego w procesie analizy obrazu
    /// </summary>
    public class DetectionResultReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Minimalna współrzędna X prostokąta ograniczającego
        /// </summary>
        public int MinX { get; private set; }
        /// <summary>
        /// Minimalna współrzędna Y prostokąta ograniczającego
        /// </summary>
        public int MinY { get; private set; }
        /// <summary>
        /// Maksymalna współrzędna X prostokąta ograniczającego
        /// </summary>
        public int MaxX { get; private set; }
        /// <summary>
        /// Maksymalna współrzędna Y prostokąta ograniczającego
        /// </summary>
        public int MaxY { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="MinX">Minimalna współrzędna X prostokąta ograniczającego</param>
        /// <param name="MinY">Minimalna współrzędna Y prostokąta ograniczającego</param>
        /// <param name="MaxX">Maksymalna współrzędna X prostokąta ograniczającego</param>
        /// <param name="MaxY">Maksymalna współrzędna Y prostokąta ograniczającego</param>
        public DetectionResultReceivedEventArgs(int MinX, int MinY, int MaxX, int MaxY)
        {
            this.MinX = MinX;
            this.MinY = MinY;
            this.MaxX = MaxX;
            this.MaxY = MaxY;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania danych o nagranej ścieżce
    /// </summary>
    public class TrackDataReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Dane nagranej ścieżki. Kolejne elementy reprezentują zmiany kierunku poruszania się.
        /// </summary>
        public int[] PathData { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="PathData">parametry nagranej ścieżki</param>
        public TrackDataReceivedEventArgs(int[] PathData)
        {
            this.PathData = PathData;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania danych o przyspieszeniu z akcelerometra
    /// </summary>
    public class AccelerationDataReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Wartość przyspieszenia odczytana na osi X
        /// </summary>
        public double AxisX { get; private set; }
        /// <summary>
        /// Wartość przyspieszenia odczytana na osi Y
        /// </summary>
        public double AxisY { get; private set; }
        /// <summary>
        /// Wartość przyspieszenia odczytana na osi Z
        /// </summary>
        public double AxisZ { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="AxisX">Wartość odczytana na osi X</param>
        /// <param name="AxisY">Wartość odczytana na osi Y</param>
        /// <param name="AxisZ">Wartość odczytana na osi Z</param>
        public AccelerationDataReceivedEventArgs(int AxisX, int AxisY, int AxisZ)
        {
            this.AxisX = AxisX;
            this.AxisY = AxisY;
            this.AxisZ = AxisZ;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania danych o odległości z dalmierzy
    /// </summary>
    public class RangefinderDataReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Dane pomiarowe z lewego czujnika odległości
        /// </summary>
        public int LeftSensorData { get; private set; }
        /// <summary>
        /// Dane pomiarowe z prawego czujnika odległości
        /// </summary>
        public int RightSensorData { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia otrzymania danych z dalmierzy
        /// </summary>
        /// <param name="LeftSensorData">Dane pomiarowe z lewego czujnika odległości</param>
        /// <param name="RightSensorData">Dane pomiarowe z prawego czujnika odległości</param>
        public RangefinderDataReceivedEventArgs(int LeftSensorData, int RightSensorData)
        {
            this.LeftSensorData = LeftSensorData;
            this.RightSensorData = RightSensorData;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania danych pomiarowych z magnetometru
    /// </summary>
    public class MagnetometerDataReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Składowa X wektora indukcji pola magnetycznego
        /// </summary>
        public int ValueX { get; private set; }
        /// <summary>
        /// Składowa Y wektora indukcji pola magnetycznego
        /// </summary>
        public int ValueY { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="ValueX">Wartość składowej X wektora indukcji pola magnetycznego</param>
        /// <param name="ValueY">Wartość składowej Y wektora indukcji pola megnetycznego</param>
        public MagnetometerDataReceivedEventArgs(int ValueX, int ValueY)
        {
            this.ValueX = ValueX;
            this.ValueY = ValueY;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania danych pomiarowych z żyroskopu
    /// </summary>
    public class GyroscopeDataReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Kąt obrotu wykryty na osi X
        /// </summary>
        public int AngleX { get; private set; }
        /// <summary>
        /// Kąt obrotu wykryty na osi Y
        /// </summary>
        public int AngleY { get; private set; }
        /// <summary>
        /// Kąt obrotu wykryty na osi Z
        /// </summary>
        public int AngleZ { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="AngleX">Kąt obrotu na osi X</param>
        /// <param name="AngleY">Kąt obrotu na osi Y</param>
        /// <param name="AngleZ">Kąt obrotu na osi Z</param>
        public GyroscopeDataReceivedEventArgs(int AngleX, int AngleY, int AngleZ)
        {
            this.AngleX = AngleX;
            this.AngleY = AngleY;
            this.AngleZ = AngleZ;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania danych o temperaturze
    /// </summary>
    public class TemperatureDataReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Wartość temperatury odczytanej za pomocą robota
        /// </summary>
        public int Temperature { get; private set; }

        /// <summary>
        /// Domyślny konstruktor
        /// </summary>
        /// <param name="Temperature">Informacja o odczytanej temperaturze</param>
        public TemperatureDataReceivedEventArgs(int Temperature)
        {
            this.Temperature = Temperature;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania potwierdzenia realizacji wysłanej komendy
    /// </summary>
    public class AcknowledgementReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// Rodzaj komendy której dotyczy powtwierdzenie
        /// </summary>
        public RequestType Type { get; private set; }
        /// <summary>
        /// Wynik zakończenia akcji, <code>true</code> jeżeli wystąpił błąd, <code>false</code> w przeciwnym wypadku
        /// </summary>
        public Boolean State { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="Type">rodzaj akcji której dotyczy potwierdzenie</param>
        /// <param name="State">wynik zakończenia akcji</param>
        public AcknowledgementReceivedEventArgs(RequestType Type, Boolean State)
        {
            this.Type = Type;
            this.State = State;
        }
    }

    /// <summary>
    /// Zdarzenie otrzymania danych z kamery
    /// </summary>
    public class CameraDataEventArgs : EventArgs
    {
        /// <summary>
        /// Strumień danych zdjęcia wykonanego za pomocą robota
        /// </summary>
        public byte[] RawInputData { get; private set; }
        /// <summary>
        /// Obraz w postaci mapy bitowej
        /// </summary>
        public Bitmap BmpImage { get; private set; }
        /// <summary>
        /// Rozmiar obrazu
        /// </summary>
        public ImageSize Size { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="RawInputData">Dane strumienia obrazu przesłanego z robota</param>
        /// <param name="Size">Rozmiar obrazu</param>
        public CameraDataEventArgs(byte[] RawInputData, ImageSize Size)
        {
            this.RawInputData = RawInputData;
            this.Size = Size;
        }

        /// <summary>
        /// 
        /// </summary>
        public ImageQuality Quality { get; private set; }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="RawInputData"></param>
        /// <param name="Quality"></param>
        public CameraDataEventArgs(byte[] RawInputData, ImageQuality Quality)
        {
            this.RawInputData = RawInputData;
            this.Quality = Quality;
            readByteArray();
        }

        private void readByteArray()
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
                for (int x = 0; x < width; x++)
                {
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

    /// <summary>
    /// Zdarzenie otrzymania danych o poziomie nadładowania baterii
    /// </summary>
    public class PowerLevelEventArgs : EventArgs
    {
        /// <summary>
        /// Wartość napięcia zmierzona na bateriach
        /// </summary>
        public double Voltage { get; private set; }

        /// <summary>
        /// Poziom naładowania baterii
        /// </summary>
        public double PowerLevel { get; private set; }

        /// <summary>
        /// Domyślny konstruktor zdarzenia
        /// </summary>
        /// <param name="Voltage">Napięcie zmierzone na baterii</param>
        /// <param name="PowerLevel">Poziom naładowania baterii</param>
        public PowerLevelEventArgs(double Voltage, double PowerLevel){
            this.PowerLevel = PowerLevel;
            this.Voltage = Voltage;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="input"></param>
        public PowerLevelEventArgs(byte[] input)
        {
            int val_rx = (input[0] << 8) + input[1];
            this.Voltage = 3.3f / 1023;
            this.Voltage *= val_rx * 3f;

            this.PowerLevel = (this.Voltage - 6.0) / 2.5;
        }
    }

    /// <summary>
    /// Zdarzenie otwarcia połączenia
    /// </summary>
    public class ConnectionOpenedEventArgs : EventArgs
    {
        /// <summary>
        /// Port na którym połączenie zostało otwarte
        /// </summary>
        public SerialPort Port { get; private set; }

        /// <summary>
        /// Domyślny konstruktor
        /// </summary>
        /// <param name="Port">Port kótrego dotyczy zdarzenie</param>
        public ConnectionOpenedEventArgs(SerialPort Port)
        {
            this.Port = Port;
        }
    }

    /// <summary>
    /// Zdarzenie zamknięcia połączenia
    /// </summary>
    public class ConnectionClosedEventArgs : EventArgs
    {
        /// <summary>
        /// Port dla którego połączenie zostało zamknięte
        /// </summary>
        public SerialPort Port { get; private set; }

        /// <summary>
        /// Domyślny konstruktor
        /// </summary>
        /// <param name="Port">Port którego dotyczy zdarzenie</param>
        public ConnectionClosedEventArgs(SerialPort Port)
        {
            this.Port = Port;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class DataPacketReceivedEventArgs : EventArgs
    {
        /// <summary>
        /// 
        /// </summary>
        public int BytesReceived { get; private set; }
        /// <summary>
        /// 
        /// </summary>
        public byte[] PacketData { get; private set; }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="BytesReceived"></param>
        /// <param name="PacketData"></param>
        public DataPacketReceivedEventArgs(int BytesReceived, byte[] PacketData)
        {
            this.BytesReceived = BytesReceived;
            this.PacketData = PacketData;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="cmd"></param>
    public delegate void RequestSender(DataRequestCommand cmd);

    /// <summary>
    /// 
    /// </summary>
    public class DataRequestCommand
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="PacketType"></param>
        /// <param name="PacketBytes"></param>
        /// <param name="PacketCount"></param>
        public DataRequestCommand(RequestType PacketType, int PacketBytes, int PacketCount)
        {
            this.PacketBytes = PacketBytes;
            this.PacketCount = PacketCount;
            this.PacketType = PacketType;
        }
        /// <summary>
        /// 
        /// </summary>
        public RequestSender Request { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int PacketBytes { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int PacketCount { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public RequestType PacketType { get; set; }

        private List<byte> receivedData;
        /// <summary>
        /// 
        /// </summary>
        public byte[] DataBuffer
        {
            get { return receivedData != null ? receivedData.ToArray() : null; }
            set { receivedData = new List<byte>(value); }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="DataToAppend"></param>
        public void AppendData(byte[] DataToAppend)
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
    /// <summary>
    /// 
    /// </summary>
    public class CameraDataRequestCommand : DataRequestCommand
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Quality"></param>
        public CameraDataRequestCommand(ImageQuality Quality)
            : base(RequestType.Camera, 3200, Quality.GetImageQualityAttributes().TotalPacketCount)
        {
            this.Quality = Quality;
        }
        /// <summary>
        /// 
        /// </summary>
        public ImageQuality Quality { get; set; }
    }
}
