/// Zdarzenie otrzymania danych prostokata ograniczajacego 
/// wyznaczonego w procesie analizy obrazu
public class DetectionResultReceivedEventArgs : EventArgs {
	///	Minimalna wspolrzedna X prostokata ograniczajacego
	public int MinX { get; private set; }
	///	Minimalna wspolrzedna Y prostokata ograniczajacego
	public int MinY { get; private set; }
	///	Maksymalna wspolrzedna X prostokata ograniczajacego
	public int MaxX { get; private set; }
	///	Maksymalna wspolrzedna Y prostokata ograniczajacego
	public int MaxY { get; private set; }
}
///	Zdarzenie otrzymania danych o nagranej sciezce
public class TrackDataReceivedEventArgs : EventArgs {
	///	Dane nagranej sciezki
	public int[] PathData { get; private set; }
}

///	Zdarzenie otrzymania danych o przyspieszeniu
public class AccelerationDataReceivedEventArgs : EventArgs {
	///	Wartosc przyspieszenia odczytana na osi X
	public double AxisX { get; private set; }
	///	Wartosc przyspieszenia odczytana na osi Y
	public double AxisY { get; private set; }
	///	Wartosc przyspieszenia odczytana na osi Z
	public double AxisZ { get; private set; }
}

/// Zdarzenie otrzymania danych o odleglosci 
/// od przeszkody
public class RangefinderDataReceivedEventArgs : EventArgs {
	///	Dane pomiarowe z lewego czujnika odleglosci
	public int LeftSensorData { get; private set; }
	///	Dane pomiarowe z prawego czujnika odleglosci
	public int RightSensorData { get; private set; }
}

///	Zdarzenie otrzymania danych pomiarowych z magnetometru
public class MagnetometerDataReceivedEventArgs : EventArgs {
	///	Skladowa X wektora indukcji pola magnetycznego
	public int ValueX { get; private set; }
	///	Skladowa Y wektora indukcji pola magnetycznego
	public int ValueY { get; private set; }
}

/// Zdarzenie otrzymania danych pomiarowych z zyroskopu
public class GyroscopeDataReceivedEventArgs : EventArgs {
	///	Kat obrotu wykryty na osi X
	public int AngleX { get; private set; }
	///	Kat obrotu wykryty na osi Y
	public int AngleY { get; private set; }
	///	Kat obrotu wykryty na osi Z
	public int AngleZ { get; private set; }
}

///	Zdarzenie otrzymania danych o temperaturze
public class TemperatureDataReceivedEventArgs : EventArgs {
	/// Wartosc temperatury odczytanej za pomoca robota
	public int Temperature { get; private set; }
}

///	Zdarzenie otrzymania potwierdzenia realizacji wyslanej komendy
public class AcknowledgementReceivedEventArgs : EventArgs {
	///	Rodzaj komendy ktorej dotyczy powtwierdzenie
	public RequestType Type { get; private set; }
	///	Rezultat zakonczenia akcji
	public Boolean State { get; private set; }
}

///	Zdarzenie otrzymania danych z kamery
public class CameraDataEventArgs : EventArgs {
	///	Strumien danych zdjecia wykonanego za pomoca robota
	public byte[] RawInputData { get; private set; }
	///	Obraz w postaci mapy bitowej
	public Bitmap BmpImage { get; private set; }
	///	Rozmiar obrazu
	public ImageSize Size { get; private set; }
}

/// Zdarzenie otrzymania danych o baterii
public class PowerLevelEventArgs : EventArgs {
	/// Wartosc napiecia zmierzona na bateriach
	public double Voltage { get; private set; }
	/// Poziom naladowania baterii
	public double PowerLevel { get; private set; }
}

/// Zdarzenie otwarcia polaczenia
public class ConnectionOpenedEventArgs : EventArgs {
	/// Port na ktorym polaczenie zostalo otwarte
	public SerialPort Port { get; private set; }
}

/// Zdarzenie zamkniecia polaczenia
public class ConnectionClosedEventArgs : EventArgs {
	/// Port dla ktorego polaczenie zostalo zamkniete
	public SerialPort Port { get; private set; }
}