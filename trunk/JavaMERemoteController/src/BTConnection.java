/**
 * 
 */

/**
 * @author m_nowy
 *
 */
import java.io.IOException;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

public class BTConnection {
	private static BTConnection instance = null;
	private StreamConnection stream = null;
	
	private final String url = "btspp://00126F0437A2:1";
	OutputStream oustream = null;

	public static BTConnection getInstance() {
		if (instance == null)
			instance = new BTConnection();
		return instance;
	}

	public void CreateNewConnection() throws IOException {
		if (stream != null)
			stream.close();
		stream = (StreamConnection) Connector.open(url,Connector.WRITE,true);
		try
		{
			Thread.sleep(2000);
		}
		catch(Exception e)
		{
			throw (IOException) e;
		}
	}

	public void WriteBtData(char c,int d) throws IOException {

		if(stream == null)
			CreateNewConnection();
		
		try
		{
			oustream = stream.openOutputStream();
			oustream.write(c);
			oustream.write(d);
			oustream.close();
		}
		catch(IOException e)
		{
			CreateNewConnection();
			oustream = stream.openOutputStream();
			oustream.write(c);
			oustream.write(d);
			oustream.close();
		}

	}

	public void CloseOpenConnection() {
		try {
			if (stream != null)
				stream.close();
			stream = null;
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}