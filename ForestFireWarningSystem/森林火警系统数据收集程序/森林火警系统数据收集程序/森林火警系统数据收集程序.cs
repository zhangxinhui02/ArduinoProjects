/***************
 * 森林火警系统上位机数据收集程序
 * Author : zhangxinhui02 SHIINASAMA
 * 2021.6.23
 * 
 * 代码借鉴了 https://codechina.csdn.net/mirrors/yuace/CSharp--SerialPort 
 ***************/


using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace 森林火警系统数据收集程序
{
    public partial class 森林火警系统数据收集程序 : Form
    {
        string server = "127.0.0.1";      // 域名或者地址
        uint port = 3306;           // 端口
        string userId = "root";      // 用户名
        string pwd = "123456";         // 密码
        string database = "test";    // 数据库名
        string table = "data";       // 表名

        public 森林火警系统数据收集程序()
        {
            InitializeComponent();
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;//设置该属性 为false
        }

        private void 森林火警系统数据收集程序_Load(object sender, EventArgs e)
        {
            RegistryKey keyCom = Registry.LocalMachine.OpenSubKey("Hardware\\DeviceMap\\SerialComm");
            if (keyCom != null)
            {
                string[] sSubKeys = keyCom.GetValueNames();
                cmbPort.Items.Clear();
                foreach (string sName in sSubKeys)
                {
                    string sValue = (string)keyCom.GetValue(sName);
                    cmbPort.Items.Add(sValue);
                }
                if (cmbPort.Items.Count > 0)
                    cmbPort.SelectedIndex = 0;
            }

            cmbBaud.Text = "115200";
        }

        bool isOpened = false;//串口状态标志

        private void button1_Click(object sender, EventArgs e)
        {
            if (cmbPort.Text == "") {
                MessageBox.Show("请输入串口名称");
                return;
            }
            if (!isOpened)
            {
                serialPort.PortName = cmbPort.Text;
                serialPort.BaudRate = Convert.ToInt32(cmbBaud.Text, 10);
                try
                {
                    serialPort.Open();     //打开串口
                    button1.Text = "停止服务";
                    label4.Text = "服务运行中，将写入温度数据到数据库……";
                    cmbPort.Enabled = false;//关闭使能
                    cmbBaud.Enabled = false;
                    isOpened = true;
                    serialPort.DataReceived += new SerialDataReceivedEventHandler(post_DataReceived);//串口接收处理函数
                }
                catch
                {
                    MessageBox.Show("串口打开失败！");
                }
            }
            else
            {
                try
                {
                    serialPort.Close();     //关闭串口
                    button1.Text = "开始服务";
                    label4.Text = "服务未运行";
                    cmbPort.Enabled = true;//打开使能
                    cmbBaud.Enabled = true;
                    isOpened = false;
                }
                catch
                {
                    MessageBox.Show("串口关闭失败！");
                }
            }
        }

        private void post_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            MySqlConnectionStringBuilder builder = new MySqlConnectionStringBuilder();
            builder.Server = this.server;
            builder.Port = this.port;
            builder.UserID = this.userId;
            builder.Password = this.pwd;
            builder.Database = this.database;

            MySqlConnection connection = new MySqlConnection(builder.ConnectionString);
            try
            {
                connection.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            string str = serialPort.ReadExisting();//字符串方式读
            ReceiveTbox.Text += str;
            DateTime dateTime = DateTime.Now;
            string date = dateTime.ToString("yyyy-MM-dd");
            string time = dateTime.ToString("H:m:ss");
            string t = cut(str);

            //string sqlString = @"insert into Data(Date,Time,Temperature) values('2020-10-20','00:00:00','30.02')";
            string sqlString = "insert into " + this.table + "(date,time,temperature) values(\'" + date + "\',\'" + time + "\',\'" + t + "\')";
            MySqlCommand cmd = new MySqlCommand(sqlString, connection);
            cmd.ExecuteNonQuery();
            connection.Close();
        }

        private void 森林火警系统数据收集程序_FormClosed(object sender, FormClosedEventArgs e)
        {

        }

        public string cut(string recStr)
        {
            int p1 = recStr.IndexOf("#");
            try
            {
                if (p1 != -1)
                {
                    int p2 = recStr.IndexOf("$", p1);
                    if (p2 != -1)
                    {
                        return recStr.Substring(p1 + 1, p2 - p1 - 1);
                        //float temp = Convert.ToInt32(tempstr);
                    }
                    return "0.0";
                }
                return "0.0";
            }
            catch (Exception e)
            {
                MessageBox.Show(e.ToString());
                return "0.0";
            }
        }
    }
}
