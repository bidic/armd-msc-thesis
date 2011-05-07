public class DarkExplorer 
{
	/// Obsluga zdarzenia odebrania danych nagranej sciezki
	public event EventHandler TrackDataReceived;
	///	Obsluga zdarzenia odebrania danych na temat temperatury
	public event EventHandler TemperatureDataReceived;
	/// Obsluga zdarzenia odebrania danych pomiarowych 
	/// z akcelerometru
	public event EventHandler AccelerationDataReceived;
	/// Obsluga zdarzenia odebrania danych pomiarowych 
	/// z dalmierzy
	public event EventHandler RangefinderDataReceived;
	///	Obsluga zdarzenia odebrania danych pomiarowych 
	///	z magnetometru
	public event EventHandler MagnetometerDataReceived;
	///	Obsluga zdarzenia odebrania danych 
	///	pomiarowych z zyroskopu
	public event EventHandler GyroscopeDataReceived;
	///	Obsluga zdarzenia odebrania rezultatow 
	///	przeprowadzonej analizy obrazu
	public event EventHandler DetectionResultReceived;
	///	Obsluga zdarzenia odebrania potwierdzenia realizacji komendy
	public event EventHandler AcknowledgementReceived;
	///	Obsluga zdarzenia odebrania danych przeslanych z robota
	public event EventHandler DataPacketReceived;
	///	Obsluga zdarzenia odebranie danych o baterii
	public event EventHandler PowerLevelReceived;
	///	Obsluga zdarzenia odebrania danych z kamery
	public event EventHandler CameraDataReceived;
	///	Obsluga zdarzenia otwarcia polaczenia
	public event EventHandler ConnectionOpened;
	///	Obsluga zdarzenia zamkniecia polaczenia
	public event EventHandler ConnectionClosed;


	/// Domyslny konstruktor
	/// <param name="port">
	/// 	Identyfikator portu na ktorym odbywac sie bedzie komunikacja
	/// </param>
	public DarkExplorer(string port);
	
	/// Metoda wysyla zadanie pobrania danych 
	/// z czujnika podanego jako parametr
	/// <param name="type">
	/// 	Typ czujnika za pomoca ktorego 
	/// 	ma zostac przeprowadzony pomiar
	/// </param>
	public void RequestSensorData(SensorType type);
	/// Metoda wysyla zadanie przeprowadzenia 
	/// kalibracji czujnika podanego jako parametr
	/// <param name="type">
	/// 	Typ czujnika dla ktorego ma zostac przeprowadzona kalibracja
	/// </param>
	public void RequestSensoreCalibartion(SensorType type);
	/// Metoda wysylajaca do robota komende obslugi 
	/// trybu rekonstrukcji sciezki powrotnej
	/// <param name="command">
	/// 	Komenda rekonstrukcji sciezki powrotnej
	/// </param>
	public void SendTrackReconstructionCommand(TrackReconstructionCommand c);
	/// Metoda wysylajaca do robota komede 
	/// obslugi trybu autonomicznego
	/// <param name="command">
	/// 	Komenda obslugi trybu autonomicznego
	/// </param>
	public void SendAutonomousModeCommand(AutonomousModeCommand command);
	/// Metoda konfigurujaca dzialanie prawych silnikow
	/// <param name="direction">Tryb pracy silnikow</param>
	/// <param name="speed">Moc silnikow</param>
	public void SetRightEnginesState(EnginesDirection direction, byte speed);
	/// Metoda konfigurujaca dzialanie lewych silnikow
	/// <param name="direction">Tryb pracy silnikow</param>
	/// <param name="speed">Moc silnikow</param>
	public void SetLeftEnginesState(EnginesDirection direction, byte speed);
	/// Metoda zatrzymujaca silniki
	public void StopEngines();	
	/// Metoda wlaczajaca i wylaczajaca diode oswietleniowa LED
	/// <param name="state"> 
	/// 	stan diody (true - wlaczona, false - wylaczona)
	/// </param>
	public void SetDiodeState(bool state);
	/// Metoda wylaczajaca serwomechanizm
	public void DisableServomechanism();
	/// Metoda wlaczajaca serwomechanizm i ustawiajaca 
	/// podana jako parametr pozycje
	/// <param name="position">Pozycja serwomechanizmu</param>
	public void SetSerwoPosition(byte position);
	///	Metoda wysylajaca zadanie pobrania danych 
	///	o stanie naladownia baterii
	public void SendPowerLevelRequest();
	///	Metoda otwierajaca polaczenie z robotem 
	public void OpenConnection();	
	///	Metoda zamykajaca port na ktorym odbywa sie komunikacja
	public void CloseConnection();
	/// Metoda zwracajaca informacje ta temat 
	/// gotowosci portu do transmisji danych
	public bool IsConnectionActive;
}