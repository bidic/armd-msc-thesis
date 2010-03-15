import java.io.IOException;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Graphics;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class RemoteController extends MIDlet implements CommandListener {

	private Display display;
	private BTConnection btconn;
	private KeyboardControllCanvas canvas;
	private int towerPosition;
	private Command exit;
	

	class KeyboardControllCanvas extends Canvas
	{
		private String message = null;
		private RemoteController midlet = null;
		
		public KeyboardControllCanvas(RemoteController midlet) 
		{
			this.midlet = midlet; 
		}
		
		protected  void keyRepeated(int keyCode)
		{
			int gameAction = getGameAction(keyCode);
			switch(gameAction)
			{
			case UP:
				midlet.GoForward();
				message = "Go forward";
				break;
			case DOWN:
				midlet.GoBackward();
				message = "Go backward";
				break;
			case LEFT:
				midlet.TurnLeft();
				message = "Turn left";
				break;
			case RIGHT:
				midlet.TurnRight();
				message = "Turn right";
				break;
			}
			
			switch(keyCode)
			{
			case KEY_NUM3:
				midlet.MoveTowerUp();
				message = "Move tower up";
				break;
			case KEY_NUM9:
				midlet.MoveTowerDown();
				message = "Move tower down";
				break;
			}
			repaint();
		}
		
		protected  void keyPressed(int keyCode)
		{
			int gameAction = getGameAction(keyCode);
			switch(gameAction)
			{
			case UP:
				midlet.GoForward();
				message = "Go forward";
				break;
			case DOWN:
				midlet.GoBackward();
				message = "Go backward";
				break;
			case LEFT:
				midlet.TurnLeft();
				message = "Turn left";
				break;
			case RIGHT:
				midlet.TurnRight();
				message = "Turn right";
				break;
			default:
				message = "Waiting for your orders.";
				break;
			}
			
			switch(keyCode)
			{
			case KEY_NUM3:
				midlet.MoveTowerUp();
				message = "Move tower up";
				break;
			case KEY_NUM9:
				midlet.MoveTowerDown();
				message = "Move tower down";
				break;
			case KEY_STAR:
				midlet.TurnLedOn();
				message = "Light ON";
				break;
			case KEY_POUND:
				midlet.TurnLedOff();
				message = "Light OFF";
				break;
			default:
				message = "Waiting for your orders.";
				break;
			}
			repaint();
		}
		
		protected  void keyReleased(int keyCode)
		{
			keyCode = getGameAction(keyCode);
			switch(keyCode)
			{
			case UP:
			case DOWN:
			case LEFT:
			case RIGHT:
				midlet.StopEngines();
			default:
				message = "Waiting for your orders.";
				break;
			}
		}
		
		protected void paint(Graphics g)
		{
			g.setColor(200, 100, 50);
			g.fillRect(0, 0, getWidth(), getHeight());

			if (message != null)
			{
				g.setColor(0, 0, 255);
			    g.drawString(message, getWidth()/2, getHeight()/2, 
			    Graphics.TOP | Graphics.HCENTER);
			}
			else
			{
				g.setColor(0, 0, 255);
				g.drawString("To move robot use joystick", getWidth()/2, 5, Graphics.TOP | Graphics.HCENTER);
				g.drawString("0 - stop engines", getWidth()/2, 20, Graphics.TOP | Graphics.HCENTER);
				g.drawString("* - turn light on", getWidth()/2, 35, Graphics.TOP | Graphics.HCENTER);
				g.drawString("# - turn light off", getWidth()/2, 50, Graphics.TOP | Graphics.HCENTER);
				g.drawString("3 - move tower up", getWidth()/2, 65, Graphics.TOP | Graphics.HCENTER);
				g.drawString("9 - move tower down", getWidth()/2, 80, Graphics.TOP | Graphics.HCENTER);
			}
		}
	}
	/**
	 * Creates several screens and navigates between them.
	 */
	public RemoteController() 
	{
		this.towerPosition = 100;
		this.exit = new Command("Exit", Command.EXIT, 0);
		this.canvas  = new KeyboardControllCanvas(this);
		this.canvas.setCommandListener(this);
		this.canvas.addCommand(this.exit);
		this.display = Display.getDisplay(this);
		this.display.setCurrent(this.canvas);
		
		btconn = BTConnection.getInstance();
		try
		{
			btconn.CreateNewConnection();
		}
		catch (IOException e)
		{				
			DisplayAlert("Error","Error during data send",AlertType.ERROR);		
		}

	}
	
	private void DisplayAlert(String title, String message, AlertType type)
	{
		Alert alert = new Alert(title,message,null,type);
		Displayable tmpDisp = display.getCurrent();
		alert.setTimeout(2000);
		display.setCurrent(alert, tmpDisp);			
	}
	
	private void StopEngines()
	{
		try
		{
			btconn.WriteBtData('l',0);
			btconn.WriteBtData('r',0);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);
		}
	}
	
	private void GoForward()
	{
		try
		{
			btconn.WriteBtData('l',1);
			btconn.WriteBtData('a',255);
			btconn.WriteBtData('r',1);
			btconn.WriteBtData('b',255);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);			
		}		
	}
	
	private void GoBackward()
	{
		try
		{
			btconn.WriteBtData('l',2);
			btconn.WriteBtData('a',255);
			btconn.WriteBtData('r',2);
			btconn.WriteBtData('b',255);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);			
		}		
	}
	
	private void TurnLeft()
	{
		try
		{
			btconn.WriteBtData('l',2);
			btconn.WriteBtData('a',255);
			btconn.WriteBtData('r',1);
			btconn.WriteBtData('b',255);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);				
		}
	}
	
	private void TurnRight()
	{
		try
		{
			btconn.WriteBtData('l',1);
			btconn.WriteBtData('a',255);
			btconn.WriteBtData('r',2);
			btconn.WriteBtData('b',255);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);				
		}
	}
	
	private void TurnLedOn()
	{
		try
		{
			btconn.WriteBtData('d',1);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);				
		}
	}
	
	private void TurnLedOff()
	{
		try
		{
			btconn.WriteBtData('d',0);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);				
		}
	}
	
	private void MoveTowerUp()
	{
		if(towerPosition>0)
			towerPosition -= 5;
		try
		{
			btconn.WriteBtData('w',1);
			btconn.WriteBtData('s',towerPosition);
			//btconn.WriteBtData('w',0);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);				
		}
	}
	
	private void MoveTowerDown()
	{
		if(towerPosition<255)
			towerPosition += 5;
		try
		{
			btconn.WriteBtData('w',1);
			btconn.WriteBtData('s',towerPosition);
			//btconn.WriteBtData('w',0);
		}
		catch (Exception e)
		{
			DisplayAlert("Error","Error during data send",AlertType.ERROR);				
		}
	}
	/* (non-Javadoc)
	 * @see javax.microedition.midlet.MIDlet#startApp()
	 */
	protected void startApp() throws MIDletStateChangeException 
	{
		display.setCurrent(this.canvas);
	}
	
	/* (non-Javadoc)
	 * @see javax.microedition.lcdui.CommandListener#commandAction(javax.microedition.lcdui.Command, javax.microedition.lcdui.Displayable)
	 */
	public void commandAction(Command command, Displayable displayable) 
	{
		if(command == this.exit)
			try
			{
				destroyApp(false);
			    notifyDestroyed();
			}
			catch(Exception e){}

	}

	/* (non-Javadoc)
	 * @see javax.microedition.midlet.MIDlet#destroyApp(boolean)
	 */
	protected void destroyApp(boolean unconditional) throws MIDletStateChangeException {}

	/* (non-Javadoc)
	 * @see javax.microedition.midlet.MIDlet#pauseApp()
	 */
	protected void pauseApp() {}
}
