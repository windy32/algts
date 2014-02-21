namespace NsReport
{
    partial class MainWindow
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.pic = new System.Windows.Forms.PictureBox();
            this.hScroll = new System.Windows.Forms.HScrollBar();
            this.vScroll = new System.Windows.Forms.VScrollBar();
            this.trkZoom = new System.Windows.Forms.TrackBar();
            this.lblZoom = new System.Windows.Forms.Label();
            this.txtZoom = new System.Windows.Forms.TextBox();
            this.btnGenerateTemp = new System.Windows.Forms.Button();
            this.lblBandwidth = new System.Windows.Forms.Label();
            this.lblSessions = new System.Windows.Forms.Label();
            this.cmbBandwidth = new System.Windows.Forms.ComboBox();
            this.cmbSessions = new System.Windows.Forms.ComboBox();
            this.lblMinDelay = new System.Windows.Forms.Label();
            this.lblMaxDelay = new System.Windows.Forms.Label();
            this.updMinDelay = new System.Windows.Forms.NumericUpDown();
            this.updMaxDelay = new System.Windows.Forms.NumericUpDown();
            this.lstTemp = new System.Windows.Forms.ListView();
            this.hdrEnable = new System.Windows.Forms.ColumnHeader();
            this.hdrBw = new System.Windows.Forms.ColumnHeader();
            this.hdrSessions = new System.Windows.Forms.ColumnHeader();
            this.hdrQueueLen = new System.Windows.Forms.ColumnHeader();
            this.hdrTokenRate = new System.Windows.Forms.ColumnHeader();
            this.hdrDelay = new System.Windows.Forms.ColumnHeader();
            this.hdrRate = new System.Windows.Forms.ColumnHeader();
            this.btnSelect = new System.Windows.Forms.Button();
            this.grpFilterOptions = new System.Windows.Forms.GroupBox();
            this.chkRemoveInner = new System.Windows.Forms.CheckBox();
            this.rdoAverage = new System.Windows.Forms.RadioButton();
            this.rdoMaximum = new System.Windows.Forms.RadioButton();
            this.lblDelayType = new System.Windows.Forms.Label();
            this.btnAdd = new System.Windows.Forms.Button();
            this.lblTotal = new System.Windows.Forms.Label();
            this.btnClear = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnGenerateFinal = new System.Windows.Forms.Button();
            this.lstFinal = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
            this.grpDisplayOptions = new System.Windows.Forms.GroupBox();
            this.chkDrawImpossible = new System.Windows.Forms.CheckBox();
            this.chkDisplayData = new System.Windows.Forms.CheckBox();
            this.chkRemoveMore = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.pic)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkZoom)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.updMinDelay)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.updMaxDelay)).BeginInit();
            this.grpFilterOptions.SuspendLayout();
            this.grpDisplayOptions.SuspendLayout();
            this.SuspendLayout();
            // 
            // pic
            // 
            this.pic.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pic.BackColor = System.Drawing.Color.White;
            this.pic.Location = new System.Drawing.Point(431, 0);
            this.pic.Name = "pic";
            this.pic.Size = new System.Drawing.Size(464, 613);
            this.pic.TabIndex = 0;
            this.pic.TabStop = false;
            this.pic.Paint += new System.Windows.Forms.PaintEventHandler(this.pic_Paint);
            // 
            // hScroll
            // 
            this.hScroll.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.hScroll.LargeChange = 50;
            this.hScroll.Location = new System.Drawing.Point(431, 613);
            this.hScroll.Maximum = 99;
            this.hScroll.Name = "hScroll";
            this.hScroll.Size = new System.Drawing.Size(464, 17);
            this.hScroll.TabIndex = 1;
            this.hScroll.ValueChanged += new System.EventHandler(this.hScroll_ValueChanged);
            // 
            // vScroll
            // 
            this.vScroll.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.vScroll.Location = new System.Drawing.Point(895, 0);
            this.vScroll.Name = "vScroll";
            this.vScroll.Size = new System.Drawing.Size(17, 613);
            this.vScroll.TabIndex = 2;
            this.vScroll.ValueChanged += new System.EventHandler(this.vScroll_ValueChanged);
            // 
            // trkZoom
            // 
            this.trkZoom.AutoSize = false;
            this.trkZoom.LargeChange = 100;
            this.trkZoom.Location = new System.Drawing.Point(75, 24);
            this.trkZoom.Maximum = 6400;
            this.trkZoom.Minimum = 100;
            this.trkZoom.Name = "trkZoom";
            this.trkZoom.Size = new System.Drawing.Size(266, 24);
            this.trkZoom.TabIndex = 3;
            this.trkZoom.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trkZoom.Value = 100;
            this.trkZoom.ValueChanged += new System.EventHandler(this.trkZoom_ValueChanged);
            // 
            // lblZoom
            // 
            this.lblZoom.AutoSize = true;
            this.lblZoom.Location = new System.Drawing.Point(7, 27);
            this.lblZoom.Name = "lblZoom";
            this.lblZoom.Size = new System.Drawing.Size(71, 12);
            this.lblZoom.TabIndex = 4;
            this.lblZoom.Text = "Zoom Factor";
            // 
            // txtZoom
            // 
            this.txtZoom.BackColor = System.Drawing.Color.White;
            this.txtZoom.Location = new System.Drawing.Point(347, 24);
            this.txtZoom.Name = "txtZoom";
            this.txtZoom.ReadOnly = true;
            this.txtZoom.Size = new System.Drawing.Size(52, 21);
            this.txtZoom.TabIndex = 5;
            this.txtZoom.Text = "1.00";
            // 
            // btnGenerateTemp
            // 
            this.btnGenerateTemp.Location = new System.Drawing.Point(93, 408);
            this.btnGenerateTemp.Name = "btnGenerateTemp";
            this.btnGenerateTemp.Size = new System.Drawing.Size(126, 23);
            this.btnGenerateTemp.TabIndex = 6;
            this.btnGenerateTemp.Text = "Generate Image";
            this.btnGenerateTemp.UseVisualStyleBackColor = true;
            this.btnGenerateTemp.Click += new System.EventHandler(this.btnGenerateTemp_Click);
            // 
            // lblBandwidth
            // 
            this.lblBandwidth.AutoSize = true;
            this.lblBandwidth.Location = new System.Drawing.Point(7, 22);
            this.lblBandwidth.Name = "lblBandwidth";
            this.lblBandwidth.Size = new System.Drawing.Size(59, 12);
            this.lblBandwidth.TabIndex = 7;
            this.lblBandwidth.Text = "Bandwidth";
            // 
            // lblSessions
            // 
            this.lblSessions.AutoSize = true;
            this.lblSessions.Location = new System.Drawing.Point(169, 22);
            this.lblSessions.Name = "lblSessions";
            this.lblSessions.Size = new System.Drawing.Size(53, 12);
            this.lblSessions.TabIndex = 8;
            this.lblSessions.Text = "Sessions";
            // 
            // cmbBandwidth
            // 
            this.cmbBandwidth.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbBandwidth.FormattingEnabled = true;
            this.cmbBandwidth.Items.AddRange(new object[] {
            "ALL",
            "0.5",
            "2",
            "8",
            "30",
            "100"});
            this.cmbBandwidth.Location = new System.Drawing.Point(82, 19);
            this.cmbBandwidth.Name = "cmbBandwidth";
            this.cmbBandwidth.Size = new System.Drawing.Size(68, 20);
            this.cmbBandwidth.TabIndex = 9;
            // 
            // cmbSessions
            // 
            this.cmbSessions.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbSessions.FormattingEnabled = true;
            this.cmbSessions.Items.AddRange(new object[] {
            "ALL",
            "1",
            "4",
            "12",
            "40"});
            this.cmbSessions.Location = new System.Drawing.Point(244, 19);
            this.cmbSessions.Name = "cmbSessions";
            this.cmbSessions.Size = new System.Drawing.Size(68, 20);
            this.cmbSessions.TabIndex = 10;
            // 
            // lblMinDelay
            // 
            this.lblMinDelay.AutoSize = true;
            this.lblMinDelay.Location = new System.Drawing.Point(7, 47);
            this.lblMinDelay.Name = "lblMinDelay";
            this.lblMinDelay.Size = new System.Drawing.Size(59, 12);
            this.lblMinDelay.TabIndex = 11;
            this.lblMinDelay.Text = "Min Delay";
            // 
            // lblMaxDelay
            // 
            this.lblMaxDelay.AutoSize = true;
            this.lblMaxDelay.Location = new System.Drawing.Point(169, 47);
            this.lblMaxDelay.Name = "lblMaxDelay";
            this.lblMaxDelay.Size = new System.Drawing.Size(59, 12);
            this.lblMaxDelay.TabIndex = 12;
            this.lblMaxDelay.Text = "Max Delay";
            // 
            // updMinDelay
            // 
            this.updMinDelay.Location = new System.Drawing.Point(82, 45);
            this.updMinDelay.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.updMinDelay.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.updMinDelay.Name = "updMinDelay";
            this.updMinDelay.Size = new System.Drawing.Size(68, 21);
            this.updMinDelay.TabIndex = 13;
            this.updMinDelay.Value = new decimal(new int[] {
            60,
            0,
            0,
            0});
            // 
            // updMaxDelay
            // 
            this.updMaxDelay.Location = new System.Drawing.Point(244, 45);
            this.updMaxDelay.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.updMaxDelay.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.updMaxDelay.Name = "updMaxDelay";
            this.updMaxDelay.Size = new System.Drawing.Size(68, 21);
            this.updMaxDelay.TabIndex = 14;
            this.updMaxDelay.Value = new decimal(new int[] {
            80,
            0,
            0,
            0});
            // 
            // lstTemp
            // 
            this.lstTemp.CheckBoxes = true;
            this.lstTemp.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.hdrEnable,
            this.hdrBw,
            this.hdrSessions,
            this.hdrQueueLen,
            this.hdrTokenRate,
            this.hdrDelay,
            this.hdrRate});
            this.lstTemp.FullRowSelect = true;
            this.lstTemp.Location = new System.Drawing.Point(12, 266);
            this.lstTemp.Name = "lstTemp";
            this.lstTemp.Size = new System.Drawing.Size(413, 136);
            this.lstTemp.TabIndex = 15;
            this.lstTemp.UseCompatibleStateImageBehavior = false;
            this.lstTemp.View = System.Windows.Forms.View.Details;
            this.lstTemp.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.lstTemp_ItemChecked);
            // 
            // hdrEnable
            // 
            this.hdrEnable.Text = "";
            this.hdrEnable.Width = 24;
            // 
            // hdrBw
            // 
            this.hdrBw.Text = "Bandwidth";
            this.hdrBw.Width = 72;
            // 
            // hdrSessions
            // 
            this.hdrSessions.Text = "Sessions";
            this.hdrSessions.Width = 68;
            // 
            // hdrQueueLen
            // 
            this.hdrQueueLen.Text = "Buffer";
            this.hdrQueueLen.Width = 55;
            // 
            // hdrTokenRate
            // 
            this.hdrTokenRate.Text = "Token Rate";
            this.hdrTokenRate.Width = 78;
            // 
            // hdrDelay
            // 
            this.hdrDelay.Text = "Delay";
            this.hdrDelay.Width = 48;
            // 
            // hdrRate
            // 
            this.hdrRate.Text = "Rate";
            this.hdrRate.Width = 44;
            // 
            // btnSelect
            // 
            this.btnSelect.Location = new System.Drawing.Point(12, 408);
            this.btnSelect.Name = "btnSelect";
            this.btnSelect.Size = new System.Drawing.Size(75, 23);
            this.btnSelect.TabIndex = 16;
            this.btnSelect.Text = "Select";
            this.btnSelect.UseVisualStyleBackColor = true;
            this.btnSelect.Click += new System.EventHandler(this.btnSelect_Click);
            // 
            // grpFilterOptions
            // 
            this.grpFilterOptions.Controls.Add(this.chkRemoveMore);
            this.grpFilterOptions.Controls.Add(this.chkRemoveInner);
            this.grpFilterOptions.Controls.Add(this.rdoAverage);
            this.grpFilterOptions.Controls.Add(this.rdoMaximum);
            this.grpFilterOptions.Controls.Add(this.lblDelayType);
            this.grpFilterOptions.Controls.Add(this.lblBandwidth);
            this.grpFilterOptions.Controls.Add(this.lblSessions);
            this.grpFilterOptions.Controls.Add(this.cmbBandwidth);
            this.grpFilterOptions.Controls.Add(this.updMaxDelay);
            this.grpFilterOptions.Controls.Add(this.cmbSessions);
            this.grpFilterOptions.Controls.Add(this.updMinDelay);
            this.grpFilterOptions.Controls.Add(this.lblMinDelay);
            this.grpFilterOptions.Controls.Add(this.lblMaxDelay);
            this.grpFilterOptions.Location = new System.Drawing.Point(12, 12);
            this.grpFilterOptions.Name = "grpFilterOptions";
            this.grpFilterOptions.Size = new System.Drawing.Size(333, 142);
            this.grpFilterOptions.TabIndex = 17;
            this.grpFilterOptions.TabStop = false;
            this.grpFilterOptions.Text = "Filter Options";
            // 
            // chkRemoveInner
            // 
            this.chkRemoveInner.AutoSize = true;
            this.chkRemoveInner.Checked = true;
            this.chkRemoveInner.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkRemoveInner.Location = new System.Drawing.Point(82, 92);
            this.chkRemoveInner.Name = "chkRemoveInner";
            this.chkRemoveInner.Size = new System.Drawing.Size(138, 16);
            this.chkRemoveInner.TabIndex = 18;
            this.chkRemoveInner.Text = "Remove Inner Points";
            this.chkRemoveInner.UseVisualStyleBackColor = true;
            this.chkRemoveInner.CheckedChanged += new System.EventHandler(this.chkRemoveInner_CheckedChanged);
            // 
            // rdoAverage
            // 
            this.rdoAverage.AutoSize = true;
            this.rdoAverage.Location = new System.Drawing.Point(153, 70);
            this.rdoAverage.Name = "rdoAverage";
            this.rdoAverage.Size = new System.Drawing.Size(65, 16);
            this.rdoAverage.TabIndex = 17;
            this.rdoAverage.Text = "Average";
            this.rdoAverage.UseVisualStyleBackColor = true;
            // 
            // rdoMaximum
            // 
            this.rdoMaximum.AutoSize = true;
            this.rdoMaximum.Checked = true;
            this.rdoMaximum.Location = new System.Drawing.Point(82, 70);
            this.rdoMaximum.Name = "rdoMaximum";
            this.rdoMaximum.Size = new System.Drawing.Size(65, 16);
            this.rdoMaximum.TabIndex = 16;
            this.rdoMaximum.TabStop = true;
            this.rdoMaximum.Text = "Maximum";
            this.rdoMaximum.UseVisualStyleBackColor = true;
            // 
            // lblDelayType
            // 
            this.lblDelayType.AutoSize = true;
            this.lblDelayType.Location = new System.Drawing.Point(7, 72);
            this.lblDelayType.Name = "lblDelayType";
            this.lblDelayType.Size = new System.Drawing.Size(65, 12);
            this.lblDelayType.TabIndex = 15;
            this.lblDelayType.Text = "Delay Type";
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(225, 408);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(75, 23);
            this.btnAdd.TabIndex = 18;
            this.btnAdd.Text = "Add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // lblTotal
            // 
            this.lblTotal.AutoSize = true;
            this.lblTotal.Location = new System.Drawing.Point(316, 584);
            this.lblTotal.Name = "lblTotal";
            this.lblTotal.Size = new System.Drawing.Size(89, 12);
            this.lblTotal.TabIndex = 20;
            this.lblTotal.Text = "Total Items: 0";
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(12, 579);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(75, 23);
            this.btnClear.TabIndex = 21;
            this.btnClear.Text = "Clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(225, 579);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 22;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnGenerateFinal
            // 
            this.btnGenerateFinal.Location = new System.Drawing.Point(93, 579);
            this.btnGenerateFinal.Name = "btnGenerateFinal";
            this.btnGenerateFinal.Size = new System.Drawing.Size(126, 23);
            this.btnGenerateFinal.TabIndex = 23;
            this.btnGenerateFinal.Text = "Generate Image";
            this.btnGenerateFinal.UseVisualStyleBackColor = true;
            this.btnGenerateFinal.Click += new System.EventHandler(this.btnGenerateFinal_Click);
            // 
            // lstFinal
            // 
            this.lstFinal.CheckBoxes = true;
            this.lstFinal.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7});
            this.lstFinal.FullRowSelect = true;
            this.lstFinal.Location = new System.Drawing.Point(12, 437);
            this.lstFinal.Name = "lstFinal";
            this.lstFinal.Size = new System.Drawing.Size(413, 136);
            this.lstFinal.TabIndex = 24;
            this.lstFinal.UseCompatibleStateImageBehavior = false;
            this.lstFinal.View = System.Windows.Forms.View.Details;
            this.lstFinal.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.lstFinal_ItemChecked);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "";
            this.columnHeader1.Width = 24;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Bandwidth";
            this.columnHeader2.Width = 72;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Sessions";
            this.columnHeader3.Width = 68;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Buffer";
            this.columnHeader4.Width = 55;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Token Rate";
            this.columnHeader5.Width = 78;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Delay";
            this.columnHeader6.Width = 48;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "Rate";
            this.columnHeader7.Width = 44;
            // 
            // grpDisplayOptions
            // 
            this.grpDisplayOptions.Controls.Add(this.chkDrawImpossible);
            this.grpDisplayOptions.Controls.Add(this.chkDisplayData);
            this.grpDisplayOptions.Controls.Add(this.lblZoom);
            this.grpDisplayOptions.Controls.Add(this.trkZoom);
            this.grpDisplayOptions.Controls.Add(this.txtZoom);
            this.grpDisplayOptions.Location = new System.Drawing.Point(12, 160);
            this.grpDisplayOptions.Name = "grpDisplayOptions";
            this.grpDisplayOptions.Size = new System.Drawing.Size(413, 100);
            this.grpDisplayOptions.TabIndex = 25;
            this.grpDisplayOptions.TabStop = false;
            this.grpDisplayOptions.Text = "Display Options";
            // 
            // chkDrawImpossible
            // 
            this.chkDrawImpossible.AutoSize = true;
            this.chkDrawImpossible.Checked = true;
            this.chkDrawImpossible.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDrawImpossible.Location = new System.Drawing.Point(82, 76);
            this.chkDrawImpossible.Name = "chkDrawImpossible";
            this.chkDrawImpossible.Size = new System.Drawing.Size(306, 16);
            this.chkDrawImpossible.TabIndex = 7;
            this.chkDrawImpossible.Text = "Draw Impossible When There\'s no Point Available";
            this.chkDrawImpossible.UseVisualStyleBackColor = true;
            // 
            // chkDisplayData
            // 
            this.chkDisplayData.AutoSize = true;
            this.chkDisplayData.Checked = true;
            this.chkDisplayData.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDisplayData.Location = new System.Drawing.Point(82, 54);
            this.chkDisplayData.Name = "chkDisplayData";
            this.chkDisplayData.Size = new System.Drawing.Size(246, 16);
            this.chkDisplayData.TabIndex = 6;
            this.chkDisplayData.Text = "Display Delay and Throughput in Graph";
            this.chkDisplayData.UseVisualStyleBackColor = true;
            // 
            // chkRemoveMore
            // 
            this.chkRemoveMore.AutoSize = true;
            this.chkRemoveMore.Checked = true;
            this.chkRemoveMore.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkRemoveMore.Location = new System.Drawing.Point(82, 114);
            this.chkRemoveMore.Name = "chkRemoveMore";
            this.chkRemoveMore.Size = new System.Drawing.Size(132, 16);
            this.chkRemoveMore.TabIndex = 19;
            this.chkRemoveMore.Text = "Remove More Points";
            this.chkRemoveMore.UseVisualStyleBackColor = true;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(913, 631);
            this.Controls.Add(this.grpDisplayOptions);
            this.Controls.Add(this.lstFinal);
            this.Controls.Add(this.btnGenerateFinal);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.lblTotal);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.grpFilterOptions);
            this.Controls.Add(this.btnSelect);
            this.Controls.Add(this.lstTemp);
            this.Controls.Add(this.btnGenerateTemp);
            this.Controls.Add(this.vScroll);
            this.Controls.Add(this.hScroll);
            this.Controls.Add(this.pic);
            this.MinimumSize = new System.Drawing.Size(900, 640);
            this.Name = "MainWindow";
            this.Text = "Ns Report";
            this.SizeChanged += new System.EventHandler(this.MainWindow_SizeChanged);
            ((System.ComponentModel.ISupportInitialize)(this.pic)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkZoom)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.updMinDelay)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.updMaxDelay)).EndInit();
            this.grpFilterOptions.ResumeLayout(false);
            this.grpFilterOptions.PerformLayout();
            this.grpDisplayOptions.ResumeLayout(false);
            this.grpDisplayOptions.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pic;
        private System.Windows.Forms.HScrollBar hScroll;
        private System.Windows.Forms.VScrollBar vScroll;
        private System.Windows.Forms.TrackBar trkZoom;
        private System.Windows.Forms.Label lblZoom;
        private System.Windows.Forms.TextBox txtZoom;
        private System.Windows.Forms.Button btnGenerateTemp;
        private System.Windows.Forms.Label lblBandwidth;
        private System.Windows.Forms.Label lblSessions;
        private System.Windows.Forms.ComboBox cmbBandwidth;
        private System.Windows.Forms.ComboBox cmbSessions;
        private System.Windows.Forms.Label lblMinDelay;
        private System.Windows.Forms.Label lblMaxDelay;
        private System.Windows.Forms.NumericUpDown updMinDelay;
        private System.Windows.Forms.NumericUpDown updMaxDelay;
        private System.Windows.Forms.ListView lstTemp;
        private System.Windows.Forms.ColumnHeader hdrBw;
        private System.Windows.Forms.ColumnHeader hdrSessions;
        private System.Windows.Forms.ColumnHeader hdrDelay;
        private System.Windows.Forms.ColumnHeader hdrRate;
        private System.Windows.Forms.Button btnSelect;
        private System.Windows.Forms.GroupBox grpFilterOptions;
        private System.Windows.Forms.ColumnHeader hdrEnable;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Label lblTotal;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnGenerateFinal;
        private System.Windows.Forms.ColumnHeader hdrQueueLen;
        private System.Windows.Forms.ColumnHeader hdrTokenRate;
        private System.Windows.Forms.ListView lstFinal;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.RadioButton rdoAverage;
        private System.Windows.Forms.RadioButton rdoMaximum;
        private System.Windows.Forms.Label lblDelayType;
        private System.Windows.Forms.CheckBox chkRemoveInner;
        private System.Windows.Forms.GroupBox grpDisplayOptions;
        private System.Windows.Forms.CheckBox chkDisplayData;
        private System.Windows.Forms.CheckBox chkDrawImpossible;
        private System.Windows.Forms.CheckBox chkRemoveMore;
    }
}

