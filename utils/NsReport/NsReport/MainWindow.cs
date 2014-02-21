using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace NsReport
{
    public partial class MainWindow : Form
    {
        // Zoom factor
        private float ZOOM = 1.0f;

        // Size constants
        private const float IMAGE_WIDTH = 1320.0f;
        private const float IMAGE_HEIGHT = 640.0f;
        private const float GRAPH_WIDTH = 200.0f;
        private const float GRAPH_HEIGHT = 124.0f;
        private const float LEFT_MARGIN = 69.0f;
        private const float TOP_MARGIN = 15.0f;
        private const float HORIZONTAL_SPACING = 47.0f;
        private const float VERTICAL_SPACING = 26.0f;
        private const float LINE_WIDTH = 1.0f;
        private const float POINT_BORDER_WIDTH = 1.0f;
        private const float POINT_SPACING_WIDTH = 1.0f;
        private const float POINT_BLOCK_SIZE = 3.0f;

        // The image
        private Bitmap m_image = null;

        // The log fine
        private byte[] m_file = null;

        // Simulation results
        private List<Point>[,] m_data = new List<Point>[4, 5];
        private List<Point>[,] m_final = new List<Point>[4, 5];

        // Rows and columns
        string[] rows = new string[] { "1", "4", "12", "40" };
        string[] cols = new string[] { "0.5", "2", "8", "80", "100" };

        // Simulation result node
        private class Point
        {
            public bool Visible;
            public float TokenRate;
            public int BufferSize;
            public int QueueDelay;
            public float Throughput;

            public Point(float tokenRate, int bufferSize, int queueDelay, float throughput)
            {
                Visible = true; // The default
                TokenRate = tokenRate;
                BufferSize = bufferSize;
                QueueDelay = queueDelay;
                Throughput = throughput;
            }
        }

        private enum DelayType
        {
            Maximum, Average
        }

        public MainWindow()
        {
            InitializeComponent();

            // Init bandwidth & session condition to "ALL"
            cmbBandwidth.SelectedIndex = 0;
            cmbSessions.SelectedIndex = 0;

            // Read the log file into memory to speed up
            using (FileStream fs = new FileStream("summary.log", FileMode.Open))
            {
                FileInfo info = new FileInfo("summary.log");
                m_file = new byte[info.Length];
                fs.Read(m_file, 0, (int)info.Length);
            }

            // Initialize the final data set
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    m_final[row, col] = new List<Point>();
                }
            }
        }

        private void Filter(string bandwidth, string sessions, int minDelay, int maxDelay, 
            DelayType type, bool removeInner, bool removeMore)
        {
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    m_data[row, col] = new List<Point>();

                    if ((bandwidth != "ALL" && bandwidth != cols[col]) ||
                        (sessions != "ALL" && sessions != rows[row]))
                    {
                        continue;
                    }

                    MemoryStream ms = new MemoryStream(m_file);
                    StreamReader sr = new StreamReader(ms);
                    string line;

                    // Skip the first two lines
                    sr.ReadLine();
                    sr.ReadLine();

                    // Process the remaining
                    while ((line = sr.ReadLine()) != null)
                    {
                        string[] fields = line.Split('\t');

                        double bw   = Convert.ToDouble(fields[0]);
                        int    num  = Convert.ToInt32(fields[1]);
                        double pct  = Convert.ToDouble(fields[2]);
                        int    qlen = Convert.ToInt32(fields[3]);
                        double maxq = Convert.ToDouble(fields[4]);
                        double avgq = Convert.ToDouble(fields[5]);
                        int    maxd = Convert.ToInt32(fields[6]);
                        int    avgd = Convert.ToInt32(fields[7]);

                        // Rate is like "26.8 (0.42)"
                        string[] rateFields = fields[8].Split(new char[] { ' ', '(', ')' }, 
                            StringSplitOptions.RemoveEmptyEntries);
                        double rate = Convert.ToDouble(rateFields[1]);

                        if (rows[row] == fields[1] && cols[col] == fields[0])
                        {
                            if (type == DelayType.Maximum)
                            {
                                if (maxd >= minDelay && maxd <= maxDelay)
                                {
                                    m_data[row, col].Add(
                                        new Point((float)pct, qlen, maxd, (float)rate));
                                }
                            }
                            else if (type == DelayType.Average)
                            {
                                if (avgd >= minDelay && avgd <= maxDelay)
                                {
                                    m_data[row, col].Add(
                                        new Point((float)pct, qlen, avgd, (float)rate));
                                }
                            }
                        }
                    }
                    sr.Close();
                    ms.Close();

                    if (removeInner) // Remove inner nodes when necessary
                    {
                        List<Point> points = m_data[row, col];
                        for (int i = 0; i < points.Count; i++)
                        {
                            Point p = points[i];
                            float pct = p.TokenRate;
                            int qlen = p.BufferSize;

                            // Determine if it's an inner point
                            bool top = false; // if there's a point on top
                            bool right = false; // if there's a point on the right
                            for (int j = i + 1; j < points.Count; j++)
                            {
                                if (points[j].BufferSize == qlen && points[j].TokenRate > pct)
                                {
                                    right = true;
                                }
                                if (points[j].TokenRate == pct && points[j].BufferSize > qlen)
                                {
                                    top = true;
                                }
                                if (right && top)
                                {
                                    break;
                                }
                            }

                            if (top && right)
                            {
                                points.RemoveAt(i--);
                            }
                            else if ((top || right) && removeMore)
                            {
                                points.RemoveAt(i--);
                            }
                        }
                    }
                }
            }
        }

        private void GenerateImage(List<Point>[,] data)
        {
            m_image = new Bitmap((int)(IMAGE_WIDTH * ZOOM), (int)(IMAGE_HEIGHT * ZOOM));
            Graphics g = Graphics.FromImage(m_image);

            Brush blackBrush = new SolidBrush(Color.Black);
            Font bandwidthFont = new Font("Tahoma", 9.0f * ZOOM, FontStyle.Bold);

            StringFormat bandwidthTextFormat = new StringFormat();
            bandwidthTextFormat.Alignment = StringAlignment.Center;

            StringFormat ftpTextFormat = new StringFormat();
            ftpTextFormat.Alignment = StringAlignment.Far;
            ftpTextFormat.LineAlignment = StringAlignment.Center;

            // Graphics settings
            g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;

            // Clear
            g.Clear(Color.White);

            // Draw graphs
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    DrawGraph(g, 
                        (LEFT_MARGIN + (GRAPH_WIDTH + HORIZONTAL_SPACING) * col) * ZOOM,
                        (TOP_MARGIN + (GRAPH_HEIGHT + VERTICAL_SPACING) * row) * ZOOM,
                        data[row, col], col >= 3, chkDrawImpossible.Checked);
                }
            }

            // Draw titles
            string[] bandwidth = new string[] { 
                "0.5 Mbps", "2 Mbps", "8 Mbps", "30 Mbps", "100 Mbps" 
            };
            for (int col = 0; col < 5; col++)
            {
                g.DrawString(bandwidth[col], bandwidthFont, blackBrush, 
                    (LEFT_MARGIN + GRAPH_WIDTH * (col + 0.5f) + HORIZONTAL_SPACING * col) * ZOOM, 
                    (TOP_MARGIN + GRAPH_HEIGHT * 4 + VERTICAL_SPACING * 4) * ZOOM, 
                    bandwidthTextFormat);
            }

            string[] ftp = new string[] { "1 FTP", "4 FTP", "12 FTP", "40 FTP" };
            for (int row = 0; row < 4; row++)
            {
                g.DrawString(ftp[row], bandwidthFont, blackBrush,
                    (LEFT_MARGIN - 18.0f) * ZOOM,
                    (TOP_MARGIN + GRAPH_HEIGHT * (row + 0.5f) + VERTICAL_SPACING * row) * ZOOM,
                    ftpTextFormat);
            }

            // Update scroll bars
            hScroll.Value = 0;
            vScroll.Value = 0;
            hScroll.Maximum = (int)(m_image.Width - 1);
            vScroll.Maximum = (int)(m_image.Height - 1);
            hScroll.LargeChange = pic.Width;
            vScroll.LargeChange = pic.Height;
        }

        private void DrawGraph(
            Graphics g, float x, float y, List<Point> points, bool drawImpossible)
        {
            DrawGraph(g, x, y, points, false, drawImpossible);
        }

        private void DrawGraph(Graphics g, float x, float y, 
            List<Point> points, bool largeScale, bool drawImpossible)
        {
            Pen blackPen = new Pen(Color.Black, LINE_WIDTH * ZOOM);
            Pen grayPen = new Pen(Color.LightGray, LINE_WIDTH * ZOOM);
            Brush blackBrush = new SolidBrush(Color.Black);
            Font scaleFont = new Font("Tahoma", 7.5f * ZOOM);

            StringFormat yAxisTextFormat = new StringFormat();
            yAxisTextFormat.Alignment = StringAlignment.Far;
            yAxisTextFormat.LineAlignment = StringAlignment.Center;

            Pen redPen = new Pen(Color.FromArgb(223, 0, 36), POINT_BORDER_WIDTH * ZOOM);
            Pen whitePen = new Pen(Color.White, POINT_SPACING_WIDTH * ZOOM);
            Brush redBrush = new SolidBrush(Color.FromArgb(223, 0, 36));
            Font pointFont = new Font("Arial", 7.0f * ZOOM);
            Brush blueBrush = new SolidBrush(Color.FromArgb(6, 105, 178));
            Brush whiteBrush = new SolidBrush(Color.White);

            StringFormat pointTextFormat = new StringFormat();
            pointTextFormat.LineAlignment = StringAlignment.Far;

            Font impossibleFont = new Font("Tahoma", 12.0f * ZOOM, FontStyle.Bold);
            StringFormat impossibleFormat = new StringFormat();
            impossibleFormat.Alignment = StringAlignment.Center;
            impossibleFormat.LineAlignment = StringAlignment.Center;

            // Border
            g.DrawRectangle(blackPen, x, y, GRAPH_WIDTH * ZOOM, GRAPH_HEIGHT * ZOOM);

            // Horizontal lines
            for (int i = 1; i <= 3; i++)
            {
                g.DrawLine(grayPen,
                    x + LINE_WIDTH * ZOOM, y + GRAPH_HEIGHT * ZOOM * i * 0.25f,
                    x + (GRAPH_WIDTH - LINE_WIDTH) * ZOOM, y + GRAPH_HEIGHT * ZOOM * i * 0.25f);
            }

            // Vertical lines
            for (int i = 1; i <= 4; i++)
            {
                g.DrawLine(grayPen,
                    x + GRAPH_WIDTH * ZOOM * i * 0.2f, y + LINE_WIDTH * ZOOM,
                    x + GRAPH_WIDTH * ZOOM * i * 0.2f, y + (GRAPH_HEIGHT - LINE_WIDTH) * ZOOM);
            }

            // X-Axis scales
            for (int i = 0; i <= 5; i++)
            {
                g.DrawString(String.Format("{0}%", 50 + i * 10), scaleFont, blackBrush,
                    x + GRAPH_WIDTH * ZOOM * i * 0.2f - 2.5f, 
                    y + GRAPH_HEIGHT * ZOOM + 2.0f * ZOOM);
            }

            // Y-Axis scales
            for (int i = 0; i <= 4; i++)
            {
                g.DrawString(String.Format("{0}", largeScale ? 1 << (i * 2 + 1) : 1 << (i + 1)), 
                    scaleFont, blackBrush,
                    x - 3.0f * ZOOM, y + GRAPH_HEIGHT * ZOOM * (1.0f - 0.25f * i) + 1.0f, 
                    yAxisTextFormat);
            }

            // Points
            if (points != null)
            {
                if (points.Count == 0 && drawImpossible)
                {
                    DrawStokeText(g, "IMPOSSIBLE", impossibleFont, blueBrush, whiteBrush,
                        x + GRAPH_WIDTH * ZOOM * 0.5f, y + GRAPH_HEIGHT * ZOOM * 0.5f,
                        impossibleFormat);
                }

                for (int i = 0; i < points.Count; i++)
                {
                    if (points[i].Visible)
                    {
                        float xPos = (points[i].TokenRate - 0.50f) * 2.0f;
                        float yPos = largeScale ? 
                            (float)((Math.Log(points[i].BufferSize, 2.0) - 1.0) / 8.0) :
                            (float)((Math.Log(points[i].BufferSize, 2.0) - 1.0) / 4.0);
                        float centerX = (float)Math.Round(x + GRAPH_WIDTH * ZOOM * xPos, 0);
                        float centerY = (float)Math.Round(
                            y + GRAPH_HEIGHT * ZOOM * (1.0f - yPos), 0);
                        float edgeOffset = POINT_SPACING_WIDTH + POINT_BORDER_WIDTH * 0.5f;

                        float offset = (POINT_BLOCK_SIZE) * 0.5f * ZOOM;

                        // Draw border
                        g.DrawRectangle(redPen,
                            centerX - offset - edgeOffset * ZOOM,
                            centerY - offset - edgeOffset * ZOOM,
                            (edgeOffset * 2 + POINT_BLOCK_SIZE) * ZOOM,
                            (edgeOffset * 2 + POINT_BLOCK_SIZE) * ZOOM);

                        // Draw spacing
                        g.DrawRectangle(whitePen,
                            centerX - offset - POINT_SPACING_WIDTH * 0.5f * ZOOM,
                            centerY - offset - POINT_SPACING_WIDTH * 0.5f * ZOOM,
                            (POINT_SPACING_WIDTH + POINT_BLOCK_SIZE) * ZOOM,
                            (POINT_SPACING_WIDTH + POINT_BLOCK_SIZE) * ZOOM);

                        // Draw block
                        g.FillRectangle(redBrush,
                            centerX - offset, centerY - offset, 
                            POINT_BLOCK_SIZE * ZOOM, POINT_BLOCK_SIZE * ZOOM);

                        if (chkDisplayData.Checked)
                        {
                            // Draw delay & throughput
                            DrawStokeText(g, String.Format("{0}ms ({1}%)",
                                points[i].QueueDelay, (int)(points[i].Throughput * 100.0 + 0.5)),
                                pointFont, blueBrush, whiteBrush,
                                centerX + (offset + edgeOffset - 2.0f) * ZOOM,
                                centerY - (offset + edgeOffset - 2.0f) * ZOOM, pointTextFormat);
                        }
                    }
                }
            }
        }

        private void DrawStokeText(Graphics g, string text, Font font, 
            Brush brush, Brush strokeBrush, float x, float y, StringFormat format)
        {
            for (double angle = 0; angle < Math.PI * 2; angle += Math.PI / 18)
            {
                float dx = ZOOM * (float)Math.Cos(angle);
                float dy = ZOOM * (float)Math.Sin(angle);
                g.DrawString(text, font, strokeBrush, x + dx, y + dy, format);
            }
            g.DrawString(text, font, brush, x, y, format);
        }

        private void pic_Paint(object sender, PaintEventArgs e)
        {
            if (m_image != null)
            {
                e.Graphics.DrawImage(m_image, -hScroll.Value, -vScroll.Value);
            }
        }

        private void hScroll_ValueChanged(object sender, EventArgs e)
        {
            pic.Refresh();
        }

        private void vScroll_ValueChanged(object sender, EventArgs e)
        {
            pic.Refresh();
        }

        private void MainWindow_SizeChanged(object sender, EventArgs e)
        {
            hScroll.LargeChange = pic.Width;
            vScroll.LargeChange = pic.Height;
            hScroll.Value = Math.Min(hScroll.Maximum - hScroll.LargeChange + 1, hScroll.Value);
            vScroll.Value = Math.Min(vScroll.Maximum - vScroll.LargeChange + 1, vScroll.Value);
        }

        private void trkZoom_ValueChanged(object sender, EventArgs e)
        {
            txtZoom.Text = String.Format("{0:0.00}", trkZoom.Value / 100.0);
        }

        private void lstTemp_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            for (int row = 0; row < 4; row++) // Update the visible field
            {
                for (int col = 0; col < 5; col++)
                {
                    for (int i = 0; i < m_data[row, col].Count; i++)
                    {
                        Point p = m_data[row, col][i];
                        if (e.Item.SubItems[1].Text == cols[col] &&
                            e.Item.SubItems[2].Text == rows[row] &&
                            e.Item.SubItems[3].Text == p.BufferSize.ToString() &&
                            e.Item.SubItems[4].Text == p.TokenRate.ToString("0.00"))
                        {
                            p.Visible = e.Item.Checked;
                            break;
                        }
                    }
                }
            }
        }

        private void lstFinal_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            for (int row = 0; row < 4; row++) // Update the visible field
            {
                for (int col = 0; col < 5; col++)
                {
                    for (int i = 0; i < m_final[row, col].Count; i++)
                    {
                        Point p = m_final[row, col][i];
                        if (e.Item.SubItems[1].Text == cols[col] &&
                            e.Item.SubItems[2].Text == rows[row] &&
                            e.Item.SubItems[3].Text == p.BufferSize.ToString() &&
                            e.Item.SubItems[4].Text == p.TokenRate.ToString("0.00"))
                        {
                            p.Visible = e.Item.Checked;
                            break;
                        }
                    }
                }
            }
        }

        private void RebuildList(ListView list, List<Point>[,] data)
        {
            list.Items.Clear();

            for (int col = 0; col < 5; col++)
            {
                for (int row = 0; row < 4; row++)
                {
                    for (int i = 0; i < data[row, col].Count; i++)
                    {
                        Point p = data[row, col][i];
                        ListViewItem item = new ListViewItem("");

                        item.Checked = p.Visible;
                        item.SubItems.Add(cols[col]);
                        item.SubItems.Add(rows[row]);
                        item.SubItems.Add(p.BufferSize.ToString());
                        item.SubItems.Add(p.TokenRate.ToString("0.00"));
                        item.SubItems.Add(p.QueueDelay.ToString());
                        item.SubItems.Add(p.Throughput.ToString("0.00"));
                        list.Items.Add(item);
                    }
                }
            }
        }

        private void UpdateTotalCount()
        {
            int count = 0;
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    count += m_final[row, col].Count;
                }
            }
            lblTotal.Text = String.Format("Total Items: {0}", count);
        }

        private void btnSelect_Click(object sender, EventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            Filter(
                cmbBandwidth.SelectedItem.ToString(),
                cmbSessions.SelectedItem.ToString(), 
                (int)(updMinDelay.Value), 
                (int)(updMaxDelay.Value),
                rdoMaximum.Checked ? DelayType.Maximum : DelayType.Average, 
                chkRemoveInner.Checked, 
                chkRemoveMore.Checked);
            RebuildList(lstTemp, m_data);
            this.Cursor = Cursors.Default;
        }

        private void btnGenerateTemp_Click(object sender, EventArgs e)
        {
            ZOOM = trkZoom.Value / 100.0f;
            this.Cursor = Cursors.WaitCursor;
            GenerateImage(m_data);
            this.Cursor = Cursors.Default;
            pic.Refresh();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    for (int i = 0; i < m_data[row, col].Count; i++)
                    {
                        Point p = m_data[row, col][i];
                        if (p.Visible)
                        {
                            m_final[row, col].Add(p);
                        }
                    }
                }
            }

            RebuildList(lstFinal, m_final);
            UpdateTotalCount();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    m_final[row, col].Clear();
                }
            }
            lstFinal.Items.Clear();
            UpdateTotalCount();
        }

        private void btnGenerateFinal_Click(object sender, EventArgs e)
        {
            ZOOM = trkZoom.Value / 100.0f;
            this.Cursor = Cursors.WaitCursor;
            GenerateImage(m_final);
            this.Cursor = Cursors.Default;
            pic.Refresh();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (m_image == null)
            {
                MessageBox.Show("The image has not been generated yet!", "Error", 
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            else
            {
                SaveFileDialog dialog = new SaveFileDialog();
                dialog.Filter = "PNG Image File (*.png)|*.png";
                dialog.CheckPathExists = true;
                dialog.InitialDirectory = Application.StartupPath;
                dialog.FileName = "Export.png";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    m_image.Save(dialog.FileName);
                    MessageBox.Show("The image has been saved!", "Ns Report",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        private void chkRemoveInner_CheckedChanged(object sender, EventArgs e)
        {
            if (chkRemoveInner.Checked == false)
            {
                chkRemoveMore.Checked = false;
            }
        }
    }
}
