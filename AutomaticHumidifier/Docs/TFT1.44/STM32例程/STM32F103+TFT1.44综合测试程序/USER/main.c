/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "TFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"


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
	
	
	while(1)
	{
		TFT_Test_Demo();
	}
}




