#ifndef __BKM12864_H
#define __BKM12864_H
#include "stdio.h"

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long

sfr P4   = 0xC0;
sfr P4SW = 0xBB; 
sbit LCD_SCLK=P2^4;  //接口定义:LCD_SCLK就是LCD的SCLK
sbit LCD_SID=P2^5;   //接口定义:LCD_SID就是LCD的SDA
sbit LCD_RS=P2^6;    //接口定义:LCD_RS就是LCD的RS,也叫“CD”
sbit LCD_RST=P2^7; //接口定义:LCD_RST就是LCD的RESET
sbit LCD_CS=P4^4;   //接口定义:LCD_CS就是LCD的CS1

sbit ROM_IN=P2^0;    //字库IC接口定义:ROM_IN就是字库IC的SI
sbit ROM_OUT=P2^1;   //字库IC接口定义:ROM_OUT就是字库IC的SO
sbit ROM_SCK=P2^2;   //字库IC接口定义:ROM_SCK就是字库IC的SCK
sbit ROM_CS=P2^3;    //字库IC接口定义ROM_CS就是字库IC的CS#



//延时
void delay(int ms)               
{
	uint a,b,c;
	for(a=ms;a>0;a--)
		for(b=10;b>0;b--)
			for(c=85;c>0;c--);
}
//短延时
void delay_us(int n_us)               
{
	int j,k;
	for(j=0;j<n_us;j++)
	for(k=0;k<1;k++);
}


//写指令到LCD模块
void Transfer_Command(int data1)   
{
	char i;
	LCD_CS=0;
	LCD_RS=0;
	for(i=0;i<8;i++)
   {
		LCD_SCLK=0; 
		//delay_us(10);	 //加少量延时
		if(data1&0x80) LCD_SID=1;
		else LCD_SID=0;
		LCD_SCLK=1;
		//delay_us(10);	 //加少量延时
	 	data1=data1<<=1;
   }
	LCD_CS=1;
}

//写数据到LCD模块
void Transfer_Data(int data1)
{
	char i;
	LCD_CS=0;
	LCD_RS=1;
	for(i=0;i<8;i++)
	{
		LCD_SCLK=0;
		if(data1&0x80) LCD_SID=1;
		else LCD_SID=0;
		LCD_SCLK=1;
		data1=data1<<=1;
	}
	LCD_CS=1;
}




//LCD模块初始化
void Initial_Lcd()
{
	LCD_RST=0;        		//低电平复位
	delay(100);
	LCD_RST=1;		    	//复位完毕
	delay(100);        
	Transfer_Command(0xe2);	 //软复位
	delay(5);
	Transfer_Command(0x2c);  //升压步聚1
	delay(50);	
	Transfer_Command(0x2e);  //升压步聚2
	delay(50);
	Transfer_Command(0x2f);  //升压步聚3
	delay(5);
	Transfer_Command(0x23);  //粗调对比度，可设置范围0x20～0x27
	Transfer_Command(0x81);  //微调对比度
	Transfer_Command(0x28);  //微调对比度的值，可设置范围0x00～0x3f
	Transfer_Command(0xa2);  //1/9偏压比（bias）
	Transfer_Command(0xc8);  //行扫描顺序：从上到下
	Transfer_Command(0xa0);  //列扫描顺序：从左到右
	Transfer_Command(0x40);  //起始行：第一行开始
	Transfer_Command(0xaf);  //开显示
}

void Lcd_Address(uint page,uint column)
{
	column=column-0x01;
	Transfer_Command(0xb0+page-1);   //设置页地址，每8行为一页，全屏共64行，被分成8页
	Transfer_Command(0x10+(column>>4&0x0f));	//设置列地址的高4位
	Transfer_Command(column&0x0f);	//设置列地址的低4位	
}

//全屏清屏
void Clear_Screen()
{
	unsigned char i,j;
	
		
	for(i=0;i<9;i++)
	{
		Transfer_Command(0xb0+i);
		Transfer_Command(0x10);
		Transfer_Command(0x00);
		for(j=0;j<132;j++)
		{
		  	Transfer_Data(0x00);
		}
	}
}

////显示128x64点阵图像
//void Display_128x64(uchar *dp)
//{
//	uint i,j;
//	
//	for(j=0;j<8;j++)
//	{
//		Lcd_Address(j+1,1);
//		for (i=0;i<128;i++)
//		{	
//			Transfer_Data(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
//			dp++;
//		}
//	}
//}


////显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
//void Display_Graphic_16x16(uchar page,uchar column,uchar *dp)
//{
//	uint i,j;
//	for(j=0;j<2;j++)
//	{
//		Lcd_Address(page+j,column);
//		for (i=0;i<16;i++)
//		{	
//			Transfer_Data(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
//			dp++;
//		}
//	}
//}


////显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
//void Display_Graphic_8x16(uchar page,uchar column,uchar *dp)
//{
//	uint i,j;
//	for(j=0;j<2;j++)
//	{
//		Lcd_Address(page+j,column);
//		for (i=0;i<8;i++)
//		{	
//			Transfer_Data(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
//			dp++;
//		}
//	}
//}


////显示5X8点阵图像、ASCII, 或5x8点阵的自造字符、其他图标
//void Display_Graphic_5x8(uchar page,uchar column,uchar *dp)
//{
//	uint i;
//	Lcd_Address(page,column);
//	for (i=0;i<6;i++)
//	{	
//		Transfer_Data(*dp);
//		dp++;
//	}
//	
//}

//送指令到晶联讯字库IC
void Send_Command_to_ROM( uchar datu )
{
	uchar i;
	for(i=0;i<8;i++ )
	{
		ROM_SCK=0;
		delay_us(10);
		if(datu&0x80)ROM_IN = 1;
		else ROM_IN = 0;
		datu = datu<<1;
		ROM_SCK=1;
		delay_us(10);
	}
}

//从晶联讯字库IC中取汉字或字符数据（1个字节）
static uchar Get_Data_From_ROM( )
{
	uchar i;
	uchar ret_data=0;
	for(i=0;i<8;i++)
	{
		ROM_OUT=1;
		ROM_SCK=0;
		//delay_us(1);
		ret_data=ret_data<<1;
		if( ROM_OUT )
			ret_data=ret_data+1;
		else
			ret_data=ret_data+0;
		ROM_SCK=1;
		//delay_us(1);
	}
	return(ret_data);
}


//从指定地址读出数据写到液晶屏指定（page,column)座标中
void Get_And_Write_16x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	ROM_CS = 0;
	Send_Command_to_ROM(0x03);
	Send_Command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	Send_Command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	Send_Command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
	for(j=0;j<2;j++)
	{
		Lcd_Address(page+j,column);
		for(i=0; i<16; i++ )
	    {
			disp_data=Get_Data_From_ROM();
			Transfer_Data(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	ROM_CS=1;
}

//从指定地址读出数据写到液晶屏指定（page,column)座标中
void Get_And_Write_8x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	ROM_CS = 0;
	Send_Command_to_ROM(0x03);
	Send_Command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	Send_Command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	Send_Command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
	for(j=0;j<2;j++)
	{
		Lcd_Address(page+j,column);
		for(i=0; i<8; i++ )
	    {
			disp_data=Get_Data_From_ROM();
			Transfer_Data(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	ROM_CS=1;
}

//从指定地址读出数据写到液晶屏指定（page,column)座标中
//void Get_And_Write_5x8(ulong fontaddr,uchar page,uchar column) 
//{
//	uchar i,disp_data;
//	ROM_CS = 0;
//	Send_Command_to_ROM(0x03);
//	Send_Command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
//	Send_Command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
//	Send_Command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位
//	Lcd_Address(page,column);
//	for(i=0; i<5; i++ )
//	{
//		disp_data=Get_Data_From_ROM();
//		Transfer_Data(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
//	}
//	ROM_CS=1;
//}


//****************************************************************


ulong  fontaddr=0;
void Display_GB2312_String(uchar page,uchar column,uchar *text)
{
	uchar i= 0;			
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);

			Get_And_Write_16x16(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
	
			Get_And_Write_16x16(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{									
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			

			Get_And_Write_8x16(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=1;
			column+=8;
		}
		else
			i++;	
	}
	
}

//void Display_String_5x8(uchar page,uchar column,uchar *text)
//{
//	unsigned char i= 0;
//	while((text[i]>0x00))
//	{
//		
//		if((text[i]>=0x20) &&(text[i]<=0x7e))	
//		{						
//			fontaddr = (text[i]- 0x20);
//			fontaddr = (unsigned long)(fontaddr*8);
//			fontaddr = (unsigned long)(fontaddr+0x3bfc0);			


//			Get_And_Write_5x8(fontaddr,page,column);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中

//			i+=1;
//			column+=6;
//		}
//		else
//		i++;	
//	}
//}



#endif