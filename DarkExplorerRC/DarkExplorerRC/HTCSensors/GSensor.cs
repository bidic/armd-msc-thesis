using System;
using System.Collections.Generic;
using System.Text;

namespace HTCSensors
{
    public enum ScreenOrientation
    {
        Landscape = 0,
        ReverseLandscape = 1,
        Portrait = 2,
        ReversePortrait = 3, // upside down
        FaceDown = 4,
        FaceUp = 5
    }

    public struct HTCGSensorData
    {
        public short TiltX;     // From -1000 to 1000 (about), 0 is flat
        public short TiltY;     // From -1000 to 1000 (about), 0 is flat
        public short TiltZ;     // From -1000 to 1000 (about), 0 = Straight up, -1000 = Flat, 1000 = Upside down
        public short Unknown1;  // Always zero
        public int AngleY;      // From 0 to 359
        public int AngleX;      // From 0 to 359
        public int Unknown2;    // Bit field?
    };

    /// <summary>
    /// Returns a vector that desribes the direction of gravity/acceleration in relation to the device screen.
    /// When the device is face up on a flat surface, this method would return 0, 0, -9.8.
    /// The Z value of -9.8 would mean that the acceleration in the opposite direction of the orientation of the screen.
    /// When the device is held up, this method would return 0, -9.8, 0.
    /// The Y value of -9.8 would mean that the device is accelerating in the direction of the bottom of the screen.
    /// Conversely, if the device is held upside down, this method would return 0, 9.8, 0.
    /// </summary>
    /// <returns>
    /// The vector returned will have a length measured in the unit meters per second square.
    /// Ideally the when the device is in a motionless state, the vector would be of length 9.8.
    /// However, the sensor is not extremely accurate, so this almost never the case.
    /// </returns>
    public struct GVector
    {
        public GVector(double x, double y, double z)
        {
            myX = x;
            myY = y;
            myZ = z;
        }
        double myX;

        public double X
        {
            get { return myX; }
            set { myX = value; }
        }
        double myY;

        public double Y
        {
            get { return myY; }
            set { myY = value; }
        }
        double myZ;

        public double Z
        {
            get { return myZ; }
            set { myZ = value; }
        }

        public GVector Normalize()
        {
            return Scale(1 / Length);
        }

        public GVector Scale(double scale)
        {
            GVector ret = this;
            ret.myX *= scale;
            ret.myY *= scale;
            ret.myZ *= scale;
            return ret;
        }

        public double Length
        {
            get
            {
                return Math.Sqrt(myX * myX + myY * myY + myZ * myZ);
            }
        }

        public override string ToString()
        {
            return string.Format("X={0} Y={1} Z={2}", Math.Round(myX, 4), Math.Round(myY, 4), Math.Round(myZ, 4));
        }
    }

    public class HTCGSensorDataArgs : EventArgs
    {
        public ScreenOrientation Orientation { get; private set; }
        public HTCGSensorData GSensorData { get; private set; }

        public HTCGSensorDataArgs(ScreenOrientation Orientation, HTCGSensorData GSensorData)
        {
            this.Orientation = Orientation;
            this.GSensorData = GSensorData;
        }

        public GVector GetGVector()
        {
            GVector ret = new GVector();
            //HTCGSensorData data = GetRawSensorData();
            ret.X = GSensorData.TiltX;
            ret.Y = GSensorData.TiltY;
            ret.Z = GSensorData.TiltZ;
            // HTC's Sensor returns a vector which is around 1000 in length on average..
            // but it really depends on how the device is oriented.
            // When simply face up, my Diamond returns a vector of around 840 in length.
            // While face down, it returns a vector of around 1200 in length.
            // The vector direction is fairly accurate, however, the length is clearly not extremely precise.
             double htcScaleFactor = 1.0 / 1000.0 * 9.8;
            //double htcScaleFactor = 1.0;
            return ret.Scale(htcScaleFactor);
        }
    }
}
