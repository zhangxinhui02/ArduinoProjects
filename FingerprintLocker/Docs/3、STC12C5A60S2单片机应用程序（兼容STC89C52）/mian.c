#include <reg51.H>
#include <intrins.h>
#include "oled.h"
#include "UART2.h"
#include "AS608.h"
#include "KEY.h"

void main(void)
{  
	
	OLED_Init();
	OLED_Clear();
	OLED_ShowCH(0,0,"ָ��ģ����Գ���");
	OLED_ShowCH(16,2,"K1�����ָ��");
	OLED_ShowCH(16,4,"K3��ɾ��ָ��");
	OLED_ShowCH(16,6,"K5����ָ֤��");
	UART2_Init();
	Timer1_Init();
	while(1)
	{
		key_num=KEY_Scan(0);
		if(key_num==1)
		{
			key_num=0;
			OLED_Clear();
			Add_FR();
		}
		if(key_num==3)
		{
			key_num=0;
			OLED_Clear();
			Del_FR();
		}
		if(key_num==5)
		{
			key_num=0;
			OLED_Clear();
			OLED_ShowCH(32,2,"�밴��ָ");
			press_FR(); 
		}			

	}
}


