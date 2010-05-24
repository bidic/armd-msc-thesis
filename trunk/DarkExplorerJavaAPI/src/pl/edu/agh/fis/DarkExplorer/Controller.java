/**
 * 
 */
package pl.edu.agh.fis.DarkExplorer;

import pl.edu.agh.fis.DarkExplorer.BTConnection;

import java.io.IOException;

/**
 * @author m_nowy
 *
 */
public class Controller 
{
	private BTConnection btconn;
	private int towerPosition;
	private boolean ledState;

	public Controller() throws IOException 
	{
		this.towerPosition = 100;
		this.ledState = false;
		
		btconn = BTConnection.getInstance();
		try
		{
			btconn.createNewConnection();
		}
		catch (IOException e)
		{				
			throw e;
		}
	}
	
	public int getTowerPosition()
	{
		return towerPosition;
	}
	
	public boolean isLenOn()
	{
		return ledState;
	}
	
	public void stopEngines() throws IOException
	{
		btconn.writeBtData('l',0);
		btconn.writeBtData('r',0);
	}
	
	public void goForward() throws IOException
	{
		btconn.writeBtData('l',1);
		btconn.writeBtData('a',255);
		btconn.writeBtData('r',1);
		btconn.writeBtData('b',255);
	}
	
	public void goBackward() throws IOException
	{
		btconn.writeBtData('l',2);
		btconn.writeBtData('a',255);
		btconn.writeBtData('r',2);
		btconn.writeBtData('b',255);	
	}
	
	public void turnLeft() throws IOException
	{
		btconn.writeBtData('l',2);
		btconn.writeBtData('a',255);
		btconn.writeBtData('r',1);
		btconn.writeBtData('b',255);
	}
	
	public void turnRight() throws IOException
	{
		btconn.writeBtData('l',1);
		btconn.writeBtData('a',255);
		btconn.writeBtData('r',2);
		btconn.writeBtData('b',255);
	}
	
	public void ledSwitch() throws IOException
	{
		if(ledState)
		{
			btconn.writeBtData('d',0);
			ledState = false;
		}
		else
		{
			btconn.writeBtData('d',1);
			ledState = true;
		}
	}
	
	public void moveTowerUp() throws IOException
	{
		if(towerPosition>0)
			towerPosition -= 5;

		btconn.writeBtData('w',1);
		btconn.writeBtData('s',towerPosition);
	}
	
	public void moveTowerDown() throws IOException
	{
		if(towerPosition<255)
			towerPosition += 5;

		btconn.writeBtData('w',1);
		btconn.writeBtData('s',towerPosition);
	}
	
	public void towerPowerOff() throws IOException
	{
		btconn.writeBtData('w',0);
	}
	
	private byte[] getImage(char cmd, int nrOfPackets) throws IOException
	{
		byte data[] = new byte[nrOfPackets * 3200];

		for(int i = 0; i < nrOfPackets; i++)
		{
			btconn.writeBtData(cmd,i);
			btconn.readBtData(data, i*3200, 3200);
		}
		

		for (int left=0, right=data.length-1; left<right; left++, right--) 
		{
		    byte temp = data[right]; 
		    data[right]  = data[left]; 
		    data[left] = temp;
		}

		return data;
	}
	
	public byte[] getImageFullRes() throws IOException
	{
		return getImage('f',20);
	}
	
	public byte[] getImageHalfRes() throws IOException
	{
		return getImage('p',5);
	}
}
