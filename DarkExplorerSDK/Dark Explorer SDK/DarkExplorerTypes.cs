using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace DarkExplorerSDK
{
    /// <summary>
    /// 
    /// </summary>
    [AttributeUsage(AttributeTargets.Field, AllowMultiple = false, Inherited = true)]
    public class EnumAttribute : Attribute
    {
        /// <summary>
        /// 
        /// </summary>
        public EnumAttribute()
        {
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public static class EnumExtension
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static ImageQualityAttribute GetImageQualityAttributes(this ImageQuality value)
        {
            Type type = value.GetType();
            FieldInfo fieldInfo = type.GetField(value.ToString());
            var atts = (ImageQualityAttribute[])fieldInfo.GetCustomAttributes(typeof(ImageQualityAttribute), false);
            return atts.Length > 0 ? atts[0] : null;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class ImageQualityAttribute : EnumAttribute
    {
        /// <summary>
        /// 
        /// </summary>
        public char Prefix { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int TotalPacketCount { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int Width { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int Height { get; set; }
    }

    /// <summary>
    /// Typ wyliczeniowy reprezentujący parametry pracy kamery robota
    /// </summary>
    public enum ImageQuality
    {
        /// <summary>
        /// 
        /// </summary>
        [ImageQualityAttribute(Prefix = 'p', TotalPacketCount = 5, Width = 160, Height = 100)]
        LowBW,
        /// <summary>
        /// 
        /// </summary>
        [ImageQualityAttribute(Prefix = 'x', TotalPacketCount = 10, Width = 160, Height = 100)]
        LowColor,
        /// <summary>
        /// 
        /// </summary>
        [ImageQualityAttribute(Prefix = 'f', TotalPacketCount = 20, Width = 320, Height = 200)]
        HighBW
    }

    /// <summary>
    /// Typ wyliczeniowy reprezentujący rozmiar obrazu jaki ma zostać pobrany z kamery
    /// </summary>
    public enum ImageSize
    {
        /// <summary>
        /// Obrazek mały rozmiar 160x100
        /// </summary>
        Small = 0,
        /// <summary>
        /// Obrazek średni rozmiar 320x200
        /// </summary>
        Medium = 1,
        /// <summary>
        /// Obrazek duży rozmiar 640x400
        /// </summary>
        High = 2
    }

    /// <summary>
    /// Typ wyliczeniowy reprezentujący tryb pracy silników
    /// </summary>
    public enum EnginesDirection
    {
        /// <summary>
        /// Tryb postoju
        /// </summary>
        Stop = 0,

        /// <summary>
        /// Tryb jazdy do przodu
        /// </summary>
        Straight = 1,

        /// <summary>
        /// Tryb jazdy wstecz
        /// </summary>
        Back = 2
    }

    /// <summary>
    /// Typ wyliczeniowy reprezentujący typy czujników
    /// </summary>
    public enum SensorType
    {
        /// <summary>
        /// Pomiar napięcia na baterii
        /// </summary>
        Battery = 0,
        /// <summary>
        /// Pomiar temperatury za pomocą termometru wbudowanego w żyroskop
        /// </summary>
        Thermometer = 1,
        /// <summary>
        /// Pomiar wartości przyspieszenia statycznego zmierzony za pomocą akcelerometru
        /// </summary>
        Accelerometer = 2,
        /// <summary>
        /// Pomiar odległości od najbliższej przeszkody wykonany za pomocą dalmierzy
        /// </summary>
        Rangefinder = 3,
        /// <summary>
        /// Pomiar orientacji względem bieguna magnetycznego wykonany za pomocą magnetometru
        /// </summary>
        Magnetometer = 4,
        /// <summary>
        /// Pomiar kąta obrotu wykonany za pomocą żyroskopu
        /// </summary>
        Gyroscope = 5
    }

    /// <summary>
    /// Typ wyliczeniowy reprezentujący polecenia obsługi trybu rekonstrukcji ścieżki
    /// </summary>
    public enum TrackReconstructionCommand
    {
        /// <summary>
        /// Komenda rozpoczynająca nagrywanie ścieżki
        /// </summary>
        StartRecordingTrack = 0,
        /// <summary>
        /// Komenda kończąca nagrywanie ścieżki
        /// </summary>
        StopRecordingTrack = 1,
        /// <summary>
        /// Komenda usuwająca informacje o nagranej ścieżce
        /// </summary>
        EraseRecordedTrack = 2,
        /// <summary>
        /// Komenda rozpoczynająca odtwarzanie nagranej ścieżki
        /// </summary>
        StartTrackReconstruction = 3,
        /// <summary>
        /// Komenda przerywająca odtwarzanie nagranej ścieżki
        /// </summary>
        StopTrackReconstruction = 4,
        /// <summary>
        /// Komenda wymuszająca przesłanie informacji o nagranej ścieżce
        /// </summary>
        SendTrackData = 5,
    }

    /// <summary>
    /// Typ wyliczeniowy reprezentujący polecenia obsługi trybu autonomicznego
    /// </summary>
    public enum AutonomousModeCommand
    {
        /// <summary>
        /// Komenda wyłączająca tryb autonomiczny
        /// </summary>
        DisableAutonomousMode = 0,
        /// <summary>
        /// Komenda uruchamiająca tryb autonomiczny z rozpoznawaniem kształtów
        /// </summary>
        EnableShapeDetection = 1,
        /// <summary>
        /// Komenda uruchamiająca tryb autonomiczny z lokalizowaniem twarzy
        /// </summary>
        EnableFaceDetection = 2,
        /// <summary>
        /// Komenda wymuszająca przesłanie współrzędnych wykrytego obiektu
        /// </summary>
        SendDetectionResult = 3,
        /// <summary>
        /// Komenda wymuszająca przesłanie obrazu zbinaryzowanego w wyniku przeprowadzonej analizy obrazu
        /// </summary>
        SendBinarizedImage = 4
    }

    /// <summary>
    /// Typ wyliczeniowy reprezentujący dostępne rodzaje poleceń sterujących
    /// </summary>
    public enum RequestType
    {
        /// <summary>
        /// Polecenie testujące połączenie (powitalne)
        /// </summary>
        Welcome = 0, 
        /// <summary>
        /// Polecenie sterowania silnikami
        /// </summary>
        Engines = 1,
        /// <summary>
        /// Polecenie sterowania serwomechanizmem
        /// </summary>
        Servomechanism = 2,
        /// <summary>
        /// Polecenie sterowania kamerą
        /// </summary>
        Camera = 3,
        /// <summary>
        /// Polecenie sterowania czujnikami
        /// </summary>
        Sensors = 4,
        /// <summary>
        /// Polecenie zarządzania trybem rekonstrukcji ścieżki
        /// </summary>
        TrackReconstruction = 5,
        /// <summary>
        /// Polecenie zarządzania trybem autonomicznym
        /// </summary>
        AutonomousMode = 6
    }
}
