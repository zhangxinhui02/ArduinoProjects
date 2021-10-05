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

	POINT_COLOR=WHITE; 
	LCD_DrawRectangle(0,0,128-1,128-1);	//������ 

	Show_Str(32,5,BLUE,YELLOW,"ϵͳ���",16,0);
	Show_Str(5,25,RED,YELLOW,"�¶�     ��",24,1);
	LCD_ShowNum2412(5+48,25,RED,YELLOW,":32",24,1);

	Show_Str(5,50,YELLOW,YELLOW,"ʪ��     ��",24,1);
	LCD_ShowNum2412(5+48,50,YELLOW,YELLOW,":20",24,1);

	Show_Str(5,75,WHITE,YELLOW,"��ѹ      ��",24,1);
	LCD_ShowNum2412(5+48,75,WHITE,YELLOW,":3.2",24,1);

	Show_Str(5,100,GREEN,YELLOW,"����      ��",24,1);
	LCD_ShowNum2412(5+48,100,GREEN,YELLOW,":0.2",24,1);	
	
	while(1)
	{
		
	}
}




