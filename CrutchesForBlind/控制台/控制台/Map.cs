using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 控制台
{
    public partial class Map : Form
    {
        public Map(Position p)
        {
            InitializeComponent();
            webBrowser1.Navigate("https://uri.amap.com/marker?position="+ p.jingdu + "," + p.weidu);
            label1.Text += p.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            webBrowser1.Refresh();
        }
    }
}
