using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Threading;


namespace DarkExplorerSDK
{
    
    /// <summary>
    /// Klasa reprezentująca robota
    /// </summary>
    public class DarkExplorer
    {
        /// <summary>
        /// Zdarzenie odebrania danych nagranej ścieżki
        /// </summary>
        public event EventHandler TrackDataReceived;
        /// <summary>
        /// Zdarzenie odebrania danych na temat temperatury
        /// </summary>
        public event EventHandler TemperatureDataReceived;
        /// <summary>
        /// Zdarzenie odebrania danych pomiarowych z akcelerometru
        /// </summary>
        public event EventHandler AccelerationDataReceived;
        /// <summary>
        /// Zdarzenie odebrania danych pomiarowych z dalmierzy
        /// </summary>
        public event EventHandler RangefinderDataReceived;
        /// <summary>
        /// Zdarzenie odebrania danych pomiarowych z magnetometru
        /// </summary>
        public event EventHandler MagnetometerDataReceived;
        /// <summary>
        /// Zdarzenie odebrania danych pomiarowych z żyroskopu
        /// </summary>
        public event EventHandler GyroscopeDataReceived;
        /// <summary>
        /// Zdarzenie odebrania rezultatów przeprowadzonej analizy obrazu
        /// </summary>
        public event EventHandler DetectionResultReceived;
        /// <summary>
        /// Zdarzenie odebrania potwierdzenia realizacji komendy
        /// </summary>
        public event EventHandler AcknowledgementReceived;
        /// <summary>
        /// Zdarzenie odebrania danych przesłanych z robota
        /// </summary>
        public event EventHandler DataPacketReceived;
        /// <summary>
        /// Zdarzenie odebranie danych o baterii
        /// </summary>
        public event EventHandler PowerLevelReceived;
        /// <summary>
        /// Zdarzenie odebrania danych z kamery
        /// </summary>
        public event EventHandler CameraDataReceived;
        /// <summary>
        /// Zdarzenie otwarcia połączenia
        /// </summary>
        public event EventHandler ConnectionOpened;
        /// <summary>
        /// Zdarzenie zamknięcia połączenia
        /// </summary>
        public event EventHandler ConnectionClosed;

        protected bool isMono = Type.GetType("Mono.Runtime") != null;
        protected bool processReq = false;

        protected List<DataRequestCommand> cmdQueue = new List<DataRequestCommand>();

        /// <summary>
        /// Port na którym odbywa się komunikacja z robotem
        /// </summary>
        protected SerialPort darkExplorerPort;


        /// <summary>
        /// Domyślny konstruktor
        /// </summary>
        /// <param name="port">Identyfikator portu na którym odbywać się będzie komunikacja</param>
        public DarkExplorer(string port)
        {
            Console.WriteLine("IsMono: " + ImageQuality.LowBW.GetImageQualityAttributes().Prefix);
            darkExplorerPort = new SerialPort(port);
            if (!isMono)
            {
                darkExplorerPort.DataReceived += new SerialDataReceivedEventHandler(DarkExplorerPort_DataReceived);
            }
        }

        void DarkExplorerPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            ProcessReceivedData();
        }

        protected void ProcessReceivedData()
        {
            Console.WriteLine("DataReceived size: " + darkExplorerPort.BytesToRead);

            if (cmdQueue.Count > 0)
            {
                Console.WriteLine("PacketBytes: " + cmdQueue[0].PacketBytes);
                Console.WriteLine("PacketCount: " + cmdQueue[0].PacketCount);

                if (cmdQueue[0].PacketBytes > 0)
                {
                    if (cmdQueue[0].PacketBytes == darkExplorerPort.BytesToRead)
                    {
                        Console.WriteLine("BytesToRead: " + darkExplorerPort.BytesToRead);
                        cmdQueue[0].PacketCount--;
                        byte[] buffer = new byte[darkExplorerPort.BytesToRead];
                        darkExplorerPort.Read(buffer, 0, darkExplorerPort.BytesToRead);
                        OnDataPacketReceived(new DataPacketReceivedEventArgs(darkExplorerPort.BytesToRead, buffer));
                        cmdQueue[0].AppendData(buffer);

                        if (cmdQueue[0].PacketCount == 0)
                        {
                            switch (cmdQueue[0].PacketType)
                            {
                                case RequestType.Sensors:
                                    OnPowerLevelReceived(new PowerLevelEventArgs(cmdQueue[0].DataBuffer));
                                    break;

                                case RequestType.Camera:
                                    CameraDataRequestCommand cameraCommand = cmdQueue[0] as CameraDataRequestCommand;
                                    OnCameraDataReceived(new CameraDataEventArgs(cameraCommand.DataBuffer, cameraCommand.Quality));
                                    break;

                            }

                            cmdQueue.RemoveAt(0);
                            if (cmdQueue.Count > 0)
                            {
                                cmdQueue[0].Request(cmdQueue[0]);
                                if (isMono)
                                {
                                    (new Thread(new ThreadStart(MonitorIncomingData))).Start();
                                }
                            }
                            else
                            {
                                processReq = false;
                            }
                        }
                        else
                        {
                            cmdQueue[0].Request(cmdQueue[0]);
                            if (isMono)
                            {
                                (new Thread(new ThreadStart(MonitorIncomingData))).Start();
                            }
                        }
                    }
                    else if (isMono)
                    {
                        (new Thread(new ThreadStart(MonitorIncomingData))).Start();
                    }
                }
                else
                {
                    byte[] buffer = new byte[darkExplorerPort.BytesToRead];
                    darkExplorerPort.Read(buffer, 0, darkExplorerPort.BytesToRead);
                    OnDataPacketReceived(new DataPacketReceivedEventArgs(darkExplorerPort.BytesToRead, buffer));
                    cmdQueue.RemoveAt(0);
                }
            }
            else
            {
                byte[] buffer = new byte[darkExplorerPort.BytesToRead];
                darkExplorerPort.Read(buffer, 0, darkExplorerPort.BytesToRead);
                OnDataPacketReceived(new DataPacketReceivedEventArgs(darkExplorerPort.BytesToRead, buffer));
            }
        }



        protected void OnCameraDataReceived(CameraDataEventArgs e)
        {
            if (CameraDataReceived != null)
            {
                Console.WriteLine("OnCameraDataReceived");
                CameraDataReceived(this, e);
            }
        }

        protected void OnConnectionOpened(ConnectionOpenedEventArgs e)
        {
            if (ConnectionOpened != null)
            {
                Console.WriteLine("OnConnectionOpened");
                ConnectionOpened(this, e);
            }
        }

        protected void OnConnectionClosed(ConnectionClosedEventArgs e)
        {
            if (ConnectionClosed != null)
            {
                Console.WriteLine("OnConnectionClosed");
                ConnectionClosed(this, e);
            }
        }

        protected void OnPowerLevelReceived(PowerLevelEventArgs e)
        {
            if (PowerLevelReceived != null)
            {
                Console.WriteLine("OnPowerLevelReceived");
                PowerLevelReceived(this, e);
            }
        }

        protected void OnDataPacketReceived(DataPacketReceivedEventArgs e)
        {
            if (DataPacketReceived != null)
            {
                Console.WriteLine("OnDataReceived");
                DataPacketReceived(this, e);
            }
        }

        /// <summary>
        /// Metoda wysyła żądanie pobrania danych z czujnika podanego jako parametr
        /// </summary>
        /// <param name="type">Typ czujnika za pomocą którego ma zostać przeprowadzony pomiar</param>
        public void RequestSensorData(SensorType type)
        {

        }

        /// <summary>
        /// Metoda wysyła żądanie przeprowadzenia kalibracji czujnika podanego jako parametr
        /// </summary>
        /// <param name="type">Typ czujnika dla którego ma zostać przeprowadzona kalibracja</param>
        public void RequestSensoreCalibartion(SensorType type)
        {

        }

        /// <summary>
        /// Metoda wysyłająca do robota komendę obsługi trybu rekonstrukcji ścieżki powrotnej
        /// </summary>
        /// <param name="command">Komenda rekonstrukcji ścieżki powrotnej</param>
        public void SendTrackReconstructionCommand(TrackReconstructionCommand command)
        {

        }

        /// <summary>
        /// Metoda wysyłająca do robota komędę obsługi trybu autonomicznego
        /// </summary>
        /// <param name="command">Komenda obsługi trybu autonomicznego</param>
        public void SendAutonomousModeCommand(AutonomousModeCommand command)
        {

        }

        /// <summary>
        /// Metoda konfigurująca działanie prawych silników
        /// </summary>
        /// <param name="direction">Tryb pracy silników</param>
        /// <param name="speed">Moc silników</param>
        public void SetRightEnginesState(EnginesDirection direction, byte speed)
        {
            if (speed < 2)
            {
                speed = 2;
            }
            else if (speed > 255)
            {
                speed = 255;
            }

            try
            {
                darkExplorerPort.Write(new char[] { 'r' }, 0, 1);
                darkExplorerPort.Write(new byte[] { (byte)direction }, 0, 1);
                if (EnginesDirection.Stop != direction)
                {
                    darkExplorerPort.Write(new char[] { 'b' }, 0, 1);
                    darkExplorerPort.Write(new byte[] { speed }, 0, 1);
                }
            }
            catch (Exception)
            {
                SetRightEnginesState(direction, speed);
            }
        }

        /// <summary>
        /// Metoda konfigurująca działanie lewych silników
        /// </summary>
        /// <param name="direction">Tryb pracy silników</param>
        /// <param name="speed">Moc silników</param>
        public void SetLeftEnginesState(EnginesDirection direction, byte speed)
        {
            if (speed < 2)
            {
                speed = 2;
            }
            else if (speed > 255)
            {
                speed = 255;
            }

            try
            {
                darkExplorerPort.Write(new char[] { 'l' }, 0, 1);
                darkExplorerPort.Write(new byte[] { (byte)direction }, 0, 1);
                if (EnginesDirection.Stop != direction)
                {
                    darkExplorerPort.Write(new char[] { 'a' }, 0, 1);
                    darkExplorerPort.Write(new byte[] { speed }, 0, 1);
                }
            }
            catch (Exception)
            {
                SetLeftEnginesState(direction, speed);
            }
        }

        /// <summary>
        /// Metoda zatrzymująca wszystkie silniki
        /// </summary>
        public void StopEngines()
        {
            if (darkExplorerPort.IsOpen)
            {
                try
                {
                    darkExplorerPort.Write(new char[] { 'l' }, 0, 1);
                    darkExplorerPort.Write(new byte[] { (byte)EnginesDirection.Stop }, 0, 1);
                    //darkExplorerPort.Write(new char[] { 'a' }, 0, 1);
                    //darkExplorerPort.Write(new byte[] { 2 }, 0, 1); 
                    darkExplorerPort.Write(new char[] { 'r' }, 0, 1);
                    darkExplorerPort.Write(new byte[] { (byte)EnginesDirection.Stop }, 0, 1);
                    //darkExplorerPort.Write(new char[] { 'b' }, 0, 1);
                    //darkExplorerPort.Write(new byte[] { speed }, 0, 1);
                }
                catch (Exception)
                {
                    StopEngines();
                }
            }
        }

        /// <summary>
        /// Metoda włączająca i wyłączająca diodę oświetleniową LED
        /// </summary>
        /// <param name="state"><code>true</code>jeżeli dioda ma zostać zapalona, <code>false</code> w przeciwnym wypadku</param>
        public void SetDiodeState(bool state)
        {
            if (darkExplorerPort.IsOpen)
            {
                darkExplorerPort.Write(new char[] { 'd' }, 0, 1);
                if (state)
                {
                    darkExplorerPort.Write(new byte[] { 1 }, 0, 1);
                }
                else
                {
                    darkExplorerPort.Write(new byte[] { 0 }, 0, 1);
                }
            }
        }

        /// <summary>
        /// Metoda wyłączająca serwomechanizm
        /// </summary>
        public void DisableServomechanism()
        {
        }

        /// <summary>
        /// Metoda włączająca serwomechanizm i ustawiająca podaną jako parametr pozycję
        /// </summary>
        /// <param name="position"></param>
        public void SetSerwoPosition(byte position)
        {
            darkExplorerPort.Write(new char[] { 's' }, 0, 1);
            darkExplorerPort.Write(new byte[] { position }, 0, 1);
        }

        /// <summary>
        /// Metoda wysyłająca rządanie pobrania danych o stanie naładownia baterii
        /// </summary>
        public void SendPowerLevelRequest()
        {
            DataRequestCommand command = new DataRequestCommand(RequestType.Sensors, 2, 1);
            command.Request = new RequestSender(RequestPowerData);
            cmdQueue.Add(command);

            ProcessRequests();
        }



        private void SendCameraDataRequest(ImageQuality quality)
        {
            DataRequestCommand command = new CameraDataRequestCommand(quality);
            command.Request = new RequestSender(RequestCameraData);
            cmdQueue.Add(command);

            ProcessRequests();
        }

        protected void ProcessRequests()
        {
            if ( cmdQueue.Count > 0)
            {
                processReq = true;
                cmdQueue[0].Request(cmdQueue[0]);

                if (isMono)
                {
                    (new Thread(new ThreadStart(MonitorIncomingData))).Start();
                }
            }
        }

        private void RequestCameraData(DataRequestCommand cmd)
        {
            CameraDataRequestCommand cdrCmd = cmd as CameraDataRequestCommand;
            darkExplorerPort.Write(new char[] { cdrCmd.Quality.GetImageQualityAttributes().Prefix }, 0, 1);
            darkExplorerPort.Write(new byte[] { (byte)(cdrCmd.Quality.GetImageQualityAttributes().TotalPacketCount - cmd.PacketCount) }, 0, 1);
        }

        private void RequestPowerData(DataRequestCommand cmd)
        {
            try
            {
                darkExplorerPort.Write(new char[] { 't' }, 0, 1);
                darkExplorerPort.Write(new byte[] { 0 }, 0, 1);
            }
            catch (Exception)
            {

                RequestPowerData(cmd);
            }
        }

        /// <summary>
        /// Metoda otwierająca połączenie umożliwiające komunikacje z robotem
        /// </summary>
        public void OpenConnection()
        {
            darkExplorerPort.Open();
            darkExplorerPort.BaudRate = 115200;
            darkExplorerPort.StopBits = StopBits.One;
            darkExplorerPort.Parity = Parity.None;
            darkExplorerPort.DataBits = 8;
            darkExplorerPort.Handshake = Handshake.RequestToSend;
            //darkExplorerPort.ReadTimeout = SerialPort.InfiniteTimeout;
            //darkExplorerPort.WriteTimeout = SerialPort.InfiniteTimeout;
            //darkExplorerPort.WriteTimeout = 200;
            //darkExplorerPort.ReadTimeout = 200;

            (new Thread(new ThreadStart(MonitorConnectionInit))).Start();
        }

        private void MonitorConnectionInit()
        {
            Console.WriteLine("MCI");
            while (!darkExplorerPort.IsOpen)
            {
                Thread.Sleep(250);
            }

            OnConnectionOpened(new ConnectionOpenedEventArgs(darkExplorerPort));
        }

        private void MonitorConnectionClose()
        {
            Console.WriteLine("MCC");
            while (darkExplorerPort.IsOpen)
            {
                Thread.Sleep(250);
            }

            OnConnectionClosed(new ConnectionClosedEventArgs(darkExplorerPort));
        }

        protected void MonitorIncomingData()
        {
            do
            {
                Thread.Sleep(250);
                Console.WriteLine("MID: " + darkExplorerPort.BytesToRead);
            } while (darkExplorerPort.BytesToRead == 0);

            ProcessReceivedData();
        }

        /// <summary>
        /// Metoda zamykająca port na którym odbywa się komunikacja
        /// </summary>
        public void CloseConnection()
        {
            darkExplorerPort.Close();

            (new Thread(new ThreadStart(MonitorConnectionClose))).Start();
        }

        /// <summary>
        /// Metoda zwracająca informacje ta temat gotowości portu do transmisji danych
        /// </summary>
        public bool IsConnectionActive
        {
            get { return darkExplorerPort.IsOpen; }
        }
    }
}
