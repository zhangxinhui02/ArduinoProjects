#include "stm32f4xx.h"
#include "sys.h"
#include "Led.h"
#include "SysTick.h"

#include "B_LUX_V32D.h"

void SysInit(void);

uint32  m_Lux = 0;
uint8   m_LuxBuf[4];
int main(void)
{
	uint32 val32 = 0;
	Stm32_Clock_Init();
	
	SysInit();

	Systick_Init();
	B_LUX_Init();

	while (1)
	{
		//¹âÕÕ
		B_LUX_GetLux(&m_Lux);
		//¹âÕÕ´æ´¢
		val32 = m_Lux;
		m_LuxBuf[0] = val32&0xFF;
		val32 >>= 8;
		m_LuxBuf[1] = val32&0xFF;
		val32 >>= 8;
		m_LuxBuf[2] = val32&0xFF;
		val32 >>= 8;
		m_LuxBuf[3] = val32&0xFF;
		
		Stk_Delay_ms(1000);
		
	}
}

void SysInit()
{
	Led_Init();
	
}
