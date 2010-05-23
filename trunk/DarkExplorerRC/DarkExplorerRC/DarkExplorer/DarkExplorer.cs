using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Threading;


namespace DarkExplorerSDK
{
    public enum EnginesDirection
    {
        Stop = 0,
        Straight = 1,
        Back = 2
    }

    class DarkExplorer
    {
        public event EventHandler DataPacketReceived;
        public event EventHandler PowerLevelReceived;
        public event EventHandler CameraDataReceived;
        public event EventHandler ConnectionOpened;
        public event EventHandler ConnectionClosed;

        protected bool isMono = Type.GetType("Mono.Runtime") != null;
        protected bool processReq = false;

        protected List<DataRequestCommand> cmdQueue = new List<DataRequestCommand>();
        protected SerialPort darkExplorerPort;



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
                                case PacketRequestType.PowerLevel:
                                    OnPowerLevelReceived(new PowerLevelArgs(cmdQueue[0].DataBuffer));
                                    break;

                                case PacketRequestType.CameraData:
                                    CameraDataRequestCommand cameraCommand = cmdQueue[0] as CameraDataRequestCommand;
                                    OnCameraDataReceived(new CameraDataArgs(cameraCommand.DataBuffer, cameraCommand.Quality));
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



        protected void OnCameraDataReceived(CameraDataArgs e)
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

        protected void OnPowerLevelReceived(PowerLevelArgs e)
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

            darkExplorerPort.Write(new char[] { 'r' }, 0, 1);
            darkExplorerPort.Write(new byte[] { (byte)direction }, 0, 1);
            if (EnginesDirection.Stop != direction)
            {
                darkExplorerPort.Write(new char[] { 'b' }, 0, 1);
                darkExplorerPort.Write(new byte[] { speed }, 0, 1);
            }
        }

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

            darkExplorerPort.Write(new char[] { 'l' }, 0, 1);
            darkExplorerPort.Write(new byte[] { (byte)direction }, 0, 1);
            if (EnginesDirection.Stop != direction)
            {
                darkExplorerPort.Write(new char[] { 'a' }, 0, 1);
                darkExplorerPort.Write(new byte[] { speed }, 0, 1);
            }
        }

        public void StopEngines()
        {
            if (darkExplorerPort.IsOpen)
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
        }

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

        public void SetSerwoState(bool state)
        {
            darkExplorerPort.Write(new char[] { 'w' }, 0, 1);
            if (state)
            {
                darkExplorerPort.Write(new byte[] { 1 }, 0, 1);
            }
            else
            {
                darkExplorerPort.Write(new byte[] { 0 }, 0, 1);
            }
        }

        public void SetSerwoPosition(byte position)
        {
            darkExplorerPort.Write(new char[] { 's' }, 0, 1);
            darkExplorerPort.Write(new byte[] { position }, 0, 1);
        }

        public void SendPowerLevelRequest()
        {
            DataRequestCommand command = new DataRequestCommand(PacketRequestType.PowerLevel, 2, 1);
            command.Request = new RequestSender(RequestPowerData);
            cmdQueue.Add(command);

            ProcessRequests();
        }

        public void SendCameraDataRequest(ImageQuality quality)
        {
            DataRequestCommand command = new CameraDataRequestCommand(quality);
            command.Request = new RequestSender(RequestCameraData);
            cmdQueue.Add(command);

            ProcessRequests();
        }

        protected void ProcessRequests()
        {
            if (!processReq && cmdQueue.Count > 0)
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

        public void OpenConnection()
        {
            darkExplorerPort.Open();
            darkExplorerPort.BaudRate = 115200;
            darkExplorerPort.StopBits = StopBits.One;
            darkExplorerPort.Parity = Parity.None;
            darkExplorerPort.DataBits = 8;
            darkExplorerPort.ReadTimeout = SerialPort.InfiniteTimeout;
            darkExplorerPort.WriteTimeout = SerialPort.InfiniteTimeout;
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

        public void CloseConnection()
        {
            darkExplorerPort.Close();

            (new Thread(new ThreadStart(MonitorConnectionClose))).Start();
        }

        public bool IsConnectionActive
        {
            get { return darkExplorerPort.IsOpen; }
        }
    }
}
