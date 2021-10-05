#ifndef __UART2_H
#define __UART2_H

#define uchar unsigned char 
#define uint unsigned int

sfr AUXR  = 0x8e;           //Auxiliary register
sfr S2CON = 0x9a;           //UART2 control register
sfr S2BUF = 0x9b;           //UART2 data buffer
sfr BRT   = 0x9c;           //Baudrate generator
sfr IE2   = 0xaf;           //Interrupt control 2

#define S2RI 0x01        //����2�����ж������־λ
#define S2TI 0x02        //����2�����ж������־λ
#define UART2_MAX_RECV_LEN		50					//�����ջ����ֽ���
#define UART2_MAX_SEND_LEN		50					//����ͻ����ֽ���
//���ڽ��ջ����� 	
uchar  xdata UART2_RX_BUF[UART2_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
uchar  xdata UART2_TX_BUF[UART2_MAX_SEND_LEN]; 			  //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
uint   UART2_RX_STA=0;  

//����2��ʼ��
void UART2_Init()
{
	AUXR |= 0x08;		//ʹ�ܲ����ʱ���λS2SMOD
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xFB;		//���������ʷ�����ʱ��ΪFosc/12,��12T
	BRT = 0xFF;		//�趨���������ʷ�������װֵ
	AUXR |= 0x10;		//�������������ʷ�����
	IE2 =0x01;        //������2�ж�  ES2=1
	EA = 1;
}

void Timer1_Init()
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD |= 0x10;		//���ö�ʱ��ģʽ
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0xDC;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

//����2����һ���ֽ�
void UART2_SendData(uchar c)
{
    S2BUF = c;
    while(!(S2CON&S2TI));  //��S2TI=0���ڴ˵ȴ�
    S2CON&=~S2TI;          //S2TI=0
}

//����2�����ַ���
//void UART2_SendString(char *s)
//{
//    while (*s)              //Check the end of the string
//    {
//        UART2_SendData(*s++);     //Send current char and increment string ptr
//    }
//}

/************���п�2�жϴ�����*************/
void UART2_Interrupt(void) interrupt 8
{
	uchar Res;
	if(S2CON&S2RI)
	{
		S2CON&=~S2RI;
		Res=S2BUF;
		if((UART2_RX_STA&(1<<15))==0)//����δ���
    {
      if(UART2_RX_STA<50)	//�����Խ�������
      {
        TL1 = 0x00;		//���ö�ʱ��ֵ
        TH1 = 0xDC;		//���ö�ʱ��ֵ
        if(UART2_RX_STA==0)
        {
          TR1 = 1;//ʹ�ܶ�ʱ��
        }
        UART2_RX_BUF[UART2_RX_STA++]=Res;	//��¼���յ���ֵ
      }
      else
      {
        UART2_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
      }
    }
	} 
}

void Timer1_Interrupt() interrupt 3
{
	UART2_RX_STA|=1<<15;	//��ǽ������
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0xDC;		//���ö�ʱ��ֵ
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
}




#endif