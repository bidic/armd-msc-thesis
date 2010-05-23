using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using HTCSensors;
using DarkExplorerSDK;
//using Microsoft.WindowsCE.Forms;


namespace DarkExplorerRC
{
    public partial class Form1 : Form
    {

        HTCGSensor GSensor;
        DarkExplorer robot;
        public Form1()
        {
            InitializeComponent();
            GSensor = new HTCGSensor();
            GSensor.ScreenOrientationChanged += new EventHandler(GSensor_ScreenOrientationChanged);
        }

        void GSensor_ScreenOrientationChanged(object sender, EventArgs e)
        {
            HTCGSensorDataArgs data = e as HTCGSensorDataArgs;
            //statusBar1.Text = data.Orientation + " && ";
        }

    

        private void exitMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void connOpenMenuItem_Click(object sender, EventArgs e)
        {
            statusLabel.Text = "Opening connection...";
            robot = new DarkExplorer("COM1");
            robot.OpenConnection();
            robot.ConnectionOpened += new EventHandler(robot_ConnectionOpened);
            robot.ConnectionClosed += new EventHandler(robot_ConnectionClosed);
            robot.PowerLevelReceived += new EventHandler(robot_PowerLevelReceived);
            robot.CameraDataReceived += new EventHandler(robot_CameraDataReceived);
            robot.DataPacketReceived += new EventHandler(robot_DataPacketReceived);
        }

        void robot_DataPacketReceived(object sender, EventArgs e)
        {
            DataPacketReceivedEventArgs ev = e as DataPacketReceivedEventArgs;
            BeginInvoke((Action)delegate() { statusLabel.Text = "Received " + ev.BytesReceived; });
        }

        void robot_CameraDataReceived(object sender, EventArgs e)
        {
            CameraDataArgs ev = e as CameraDataArgs;
            BeginInvoke((Action)delegate() { statusLabel.Text = "Picture received."; });
            BeginInvoke((Action)delegate() { cameraView.Image = ev.BmpImage; });
        }

        void robot_ConnectionClosed(object sender, EventArgs e)
        {
            BeginInvoke((Action)delegate() { statusLabel.Text = "Connection closed."; });
            BeginInvoke((Action)delegate() { connCloseMenuItem.Enabled = false; });
            BeginInvoke((Action)delegate() { connOpenMenuItem.Enabled = true; });
            BeginInvoke((Action)delegate() { powerLevelTimer.Enabled = false; });
            BeginInvoke((Action)delegate() { servoMenuItem.Enabled = false; });
            BeginInvoke((Action)delegate() { diodeMenuItem.Enabled = false; });
            BeginInvoke((Action)delegate() { pictureQualityMenuItem.Enabled = false; });
        }

        void robot_PowerLevelReceived(object sender, EventArgs e)
        {
            PowerLevelArgs ev = e as PowerLevelArgs;
            SetControlPropertyValue(powerLevelBar, "Value", Math.Max(0, (int)(((ev.Voltage - 6.0) / 2.5) * 100)));
        }

        void robot_ConnectionOpened(object sender, EventArgs e)
        {
            BeginInvoke((Action)delegate() { statusLabel.Text = "Connection established."; });
            BeginInvoke((Action) delegate() { connCloseMenuItem.Enabled = true; });
            BeginInvoke((Action) delegate() { connOpenMenuItem.Enabled = false; });
            BeginInvoke((Action) delegate() { powerLevelTimer.Enabled = true; });
            BeginInvoke((Action)delegate() { servoMenuItem.Enabled = true; });
            BeginInvoke((Action)delegate() { diodeMenuItem.Enabled = true; });
            BeginInvoke((Action)delegate() { pictureQualityMenuItem.Enabled = true; });
            robot.SendPowerLevelRequest();
        }

        private void powerLevelTimer_Tick(object sender, EventArgs e)
        {
            if (robot.IsConnectionActive)
            {
                robot.SendPowerLevelRequest();
            }
        }

        


        delegate void SetControlValueCallback(Control oControl, string propName, object propValue);
        private void SetControlPropertyValue(Control oControl, string propName, object propValue)
        {
            if (oControl.InvokeRequired)
            {
                SetControlValueCallback d = new SetControlValueCallback(SetControlPropertyValue);
                oControl.Invoke(d, new object[] { oControl, propName, propValue });
            }
            else
            {
                Type t = oControl.GetType();
                PropertyInfo[] props = t.GetProperties();
                foreach (PropertyInfo p in props)
                {
                    if (p.Name.ToUpper() == propName.ToUpper())
                    {
                        p.SetValue(oControl, propValue, null);
                    }
                }
            }
        }

        private void lowQualityMenuItem_Click(object sender, EventArgs e)
        {
            lowQualityMenuItem.Checked = true;
            lowColorQualityMenuItem.Checked = false;
            highQualityMenuItem.Checked = false;
        }

        private void lowColorQualityMenuItem_Click(object sender, EventArgs e)
        {
            lowQualityMenuItem.Checked = false;
            lowColorQualityMenuItem.Checked = true;
            highQualityMenuItem.Checked = false;
        }

        private void highQualityMenuItem_Click(object sender, EventArgs e)
        {
            lowQualityMenuItem.Checked = false;
            lowColorQualityMenuItem.Checked = false;
            highQualityMenuItem.Checked = true;
        }

        private void enableDiodeMenuItem_Click(object sender, EventArgs e)
        {
            robot.SetDiodeState(true);
            enableDiodeMenuItem.Enabled = false;
            disableDiodeMenuItem.Enabled = true;
        }

        private void disableDiodeMenuItem_Click(object sender, EventArgs e)
        {
            robot.SetDiodeState(false);
            enableDiodeMenuItem.Enabled = true;
            disableDiodeMenuItem.Enabled = false;
        }

        private void enableServoMenuItem_Click(object sender, EventArgs e)
        {
            robot.SetSerwoState(true);
            robot.SetSerwoPosition(0);
            enableServoMenuItem.Enabled = false;
            disableServoMenuItem.Enabled = true;
            servoPositionMenuItem.Enabled = true;
        }

        private void disableServoMenuItem_Click(object sender, EventArgs e)
        {
            robot.SetSerwoState(false);
            robot.SetSerwoPosition(0);
            enableServoMenuItem.Enabled = true;
            disableServoMenuItem.Enabled = false;
            servoPositionMenuItem.Enabled = false;
        }

        private void postion0MenuItem_Click(object sender, EventArgs e)
        {
            robot.SetSerwoPosition(0);
            postion0MenuItem.Checked = true;
            position45MenuItem.Checked = false;
            position90MenuItem.Checked = false;
        }

        private void position45MenuItem_Click(object sender, EventArgs e)
        {
            robot.SetSerwoPosition(225);
            postion0MenuItem.Checked = false;
            position45MenuItem.Checked = true;
            position90MenuItem.Checked = false;
        }

        private void position90MenuItem_Click(object sender, EventArgs e)
        {
            robot.SetSerwoPosition(190);
            postion0MenuItem.Checked = false;
            position45MenuItem.Checked = false;
            position90MenuItem.Checked = true;
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            if ((e.KeyCode == System.Windows.Forms.Keys.Up) || (e.KeyCode == System.Windows.Forms.Keys.Down) || (e.KeyCode == System.Windows.Forms.Keys.Left) || (e.KeyCode == System.Windows.Forms.Keys.Right))
            {
                robot.StopEngines();
            }
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {

            if ((e.KeyCode == System.Windows.Forms.Keys.Up))
            {
                robot.SetLeftEnginesState(EnginesDirection.Straight, 255);
                robot.SetRightEnginesState(EnginesDirection.Straight, 255);
                
            }
            if ((e.KeyCode == System.Windows.Forms.Keys.Down))
            {
                robot.SetLeftEnginesState(EnginesDirection.Back, 255);
                robot.SetRightEnginesState(EnginesDirection.Back, 255);
            }
            if ((e.KeyCode == System.Windows.Forms.Keys.Left))
            {
                robot.SetLeftEnginesState(EnginesDirection.Back, 255);
                robot.SetRightEnginesState(EnginesDirection.Straight, 255);
            }
            if ((e.KeyCode == System.Windows.Forms.Keys.Right))
            {
                robot.SetLeftEnginesState(EnginesDirection.Straight, 255);
                robot.SetRightEnginesState(EnginesDirection.Back, 255);
            }
            if ((e.KeyCode == System.Windows.Forms.Keys.Enter))
            {
                if (lowQualityMenuItem.Checked)
                {
                    statusLabel.Text = "Picture requested [LowBW].";
                    robot.SendCameraDataRequest(ImageQuality.LowBW);
                }
                else if (lowColorQualityMenuItem.Checked)
                {
                    statusLabel.Text = "Picture requested [LowColor].";
                    robot.SendCameraDataRequest(ImageQuality.LowColor);
                }
                else if (highQualityMenuItem.Checked)
                {
                    statusLabel.Text = "Picture requested [HighBW].";
                    robot.SendCameraDataRequest(ImageQuality.HighBW);
                }
            }
            
        }

        private void connCloseMenuItem_Click(object sender, EventArgs e)
        {
            robot.CloseConnection();
        }

        private void sensoreProbeTimer_Tick(object sender, EventArgs e)
        {
            GVector gvector = GSensor.GVector;
            HTCGSensorData data = GSensor.RawSensorData;
            //label2.Text = "x: " + gvector.X + ", y: " + gvector.Y + ", z: " + gvector.Z;
            //label1.Text = "AngleX: " + data.AngleX + ", AngleY: " + data.AngleY;
            //DarkExplorer.EnginesDirection dir;

            EnginesDirection dir = (data.AngleX > 270) || (data.AngleX < 90) ? EnginesDirection.Straight : EnginesDirection.Back;

            if (Math.Abs(data.AngleX - 270) < 20)
            {
                dir = EnginesDirection.Stop;
            }

            //label3.Text = dir.ToString();
            //int valueX = gvector.X;
            byte speedX = 255;
            if (Math.Abs(gvector.X) > 600)
            {
                speedX = (byte)(((1000 - Math.Abs(gvector.X)) * 255) / 400);
            }
            //byte speedY = (byte)Math.Abs(gvector.Y * 255 * 0.001);
            byte speedY = 0;
            if (Math.Abs(gvector.Y) > 350)
            {
                speedY = (byte)(speedX * 0.5);
            }
            else if (Math.Abs(gvector.Y) > 200)
            {
                speedY = (byte)(speedX * 0.9);
            }

            //label4.Text = "SpeedX: " + speedX + ", SpeedY: " + speedY;

            if (robot.IsConnectionActive)
            {
                if (dir == EnginesDirection.Stop)
                {
                    robot.StopEngines();
                }
                else
                {
                    if (gvector.Y < 0)
                    {

                        if (Math.Abs(gvector.Y) < 300)
                        {
                            robot.SetLeftEnginesState(dir, speedX);
                            robot.SetRightEnginesState(dir, (byte)(speedX - speedY));
                        }
                        else
                        {
                            robot.SetLeftEnginesState(dir, (byte)speedX);
                            robot.SetRightEnginesState(dir == EnginesDirection.Back ? EnginesDirection.Straight : EnginesDirection.Back, speedY);
                        }
                    }
                    else
                    {
                        if (Math.Abs(gvector.Y) < 300)
                        {
                            robot.SetLeftEnginesState(dir, (byte)(speedX - speedY));
                            robot.SetRightEnginesState(dir, speedX);
                        }
                        else
                        {
                            robot.SetRightEnginesState(dir, (byte)speedX);
                            robot.SetLeftEnginesState(dir == EnginesDirection.Back ? EnginesDirection.Straight : EnginesDirection.Back, speedY);
                        }
                    }
                }
            }
        }
    }
}