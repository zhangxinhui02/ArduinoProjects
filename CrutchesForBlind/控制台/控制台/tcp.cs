using System;
using System.IO;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace 控制台
{
    class tcp
    {
        Form1 c;
        public string recStr = "";
        TcpClient client;
        TcpListener listener = new TcpListener(IPAddress.Any, 9999);
        NetworkStream stream;
        BinaryWriter writer;
        BinaryReader reader;
        public bool isStart = true;
        public void start()
        {
            Thread.CurrentThread.IsBackground = true;
            listener.Start();
            //Console.WriteLine("服务器已启动");
            //c.textBox1.Text += ("Done");
            while (true)
            {
                try
                {
                    client = listener.AcceptTcpClient();

                    stream = client.GetStream();
                    writer = new BinaryWriter(stream);
                    reader = new BinaryReader(stream);
                    while (isStart)
                    {
                        try
                        {
                            char str = (char)reader.ReadByte();
                            recStr += str;
                            //Console.Write(str);

                            c.textBox1.Invoke(new MethodInvoker(delegate
                            {
                                c.textBox1.Text=recStr;
                            }));
                            //c.textBox1.Text += (str);
                        }
                        catch
                        {
                            break;
                        }
                    }
                }
                catch
                {

                }
            }
        }

        public void send(string str)
        {
            writer.Write(str);
        }

        public tcp(Form1 c)
        {
            this.c = c;
        }
    }
}
