/**
 * 
 */

/**
 * @author m_nowy
 *
 */

package pl.edu.agh.fis.DarkExplorer;

import java.io.IOException;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

public class BTConnection {
	private static BTConnection instance = null;
	private StreamConnection stream = null;
	
	private final String url = "btspp://00126F0437A2:1";
	private OutputStream outstream = null;
	
	private BTConnection()
	{

	}
	public static BTConnection getInstance() {
		if (instance == null)
			instance = new BTConnection();
		return instance;
	}

	public void createNewConnection() throws IOException {
		
		System.out.println("New connection");
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

	public void writeBtData(char c,int d) throws IOException {

		if(stream == null)
			createNewConnection();
		
		try
		{
			if(outstream == null)
				outstream = stream.openOutputStream();
			
			outstream.write(c);
			outstream.write(d);
			outstream.flush();
		}
		catch(IOException e)
		{
			e.printStackTrace();
			createNewConnection();
			
			if(outstream == null)
				outstream = stream.openOutputStream();
			
			outstream.write(c);
			outstream.write(d);
			outstream.flush();
		}

	}

	public void closeOpenConnection() {
		try {
			if (stream != null)
				stream.close();
			stream = null;
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
