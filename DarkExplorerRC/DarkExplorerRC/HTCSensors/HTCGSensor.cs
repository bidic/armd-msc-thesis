using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using Microsoft.WindowsMobile.Status;

namespace HTCSensors
{
    public class HTCGSensor : IDisposable
    {
        #region HTCSensorSDK
        enum HTCSensor : uint
        {
            GSensor = 1
        }

        // The following PInvokes were ported from the results of the reverse engineering done
        // by Scott at scottandmichelle.net.
        // Blog post: http://scottandmichelle.net/scott/comments.html?entry=784
        [DllImport("HTCSensorSDK")]
        extern static IntPtr HTCSensorOpen(HTCSensor sensor);

        [DllImport("HTCSensorSDK")]
        extern static void HTCSensorClose(IntPtr handle);

        [DllImport("HTCSensorSDK")]
        extern static IntPtr HTCSensorGetDataOutput(IntPtr handle, out HTCGSensorData sensorData);
        #endregion

        #region CoreDll
        [DllImport("coredll")]
        extern static bool CloseHandle(IntPtr handle);

        [DllImport("coredll", SetLastError = true)]
        extern static IntPtr CreateEvent(IntPtr eventAttributes, bool manualReset, bool intialState, string name);

        [DllImport("coredll", SetLastError = true)]
        extern static bool EventModify(IntPtr handle, uint func);

        static bool SetEvent(IntPtr handle)
        {
            return EventModify(handle, 3);
        }
        #endregion CoreDll

        #region IDisposable Members
        public void Dispose()
        {
            if (sensorHandle != IntPtr.Zero)
            {
                HTCSensorClose(sensorHandle);
                sensorHandle = IntPtr.Zero;
            }
            using (sensorOrientationState)
            {
                sensorOrientationState.Changed -= new ChangeEventHandler(sensorOrientationState_Changed);
            }
            sensorOrientationState = null;
            ScreenOrientationChanged = null;

            IntPtr hEvent = CreateEvent(IntPtr.Zero, true, false, "HTC_GSENSOR_SERVICESTOP");
            SetEvent(hEvent);
            CloseHandle(hEvent);
        }
        #endregion

        IntPtr sensorHandle = HTCSensorOpen(HTCSensor.GSensor);
        RegistryState sensorOrientationState = new RegistryState(@"HKEY_LOCAL_MACHINE\Software\HTC\HTCSensor\GSensor", "EventChanged");
        public event EventHandler ScreenOrientationChanged;

        public HTCGSensor()
        {
            IntPtr hEvent = CreateEvent(IntPtr.Zero, true, false, "HTC_GSENSOR_SERVICESTART");
            SetEvent(hEvent);
            CloseHandle(hEvent);

            sensorOrientationState.Changed += new ChangeEventHandler(sensorOrientationState_Changed);
        }

        private void sensorOrientationState_Changed(object sender, ChangeEventArgs args)
        {
            OnScreenOrientationChanged(new HTCGSensorDataArgs(Orientation, RawSensorData));
        }

        private void OnScreenOrientationChanged(HTCGSensorDataArgs e)
        {
            if (ScreenOrientationChanged != null)
            {
                ScreenOrientationChanged(this, e);
            }
        }

        public HTCGSensorData RawSensorData
        {
            get
            {
                HTCGSensorData data;
                HTCSensorGetDataOutput(sensorHandle, out data);
                return data;
            }
        }

        public ScreenOrientation Orientation
        {
            get
            {
                return (ScreenOrientation)((int)sensorOrientationState.CurrentValue & 0xF);
            }
        }

        public GVector GVector
        {
            get
            {
                GVector ret = new GVector();
                HTCGSensorData data = RawSensorData;
                ret.X = data.TiltX;
                ret.Y = data.TiltY;
                ret.Z = data.TiltZ;
                // HTC's Sensor returns a vector which is around 1000 in length on average..
                // but it really depends on how the device is oriented.
                // When simply face up, my Diamond returns a vector of around 840 in length.
                // While face down, it returns a vector of around 1200 in length.
                // The vector direction is fairly accurate, however, the length is clearly not extremely precise.
                // double htcScaleFactor = 1.0 / 1000.0 * 9.8;
                double htcScaleFactor = 1.0;
                return ret.Scale(htcScaleFactor);
            }
        }
    }
}
