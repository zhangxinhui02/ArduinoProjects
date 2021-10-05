/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "GUI.h"
#include "lcd.h"


// STM32接口------->TFT接口
// PB0 				连接至TFT -LED
// PB1					连接至TFT --RS
// PB12 				连接至TFT --CS
// RST(也可以IO控制)	TFT--RST
// PB13				连接至TFT -- CLK
// PB15				连接至TFT - SDI


// 可以直接配合本店的STM32F103R最小系统使用，带有TFT接口：
// https://item.taobao.com/item.htm?id=523336310868

// TFT1.44液晶屏购买链接：
// https://item.taobao.com/item.htm?id=522808579863

int main(void)
{		
	delay_init();
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	
	LCD_Init();;	   //液晶屏初始化
	LCD_Clear(BLACK); //清屏

	POINT_COLOR=WHITE; 
	LCD_DrawRectangle(0,0,128-1,128-1);	//画矩形 

	Show_Str(32,5,BLUE,YELLOW,"系统监控",16,0);
	Show_Str(5,25,RED,YELLOW,"温度     ℃",24,1);
	LCD_ShowNum2412(5+48,25,RED,YELLOW,":32",24,1);

	Show_Str(5,50,YELLOW,YELLOW,"湿度     ％",24,1);
	LCD_ShowNum2412(5+48,50,YELLOW,YELLOW,":20",24,1);

	Show_Str(5,75,WHITE,YELLOW,"电压      Ｖ",24,1);
	LCD_ShowNum2412(5+48,75,WHITE,YELLOW,":3.2",24,1);

	Show_Str(5,100,GREEN,YELLOW,"电流      Ａ",24,1);
	LCD_ShowNum2412(5+48,100,GREEN,YELLOW,":0.2",24,1);	
	
	while(1)
	{
		
	}
}




