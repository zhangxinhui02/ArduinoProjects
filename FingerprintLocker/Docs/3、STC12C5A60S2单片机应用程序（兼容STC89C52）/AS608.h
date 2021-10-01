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
ulong AS608Addr = 0XFFFFFFFF; //默认模块地址

char str2[6]={0};

typedef struct  
{
	uint pageID;//指纹ID
	uint mathscore;//匹配得分
}SearchResult;

typedef struct
{
	uint PS_max;//指纹最大容量
	uchar  PS_level;//安全等级
	ulong PS_addr;
	uchar  PS_size;//通讯数据包大小
	uchar  PS_N;//波特率基数N
}SysPara;

const char *EnsureMessage(uchar ensure);

//发送包头
static void SendHead(void)
{
	UART2_SendData(0xEF);
	UART2_SendData(0x01);
}
//发送地址
static void SendAddr(void)
{
	UART2_SendData(AS608Addr>>24);
	UART2_SendData(AS608Addr>>16);
	UART2_SendData(AS608Addr>>8);
	UART2_SendData(AS608Addr);
}
//发送包标识,
static void SendFlag(uchar flag)
{
	UART2_SendData(flag);
}
//发送包长度
static void SendLength(int length)
{
	UART2_SendData(length>>8);
	UART2_SendData(length);
}
//发送指令码
static void Sendcmd(uchar cmd)
{
	UART2_SendData(cmd);
}
//发送校验和
static void SendCheck(uint check)
{
	UART2_SendData(check>>8);
	UART2_SendData(check);
}

//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
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
		if(UART2_RX_STA&0X8000)//接收到一次数据
		{
			UART2_RX_STA=0;
			rdata=strstr((const char*)UART2_RX_BUF,(const char*)str);
			if(rdata)
				return (uchar*)rdata;	
		}		
	}
	return 0;
}

//录入图像 PS_GetImage
//功能:探测手指，探测到后录入指纹图像存于ImageBuffer。 
//模块返回确认字
uchar PS_GetImage(void)
{
  uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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

//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2			 
//参数:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//模块返回确认字
uchar PS_GenChar(uchar BufferID)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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

//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件 
//模块返回确认字
uchar PS_Match(void)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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

//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。			
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
//uchar PS_Search(uchar BufferID,uint StartPage,uint PageNum,SearchResult *p)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	memset(UART2_RX_BUF,'\0',sizeof(UART2_RX_BUF));
//	UART2_RX_STA=0;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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

//合并特征（生成模板）PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2	
//说明:  模块返回确认字
uchar PS_RegModel(void)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置。			
//参数:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID（指纹库位置号）
//说明:  模块返回确认字
uchar PS_StoreChar(uchar BufferID,uint PageID)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号)，N删除的模板个数。
//说明:  模块返回确认字
uchar PS_DeletChar(uint PageID,uint N)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
uchar PS_Empty(void)
{
	uint temp;
  uchar  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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

//写系统寄存器 PS_WriteReg
//功能:  写模块寄存器
//参数:  寄存器序号RegNum:4\5\6
//说明:  模块返回确认字
//uchar PS_WriteReg(uchar RegNum,uchar DATA)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//		printf("\r\n设置参数成功！");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}
//读系统基本参数 PS_ReadSysPara
//功能:  读取模块的基本参数（波特率，包大小等)
//参数:  无
//说明:  模块返回确认字 + 基本参数（16bytes）
//uchar PS_ReadSysPara(SysPara *p)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//		printf("\r\n模块最大指纹容量=%d",p->PS_max);
//		printf("\r\n对比等级=%d",p->PS_level);
//		printf("\r\n地址=%x",p->PS_addr);
//		printf("\r\n波特率=%d",p->PS_N*9600);
//	}
//	else 
//			printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}
//设置模块地址 PS_SetAddr
//功能:  设置模块地址
//参数:  PS_addr
//说明:  模块返回确认字
//uchar PS_SetAddr(ulong PS_addr)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//	AS608Addr=PS_addr;//发送完指令，更换地址
//	Delay_Ms(100);
//  rdata=JudgeStr(2000);
//	if(rdata)
//		ensure=rdata[9];
//	else
//		ensure=0xff;	
//		AS608Addr = PS_addr;
//	if(ensure==0x00)
//		printf("\r\n设置地址成功！");
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}

//高速搜索PS_HighSpeedSearch
//功能：以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
//		  若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量
//		  很好的指纹，会很快给出搜索结果。
//参数:  BufferID， StartPage(起始页)，PageNum（页数）
//说明:  模块返回确认字+页码（相配指纹模板）
uchar PS_HighSpeedSearch(uchar BufferID,uint StartPage,uint PageNum,SearchResult *p)
{
	uint temp;
  uint  ensure;
	uchar  *rdata;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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
//读有效模板个数 PS_ValidTempleteNum
//功能：读有效模板个数
//参数: 无
//说明: 模块返回确认字+有效模板个数ValidN
//uchar PS_ValidTempleteNum(uint *ValidN)
//{
//	uint temp;
//  uchar  ensure;
//	uchar  *rdata;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//		printf("\r\n有效指纹个数=%d",(rdata[10]<<8)+rdata[11]);
//	}
//	else
//		printf("\r\n%s",EnsureMessage(ensure));
//	return ensure;
//}
//与AS608握手 PS_HandShake
//参数: PS_Addr地址指针
//说明: 模块返新地址（正确地址）	
//uchar PS_HandShake(ulong *PS_Addr)
//{
//	SendHead();
//	SendAddr();
//	UART2_SendData(0X01);
//	UART2_SendData(0X00);
//	UART2_SendData(0X00);	
//	Delay_Ms(200);	
//	if(	UART2_RX_BUF[0]==0XEF && UART2_RX_BUF[1]==0X01 && UART2_RX_BUF[6]==0X07)//判断是不是模块返回的应答包	
//		{
//			*PS_Addr=(UART2_RX_BUF[2]<<24) + (UART2_RX_BUF[3]<<16)
//							+(UART2_RX_BUF[4]<<8) + (UART2_RX_BUF[5]);
//			UART2_RX_STA=0;
//			return 0;
//		}
//	UART2_RX_STA=0;					
//	return 1;		
//}
//模块应答包确认码信息解析
//功能：解析确认码错误信息返回信息
//参数: ensure
const char *EnsureMessage(uchar ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="       OK       ";break;		
		case  0x01:
			p=" 数据包接收错误 ";break;
		case  0x02:
			p="传感器上没有手指";break;
		case  0x03:
			p="录入指纹图像失败";break;
		case  0x04:
			p=" 指纹太干或太淡 ";break;
		case  0x05:
			p=" 指纹太湿或太糊 ";break;
		case  0x06:
			p="  指纹图像太乱  ";break;
		case  0x07:
			p=" 指纹特征点太少 ";break;
		case  0x08:
			p="   指纹不匹配   ";break;
		case  0x09:
			p=" 没有搜索到指纹 ";break;
		case  0x0a:
			p="   特征合并失败 ";break;
		case  0x0b:
			p="地址序号超出范围";
		case  0x10:
			p="  删除模板失败  ";break;
		case  0x11:
			p=" 清空指纹库失败 ";break;	
		case  0x15:
			p="缓冲区内无有效图";break;
		case  0x18:
			p=" 读写FLASH出错  ";break;
		case  0x19:
			p="   未定义错误   ";break;
		case  0x1a:
			p="  无效寄存器号  ";break;
		case  0x1b:
			p=" 寄存器内容错误 ";break;
		case  0x1c:
			p=" 记事本页码错误 ";break;
		case  0x1f:
			p="    指纹库满    ";break;
		case  0x20:
			p="    地址错误    ";break;
		default :
			p=" 返回确认码有误 ";break;
	}
 return p;	
}

//显示确认码错误信息
void ShowErrMessage(uchar ensure)
{
	EnsureMessage(ensure);
}

//录指纹
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
				OLED_ShowCH(0,2,"    请按手指    ");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						Delay_Ms(1);
						OLED_ShowCH(0,2,"    指纹正常    ");
						OLED_ShowCH(0,4,"                ");
						Delay_Ms(100);
						i=0;
						processnum=1;//跳到第二步						
					}else {};				
				}else {};						
				break;
			
			case 1:
				i++;
				OLED_ShowCH(0,2,"   请再按一次   ");
				OLED_ShowCH(0,4,"                ");
				Delay_Ms(100);
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征
					if(ensure==0x00)
					{
						Delay_Ms(1);
						OLED_ShowCH(0,2,"    指纹正常    ");
						OLED_ShowCH(0,4,"                ");
						Delay_Ms(100);
						i=0;
						processnum=2;//跳到第三步
					}else {};	
				}else {};		
				break;

			case 2:
				OLED_ShowCH(0,2,"  对比两次指纹  ");
				OLED_ShowCH(0,4,"                ");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					OLED_ShowCH(0,2,"    对比成功    ");
					OLED_ShowCH(0,4,"                ");
					processnum=3;//跳到第四步
				}
				else
				{					
					OLED_ShowCH(0,2,"    对比失败    ");
					OLED_ShowCH(0,4,"                ");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				Delay_Ms(500);
				break;

			case 3:
				OLED_ShowCH(0,2,"  生成指纹模板  ");
				OLED_ShowCH(0,4,"                ");
				Delay_Ms(500);
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					OLED_ShowCH(0,2,"生成指纹模板成功");
					OLED_ShowCH(0,4,"                ");
					processnum=4;//跳到第五步
				}else {processnum=0;}
				Delay_Ms(1000);
				break;
				
			case 4:	
				OLED_ShowCH(0,0," 按K4加,按K2减 ");
				OLED_ShowCH(0,2,"    按K3保存    ");
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
				ensure=PS_StoreChar(CharBuffer2,ID_NUM);//储存模板
				if(ensure==0x00) 
				{		
					OLED_Clear();
					OLED_ShowCH(0,2,"  录入指纹成功  ");
					OLED_ShowCH(0,4,"                ");
					Delay_Ms(1500);
					OLED_Clear();
					OLED_ShowCH(0,0,"指纹模块测试程序");
					OLED_ShowCH(16,2,"K1键添加指纹");
					OLED_ShowCH(16,4,"K3键删除指纹");
					OLED_ShowCH(16,6,"K5键验证指纹");
					return ;
				}else {OLED_Clear();processnum=0;}					
				break;				
		}
		Delay_Ms(400);
		if(i==10)//超过5次没有按手指则退出
		{
			OLED_Clear();
			break;	
		}
	}		
}

SysPara AS608Para;//指纹模块AS608参数
//刷指纹
void press_FR(void)
{
	SearchResult seach;
	uchar ensure;
	char str[20];
	while(key_num!=1)
	{
		key_num=KEY_Scan(0);
		ensure=PS_GetImage();
		if(ensure==0x00)//获取图像成功 
		{	
			ensure=PS_GenChar(CharBuffer1);
			if(ensure==0x00) //生成特征成功
			{			
				ensure=PS_HighSpeedSearch(CharBuffer1,0,99,&seach);
				if(ensure==0x00)//搜索成功
				{				
					OLED_ShowCH(0,2,"  指纹验证成功  ");				
					sprintf(str," ID:%d 得分:%d ",seach.pageID,seach.mathscore);
					OLED_ShowCH(0,4,(u8*)str);
					Delay_Ms(3000);
				}
				if(ensure==0x09) 
				{
					OLED_ShowCH(32,2,"验证失败");
					Delay_Ms(1500);
				}		
			}
			else
				{};
			OLED_Clear();
			OLED_ShowCH(32,2,"请按手指");
		}
	}	
	OLED_Clear();
	OLED_ShowCH(0,0,"指纹模块测试程序");
	OLED_ShowCH(16,2,"K1键添加指纹");
	OLED_ShowCH(16,4,"K3键删除指纹");
	OLED_ShowCH(16,6,"K5键验证指纹");
}

//删除指纹
void Del_FR(void)
{
	uchar  ensure;
	uint ID_NUM=0;
	OLED_ShowCH(0,0,"K4加 K2减 K3确认");
	OLED_ShowCH(0,2,"  K5清空指纹库  ");
	OLED_ShowCH(0,4,"K1返回 0=<ID<=99");
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
			goto MENU ; //返回主页面
		if(key_num==5)
		{
			key_num=0;
			ensure=PS_Empty();//清空指纹库
			if(ensure==0)
			{
				OLED_Clear();
				OLED_ShowCH(0,2," 清空指纹库成功 ");		
			}
			else
				{};
			Delay_Ms(1500);
			goto MENU ; //返回主页面
		}		
		OLED_ShowCH(40,6,"ID=");
		OLED_ShowNum(65,6,ID_NUM,2,1);
	}
	ensure=PS_DeletChar(ID_NUM,1);//删除单个指纹
	if(ensure==0)
	{
		OLED_Clear();
		OLED_ShowCH(0,2,"  删除指纹成功  ");		
	}
  else
		{};	
	Delay_Ms(1500);
MENU:	
	OLED_Clear();
	OLED_ShowCH(0,0,"指纹模块测试程序");
	OLED_ShowCH(16,2,"K1键添加指纹");
	OLED_ShowCH(16,4,"K3键删除指纹");
	OLED_ShowCH(16,6,"K5键验证指纹");
	key_num=0;
}

#endif
