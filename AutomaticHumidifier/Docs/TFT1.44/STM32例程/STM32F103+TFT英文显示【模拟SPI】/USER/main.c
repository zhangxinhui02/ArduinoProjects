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
	gui_circle(128/2,128/2,WHITE,128/2-2, 0); //画圆
	gui_circle(128/2,128/2,WHITE,128/2-3, 0);//画圆

	Gui_StrCenter(0,10,BLUE,YELLOW,"SysParam",16,1);//居中显示
	Gui_StrCenter(0,28,RED,BLUE,"Temp:32℃",16,1);//居中显示
	Gui_StrCenter(0,46,YELLOW,BLUE,"Hum:20%",16,1);//居中显示
	Gui_StrCenter(0,64,WHITE,BLUE,"VOL:3.2V",16,1);//居中显示
	Gui_StrCenter(0,82,GREEN,BLUE,"I:1.25mA",16,1);//居中显示
	Gui_StrCenter(0,100,RED,BLUE,"12:20AM",16,1);//居中显示
	
	while(1)
	{
		
	}
}




