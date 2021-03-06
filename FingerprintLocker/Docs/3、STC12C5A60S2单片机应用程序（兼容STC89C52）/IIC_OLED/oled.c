#include <string.h>
#include "oled.h"
#include "oledfont.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

u32 HZNUM;

//返回GB16字库里汉字个数
u32 GB16_NUM(void)
{
  u32 HZ_NUM;
  u8 *PT;
  PT = hz_index;

  while(*PT++ != '\0')
  {
    HZ_NUM++;
  }
  return HZ_NUM / 2;
}

/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{

  OLED_SCL = 1;
  OLED_SDIN = 1;
  OLED_SDIN = 0;
  OLED_SCL = 0;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
  OLED_SCL = 1 ;
//	OLED_SCL=0;
  OLED_SDIN = 0;
  OLED_SDIN = 1;

}

void IIC_Wait_Ack()
{
  OLED_SCL = 1 ;
  OLED_SCL = 0;
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(u8 IIC_Byte)
{
  u8 i;
  u8 m, da;
  da = IIC_Byte;
  OLED_SCL = 0;
  for(i = 0; i < 8; i++)
  {
    m = da;
    //	OLED_SCL=0;
    m = m & 0x80;
    if(m == 0x80)
    {
      OLED_SDIN = 1;
    }
    else OLED_SDIN = 0;
    da = da << 1;
    OLED_SCL = 1;
    OLED_SCL = 0;
  }


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(u8 IIC_Command)
{
  IIC_Start();
  Write_IIC_Byte(0x78);            //Slave address,SA0=0
  IIC_Wait_Ack();
  Write_IIC_Byte(0x00);			//write command
  IIC_Wait_Ack();
  Write_IIC_Byte(IIC_Command);
  IIC_Wait_Ack();
  IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(u8 IIC_Data)
{
  IIC_Start();
  Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
  IIC_Wait_Ack();
  Write_IIC_Byte(0x40);			//write data
  IIC_Wait_Ack();
  Write_IIC_Byte(IIC_Data);
  IIC_Wait_Ack();
  IIC_Stop();
}
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
  if(cmd)
    Write_IIC_Data(dat);
  else
    Write_IIC_Command(dat);
}

/********************************************
// fill_Picture
********************************************/
void OLED_Fill(u8 fill_Data)
{
  u8 m, n;
  for(m = 0; m < 8; m++)
  {
    OLED_WR_Byte(0xb0 + m, 0);		//page0-page1
    OLED_WR_Byte(0x00, 0);		//low column start address
    OLED_WR_Byte(0x10, 0);		//high column start address
    for(n = 0; n < 130; n++)
    {
      OLED_WR_Byte(fill_Data, 1);
    }
  }
}

//坐标设置

void OLED_Set_Pos(u8 x, u8 y)
{
  OLED_WR_Byte(0xb0 + y, OLED_CMD);
  OLED_WR_Byte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);
}
////开启OLED显示
//void OLED_Display_On(void)
//{
//  OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
//  OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
//  OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
//}
////关闭OLED显示
//void OLED_Display_Off(void)
//{
//  OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
//  OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
//  OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
//}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
  OLED_Fill(0x00);
}
//void OLED_On(void)
//{
//  u8 i, n;
//  for(i = 0; i < 8; i++)
//  {
//    OLED_WR_Byte (0xb0 + i, OLED_CMD); //设置页地址（0~7）
//    OLED_WR_Byte (0x00, OLED_CMD);     //设置显示位置—列低地址
//    OLED_WR_Byte (0x10, OLED_CMD);     //设置显示位置—列高地址
//    for(n = 0; n < 128; n++)OLED_WR_Byte(1, OLED_DATA);
//  } //更新显示
//}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
void OLED_ShowChar(u8 x, u8 y, u8 chr)
{
  u8 c = 0, i = 0;
  c = chr - ' '; //得到偏移后的值
  if(x > Max_Column - 1)
  {
    x = 0;
    y = y + 2;
  }
  OLED_Set_Pos(x, y);
  for(i = 0; i < 8; i++)
    OLED_WR_Byte(zf[c * 16 + i], OLED_DATA);
  OLED_Set_Pos(x, y + 1);
  for(i = 0; i < 8; i++)
    OLED_WR_Byte(zf[c * 16 + i + 8], OLED_DATA);

}
//m^n函数
u32 oled_pow(u8 m, u8 n)
{
  u32 result = 1;
  while(n--)result *= m;
  return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//num:数值(0~4294967295);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 mode)
{
  u8 t, temp;
  u8 enshow = 0;
  for(t = 0; t < len; t++)
  {
    temp = (num / oled_pow(10, len - t - 1)) % 10;
    if(enshow == 0 && t < (len - 1))
    {
      if(temp == 0)
      {
        if(mode == 0)
          OLED_ShowChar(x + 8 * t, y, ' ');
        else
          OLED_ShowChar(x + 8 * t, y, '0');
        continue;
      }
      else enshow = 1;
    }
    OLED_ShowChar(x + 8 * t, y, temp + '0');
  }
}

//显示中英文字符
void OLED_ShowCH(u8 x, u8 y, u8 *chs)
{
  u32 i = 0;
  u32 j;
	char* m;
  while (*chs != '\0')
  {
    if (*chs > 0xa0)				//汉字内码都是大于0xa0
    {
      for (i = 0 ; i < HZNUM; i++)
      {
        if(x > 112)
        {
          x = 0;
          y = y + 2;
        }
        if ((*chs == hz_index[i * 2]) && (*(chs + 1) == hz_index[i * 2 + 1]))
        {
          OLED_Set_Pos(x, y);
          for(j = 0; j < 16; j++)
            OLED_WR_Byte(hz[i * 32 + j], OLED_DATA);
          OLED_Set_Pos(x, y + 1);
          for(j = 0; j < 16; j++)
            OLED_WR_Byte(hz[i * 32 + j + 16], OLED_DATA);
          x += 16;
          break;
        }
      }
      chs += 2;
    }
    else
    {
      if(x > 120)
      {
        x = 0;
        y = y + 2;
      }
      m=strchr(zf_index,*chs);
			if (m!=NULL)
			{
				OLED_Set_Pos(x, y);
				for(j = 0; j < 8; j++)
					OLED_WR_Byte(zf[((u8)*m-32) * 16 + j], OLED_DATA);
				OLED_Set_Pos(x, y + 1);
				for(j = 0; j < 8; j++)
					OLED_WR_Byte(zf[((u8)*m-32) * 16 + j + 8], OLED_DATA);
				x += 8;
			}
      chs++;
    }
  }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
//void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[])
//{
//  u32 j = 0;
//  u8 x, y;

//  if(y1 % 8 == 0) y = y1 / 8;
//  else y = y1 / 8 + 1;
//  for(y = y0; y < y1; y++)
//  {
//    OLED_Set_Pos(x0, y);
//    for(x = x0; x < x1; x++)
//    {
//      OLED_WR_Byte(BMP[j++], OLED_DATA);
//    }
//  }
//}

//初始化SSD1306
void OLED_Init(void)
{
	HZNUM=GB16_NUM();
  OLED_WR_Byte(0xAE, OLED_CMD); //--display off
  OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
  OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
  OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
  OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
  OLED_WR_Byte(0x81, OLED_CMD); // contract control
  OLED_WR_Byte(0xFF, OLED_CMD); //--128
  OLED_WR_Byte(0xA1, OLED_CMD); //set segment remap
  OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
  OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
  OLED_WR_Byte(0xC8, OLED_CMD); //Com scan direction
  OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
  OLED_WR_Byte(0x00, OLED_CMD); //

  OLED_WR_Byte(0xD5, OLED_CMD); //set osc division
  OLED_WR_Byte(0x80, OLED_CMD); //

  OLED_WR_Byte(0xD8, OLED_CMD); //set area color mode off
  OLED_WR_Byte(0x05, OLED_CMD); //

  OLED_WR_Byte(0xD9, OLED_CMD); //Set Pre-Charge Period
  OLED_WR_Byte(0xF1, OLED_CMD); //

  OLED_WR_Byte(0xDA, OLED_CMD); //set com pin configuartion
  OLED_WR_Byte(0x12, OLED_CMD); //

  OLED_WR_Byte(0xDB, OLED_CMD); //set Vcomh
  OLED_WR_Byte(0x30, OLED_CMD); //

  OLED_WR_Byte(0x8D, OLED_CMD); //set charge pump enable
  OLED_WR_Byte(0x14, OLED_CMD); //

  OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}

