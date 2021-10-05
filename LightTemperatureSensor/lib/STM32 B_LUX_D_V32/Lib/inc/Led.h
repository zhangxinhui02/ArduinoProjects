#ifndef _LED_H
#define _LED_H


/*--------------------------ͷ�ļ�����--------------------------------*/
#include "stm32f4xx.h"
#include "DataType.h"

/*-----------------------------�ṹ�嶨��---------------------------------*/
enum {Gled, Rled};


/*-----------------------------�궨��---------------------------------*/
#define 	GLED_O					GPIOG->MODER 	|= 	(0x01<<26);	GPIOG->PUPDR |= (0x01<<26);				//����PG13������� Ĭ�����⡢2Mhz�������
#define		GLED_H					GPIOG->BSRRL 	|= 	(0x01<<13);																		//PG13��λ
#define 	GLED_L					GPIOG->BSRRH 	|=	(0x01<<13);																		//PG13��λ
#define		GLED_DAT				(GPIOG->ODR>>13)																								//PG13�������λ

//#define 	GLED_O					GPIOC->MODER 	|= 	(0x01<<8);	GPIOC->PUPDR |= (0x01<<8);				//����PG13������� Ĭ�����⡢2Mhz�������
//#define		GLED_H					GPIOC->BSRRL 	|= 	(0x01<<4);																		//PG13��λ
//#define 	GLED_L					GPIOC->BSRRH 	|=	(0x01<<4);																		//PG13��λ
//#define		GLED_DAT				(GPIOC->ODR>>4)																									//PG13�������λ

#define 	RLED_O					GPIOG->MODER 	|= 	(0x01<<28);	GPIOG->PUPDR |= (0x00<<28);				//����PG14������� Ĭ�����⡢2Mhz�������
#define		RLED_H					GPIOG->BSRRL 	|= 	(0x01<<14);																		//PG14��λ
#define 	RLED_L					GPIOG->BSRRH 	|=	(0x01<<14);																		//PG14��λ
#define		RLED_DAT				(GPIOG->ODR>>14)																								//PG14�������λ

/*---------------------------�ⲿ��������-----------------------------*/

/*-----------------------------��������-------------------------------*/
void Led_Init(void);
void Set_LedOn(uint8 vSel);
void Set_LedOff(uint8 vSel);
void Set_LedTranspose(uint8 vSel);
	
#endif
