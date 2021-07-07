using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace 控制台
{
    public class Position
    {
        public string kind = "";
        public  string weidu = "";
        public string jingdu = "";
        public string weidubq = "";
        public string jingdubq = "";
        public DateTime dt = DateTime.Now;
        public string text = "";
        public Position(string a,string b,string c,string d,string e)
        {
            kind = a;
            weidu = b;
            weidu = pConvert(weidu);
            jingdu = c;
            jingdu = pConvert(jingdu);
            weidubq = d;
            jingdubq = e;
            text = "定位系统：" + kind + " 纬度：" + weidu + "," + weidubq + " 经度：" + jingdu + "," + jingdubq+" 时间：" +dt.ToShortDateString()+" "+dt.ToLongTimeString();
        }

        public override string ToString()
        {
            return text;
        }

        string pConvert(string str)
        {
            double yuanxiaoshu=Convert.ToDouble(str) / 100;
            int zhengshu = (int)yuanxiaoshu;
            double xiaoshu = yuanxiaoshu - zhengshu;
            double zhuanhuan = xiaoshu * 5/3;
            zhuanhuan = (int)(zhuanhuan * 1000000);
            zhuanhuan /= 1000000;
            //xiaoshu*10/6
            return Convert.ToString(zhengshu + zhuanhuan);
        }
    }
}
