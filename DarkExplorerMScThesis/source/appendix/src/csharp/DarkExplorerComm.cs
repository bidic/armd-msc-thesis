/// Klasa reprezentujaca polecenia wysylane 
/// do robota za posrednictwem bluetooth
public class Request {
	/// Unikalny typ polecenia zwiazanego z zadaniem
	public RequestType CommandType { get; set; }
	/// Flaga informujaca o koniecznosci potwierdzenia odebrania pakietu
	public bool RequireAcknowledgement { get; set; }
	/// Flaga informujaca o tym czy polecenie 
	/// posiada sekcje rozszerzajaca parametry podstawowe
	public bool HasExtendedData { get; set; }
	/// Maska parametrow polecenia
	public byte ParamMask { get; set; }
	/// Rozszerzone parametry polecenia
	public byte[] ExtendedParamData { get; set; }
}

/// Klasa reprezntujaca odpowiedz wyslana przez robota
public class Response {
	/// Naglowek odpowiedzi
	public ResponseHeader Header { get; set; }
	/// Dane przesylane w ramach sekcji DATA
	public byte[] ResponseData { get; set; }
}
/// Klasa reprezentujaca naglowek odpowiedzi robota
public class ResponseHeader {
	/// Unikalny typ polecenia zwiazanego z rzadaniem
	public RequestType CommandType { get; set; }
	/// Flaga informujaca o wystapieniu bledow
	/// podczas realizacji polecenia
	public bool HasErrorsOccured { get; set; }
	/// Flaga informujaca o koncu transmisji
	public bool IsEndOfTransmission { get; set; }
	/// Ilosc danych przesylanych w sekcji DATA
	public byte DataSize { get; set; }
}
/// Klasa umozliwiajaca konwersje pomiedzy instancjami 
/// obiektow a strumieniem bajtow do przeslania
public class CommunicationProcessor {
	/// Metoda kodujaca zadanie do postaci tablicy bajtow
	/// ktora moze zostac przeslana bezposrednio do robota
	/// <param name="request">Instancja obiektu zadnia</param>
	/// <returns>
	/// 	Tablica bajtow z zakodowanym zadaniem do przeslania
	/// </returns>
	public static byte[] EncodeRequest(Request request);
	/// Metoda dekodujaca strumien bajtow przeslanych 
	/// przez robota jako naglowek odpowiedzi
	/// <param name="headerStream">
	/// 	Tablica bajtow z naglowkiem odpowiedzi
	/// </param>
	/// <returns>Naglowek odpowiedzi po zdekodowaniu</returns>
	public static ResponseHeader DecodeResponseHeader(byte[] stream);
}