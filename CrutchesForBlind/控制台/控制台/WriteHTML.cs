using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace 控制台
{
    class WriteHTML
    {
        public static string path = @"C:\inetpub\wwwroot\position.html";
        //public static FileStream writer = File.OpenWrite(path);
        public static string text = "<!DOCTYPE html><html><head><title>新一代电子导盲犬 定位数据</title></head><body><h1>新一代电子导盲犬 定位数据</h1><br/><a href=\"http://mikumikumi.xyz:520/position.html\">刷新</a><hr><!--write--></body></html>";
        public static int index = text.IndexOf("<!--write-->") + 12;

        public void write(Position p)
        {
            text=text.Insert(index, "<p>"+p.ToString()+"</p><br/><a href=\"https://uri.amap.com/marker?position=" + p.jingdu + "," + p.weidu + "\">查看地图</a><br/><br/>");
            File.WriteAllText(path, text, Encoding.UTF8);
            
        }
    }
}
