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

	public Controller() throws IOException 
	{
		this.towerPosition = 100;
		
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
	
	public void ledOn() throws IOException
	{
		btconn.writeBtData('d',1);
	}
	
	public void ledOff() throws IOException
	{
		btconn.writeBtData('d',0);
	}
	
	public void moveTowerUp() throws IOException
	{
		if(towerPosition>0)
			towerPosition -= 5;

		btconn.writeBtData('w',1);
		btconn.writeBtData('s',towerPosition);
		//btconn.WriteBtData('w',0);
	}
	
	public void moveTowerDown() throws IOException
	{
		if(towerPosition<255)
			towerPosition += 5;

		btconn.writeBtData('w',1);
		btconn.writeBtData('s',towerPosition);
		//btconn.WriteBtData('w',0);
	}
}
