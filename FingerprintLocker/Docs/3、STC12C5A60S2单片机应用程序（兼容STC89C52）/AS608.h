#ifndef __AS608_H
#define __AS608_H
#include <stdio.h>
#include <string.h>
#include "UART2.h"
//#include "oled.h"
#include "KEY.h"

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long


#define CharBuffer1 0x01
#define CharBuffer2 0x02
uint CuoWu,jiaoyanwei;
ulong AS608Addr = 0XFFFFFFFF; //Ĭ��ģ���ַ

char str2[6]={0};

typedef struct  
{
	uint pageID;//ָ��ID
	uint mathscore;//ƥ��÷�
}SearchResult;

typedef struct
{
	uint PS_max;//ָ���������
	uchar  PS_level;//��ȫ�ȼ�
	ulong PS_addr;
	uchar  PS_size;//ͨѶ���ݰ���С
	uchar  PS_N;//�����ʻ���N
}SysPara;

const char *EnsureMessage(uchar ensure);

//���Ͱ�ͷ
static void SendHead(void)
{
	UART2_SendData(0xEF);
	UART2_SendData(0x01);
}
//���͵�ַ
static void SendAddr(void)
{
	UART2_SendData(AS608Addr>>24);
	UART2_SendData(AS608Addr>>16);
	UART2_SendData(AS608Addr>>8);
	UART2_SendData(AS608Addr);
}
//���Ͱ���ʶ,
static void SendFlag(uchar flag)
{
	UART2_SendData(flag);
}
//���Ͱ�����
static void SendLength(int length)
{
	UART2_SendData(length>>8);
	UART2_SendData(length);
}
//����ָ����
static void Sendcmd(uchar cmd)
{
	UART2_SendData(cmd);
}
//����У���
static void SendCheck(uint check)
{
	UART2_SendData(check>>8);
	UART2_SendData(check);
}

//�ж��жϽ��յ�������û��Ӧ���
//waittimeΪ�ȴ��жϽ������ݵ�ʱ�䣨��λ1ms��
//����ֵ�����ݰ��׵�ַ
static uchar *JudgeStr(uint waittime)
{
//	uint temp;
	char *rdata;
	uchar str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0';   
	UART2_RX_STA=0;
	while(--waittime)
	{
		Delay_Ms(1);
		if(UART2_RX_STA&0X8000)//���յ�һ������
		{
			UART2_RX_STA=0;
			rdata=strstr((const char*)UART2_RX_BUF,(const char*)str);
			if(rdata)
				return (uchar*)rdata;	
		}		
	}
	return 0;
}

//¼��ͼ�� PS_GetImage
//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer�� 
//ģ�鷵��ȷ����
uchar PS_GetImage(void)
{
  uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x01);
  temp =  0x01+0x03+0x01;
	SendCheck(temp);
	rdata=JudgeStr(2000);
	if(rdata)
		ensure=rdata[9];
	else
		ensure=0xff;
	return ensure;
}

//�������� PS_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2			 
//����:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//ģ�鷵��ȷ����
uchar PS_GenChar(uchar BufferID)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(0x02);
	UART2_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	rdata=JudgeStr(2000);
	if(rdata)
		ensure=rdata[9];
	else
		ensure=0xff;
	return ensure;
}

//��ȷ�ȶ���öָ������ PS_Match
//����:��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ� 
//ģ�鷵��ȷ����
uchar PS_Match(void)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	rdata=JudgeStr(2000);
	if(rdata)
		ensure=rdata[9];
	else
		ensure=0xff;
	return ensure;
}

//����ָ�� PS_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣			
//����:  BufferID @ref CharBuffer1	CharBuffer2
//˵��:  ģ�鷵��ȷ���֣�ҳ�루����ָ��ģ�壩
//uchar PS_Search(uchar BufferID,uint StartPage,uint PageNum,SearchResult *p)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	memset(UART2_RX_BUF,'\0',sizeof(UART2_RX_BUF));
//	UART2_RX_STA=0;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//�������ʶ
//	SendLength(0x08);
//	Sendcmd(0x04);
//	UART2_SendData(BufferID);
//	UART2_SendData(StartPage>>8);
//	UART2_SendData(StartPage);
//	UART2_SendData(PageNum>>8);
//	UART2_SendData(PageNum);
//	temp = 0x01+0x08+0x04+BufferID
//	+(StartPage>>8)+(uchar)StartPage
//	+(PageNum>>8)+(uchar)PageNum;
//	SendCheck(temp);
//	Delay_Ms(100);
//	rdata=JudgeStr(2000);
//	if(rdata)
//	{
//		ensure = rdata[9];
//		p->pageID   =(rdata[10]<<8)+rdata[11];
//		p->mathscore=(rdata[12]<<8)+rdata[13];	
//	}
//	else
//		ensure = 0xff;
//	return ensure;	
//}

//�ϲ�����������ģ�壩PS_RegModel
//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2	
//˵��:  ģ�鷵��ȷ����
uchar PS_RegModel(void)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	rdata=JudgeStr(2000);
	if(rdata)
		ensure=rdata[9];
	else
		ensure=0xff;
	return ensure;		
}
//����ģ�� PS_StoreChar
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�			
//����:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID��ָ�ƿ�λ�úţ�
//˵��:  ģ�鷵��ȷ����
uchar PS_StoreChar(uchar BufferID,uint PageID)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x06);
	Sendcmd(0x06);
	UART2_SendData(BufferID);
	UART2_SendData(PageID>>8);
	UART2_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(uchar)PageID;
	SendCheck(temp);
	rdata=JudgeStr(2000);
	if(rdata)
		ensure=rdata[9];
	else
		ensure=0xff;
	return ensure;	
}
//ɾ��ģ�� PS_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//����:  PageID(ָ�ƿ�ģ���)��Nɾ����ģ�������
//˵��:  ģ�鷵��ȷ����
uchar PS_DeletChar(uint PageID,uint N)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(0x0C);
	UART2_SendData(PageID>>8);
	UART2_SendData(PageID);
	UART2_SendData(N>>8);
	UART2_SendData(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(uchar)PageID
	+(N>>8)+(uchar)N;
	SendCheck(temp);
	rdata=JudgeStr(2000);
	if(rdata)
		ensure=rdata[9];
	else
		ensure=0xff;
	return ensure;
}
//���ָ�ƿ� PS_Empty
//����:  ɾ��flash���ݿ�������ָ��ģ��
//����:  ��
//˵��:  ģ�鷵��ȷ����
uchar PS_Empty(void)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	rdata=JudgeStr(2000);
	if(rdata)
		ensure=rdata[9];
	else
		ensure=0xff;
	return ensure;
}

//дϵͳ�Ĵ��� PS_WriteReg
//����:  дģ��Ĵ���
//����:  �Ĵ������RegNum:4\5\6
//˵��:  ģ�鷵��ȷ����
//uchar PS_WriteReg(uchar RegNum,uchar DATA)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//�������ʶ
//	SendLength(0x05);
//	Sendcmd(0x0E);
//	UART2_SendData(RegNum);
//	UART2_SendData(DATA);
//	temp = RegNum+DATA+0x01+0x05+0x0E;
//	SendCheck(temp);
//	Delay_Ms(100);
//	rdata=JudgeStr(2000);
//	if(rdata)
//		ensure=rdata[9];
//	else
//		ensure=0xff;
//	if(ensure==0)
//		printf("\r\n���ò����ɹ���");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}
//��ϵͳ�������� PS_ReadSysPara
//����:  ��ȡģ��Ļ��������������ʣ�����С��)
//����:  ��
//˵��:  ģ�鷵��ȷ���� + ����������16bytes��
//uchar PS_ReadSysPara(SysPara *p)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//�������ʶ
//	SendLength(0x03);
//	Sendcmd(0x0F);
//	temp = 0x01+0x03+0x0F;
//	SendCheck(temp);
//	Delay_Ms(100);
//	rdata=JudgeStr(1000);
//	if(rdata)
//	{
//		ensure = rdata[9];
//		p->PS_max = (rdata[14]<<8)+rdata[15];
//		p->PS_level = rdata[17];
//		p->PS_addr=(rdata[18]<<24)+(rdata[19]<<16)+(rdata[20]<<8)+rdata[21];
//		p->PS_size = rdata[23];
//		p->PS_N = rdata[25];
//	}		
//	else
//		ensure=0xff;
//	if(ensure==0x00)
//	{
//		printf("\r\nģ�����ָ������=%d",p->PS_max);
//		printf("\r\n�Աȵȼ�=%d",p->PS_level);
//		printf("\r\n��ַ=%x",p->PS_addr);
//		printf("\r\n������=%d",p->PS_N*9600);
//	}
//	else 
//			printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}
//����ģ���ַ PS_SetAddr
//����:  ����ģ���ַ
//����:  PS_addr
//˵��:  ģ�鷵��ȷ����
//uchar PS_SetAddr(ulong PS_addr)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//�������ʶ
//	SendLength(0x07);
//	Sendcmd(0x15);
//	UART2_SendData(PS_addr>>24);
//	UART2_SendData(PS_addr>>16);
//	UART2_SendData(PS_addr>>8);
//	UART2_SendData(PS_addr);
//	temp = 0x01+0x07+0x15
//	+(uchar)(PS_addr>>24)+(uchar)(PS_addr>>16)
//	+(uchar)(PS_addr>>8) +(uchar)PS_addr;				
//	SendCheck(temp);
//	AS608Addr=PS_addr;//������ָ�������ַ
//	Delay_Ms(100);
//  rdata=JudgeStr(2000);
//	if(rdata)
//		ensure=rdata[9];
//	else
//		ensure=0xff;	
//		AS608Addr = PS_addr;
//	if(ensure==0x00)
//		printf("\r\n���õ�ַ�ɹ���");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}

//��������PS_HighSpeedSearch
//���ܣ��� CharBuffer1��CharBuffer2�е������ļ��������������򲿷�ָ�ƿ⡣
//		  �����������򷵻�ҳ��,��ָ����ڵ�ȷ������ָ�ƿ��� ���ҵ�¼ʱ����
//		  �ܺõ�ָ�ƣ���ܿ�������������
//����:  BufferID�� StartPage(��ʼҳ)��PageNum��ҳ����
//˵��:  ģ�鷵��ȷ����+ҳ�루����ָ��ģ�壩
uchar PS_HighSpeedSearch(uchar BufferID,uint StartPage,uint PageNum,SearchResult *p)
{
	uint temp;
  uint  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(0x1b);
	UART2_SendData(BufferID);
	UART2_SendData(StartPage>>8);
	UART2_SendData(StartPage);
	UART2_SendData(PageNum>>8);
	UART2_SendData(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
	+(StartPage>>8)+(uchar)StartPage
	+(PageNum>>8)+(uchar)PageNum;
	SendCheck(temp);
	rdata=JudgeStr(2000);
 	if(rdata)
	{
		ensure=rdata[9];
		p->pageID 	=   (uint)(rdata[10]<<8) +rdata[11];
		p->mathscore=		(uint)(rdata[12]<<8) +rdata[13];
	}
	else
		ensure=0xff;
	return ensure;
}
//����Чģ����� PS_ValidTempleteNum
//���ܣ�����Чģ�����
//����: ��
//˵��: ģ�鷵��ȷ����+��Чģ�����ValidN
//uchar PS_ValidTempleteNum(uint *ValidN)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//�������ʶ
//	SendLength(0x03);
//	Sendcmd(0x1d);
//	temp = 0x01+0x03+0x1d;
//	SendCheck(temp);
//  rdata=JudgeStr(2000);
//	if(rdata)
//	{
//		ensure=rdata[9];
//		*ValidN = (rdata[10]<<8) +rdata[11];
//	}		
//	else
//		ensure=0xff;
//	
//	if(ensure==0x00)
//	{
//		printf("\r\n��Чָ�Ƹ���=%d",(rdata[10]<<8)+rdata[11]);
//	}
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}
//��AS608���� PS_HandShake
//����: PS_Addr��ַָ��
//˵��: ģ�鷵�µ�ַ����ȷ��ַ��	
//uchar PS_HandShake(ulong *PS_Addr)
//{
//	SendHead();
//	SendAddr();
//	UART2_SendData(0X01);
//	UART2_SendData(0X00);
//	UART2_SendData(0X00);	
//	Delay_Ms(200);	
//	if(	UART2_RX_BUF[0]==0XEF && UART2_RX_BUF[1]==0X01 && UART2_RX_BUF[6]==0X07)//�ж��ǲ���ģ�鷵�ص�Ӧ���	
//		{
//			*PS_Addr=(UART2_RX_BUF[2]<<24) + (UART2_RX_BUF[3]<<16)
//							+(UART2_RX_BUF[4]<<8) + (UART2_RX_BUF[5]);
//			UART2_RX_STA=0;
//			return 0;
//		}
//	UART2_RX_STA=0;					
//	return 1;		
//}
//ģ��Ӧ���ȷ������Ϣ����
//���ܣ�����ȷ���������Ϣ������Ϣ
//����: ensure
const char *EnsureMessage(uchar ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="       OK       ";break;		
		case  0x01:
			p=" ���ݰ����մ��� ";break;
		case  0x02:
			p="��������û����ָ";break;
		case  0x03:
			p="¼��ָ��ͼ��ʧ��";break;
		case  0x04:
			p=" ָ��̫�ɻ�̫�� ";break;
		case  0x05:
			p=" ָ��̫ʪ��̫�� ";break;
		case  0x06:
			p="  ָ��ͼ��̫��  ";break;
		case  0x07:
			p=" ָ��������̫�� ";break;
		case  0x08:
			p="   ָ�Ʋ�ƥ��   ";break;
		case  0x09:
			p=" û��������ָ�� ";break;
		case  0x0a:
			p="   �����ϲ�ʧ�� ";break;
		case  0x0b:
			p="��ַ��ų�����Χ";
		case  0x10:
			p="  ɾ��ģ��ʧ��  ";break;
		case  0x11:
			p=" ���ָ�ƿ�ʧ�� ";break;	
		case  0x15:
			p="������������Чͼ";break;
		case  0x18:
			p=" ��дFLASH����  ";break;
		case  0x19:
			p="   δ�������   ";break;
		case  0x1a:
			p="  ��Ч�Ĵ�����  ";break;
		case  0x1b:
			p=" �Ĵ������ݴ��� ";break;
		case  0x1c:
			p=" ���±�ҳ����� ";break;
		case  0x1f:
			p="    ָ�ƿ���    ";break;
		case  0x20:
			p="    ��ַ����    ";break;
		default :
			p=" ����ȷ�������� ";break;
	}
 return p;	
}

//��ʾȷ���������Ϣ
void ShowErrMessage(uchar ensure)
{
	EnsureMessage(ensure);
}

//¼ָ��
void Add_FR(void)
{
	uchar i,ensure ,processnum=0;
	uint ID_NUM=0;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				OLED_ShowCH(0,2,"    �밴��ָ    ");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						Delay_Ms(1);
						OLED_ShowCH(0,2,"    ָ������    ");
						OLED_ShowCH(0,4,"                ");
						Delay_Ms(100);
						i=0;
						processnum=1;//�����ڶ���						
					}else {};				
				}else {};						
				break;
			
			case 1:
				i++;
				OLED_ShowCH(0,2,"   ���ٰ�һ��   ");
				OLED_ShowCH(0,4,"                ");
				Delay_Ms(100);
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//��������
					if(ensure==0x00)
					{
						Delay_Ms(1);
						OLED_ShowCH(0,2,"    ָ������    ");
						OLED_ShowCH(0,4,"                ");
						Delay_Ms(100);
						i=0;
						processnum=2;//����������
					}else {};	
				}else {};		
				break;

			case 2:
				OLED_ShowCH(0,2,"  �Ա�����ָ��  ");
				OLED_ShowCH(0,4,"                ");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					OLED_ShowCH(0,2,"    �Աȳɹ�    ");
					OLED_ShowCH(0,4,"                ");
					processnum=3;//�������Ĳ�
				}
				else
				{					
					OLED_ShowCH(0,2,"    �Ա�ʧ��    ");
					OLED_ShowCH(0,4,"                ");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				Delay_Ms(500);
				break;

			case 3:
				OLED_ShowCH(0,2,"  ����ָ��ģ��  ");
				OLED_ShowCH(0,4,"                ");
				Delay_Ms(500);
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					OLED_ShowCH(0,2,"����ָ��ģ��ɹ�");
					OLED_ShowCH(0,4,"                ");
					processnum=4;//�������岽
				}else {processnum=0;}
				Delay_Ms(1000);
				break;
				
			case 4:	
				OLED_ShowCH(0,0," ��K4��,��K2�� ");
				OLED_ShowCH(0,2,"    ��K3����    ");
				OLED_ShowCH(0,4,"  0=< ID <=99   ");
			  ID_NUM=0;
				while(key_num!=3)
				{
					key_num=KEY_Scan(0);
					if(key_num==2)
					{
						key_num=0;
						if(ID_NUM>0)
							ID_NUM--;
					}
					if(key_num==4)
					{
						key_num=0;
						if(ID_NUM<99)
							ID_NUM++;
					}
					OLED_ShowCH(40,6,"ID=");
					OLED_ShowNum(65,6,ID_NUM,2,1);
				}
				key_num=0;
				ensure=PS_StoreChar(CharBuffer2,ID_NUM);//����ģ��
				if(ensure==0x00) 
				{		
					OLED_Clear();
					OLED_ShowCH(0,2,"  ¼��ָ�Ƴɹ�  ");
					OLED_ShowCH(0,4,"                ");
					Delay_Ms(1500);
					OLED_Clear();
					OLED_ShowCH(0,0,"ָ��ģ����Գ���");
					OLED_ShowCH(16,2,"K1�����ָ��");
					OLED_ShowCH(16,4,"K3��ɾ��ָ��");
					OLED_ShowCH(16,6,"K5����ָ֤��");
					return ;
				}else {OLED_Clear();processnum=0;}					
				break;				
		}
		Delay_Ms(400);
		if(i==10)//����5��û�а���ָ���˳�
		{
			OLED_Clear();
			break;	
		}
	}		
}

SysPara AS608Para;//ָ��ģ��AS608����
//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	uchar ensure;
	char str[20];
	while(key_num!=1)
	{
		key_num=KEY_Scan(0);
		ensure=PS_GetImage();
		if(ensure==0x00)//��ȡͼ��ɹ� 
		{	
			ensure=PS_GenChar(CharBuffer1);
			if(ensure==0x00) //���������ɹ�
			{			
				ensure=PS_HighSpeedSearch(CharBuffer1,0,99,&seach);
				if(ensure==0x00)//�����ɹ�
				{				
					OLED_ShowCH(0,2,"  ָ����֤�ɹ�  ");				
					sprintf(str," ID:%d �÷�:%d ",seach.pageID,seach.mathscore);
					OLED_ShowCH(0,4,(u8*)str);
					Delay_Ms(3000);
				}
				if(ensure==0x09) 
				{
					OLED_ShowCH(32,2,"��֤ʧ��");
					Delay_Ms(1500);
				}		
			}
			else
				{};
			OLED_Clear();
			OLED_ShowCH(32,2,"�밴��ָ");
		}
	}	
	OLED_Clear();
	OLED_ShowCH(0,0,"ָ��ģ����Գ���");
	OLED_ShowCH(16,2,"K1�����ָ��");
	OLED_ShowCH(16,4,"K3��ɾ��ָ��");
	OLED_ShowCH(16,6,"K5����ָ֤��");
}

//ɾ��ָ��
void Del_FR(void)
{
	uchar  ensure;
	uint ID_NUM=0;
	OLED_ShowCH(0,0,"K4�� K2�� K3ȷ��");
	OLED_ShowCH(0,2,"  K5���ָ�ƿ�  ");
	OLED_ShowCH(0,4,"K1���� 0=<ID<=99");
	while(key_num!=3)
	{
		key_num=KEY_Scan(0);
		if(key_num==2)
		{
			key_num=0;
			if(ID_NUM>0)
				ID_NUM--;
		}
		if(key_num==4)
		{
			key_num=0;
			if(ID_NUM<99)
				ID_NUM++;
		}
		if(key_num==1)
			goto MENU ; //������ҳ��
		if(key_num==5)
		{
			key_num=0;
			ensure=PS_Empty();//���ָ�ƿ�
			if(ensure==0)
			{
				OLED_Clear();
				OLED_ShowCH(0,2," ���ָ�ƿ�ɹ� ");		
			}
			else
				{};
			Delay_Ms(1500);
			goto MENU ; //������ҳ��
		}		
		OLED_ShowCH(40,6,"ID=");
		OLED_ShowNum(65,6,ID_NUM,2,1);
	}
	ensure=PS_DeletChar(ID_NUM,1);//ɾ������ָ��
	if(ensure==0)
	{
		OLED_Clear();
		OLED_ShowCH(0,2,"  ɾ��ָ�Ƴɹ�  ");		
	}
  else
		{};	
	Delay_Ms(1500);
MENU:	
	OLED_Clear();
	OLED_ShowCH(0,0,"ָ��ģ����Գ���");
	OLED_ShowCH(16,2,"K1�����ָ��");
	OLED_ShowCH(16,4,"K3��ɾ��ָ��");
	OLED_ShowCH(16,6,"K5����ָ֤��");
	key_num=0;
}

#endif
