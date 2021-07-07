#ifndef __SERIAL__
#define __SERIAL__
#include "DataType.h"

sbit RS485_EN = P3^2;
/************************************************
�������������ڳ�ʼ������
************************************************/
void serial_init()
{
    TMOD = TMOD | 0x20;//���ö�ʱ��1Ϊ������ʽ2
	SCON = 0x50;
	TH1 = 0xf3;//��ʼ��������Ϊ 9600 ˫��Ƶ 24Mhz
	TL1 = 0xf3;
	PCON  = 0x00 ; // SMOD = 1 
	EA  = 1 ;
	ES  = 1 ;
	TR1 = 1 ;
	TI = 0;

	RS485_EN = 0;
}

/************************************************
�������������ڷ�������
************************************************/
void serial_send(uint8 *da, uint8 len)
{
    uint8 i=0;

	RS485_EN = 1;
    for ( i=0; i<len; i++)
	{
	    
		ES=0;
		SBUF = *da;	
		while(!TI);	
		TI=0;
		ES=1;

		da++;
	}

	RS485_EN = 0;
}

/************************************************
�������������ڷ��͵��ֽ�����
************************************************/
void serial_send_u8(uint8 da)
{
    RS485_EN = 1;
	ES=0;
	SBUF = da;	
	while(!TI);	
	TI=0;
	ES=1;
	RS485_EN = 0;
}

/************************************************
�������������ڽ����жϺ���
************************************************/
void ser() interrupt 4
{
	RI = 0;                        // ����жϱ�־
	if(SBUF == 0x49)
	{
	   
	}
}

#endif