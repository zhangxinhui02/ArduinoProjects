#ifndef __UART2_H
#define __UART2_H

#define uchar unsigned char 
#define uint unsigned int

sfr AUXR  = 0x8e;           //Auxiliary register
sfr S2CON = 0x9a;           //UART2 control register
sfr S2BUF = 0x9b;           //UART2 data buffer
sfr BRT   = 0x9c;           //Baudrate generator
sfr IE2   = 0xaf;           //Interrupt control 2

#define S2RI 0x01        //串口2接收中断请求标志位
#define S2TI 0x02        //串口2发送中断请求标志位
#define UART2_MAX_RECV_LEN		50					//最大接收缓存字节数
#define UART2_MAX_SEND_LEN		50					//最大发送缓存字节数
//串口接收缓存区 	
uchar  xdata UART2_RX_BUF[UART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
uchar  xdata UART2_TX_BUF[UART2_MAX_SEND_LEN]; 			  //发送缓冲,最大USART3_MAX_SEND_LEN字节
uint   UART2_RX_STA=0;  

//串口2初始化
void UART2_Init()
{
	AUXR |= 0x08;		//使能波特率倍速位S2SMOD
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//独立波特率发生器时钟为Fosc/12,即12T
	BRT = 0xFF;		//设定独立波特率发生器重装值
	AUXR |= 0x10;		//启动独立波特率发生器
	IE2 =0x01;        //开串口2中断  ES2=1
	EA = 1;
}

void Timer1_Init()
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0xDC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
	ET1 = 1;
}

//串口2发送一个字节
void UART2_SendData(uchar c)
{
    S2BUF = c;
    while(!(S2CON&S2TI));  //若S2TI=0，在此等待
    S2CON&=~S2TI;          //S2TI=0
}

//串口2发送字符串
//void UART2_SendString(char *s)
//{
//    while (*s)              //Check the end of the string
//    {
//        UART2_SendData(*s++);     //Send current char and increment string ptr
//    }
//}

/************串行口2中断处理函数*************/
void UART2_Interrupt(void) interrupt 8
{
	uchar Res;
	if(S2CON&S2RI)
	{
		S2CON&=~S2RI;
		Res=S2BUF;
		if((UART2_RX_STA&(1<<15))==0)//接收未完成
    {
      if(UART2_RX_STA<50)	//还可以接收数据
      {
        TL1 = 0x00;		//设置定时初值
        TH1 = 0xDC;		//设置定时初值
        if(UART2_RX_STA==0)
        {
          TR1 = 1;//使能定时器
        }
        UART2_RX_BUF[UART2_RX_STA++]=Res;	//记录接收到的值
      }
      else
      {
        UART2_RX_STA|=1<<15;				//强制标记接收完成
      }
    }
	} 
}

void Timer1_Interrupt() interrupt 3
{
	UART2_RX_STA|=1<<15;	//标记接收完成
	TL1 = 0x00;		//设置定时初值
	TH1 = 0xDC;		//设置定时初值
	TR1 = 0;		//定时器1开始计时
}




#endif