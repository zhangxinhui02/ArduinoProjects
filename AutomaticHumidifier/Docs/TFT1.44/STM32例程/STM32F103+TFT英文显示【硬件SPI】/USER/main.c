/************************************************************************************
							�������ṩ�����µ��̣�
								Ilovemcu.taobao.com
							ʵ�������Χ��չģ����������ϵ���
							���ߣ����زر���							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "GUI.h"
#include "lcd.h"


// STM32�ӿ�------->TFT�ӿ�
// PB0 				������TFT -LED
// PB1					������TFT --RS
// PB12 				������TFT --CS
// RST(Ҳ����IO����)	TFT--RST
// PB13				������TFT -- CLK
// PB15				������TFT - SDI


// ����ֱ����ϱ����STM32F103R��Сϵͳʹ�ã�����TFT�ӿڣ�
// https://item.taobao.com/item.htm?id=523336310868

// TFT1.44Һ�����������ӣ�
// https://item.taobao.com/item.htm?id=522808579863

int main(void)
{		
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	
	LCD_Init();;	   //Һ������ʼ��
	LCD_Clear(BLACK); //����
	gui_circle(128/2,128/2,WHITE,128/2-2, 0); //��Բ
	gui_circle(128/2,128/2,WHITE,128/2-3, 0);//��Բ

	Gui_StrCenter(0,10,BLUE,YELLOW,"SysParam",16,1);//������ʾ
	Gui_StrCenter(0,28,RED,BLUE,"Temp:32��",16,1);//������ʾ
	Gui_StrCenter(0,46,YELLOW,BLUE,"Hum:20%",16,1);//������ʾ
	Gui_StrCenter(0,64,WHITE,BLUE,"VOL:3.2V",16,1);//������ʾ
	Gui_StrCenter(0,82,GREEN,BLUE,"I:1.25mA",16,1);//������ʾ
	Gui_StrCenter(0,100,RED,BLUE,"12:20AM",16,1);//������ʾ
	
	while(1)
	{
		
	}
}




