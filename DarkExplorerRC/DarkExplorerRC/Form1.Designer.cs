namespace DarkExplorerRC
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.MainMenu appMainMenu;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.appMainMenu = new System.Windows.Forms.MainMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem3 = new System.Windows.Forms.MenuItem();
            this.connOpenMenuItem = new System.Windows.Forms.MenuItem();
            this.connCloseMenuItem = new System.Windows.Forms.MenuItem();
            this.exitMenuItem = new System.Windows.Forms.MenuItem();
            this.cameraView = new System.Windows.Forms.PictureBox();
            this.powerLevelBar = new System.Windows.Forms.ProgressBar();
            this.connOpenedNotification = new Microsoft.WindowsCE.Forms.Notification();
            this.statusLabel = new System.Windows.Forms.Label();
            this.diodeMenuItem = new System.Windows.Forms.MenuItem();
            this.enableDiodeMenuItem = new System.Windows.Forms.MenuItem();
            this.disableDiodeMenuItem = new System.Windows.Forms.MenuItem();
            this.menuItem8 = new System.Windows.Forms.MenuItem();
            this.menuItem9 = new System.Windows.Forms.MenuItem();
            this.menuItem10 = new System.Windows.Forms.MenuItem();
            this.servoMenuItem = new System.Windows.Forms.MenuItem();
            this.enableServoMenuItem = new System.Windows.Forms.MenuItem();
            this.disableServoMenuItem = new System.Windows.Forms.MenuItem();
            this.servoPositionMenuItem = new System.Windows.Forms.MenuItem();
            this.postion0MenuItem = new System.Windows.Forms.MenuItem();
            this.position45MenuItem = new System.Windows.Forms.MenuItem();
            this.position90MenuItem = new System.Windows.Forms.MenuItem();
            this.powerLevelTimer = new System.Windows.Forms.Timer();
            this.pictureQualityMenuItem = new System.Windows.Forms.MenuItem();
            this.lowQualityMenuItem = new System.Windows.Forms.MenuItem();
            this.lowColorQualityMenuItem = new System.Windows.Forms.MenuItem();
            this.highQualityMenuItem = new System.Windows.Forms.MenuItem();
            this.sensoreProbeTimer = new System.Windows.Forms.Timer();
            this.SuspendLayout();
            // 
            // appMainMenu
            // 
            this.appMainMenu.MenuItems.Add(this.menuItem1);
            this.appMainMenu.MenuItems.Add(this.exitMenuItem);
            // 
            // menuItem1
            // 
            this.menuItem1.MenuItems.Add(this.menuItem3);
            this.menuItem1.MenuItems.Add(this.diodeMenuItem);
            this.menuItem1.MenuItems.Add(this.servoMenuItem);
            this.menuItem1.MenuItems.Add(this.pictureQualityMenuItem);
            this.menuItem1.MenuItems.Add(this.menuItem8);
            this.menuItem1.Text = "Menu";
            // 
            // menuItem3
            // 
            this.menuItem3.MenuItems.Add(this.connOpenMenuItem);
            this.menuItem3.MenuItems.Add(this.connCloseMenuItem);
            this.menuItem3.Text = "Connection";
            // 
            // connOpenMenuItem
            // 
            this.connOpenMenuItem.Text = "Open";
            this.connOpenMenuItem.Click += new System.EventHandler(this.connOpenMenuItem_Click);
            // 
            // connCloseMenuItem
            // 
            this.connCloseMenuItem.Enabled = false;
            this.connCloseMenuItem.Text = "Close";
            this.connCloseMenuItem.Click += new System.EventHandler(this.connCloseMenuItem_Click);
            // 
            // exitMenuItem
            // 
            this.exitMenuItem.Text = "Exit";
            this.exitMenuItem.Click += new System.EventHandler(this.exitMenuItem_Click);
            // 
            // cameraView
            // 
            this.cameraView.Location = new System.Drawing.Point(4, 29);
            this.cameraView.Name = "cameraView";
            this.cameraView.Size = new System.Drawing.Size(233, 262);
            this.cameraView.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            // 
            // powerLevelBar
            // 
            this.powerLevelBar.Location = new System.Drawing.Point(180, 3);
            this.powerLevelBar.Name = "powerLevelBar";
            this.powerLevelBar.Size = new System.Drawing.Size(57, 20);
            // 
            // connOpenedNotification
            // 
            this.connOpenedNotification.Caption = "Connection Opened";
            this.connOpenedNotification.Text = "Connection to DarkExplorer established successfully.";
            // 
            // statusLabel
            // 
            this.statusLabel.Location = new System.Drawing.Point(4, 4);
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(170, 20);
            this.statusLabel.Text = "DarkExporer RC";
            this.statusLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // diodeMenuItem
            // 
            this.diodeMenuItem.Enabled = false;
            this.diodeMenuItem.MenuItems.Add(this.enableDiodeMenuItem);
            this.diodeMenuItem.MenuItems.Add(this.disableDiodeMenuItem);
            this.diodeMenuItem.Text = "Diode";
            // 
            // enableDiodeMenuItem
            // 
            this.enableDiodeMenuItem.Text = "Enable";
            this.enableDiodeMenuItem.Click += new System.EventHandler(this.enableDiodeMenuItem_Click);
            // 
            // disableDiodeMenuItem
            // 
            this.disableDiodeMenuItem.Enabled = false;
            this.disableDiodeMenuItem.Text = "Disable";
            this.disableDiodeMenuItem.Click += new System.EventHandler(this.disableDiodeMenuItem_Click);
            // 
            // menuItem8
            // 
            this.menuItem8.MenuItems.Add(this.menuItem9);
            this.menuItem8.MenuItems.Add(this.menuItem10);
            this.menuItem8.Text = "GSensor";
            // 
            // menuItem9
            // 
            this.menuItem9.Text = "Enable";
            // 
            // menuItem10
            // 
            this.menuItem10.Enabled = false;
            this.menuItem10.Text = "Disable";
            // 
            // servoMenuItem
            // 
            this.servoMenuItem.Enabled = false;
            this.servoMenuItem.MenuItems.Add(this.enableServoMenuItem);
            this.servoMenuItem.MenuItems.Add(this.disableServoMenuItem);
            this.servoMenuItem.MenuItems.Add(this.servoPositionMenuItem);
            this.servoMenuItem.Text = "Servomechanism";
            // 
            // enableServoMenuItem
            // 
            this.enableServoMenuItem.Text = "Enable";
            this.enableServoMenuItem.Click += new System.EventHandler(this.enableServoMenuItem_Click);
            // 
            // disableServoMenuItem
            // 
            this.disableServoMenuItem.Enabled = false;
            this.disableServoMenuItem.Text = "Disable";
            this.disableServoMenuItem.Click += new System.EventHandler(this.disableServoMenuItem_Click);
            // 
            // servoPositionMenuItem
            // 
            this.servoPositionMenuItem.Enabled = false;
            this.servoPositionMenuItem.MenuItems.Add(this.postion0MenuItem);
            this.servoPositionMenuItem.MenuItems.Add(this.position45MenuItem);
            this.servoPositionMenuItem.MenuItems.Add(this.position90MenuItem);
            this.servoPositionMenuItem.Text = "Set position";
            // 
            // postion0MenuItem
            // 
            this.postion0MenuItem.Checked = true;
            this.postion0MenuItem.Text = "0 degrees";
            this.postion0MenuItem.Click += new System.EventHandler(this.postion0MenuItem_Click);
            // 
            // position45MenuItem
            // 
            this.position45MenuItem.Text = "45 degrees";
            this.position45MenuItem.Click += new System.EventHandler(this.position45MenuItem_Click);
            // 
            // position90MenuItem
            // 
            this.position90MenuItem.Text = "90 degrees";
            this.position90MenuItem.Click += new System.EventHandler(this.position90MenuItem_Click);
            // 
            // powerLevelTimer
            // 
            this.powerLevelTimer.Interval = 60000;
            this.powerLevelTimer.Tick += new System.EventHandler(this.powerLevelTimer_Tick);
            // 
            // pictureQualityMenuItem
            // 
            this.pictureQualityMenuItem.Enabled = false;
            this.pictureQualityMenuItem.MenuItems.Add(this.lowQualityMenuItem);
            this.pictureQualityMenuItem.MenuItems.Add(this.lowColorQualityMenuItem);
            this.pictureQualityMenuItem.MenuItems.Add(this.highQualityMenuItem);
            this.pictureQualityMenuItem.Text = "Image Quality";
            // 
            // lowQualityMenuItem
            // 
            this.lowQualityMenuItem.Checked = true;
            this.lowQualityMenuItem.Text = "160x100 BW";
            this.lowQualityMenuItem.Click += new System.EventHandler(this.lowQualityMenuItem_Click);
            // 
            // lowColorQualityMenuItem
            // 
            this.lowColorQualityMenuItem.Text = "160x100 Color";
            this.lowColorQualityMenuItem.Click += new System.EventHandler(this.lowColorQualityMenuItem_Click);
            // 
            // highQualityMenuItem
            // 
            this.highQualityMenuItem.Text = "320x200 BW";
            this.highQualityMenuItem.Click += new System.EventHandler(this.highQualityMenuItem_Click);
            // 
            // sensoreProbeTimer
            // 
            this.sensoreProbeTimer.Interval = 500;
            this.sensoreProbeTimer.Tick += new System.EventHandler(this.sensoreProbeTimer_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(240, 294);
            this.Controls.Add(this.statusLabel);
            this.Controls.Add(this.powerLevelBar);
            this.Controls.Add(this.cameraView);
            this.KeyPreview = true;
            this.Location = new System.Drawing.Point(0, 0);
            this.Menu = this.appMainMenu;
            this.Name = "Form1";
            this.Text = "Dark Explorer RC";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.MenuItem exitMenuItem;
        private System.Windows.Forms.MenuItem menuItem3;
        private System.Windows.Forms.MenuItem connOpenMenuItem;
        private System.Windows.Forms.MenuItem connCloseMenuItem;
        private System.Windows.Forms.PictureBox cameraView;
        private System.Windows.Forms.ProgressBar powerLevelBar;
        private Microsoft.WindowsCE.Forms.Notification connOpenedNotification;
        private System.Windows.Forms.MenuItem diodeMenuItem;
        private System.Windows.Forms.MenuItem enableDiodeMenuItem;
        private System.Windows.Forms.MenuItem disableDiodeMenuItem;
        private System.Windows.Forms.MenuItem menuItem8;
        private System.Windows.Forms.Label statusLabel;
        private System.Windows.Forms.MenuItem menuItem9;
        private System.Windows.Forms.MenuItem menuItem10;
        private System.Windows.Forms.MenuItem servoMenuItem;
        private System.Windows.Forms.MenuItem enableServoMenuItem;
        private System.Windows.Forms.MenuItem disableServoMenuItem;
        private System.Windows.Forms.MenuItem servoPositionMenuItem;
        private System.Windows.Forms.MenuItem postion0MenuItem;
        private System.Windows.Forms.MenuItem position45MenuItem;
        private System.Windows.Forms.MenuItem position90MenuItem;
        private System.Windows.Forms.Timer powerLevelTimer;
        private System.Windows.Forms.MenuItem pictureQualityMenuItem;
        private System.Windows.Forms.MenuItem lowQualityMenuItem;
        private System.Windows.Forms.MenuItem lowColorQualityMenuItem;
        private System.Windows.Forms.MenuItem highQualityMenuItem;
        private System.Windows.Forms.Timer sensoreProbeTimer;

    }
}

