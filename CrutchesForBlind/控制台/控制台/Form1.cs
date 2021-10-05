using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace 控制台
{
    public partial class Form1 : Form
    {
        tcp Tcp;
        List<Position> positions = new List<Position>();
        Thread th1;
        Thread th2;
        WriteHTML html = new WriteHTML();
        public Form1()
        {
            Tcp = new tcp(this);
            InitializeComponent();
            th1 = new Thread(new ThreadStart(Tcp.start));
            th2 = new Thread(new ThreadStart(check));
            th1.Start();
            th2.Start();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Tcp.isStart = true;
        }

        private void button4_Click(object sender, EventArgs e)
        {

            Tcp.isStart = false;
        }

        public void check()
        {
            Thread.CurrentThread.IsBackground = true;
            while (true)
            {
                Thread.Sleep(1000);
                string recStr = Tcp.recStr;
                int gps = recStr.IndexOf("$GNGGA");
                int bd = recStr.IndexOf("$BDGGA");
                //bool isGpsFound = false;
                //bool isBdFound = false;
                try
                {

                    if (gps != -1)
                    {
                        int p1 = recStr.IndexOf(",", gps);
                        int p2 = recStr.IndexOf(",", p1 + 1);
                        int p3 = recStr.IndexOf(",", p2 + 1);
                        int p4 = recStr.IndexOf(",", p3 + 1);
                        int p5 = recStr.IndexOf(",", p4 + 1);
                        int p6 = recStr.IndexOf(",", p5 + 1);
                        if (p6 != -1)
                        {
                            string gpsStr = recStr.Substring(p1 + 1, p6 - p1 - 1);
                            string[] gpsStrs = gpsStr.Split(',');
                            positions.Add(new Position("GPS", gpsStrs[1], gpsStrs[3], gpsStrs[2], gpsStrs[4]));
                            //listBox1.Items.Add(positions[positions.Count - 1]);
                            //isGpsFound = true;
                            flashText();
                        }
                    }
                    if (bd != -1)
                    {
                        int b1 = recStr.IndexOf(",", bd);
                        int b2 = recStr.IndexOf(",", b1 + 1);
                        int b3 = recStr.IndexOf(",", b2 + 1);
                        int b4 = recStr.IndexOf(",", b3 + 1);
                        int b5 = recStr.IndexOf(",", b4 + 1);
                        int b6 = recStr.IndexOf(",", b5 + 1);
                        if (b6 != -1)
                        {
                            string bdStr = recStr.Substring(b1 + 1, b6 - b1 - 1);
                            string[] bdStrs = bdStr.Split(',');
                            positions.Add(new Position("北斗", bdStrs[1], bdStrs[3], bdStrs[2], bdStrs[4]));
                            //listBox1.Items.Add(positions[positions.Count - 1]);
                            //isBdFound = true;
                            flashText();
                        }
                    }
                    Tcp.recStr = "";
                }
                catch
                {
                    
                }
            }
        }

        public void flashText()
        {
            listBox1.Invoke(new MethodInvoker(delegate
            {
                listBox1.Items.Add(positions[positions.Count - 1]);

            }));

            html.write(positions[positions.Count - 1]);
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
        }

        private void button5_Click(object sender, EventArgs e)
        {
            try
            {
                positions.RemoveAt(listBox1.SelectedIndex);
                listBox1.Items.Remove(listBox1.SelectedItem);
            }
            catch (Exception f)
            {
                MessageBox.Show("未选择记录"+f.ToString());
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                new Map(positions[listBox1.SelectedIndex]).Show();
            }
            catch (Exception f)
            {
                MessageBox.Show("未选择记录"+f.ToString());
            }
        }
    }
}
