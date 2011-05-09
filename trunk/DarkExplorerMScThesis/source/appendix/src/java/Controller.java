/**Klasa zarzadzajaca konkretnymi funkcjonalosciami robota */
public class Controller 
{
	/** Obiekt klasy zarzadzajacej polaczeniem bluetooth @see BTConnection */
	private BTConnection btconn;
	/** Aktualna pozycja wiezy */
	private int towerPosition;
	/** Aktualny stan diody oswietleniowej*/
	private boolean ledState;
	/** Os ukladu wspolrzednych */
	public enum Axis {X,Y,Z}
	/** Identyfikatory dalmierzy */
	public enum IR {IR_Left, IR_Right};

	/** Konstruktor */
	public Controller() throws IOException 

	/** Funkcja zwracajaca informacje o aktualnej pozycji wiezy z kamera 
	 * @return wartosc odchylenia wierzy od poziomu*/
	public int getTowerPosition()

	/** Zwraca informacje o stanie diody oswietleniowej
	 * @return stan diody oswietleniowej*/
	public boolean isLenOn()

	/** Zatrzymanie silnikow */
	public void stopEngines() throws IOException

	/** Poruszanie robotem do przodu */
	public void goForward() throws IOException

	/** Poruszanie robotem do tylu */
	public void goBackward() throws IOException

	/** Poruszanie robotem w lewo */
	public void turnLeft() throws IOException

	/** Poruszanie robotem w prawo */
	public void turnRight() throws IOException

	/** Zmiana stanu diody oswietleniowej wlacz/wylacz */
	public void ledSwitch() throws IOException

	/** Podnies wieze */
	public void moveTowerUp() throws IOException

	/** Opusc wieze */
	public void moveTowerDown() throws IOException

	/** Wylacz zasilanie serwomechanizmu wiezy */
	public void towerPowerOff() throws IOException

	/** Pobiera okreslony obraz z robota za pomoca modulu bluetooth.
	 * Funkcja bedzie czekala az robot przesle okreslona ilosc danych
	 * @param cmd komenda inicjalizujaca wysylanie obrazu na robocie
	 * @param packSize rozmiar paczki z danymi
	 * @param width szerokosc pobieranego obrazu
	 * @param height wysokosc pobieranego obrazu
	 * @param isColor parametr okreslajacy to czy obraz ma byc kolorowy czy tez w odcieniach szarosci
	 * @return tablica bajtow z pikselami obrazu*/
	private byte[] getImage(char cmd, int packSize, int width, int height, boolean isColor) throws IOException

	/** Rozmiar paczki do pobrania z robota */
	public static final int packSize = 1280;

	/** Pobiera kolorowy obraz 640x480 z robota 
	 * @return tablica bajtow z pikselami obrazu*/
	public byte[] getImageFullResColor() throws IOException

	/** Pobiera kolorowy obraz 320x240 z robota 
	 * @return tablica bajtow z pikselami obrazu*/
	public byte[] getImageHalfResColor() throws IOException

	/** Pobiera kolorowy obraz 160x120 z robota 
	 * @return tablica bajtow z pikselami obrazu*/
	public byte[] getImageQuaterResColor() throws IOException

	/** Pobiera obraz 640x480 w odcieniach szarosci 
	 * @return tablica bajtow z pikselami obrazu*/
	public byte[] getImageFullRes() throws IOException

	/** Pobiera obraz 320x240 w odcieniach szarosci 
	 * @return tablica bajtow z pikselami obrazu*/
	public byte[] getImageHalfRes() throws IOException

	/** Pobiera obraz 160x120 w odcieniach szarosci 
	 * @return tablica bajtow z pikselami obrazu*/
	public byte[] getImageQuaterRes() throws IOException

	/** Wykrywa i pobiera kolorowy obraz w rozdzielczosci 640x480  
	 * @return tablica bajtow z pikselami obrazu*/
	public byte[] getImageWithFaceDetection() throws IOException

	/** Pobiera informacje o temperaturze 
	 * @return temperatura*/
	public double getBatteryStatus()

	/** Pobiera dane z czujnika przyspieszenia 
	 * @param ax okresla os z ktorej chcemy uzyskac informacje
	 * @return odczyt z danej osi czujnika*/
	public double getAccelerometerAxisData(Axis ax)

	/** Uruchamianie procedury kalibracji akcelerometru*/
	public void calibrateAccelerometer()

	/** Pobiera dane z zyroskopu 
	 * @param ax okresla os z ktorej chcemy uzyskac informacje
	 * @return odczyt z danej osi czujnika*/
	public double getGyroscopeAxisData(Axis ax)

	/** Uruchamianie procedury kalibracji zyroskopu*/
	public void calibrateGyroscope()

	/** Pobiera dane z czujnika odleglosci 
	 * @param ir okresla czujnik z ktorgo chcemy uzyskac informacje
	 * @return odczyt z danego czujnika */
	public double getIRData(IR ir)

	/** Pobiera dane z zyroskopu 
	 * @param ax okresla os z ktorej chcemy uzyskac informacje
	 * @return odczyt z danej osi czujnika */
	public double getMagnetometerData(Axis ax)

	/** Uruchamianie procedury kalibracji magnetometru */
	public void calibrateMagnetometer()

	/** Pobiera dane z czujnika temperatury
	 * @return odczyt czujnika*/
	public double getTemperature()

	/** Wyswietla tekst na ekranie LCD robota
	 * @param lcdText tekst do wyswietlenia*/
	public void setLCDText(String lcdText)

	/** Wlaczenie trybu autonomicznego
	 * @param mode rodzaj trybu autonomicznego*/
	public void runAutonomousMode(int mode)

	/** Wlaczenie nagrywania sciezki do algorytmu rekonstrukcji trasy powrotnej*/
	public void startRecordingTrack()

	/** Wylaczenie nagrywania sciezki dla algorytmu rekonstrukcji trasy powrotnej*/
	public void stopRecordingTrack()

	/** Uruchomienie algorytmu rekonstrukcji sciezki powrotnej*/
	public void reconstructTrack()
}
