import java.io.IOException;
import java.util.Calendar;

import javax.microedition.io.Connection;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import javax.microedition.m3g.Image2D;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import com.sun.amms.control.ImageFormatCtrl;
import com.sun.amms.control.imageeffect.ImageEffectCtrl;
import com.sun.amms.control.imageeffect.ImageTransformCtrl;
import com.sun.perseus.j2d.RasterImage;

import pl.edu.agh.fis.DarkExplorer.Controller;

public class RemoteController extends MIDlet implements CommandListener {

	private Display display;
	private pl.edu.agh.fis.DarkExplorer.Controller controller = null;
	private Canvas canvas;
	private Command exit;
	

	class KeyboardControllCanvas extends Canvas
	{
		private String message = null;
		public KeyboardControllCanvas() 
		{ 
		}
		
		protected  void keyRepeated(int keyCode)
		{
			int gameAction = getGameAction(keyCode);
			try
			{
				switch(gameAction)
				{
				case UP:
					controller.goForward();
					message = "Go forward";
					break;
				case DOWN:
					controller.goBackward();
					message = "Go backward";
					break;
				case LEFT:
					controller.turnLeft();
					message = "Turn left";
					break;
				case RIGHT:
					controller.turnRight();
					message = "Turn right";
					break;
				}
				
				switch(keyCode)
				{
				case KEY_NUM3:
					controller.moveTowerUp();
					message = "Move tower up";
					break;
				case KEY_NUM9:
					controller.moveTowerDown();
					message = "Move tower down";
					break;
				}
			}
			catch(IOException e)
			{
				displayAlert("Error",e.getMessage(),AlertType.ERROR);
			}
			repaint();
		}
		
		protected void keyPressed(int keyCode)
		{
			int gameAction = getGameAction(keyCode);
			try
			{
				switch(gameAction)
				{
				case UP:
					controller.goForward();
					message = "Go forward";
					break;
				case DOWN:
					controller.goBackward();
					message = "Go backward";
					break;
				case LEFT:
					controller.turnLeft();
					message = "Turn left";
					break;
				case RIGHT:
					controller.turnRight();
					message = "Turn right";
					break;
				default:
					message = "Waiting for your orders. ";
					break;
				}
				
				switch(keyCode)
				{
				case KEY_NUM5:
					controller.stopEngines();
					message = "Engines stopped";
					break;
				case KEY_NUM7:
					controller.moveTowerUp();
					message = "Move tower up";
					break;
				case KEY_NUM0:
					controller.towerPowerOff();
					message = "Tower power off";
					break;
				case KEY_STAR:
					controller.moveTowerDown();
					message = "Move tower down";
					break;
				case KEY_POUND:
					controller.ledSwitch();
					if(controller.isLenOn())
						message = "Light ON";
					else
						message = "Light OFF";
					break;
				case -5:
					switchCanvasTo(new CameraViewCanvas());
					break;
				default:
					message = "Waiting for your orders. ";
					break;
				}
			}
			catch(IOException e)
			{
				displayAlert("Error",e.getMessage(),AlertType.ERROR);
			}
			repaint();
		}
		
		protected  void keyReleased(int keyCode)
		{
			keyCode = getGameAction(keyCode);
			try
			{
				switch(keyCode)
				{
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					controller.stopEngines();
				default:
					message = "Waiting for your orders.";
					break;
				}
			}
			catch(IOException e)
			{
				displayAlert("Error",e.getMessage(),AlertType.ERROR);
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
				g.drawString("5 - stop engines", getWidth()/2, 20, Graphics.TOP | Graphics.HCENTER);
				g.drawString("Select - picture mode", getWidth()/2, 35, Graphics.TOP | Graphics.HCENTER);
				g.drawString("# - led ON/OFF switch", getWidth()/2, 50, Graphics.TOP | Graphics.HCENTER);
				g.drawString("7 - move tower up", getWidth()/2, 65, Graphics.TOP | Graphics.HCENTER);
				g.drawString("0 - switch tower off", getWidth()/2, 80, Graphics.TOP | Graphics.HCENTER);
				g.drawString("* - move tower down", getWidth()/2, 95, Graphics.TOP | Graphics.HCENTER);
			}
		}
	}
	
	class CameraViewCanvas extends Canvas
	{
		private byte imgData[];
		private int imgType = 2;
		private boolean pictTaken = false;
		
		public CameraViewCanvas() 
		{

		}
		
		protected  void keyPressed(int keyCode)
		{
			try
			{
				switch(keyCode)
				{
				case KEY_NUM5:
					imgType = (imgType == 1) ? 2 : 1;
					break;
				case KEY_POUND:
					controller.ledSwitch();
					break;
				case -5:
					if(imgType == 1)
						imgData = controller.getImageFullRes();
					else
						imgData = controller.getImageHalfRes();
					pictTaken = true;
					break;
				case -8:
					switchCanvasTo(new KeyboardControllCanvas());
					break;
				default:
					break;
				}
			}
			catch(IOException e)
			{
				displayAlert("Error",e.getMessage(),AlertType.ERROR);
			}
			
			repaint();
		}
		
		protected void paint(Graphics g)
		{
			int imgW = 0;
			int imgH = 0;
			g.setColor(200, 100, 50);
			g.fillRect(0, 0, getWidth(), getHeight());

			g.setColor(0, 0, 255);
			
			switch(imgType)
			{
			case 1:
				imgW = 320;
				imgH = 200;
				break;
			case 2:
				imgW = 160;
				imgH = 100;
				break;
			default:
				break;
			}
			
			if(pictTaken)
			{
				int[] dat = new int[imgData.length];
				for(int i=0;i<imgData.length;i++)
				{
					dat[i] = 0xFF000000 | (imgData[i] << 16) & 0xFF0000 | (imgData[i] << 8) & 0xFF00 | imgData[i] & 0xFF;
				}

				g.drawRGB(dat, 0, imgW, (getWidth()-imgW)/2, 0, imgW, imgH, false);
				pictTaken = false;
			}

			g.drawString("Image properties", getWidth()/2, getHeight() - 80, Graphics.TOP | Graphics.HCENTER);
			g.drawString("Width: " + imgW, getWidth()/2, getHeight()-65, Graphics.TOP | Graphics.HCENTER);
			g.drawString("Height: " + imgH, getWidth()/2, getHeight()-50, Graphics.TOP | Graphics.HCENTER);
			g.drawString("5 - change resolution ", getWidth()/2, getHeight()-35, Graphics.TOP | Graphics.HCENTER);
			g.drawString("Select-picture, Clear-back", getWidth()/2, getHeight()-20, Graphics.TOP | Graphics.HCENTER);
		}
	}
	/**
	 * Creates several screens and navigates between them.
	 */
	public RemoteController() 
	{
		this.exit = new Command("Exit", Command.EXIT, 0);
		switchCanvasTo(new KeyboardControllCanvas());
		
		try
		{
			controller = new Controller();
		}
		catch (IOException e)
		{				
			displayAlert("Error","Error during data send",AlertType.ERROR);		
		}

	}
	
	private void switchCanvasTo(Canvas canv)
	{
		this.canvas  = canv;
		this.canvas.setCommandListener(this);
		this.canvas.addCommand(this.exit);
		this.canvas.setFullScreenMode(true);
		this.display = Display.getDisplay(this);
		this.display.setCurrent(this.canvas);
	}
	
	private void displayAlert(String title, String message, AlertType type)
	{
		Alert alert = new Alert(title,message,null,type);
		Displayable tmpDisp = display.getCurrent();
		alert.setTimeout(2000);
		display.setCurrent(alert, tmpDisp);			
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
