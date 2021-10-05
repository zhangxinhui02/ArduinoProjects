#include "SysTick.h"

volatile uint16 m_SystickCounter;																//ϵͳ�δ�ʱ����ֵ volatileȷ�����ᱻ�Ż��� ��ȫ�ֱ���������volatile��

void Systick_Init()
{
	SysTick->CTRL &= ~(0x01<<2);															//ѡ���ⲿʱ����Ϊʱ��Դ
	
	SysTick->LOAD = (210000&0xFFFFFF);											//�ⲿʱ��10ms��ʱ AHBʱ��168Mhz/8=21Mhz/100
	
	SysTick->CTRL |= (0x01<<1);											//����Systick�쳣�ж�����
	SysTick->CTRL &= ~(0x01<<0);										//�ر�Systick
	
	NVIC_SetPriority(SysTick_IRQn, 0x0A);
}

void Stk_Delay_ms(uint16 vDly)
{
	m_SystickCounter = vDly;
	STK_1MS;
	SYSTICK_ON;
	while(m_SystickCounter!=0);
	SYSTICK_OFF;
}

void Stk_Delay_us(uint16 vDly)
{
	m_SystickCounter = vDly;
	STK_1US;
	SYSTICK_ON;
	while(m_SystickCounter!=0);
	SYSTICK_OFF;
}

void SysTick_Handler()
{
	m_SystickCounter--; 
}

