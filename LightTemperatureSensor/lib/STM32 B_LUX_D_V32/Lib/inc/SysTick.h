#ifndef _SYSTICK_H
#define _SYSTICK_H

/*--------------------------ͷ�ļ�����--------------------------------*/
#include "stm32f4xx.h"
#include "DataType.h"

/*-----------------------------�ṹ�嶨��---------------------------------*/


/*-----------------------------�궨��---------------------------------*/
#define SYSTICK_OFF				SysTick->CTRL &= ~(0x01<<0);								//�ر�SysTick��ʱ��
#define SYSTICK_ON				SysTick->CTRL |= (0x01<<0);									//ʹ��SysTick��ʱ��

#define	STK_1MS						SysTick->LOAD = (21000&0xFFFFFF);						//���붨ʱ��Ƶֵ
#define STK_1US						SysTick->LOAD = (21&0xFFFFFF);							//΢�׶�ʱ��Ƶֵ

/*---------------------------�ⲿ��������-----------------------------*/

/*-----------------------------��������-------------------------------*/
void Systick_Init(void);																	
void SysTick_Handler(void);
void Stk_Delay_ms(uint16 vDly);
void Stk_Delay_us(uint16 vDly);

#endif
