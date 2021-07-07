/***************************************************************************
标题：	串口调试程序，查询法接收数据
效果：	通过MAX485模块发送和接收数量
工作于：LY5A-L2A开发板
作者：	林洋电子-单片机
客服QQ：52927029
邮箱：	52927029@qq.com
******************************************************************************/
//头文件:
#include "reg51.h"

unsigned char RDAT = 0;
unsigned char TDAT = 0;

sbit Key1 = P3^7;
sbit  RE_DE = P3^2;	//本例程采用方式二连接，即RE和DE相连接用一个IO口控制

void Delay(unsigned int k)
{
    while(k--);       
}

void Init(void)
{
    TMOD=0x20;   	//定时器1，工作方式2              
    TH1=0xfd;		//波特率为9600
    TL1=0xfd;
    PCON=0x00;              
    TR1=1;     		//启动波特率发生
    SCON=0x50;		//工作方式1,8位UART，波特率可变，允许接收             
	RI = 0;
	TI = 0;   
	RE_DE=0;		//关闭485模块发射使能，打开接收功能
} 

void Send(unsigned char ldata)
{
	RE_DE=1;		//打开485模块发射使能，关闭接收功能
	SBUF=ldata;		//写发送寄存器
	while(!TI);		//等待发送完成
	TI=0; 			//对标志位清零
	RE_DE=0;		//关闭485模块发射使能，打开接收功能
}
void KeyCount(void)
{
	if(Key1 == 0)
	{
		Delay(120);
		if(Key1 == 0)
		{
		  	TDAT++;
			Send(TDAT);
		}
		while(Key1==0);
	 }
}
//主函数，C语言的入口函数:	
void main(void)  
{
	Init();

	while(1)
	{
	  	if(RI){				//查询是否有数据接收
			RDAT=SBUF;		//数据存于变量中
			RI=0;			//重新清0等待接收
			//P2=RDAT;
		}
		KeyCount();		//检测按键来发送数据
	}	
}
