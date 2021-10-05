#ifndef _LED_H
#define _LED_H


/*--------------------------头文件引用--------------------------------*/
#include "stm32f4xx.h"
#include "DataType.h"

/*-----------------------------结构体定义---------------------------------*/
enum {Gled, Rled};


/*-----------------------------宏定义---------------------------------*/
#define 	GLED_O					GPIOG->MODER 	|= 	(0x01<<26);	GPIOG->PUPDR |= (0x01<<26);				//配置PG13上拉输出 默认推免、2Mhz低速输出
#define		GLED_H					GPIOG->BSRRL 	|= 	(0x01<<13);																		//PG13置位
#define 	GLED_L					GPIOG->BSRRH 	|=	(0x01<<13);																		//PG13复位
#define		GLED_DAT				(GPIOG->ODR>>13)																								//PG13输出数据位

//#define 	GLED_O					GPIOC->MODER 	|= 	(0x01<<8);	GPIOC->PUPDR |= (0x01<<8);				//配置PG13上拉输出 默认推免、2Mhz低速输出
//#define		GLED_H					GPIOC->BSRRL 	|= 	(0x01<<4);																		//PG13置位
//#define 	GLED_L					GPIOC->BSRRH 	|=	(0x01<<4);																		//PG13复位
//#define		GLED_DAT				(GPIOC->ODR>>4)																									//PG13输出数据位

#define 	RLED_O					GPIOG->MODER 	|= 	(0x01<<28);	GPIOG->PUPDR |= (0x00<<28);				//配置PG14上拉输出 默认推免、2Mhz低速输出
#define		RLED_H					GPIOG->BSRRL 	|= 	(0x01<<14);																		//PG14置位
#define 	RLED_L					GPIOG->BSRRH 	|=	(0x01<<14);																		//PG14复位
#define		RLED_DAT				(GPIOG->ODR>>14)																								//PG14输出数据位

/*---------------------------外部变量声明-----------------------------*/

/*-----------------------------函数声明-------------------------------*/
void Led_Init(void);
void Set_LedOn(uint8 vSel);
void Set_LedOff(uint8 vSel);
void Set_LedTranspose(uint8 vSel);
	
#endif
