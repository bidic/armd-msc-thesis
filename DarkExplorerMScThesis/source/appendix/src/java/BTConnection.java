/** Singleton obslugujacy polaczenie bluetooth*/
public class BTConnection {

	/** Instancja klasy */
	private static BTConnection instance = null;
	/** Strumien polaczenia */
	private StreamConnection stream = null;
	/** String polaczenia z adresem MAC oraz portem Dark Explorera */
	private final String url = "btspp://00126F0437A2:1";
	/** Strumien wyjscowy */
	private OutputStream outstream = null;
	/** Strumien wejscowy */
	private InputStream instream = null;

	/** Konstruktor */
	private BTConnection()

	/** Metoda zwracajaca instancje singletonu zarzadzajacego polaczeniem bluetooth
	 * @return instancja klasy BTConnection*/
	public static BTConnection getInstance()

	/** Nawiazywanie polaczenia*/
	public void createNewConnection() throws IOException 

	/** Wysylanie danych do robota
	 * @param c komenda
	 * @param d dane
	 * @throws IOException*/
	public void writeBtData(char c,int d) throws IOException 

	/** Odbieranie danych od robota
	 * @param data odebrane dane
	 * @param currOffset obecny offset (ile danych odebrano do tej pory)
	 * @param len dlugosc paczki do odebrania*/
	public void readBtData(byte[] data, int currOffset, int len) throws IOException

	/** Zamykanie polaczenia bluetooth*/
	public void closeConnection() 
}
