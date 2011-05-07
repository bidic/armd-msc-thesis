using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DarkExplorerSDK
{
    /// <summary>
    /// Klasa reprezentująca żądania wysyłane do robota za pośrednictwem bluetooth
    /// </summary>
    public class Request
    {
        /// <summary>
        /// Unikalny typ polecenia związanego z rządaniem
        /// </summary>
        public RequestType CommandType { get; set; }
        /// <summary>
        /// Flaga informująca o konieczności potwierdzenia odebrania pakietu
        /// </summary>
        public bool RequireAcknowledgement { get; set; }
        /// <summary>
        /// Flaga informująca o tym czy polecenie posiada sekcje rozszrzającą parametry podstawowe
        /// </summary>
        public bool HasExtendedData { get; set; }
        /// <summary>
        /// Maska bitowa parametrów polecenia
        /// </summary>
        public byte ParamMask { get; set; }
        /// <summary>
        /// Dane parametrów rozszerzonych polecenia
        /// </summary>
        public byte[] ExtendedParamData { get; set; }
    }

    /// <summary>
    /// Klasa reprezntująca odpowiedź wysłaną przez robota
    /// </summary>
    public class Response
    {
        /// <summary>
        /// Nagłówek odpowiedzi
        /// </summary>
        public ResponseHeader Header { get; set; }
        /// <summary>
        /// Dane przesyłane w ramach sekcji DATA
        /// </summary>
        public byte[] ResponseData { get; set; }
    }

    /// <summary>
    /// Klasa reprezentująca nagłówek odpowiedzi robota
    /// </summary>
    public class ResponseHeader
    {
        /// <summary>
        /// Unikalny typ polecenia związanego z rządaniem
        /// </summary>
        public RequestType CommandType { get; set; }
        /// <summary>
        /// Flaga informująca o wystąpieniu błędów podczas realizacji polecenia
        /// </summary>
        public bool HasErrorsOccured { get; set; }
        /// <summary>
        /// Flaga informująca o końcu transmisji
        /// </summary>
        public bool IsEndOfTransmission { get; set; }
        /// <summary>
        /// Ilość danych przesyłanych w sekcji DATA
        /// </summary>
        public byte DataSize { get; set; }
    }

    /// <summary>
    /// Klasa umożliwiająca konwersję pomiędzy instancjami obiektów a strumieniem bajtów do przesłania
    /// </summary>
    public class CommunicationProcessor
    {
        /// <summary>
        /// Metoda kodująca żądanie do postaci tablicy bajtów która może zostać przesłana bezpośrednio do robota
        /// </summary>
        /// <param name="request">Instancja obiektu żądnia</param>
        /// <returns>Tablica bajtów z zakodowanym żądaniem do przesłania</returns>
        public static byte[] EncodeRequest(Request request)
        {
            return null;
        }

        /// <summary>
        /// Metoda dekodująca strumień bajtów przesłanych przez robota jako nagłówek odpowiedzi
        /// </summary>
        /// <param name="headerStream">Tablica bajtów z nagłówkiem odpowiedzi</param>
        /// <returns>Nagłówek odpowiedzi po zdekodowaniu</returns>
        public static ResponseHeader DecodeResponseHeader(byte[] headerStream)
        {
            return null;
        }
    }
}
