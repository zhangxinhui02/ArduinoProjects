#ifndef __BKM12864_H
#define __BKM12864_H
#include "stdio.h"

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long

sfr P4   = 0xC0;
sfr P4SW = 0xBB; 
sbit LCD_SCLK=P2^4;  //�ӿڶ���:LCD_SCLK����LCD��SCLK
sbit LCD_SID=P2^5;   //�ӿڶ���:LCD_SID����LCD��SDA
sbit LCD_RS=P2^6;    //�ӿڶ���:LCD_RS����LCD��RS,Ҳ�С�CD��
sbit LCD_RST=P2^7; //�ӿڶ���:LCD_RST����LCD��RESET
sbit LCD_CS=P4^4;   //�ӿڶ���:LCD_CS����LCD��CS1

sbit ROM_IN=P2^0;    //�ֿ�IC�ӿڶ���:ROM_IN�����ֿ�IC��SI
sbit ROM_OUT=P2^1;   //�ֿ�IC�ӿڶ���:ROM_OUT�����ֿ�IC��SO
sbit ROM_SCK=P2^2;   //�ֿ�IC�ӿڶ���:ROM_SCK�����ֿ�IC��SCK
sbit ROM_CS=P2^3;    //�ֿ�IC�ӿڶ���ROM_CS�����ֿ�IC��CS#



//��ʱ
void delay(int ms)               
{
	uint a,b,c;
	for(a=ms;a>0;a--)
		for(b=10;b>0;b--)
			for(c=85;c>0;c--);
}
//����ʱ
void delay_us(int n_us)               
{
	int j,k;
	for(j=0;j<n_us;j++)
	for(k=0;k<1;k++);
}


//дָ�LCDģ��
void Transfer_Command(int data1)   
{
	char i;
	LCD_CS=0;
	LCD_RS=0;
	for(i=0;i<8;i++)
   {
		LCD_SCLK=0; 
		//delay_us(10);	 //��������ʱ
		if(data1&0x80) LCD_SID=1;
		else LCD_SID=0;
		LCD_SCLK=1;
		//delay_us(10);	 //��������ʱ
	 	data1=data1<<=1;
   }
	LCD_CS=1;
}

//д���ݵ�LCDģ��
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




//LCDģ���ʼ��
void Initial_Lcd()
{
	LCD_RST=0;        		//�͵�ƽ��λ
	delay(100);
	LCD_RST=1;		    	//��λ���
	delay(100);        
	Transfer_Command(0xe2);	 //��λ
	delay(5);
	Transfer_Command(0x2c);  //��ѹ����1
	delay(50);	
	Transfer_Command(0x2e);  //��ѹ����2
	delay(50);
	Transfer_Command(0x2f);  //��ѹ����3
	delay(5);
	Transfer_Command(0x23);  //�ֵ��Աȶȣ������÷�Χ0x20��0x27
	Transfer_Command(0x81);  //΢���Աȶ�
	Transfer_Command(0x28);  //΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f
	Transfer_Command(0xa2);  //1/9ƫѹ�ȣ�bias��
	Transfer_Command(0xc8);  //��ɨ��˳�򣺴��ϵ���
	Transfer_Command(0xa0);  //��ɨ��˳�򣺴�����
	Transfer_Command(0x40);  //��ʼ�У���һ�п�ʼ
	Transfer_Command(0xaf);  //����ʾ
}

void Lcd_Address(uint page,uint column)
{
	column=column-0x01;
	Transfer_Command(0xb0+page-1);   //����ҳ��ַ��ÿ8��Ϊһҳ��ȫ����64�У����ֳ�8ҳ
	Transfer_Command(0x10+(column>>4&0x0f));	//�����е�ַ�ĸ�4λ
	Transfer_Command(column&0x0f);	//�����е�ַ�ĵ�4λ	
}

//ȫ������
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

////��ʾ128x64����ͼ��
//void Display_128x64(uchar *dp)
//{
//	uint i,j;
//	
//	for(j=0;j<8;j++)
//	{
//		Lcd_Address(j+1,1);
//		for (i=0;i<128;i++)
//		{	
//			Transfer_Data(*dp);					//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
//			dp++;
//		}
//	}
//}


////��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��
//void Display_Graphic_16x16(uchar page,uchar column,uchar *dp)
//{
//	uint i,j;
//	for(j=0;j<2;j++)
//	{
//		Lcd_Address(page+j,column);
//		for (i=0;i<16;i++)
//		{	
//			Transfer_Data(*dp);					//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
//			dp++;
//		}
//	}
//}


////��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��
//void Display_Graphic_8x16(uchar page,uchar column,uchar *dp)
//{
//	uint i,j;
//	for(j=0;j<2;j++)
//	{
//		Lcd_Address(page+j,column);
//		for (i=0;i<8;i++)
//		{	
//			Transfer_Data(*dp);					//д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
//			dp++;
//		}
//	}
//}


////��ʾ5X8����ͼ��ASCII, ��5x8����������ַ�������ͼ��
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

//��ָ�����Ѷ�ֿ�IC
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

//�Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�
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


//��ָ����ַ��������д��Һ����ָ����page,column)������
void Get_And_Write_16x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	ROM_CS = 0;
	Send_Command_to_ROM(0x03);
	Send_Command_to_ROM((fontaddr&0xff0000)>>16);	 //��ַ�ĸ�8λ,��24λ
	Send_Command_to_ROM((fontaddr&0xff00)>>8);		 //��ַ����8λ,��24λ
	Send_Command_to_ROM(fontaddr&0xff);				 //��ַ�ĵ�8λ,��24λ
	for(j=0;j<2;j++)
	{
		Lcd_Address(page+j,column);
		for(i=0; i<16; i++ )
	    {
			disp_data=Get_Data_From_ROM();
			Transfer_Data(disp_data);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
		}
	}
	ROM_CS=1;
}

//��ָ����ַ��������д��Һ����ָ����page,column)������
void Get_And_Write_8x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	ROM_CS = 0;
	Send_Command_to_ROM(0x03);
	Send_Command_to_ROM((fontaddr&0xff0000)>>16);	 //��ַ�ĸ�8λ,��24λ
	Send_Command_to_ROM((fontaddr&0xff00)>>8);		 //��ַ����8λ,��24λ
	Send_Command_to_ROM(fontaddr&0xff);				 //��ַ�ĵ�8λ,��24λ
	for(j=0;j<2;j++)
	{
		Lcd_Address(page+j,column);
		for(i=0; i<8; i++ )
	    {
			disp_data=Get_Data_From_ROM();
			Transfer_Data(disp_data);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
		}
	}
	ROM_CS=1;
}

//��ָ����ַ��������д��Һ����ָ����page,column)������
//void Get_And_Write_5x8(ulong fontaddr,uchar page,uchar column) 
//{
//	uchar i,disp_data;
//	ROM_CS = 0;
//	Send_Command_to_ROM(0x03);
//	Send_Command_to_ROM((fontaddr&0xff0000)>>16);	 //��ַ�ĸ�8λ,��24λ
//	Send_Command_to_ROM((fontaddr&0xff00)>>8);		 //��ַ����8λ,��24λ
//	Send_Command_to_ROM(fontaddr&0xff);				 //��ַ�ĵ�8λ,��24λ
//	Lcd_Address(page,column);
//	for(i=0; i<5; i++ )
//	{
//		disp_data=Get_Data_From_ROM();
//		Transfer_Data(disp_data);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
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
			//������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);

			Get_And_Write_16x16(fontaddr,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������
			i+=2;
			column+=16;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			//������壨GB2312��15x16����ַ��ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
	
			Get_And_Write_16x16(fontaddr,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������
			i+=2;
			column+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{									
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			

			Get_And_Write_8x16(fontaddr,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������
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


//			Get_And_Write_5x8(fontaddr,page,column);	 //��ָ����ַ��������д��Һ����ָ����page,column)������

//			i+=1;
//			column+=6;
//		}
//		else
//		i++;	
//	}
//}



#endif