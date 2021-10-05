#ifndef _SYSTICK_H
#define _SYSTICK_H

/*--------------------------头文件引用--------------------------------*/
#include "stm32f4xx.h"
#include "DataType.h"

/*-----------------------------结构体定义---------------------------------*/


/*-----------------------------宏定义---------------------------------*/
#define SYSTICK_OFF				SysTick->CTRL &= ~(0x01<<0);								//关闭SysTick定时器
#define SYSTICK_ON				SysTick->CTRL |= (0x01<<0);									//使能SysTick定时器

#define	STK_1MS						SysTick->LOAD = (21000&0xFFFFFF);						//毫秒定时分频值
#define STK_1US						SysTick->LOAD = (21&0xFFFFFF);							//微米定时分频值

/*---------------------------外部变量声明-----------------------------*/

/*-----------------------------函数声明-------------------------------*/
void Systick_Init(void);																	
void SysTick_Handler(void);
void Stk_Delay_ms(uint16 vDly);
void Stk_Delay_us(uint16 vDly);

#endif
