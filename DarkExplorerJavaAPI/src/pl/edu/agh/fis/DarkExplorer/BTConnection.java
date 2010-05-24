/**
 * 
 */

/**
 * @author m_nowy
 *
 */

package pl.edu.agh.fis.DarkExplorer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Calendar;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

public class BTConnection {
	private static BTConnection instance = null;
	private StreamConnection stream = null;
	
	private final String url = "btspp://00126F0437A2:1";
	private OutputStream outstream = null;
	private InputStream instream = null;
	
	private BTConnection()
	{

	}
	public static BTConnection getInstance() {
		if (instance == null)
			instance = new BTConnection();
		return instance;
	}

	public void createNewConnection() throws IOException {
		if (stream != null)
			stream.close();
		stream = (StreamConnection) Connector.open(url,Connector.READ_WRITE,true);
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
	
	public void readBtData(byte[] data, int currOffset, int len) throws IOException
	{
		if(instream == null)
			instream = stream.openInputStream();

		try	//give some time for DarkExplorer to make a picture
		{
				Thread.sleep(10);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	    long startTime = Calendar.getInstance().getTime().getTime();
	    long currTime = startTime;
	    int startOffset = currOffset;
	    int summaryDataRead = 0;

		do
		{
			summaryDataRead = currOffset - startOffset;
			int singleDataRead = instream.read(data, currOffset, len - summaryDataRead );
			currOffset += singleDataRead;
			currTime = Calendar.getInstance().getTime().getTime();
		}
		while(summaryDataRead < len && currTime < startTime + 2000);
		
		if(summaryDataRead < len)
			throw new IOException("Error in bluetooth communication.");
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
