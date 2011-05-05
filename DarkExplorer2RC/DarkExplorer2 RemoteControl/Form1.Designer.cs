namespace DarkExplorer2_RemoteControl
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

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
            this.camGroupBox = new System.Windows.Forms.GroupBox();
            this.resolutionLabel = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            this.trackReconstructionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.startRecordingTrackToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stopRecordingTrackToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.clearToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.startTrackReconstructionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cancelTrackReconstructionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.downloadTrackDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.accelometerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.calibrateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.readStateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.magnetometerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.calibrateToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.readStateToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.gyroscopeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.calibrateToolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.resetStatusToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.getStateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.odometersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.getStatusToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.disconnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.checkPowerLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.msgColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.timestampColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.manualSteeringToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configurationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showScreenPadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.button1 = new System.Windows.Forms.Button();
            this.objectDetectButton = new System.Windows.Forms.Button();
            this.faceDetectButton = new System.Windows.Forms.Button();
            this.takePictureButton = new System.Windows.Forms.Button();
            this.camPictureBox = new System.Windows.Forms.PictureBox();
            this.camGroupBox.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.camPictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // camGroupBox
            // 
            this.camGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.camGroupBox.Controls.Add(this.button1);
            this.camGroupBox.Controls.Add(this.objectDetectButton);
            this.camGroupBox.Controls.Add(this.faceDetectButton);
            this.camGroupBox.Controls.Add(this.takePictureButton);
            this.camGroupBox.Controls.Add(this.camPictureBox);
            this.camGroupBox.Location = new System.Drawing.Point(12, 27);
            this.camGroupBox.Name = "camGroupBox";
            this.camGroupBox.Size = new System.Drawing.Size(510, 420);
            this.camGroupBox.TabIndex = 1;
            this.camGroupBox.TabStop = false;
            // 
            // resolutionLabel
            // 
            this.resolutionLabel.AutoSize = true;
            this.resolutionLabel.Location = new System.Drawing.Point(6, 21);
            this.resolutionLabel.Name = "resolutionLabel";
            this.resolutionLabel.Size = new System.Drawing.Size(60, 13);
            this.resolutionLabel.TabIndex = 2;
            this.resolutionLabel.Text = "Image size:";
            // 
            // comboBox1
            // 
            this.comboBox1.DisplayMember = "640 x 480";
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(113, 19);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(175, 21);
            this.comboBox1.TabIndex = 1;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.aboutToolStripMenuItem,
            this.trackReconstructionToolStripMenuItem,
            this.aboutToolStripMenuItem1});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(534, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectToolStripMenuItem,
            this.disconnectToolStripMenuItem,
            this.checkPowerLevelToolStripMenuItem,
            this.manualSteeringToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(51, 20);
            this.fileToolStripMenuItem.Text = "Robot";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.accelometerToolStripMenuItem,
            this.magnetometerToolStripMenuItem,
            this.gyroscopeToolStripMenuItem,
            this.odometersToolStripMenuItem});
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
            this.aboutToolStripMenuItem.Text = "Sensors";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripProgressBar1});
            this.statusStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
            this.statusStrip1.Location = new System.Drawing.Point(0, 740);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(534, 22);
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(39, 17);
            this.toolStripStatusLabel1.Text = "Ready";
            // 
            // toolStripProgressBar1
            // 
            this.toolStripProgressBar1.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripProgressBar1.Name = "toolStripProgressBar1";
            this.toolStripProgressBar1.Size = new System.Drawing.Size(150, 16);
            // 
            // trackReconstructionToolStripMenuItem
            // 
            this.trackReconstructionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.startRecordingTrackToolStripMenuItem,
            this.stopRecordingTrackToolStripMenuItem,
            this.clearToolStripMenuItem,
            this.startTrackReconstructionToolStripMenuItem,
            this.cancelTrackReconstructionToolStripMenuItem,
            this.downloadTrackDataToolStripMenuItem});
            this.trackReconstructionToolStripMenuItem.Name = "trackReconstructionToolStripMenuItem";
            this.trackReconstructionToolStripMenuItem.Size = new System.Drawing.Size(128, 20);
            this.trackReconstructionToolStripMenuItem.Text = "Track reconstruction";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.trackBar1);
            this.groupBox1.Controls.Add(this.resolutionLabel);
            this.groupBox1.Controls.Add(this.comboBox1);
            this.groupBox1.Location = new System.Drawing.Point(13, 453);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(509, 101);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Camera configuration";
            // 
            // startRecordingTrackToolStripMenuItem
            // 
            this.startRecordingTrackToolStripMenuItem.Name = "startRecordingTrackToolStripMenuItem";
            this.startRecordingTrackToolStripMenuItem.Size = new System.Drawing.Size(219, 22);
            this.startRecordingTrackToolStripMenuItem.Text = "Start recording track";
            // 
            // stopRecordingTrackToolStripMenuItem
            // 
            this.stopRecordingTrackToolStripMenuItem.Name = "stopRecordingTrackToolStripMenuItem";
            this.stopRecordingTrackToolStripMenuItem.Size = new System.Drawing.Size(219, 22);
            this.stopRecordingTrackToolStripMenuItem.Text = "Stop recording track";
            // 
            // clearToolStripMenuItem
            // 
            this.clearToolStripMenuItem.Name = "clearToolStripMenuItem";
            this.clearToolStripMenuItem.Size = new System.Drawing.Size(219, 22);
            this.clearToolStripMenuItem.Text = "Clear track data";
            // 
            // startTrackReconstructionToolStripMenuItem
            // 
            this.startTrackReconstructionToolStripMenuItem.Name = "startTrackReconstructionToolStripMenuItem";
            this.startTrackReconstructionToolStripMenuItem.Size = new System.Drawing.Size(219, 22);
            this.startTrackReconstructionToolStripMenuItem.Text = "Start track reconstruction";
            // 
            // cancelTrackReconstructionToolStripMenuItem
            // 
            this.cancelTrackReconstructionToolStripMenuItem.Name = "cancelTrackReconstructionToolStripMenuItem";
            this.cancelTrackReconstructionToolStripMenuItem.Size = new System.Drawing.Size(219, 22);
            this.cancelTrackReconstructionToolStripMenuItem.Text = "Cancel track reconstruction";
            // 
            // downloadTrackDataToolStripMenuItem
            // 
            this.downloadTrackDataToolStripMenuItem.Name = "downloadTrackDataToolStripMenuItem";
            this.downloadTrackDataToolStripMenuItem.Size = new System.Drawing.Size(219, 22);
            this.downloadTrackDataToolStripMenuItem.Text = "Download track data";
            // 
            // accelometerToolStripMenuItem
            // 
            this.accelometerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.calibrateToolStripMenuItem,
            this.readStateToolStripMenuItem});
            this.accelometerToolStripMenuItem.Name = "accelometerToolStripMenuItem";
            this.accelometerToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.accelometerToolStripMenuItem.Text = "Accelometer";
            // 
            // calibrateToolStripMenuItem
            // 
            this.calibrateToolStripMenuItem.Name = "calibrateToolStripMenuItem";
            this.calibrateToolStripMenuItem.Size = new System.Drawing.Size(128, 22);
            this.calibrateToolStripMenuItem.Text = "Calibrate";
            // 
            // readStateToolStripMenuItem
            // 
            this.readStateToolStripMenuItem.Name = "readStateToolStripMenuItem";
            this.readStateToolStripMenuItem.Size = new System.Drawing.Size(128, 22);
            this.readStateToolStripMenuItem.Text = "Read state";
            // 
            // magnetometerToolStripMenuItem
            // 
            this.magnetometerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.calibrateToolStripMenuItem1,
            this.readStateToolStripMenuItem1});
            this.magnetometerToolStripMenuItem.Name = "magnetometerToolStripMenuItem";
            this.magnetometerToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.magnetometerToolStripMenuItem.Text = "Magnetometer";
            // 
            // calibrateToolStripMenuItem1
            // 
            this.calibrateToolStripMenuItem1.Name = "calibrateToolStripMenuItem1";
            this.calibrateToolStripMenuItem1.Size = new System.Drawing.Size(128, 22);
            this.calibrateToolStripMenuItem1.Text = "Calibrate";
            // 
            // readStateToolStripMenuItem1
            // 
            this.readStateToolStripMenuItem1.Name = "readStateToolStripMenuItem1";
            this.readStateToolStripMenuItem1.Size = new System.Drawing.Size(128, 22);
            this.readStateToolStripMenuItem1.Text = "Read state";
            // 
            // gyroscopeToolStripMenuItem
            // 
            this.gyroscopeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.calibrateToolStripMenuItem2,
            this.resetStatusToolStripMenuItem,
            this.getStateToolStripMenuItem});
            this.gyroscopeToolStripMenuItem.Name = "gyroscopeToolStripMenuItem";
            this.gyroscopeToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.gyroscopeToolStripMenuItem.Text = "Gyroscope";
            // 
            // calibrateToolStripMenuItem2
            // 
            this.calibrateToolStripMenuItem2.Name = "calibrateToolStripMenuItem2";
            this.calibrateToolStripMenuItem2.Size = new System.Drawing.Size(130, 22);
            this.calibrateToolStripMenuItem2.Text = "Calibrate";
            // 
            // resetStatusToolStripMenuItem
            // 
            this.resetStatusToolStripMenuItem.Name = "resetStatusToolStripMenuItem";
            this.resetStatusToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.resetStatusToolStripMenuItem.Text = "Reset state";
            // 
            // getStateToolStripMenuItem
            // 
            this.getStateToolStripMenuItem.Name = "getStateToolStripMenuItem";
            this.getStateToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.getStateToolStripMenuItem.Text = "Get state";
            // 
            // odometersToolStripMenuItem
            // 
            this.odometersToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.getStatusToolStripMenuItem});
            this.odometersToolStripMenuItem.Name = "odometersToolStripMenuItem";
            this.odometersToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.odometersToolStripMenuItem.Text = "Odometers";
            // 
            // getStatusToolStripMenuItem
            // 
            this.getStatusToolStripMenuItem.Name = "getStatusToolStripMenuItem";
            this.getStatusToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.getStatusToolStripMenuItem.Text = "Get status";
            // 
            // connectToolStripMenuItem
            // 
            this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
            this.connectToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.connectToolStripMenuItem.Text = "Connect";
            // 
            // disconnectToolStripMenuItem
            // 
            this.disconnectToolStripMenuItem.Name = "disconnectToolStripMenuItem";
            this.disconnectToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.disconnectToolStripMenuItem.Text = "Disconnect";
            // 
            // checkPowerLevelToolStripMenuItem
            // 
            this.checkPowerLevelToolStripMenuItem.Name = "checkPowerLevelToolStripMenuItem";
            this.checkPowerLevelToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.checkPowerLevelToolStripMenuItem.Text = "Check power level";
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(113, 50);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(385, 45);
            this.trackBar1.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 65);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Tower position:";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.dataGridView1);
            this.groupBox2.Location = new System.Drawing.Point(12, 560);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(510, 177);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Messages";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.msgColumn,
            this.timestampColumn});
            this.dataGridView1.Location = new System.Drawing.Point(10, 19);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.Size = new System.Drawing.Size(489, 152);
            this.dataGridView1.TabIndex = 0;
            // 
            // msgColumn
            // 
            this.msgColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.msgColumn.HeaderText = "Message text";
            this.msgColumn.Name = "msgColumn";
            this.msgColumn.ReadOnly = true;
            this.msgColumn.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // timestampColumn
            // 
            this.timestampColumn.HeaderText = "Timestamp";
            this.timestampColumn.Name = "timestampColumn";
            this.timestampColumn.ReadOnly = true;
            // 
            // aboutToolStripMenuItem1
            // 
            this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
            this.aboutToolStripMenuItem1.Size = new System.Drawing.Size(52, 20);
            this.aboutToolStripMenuItem1.Text = "About";
            // 
            // manualSteeringToolStripMenuItem
            // 
            this.manualSteeringToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.configurationToolStripMenuItem,
            this.showScreenPadToolStripMenuItem});
            this.manualSteeringToolStripMenuItem.Name = "manualSteeringToolStripMenuItem";
            this.manualSteeringToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.manualSteeringToolStripMenuItem.Text = "Manual steering";
            // 
            // configurationToolStripMenuItem
            // 
            this.configurationToolStripMenuItem.Name = "configurationToolStripMenuItem";
            this.configurationToolStripMenuItem.Size = new System.Drawing.Size(163, 22);
            this.configurationToolStripMenuItem.Text = "Configuration";
            // 
            // showScreenPadToolStripMenuItem
            // 
            this.showScreenPadToolStripMenuItem.Name = "showScreenPadToolStripMenuItem";
            this.showScreenPadToolStripMenuItem.Size = new System.Drawing.Size(163, 22);
            this.showScreenPadToolStripMenuItem.Text = "Show screen pad";
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.button1.Image = global::DarkExplorer2_RemoteControl.Properties.Resources.lightbulb_on_icon;
            this.button1.Location = new System.Drawing.Point(6, 385);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(25, 25);
            this.button1.TabIndex = 6;
            this.button1.UseVisualStyleBackColor = true;
            // 
            // objectDetectButton
            // 
            this.objectDetectButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.objectDetectButton.Image = global::DarkExplorer2_RemoteControl.Properties.Resources.obj_detection_icon;
            this.objectDetectButton.Location = new System.Drawing.Point(474, 385);
            this.objectDetectButton.Name = "objectDetectButton";
            this.objectDetectButton.Size = new System.Drawing.Size(25, 25);
            this.objectDetectButton.TabIndex = 5;
            this.objectDetectButton.UseVisualStyleBackColor = true;
            // 
            // faceDetectButton
            // 
            this.faceDetectButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.faceDetectButton.Image = global::DarkExplorer2_RemoteControl.Properties.Resources.face_detection_icon;
            this.faceDetectButton.Location = new System.Drawing.Point(443, 384);
            this.faceDetectButton.Name = "faceDetectButton";
            this.faceDetectButton.Size = new System.Drawing.Size(25, 25);
            this.faceDetectButton.TabIndex = 4;
            this.faceDetectButton.UseVisualStyleBackColor = true;
            // 
            // takePictureButton
            // 
            this.takePictureButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.takePictureButton.Image = global::DarkExplorer2_RemoteControl.Properties.Resources.picture_icon;
            this.takePictureButton.Location = new System.Drawing.Point(412, 384);
            this.takePictureButton.Name = "takePictureButton";
            this.takePictureButton.Size = new System.Drawing.Size(25, 25);
            this.takePictureButton.TabIndex = 3;
            this.takePictureButton.UseVisualStyleBackColor = true;
            // 
            // camPictureBox
            // 
            this.camPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.camPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.camPictureBox.Image = global::DarkExplorer2_RemoteControl.Properties.Resources.de2_logo;
            this.camPictureBox.InitialImage = global::DarkExplorer2_RemoteControl.Properties.Resources.de2_logo;
            this.camPictureBox.Location = new System.Drawing.Point(9, 19);
            this.camPictureBox.Name = "camPictureBox";
            this.camPictureBox.Size = new System.Drawing.Size(490, 360);
            this.camPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.camPictureBox.TabIndex = 0;
            this.camPictureBox.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(534, 762);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.camGroupBox);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.MinimumSize = new System.Drawing.Size(550, 800);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Dark Explorer 2 Remote Control";
            this.camGroupBox.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.camPictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox camPictureBox;
        private System.Windows.Forms.GroupBox camGroupBox;
        private System.Windows.Forms.Label resolutionLabel;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button objectDetectButton;
        private System.Windows.Forms.Button faceDetectButton;
        private System.Windows.Forms.Button takePictureButton;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
        private System.Windows.Forms.ToolStripMenuItem trackReconstructionToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ToolStripMenuItem startRecordingTrackToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem stopRecordingTrackToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem accelometerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem calibrateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem readStateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem magnetometerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem calibrateToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem readStateToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem gyroscopeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem calibrateToolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem resetStatusToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem getStateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem odometersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem startTrackReconstructionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cancelTrackReconstructionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem downloadTrackDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem getStatusToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem disconnectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem checkPowerLevelToolStripMenuItem;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn msgColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn timestampColumn;
        private System.Windows.Forms.ToolStripMenuItem manualSteeringToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem configurationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showScreenPadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem1;
        private System.Windows.Forms.Button button1;
    }
}

