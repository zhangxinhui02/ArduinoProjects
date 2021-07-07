#include <reg52.h>
#include <intrins.h>
#include "B_LUX_V32.h"
#include "Delay.h"
#include "SERIAL485.h"

uint32  m_Lux = 0;
uint8   m_LuxBuf[6];
int main(void)
{     
      uint32 val32 = 0;
         
      B_LUX_Init();
      
      while(1)
      {
         //����
        B_LUX_GetLux(&m_Lux);
				
				//m_Lux = 276987;
				
        m_LuxBuf[0] = ( (m_Lux/100000) + 0x30 );			//10��
        val32 = m_Lux%100000;
				
				m_LuxBuf[1] = ( (val32/10000) + 0x30 );						//1��
        val32 = val32%10000;
				
				m_LuxBuf[2] = ( (val32/1000) + 0x30 );						//ǧ
        val32 = val32%1000;
				
				m_LuxBuf[3] = ( (val32/100) + 0x30 );							//��
        val32 = val32%100;
				
				m_LuxBuf[4] = ( (val32/10) + 0x30 );								//ʮ
        val32 = val32%10;
				
				m_LuxBuf[5] = ( val32 + 0x30 );										  //��
    
				serial_send("LUX=",  4);
				serial_send(m_LuxBuf, 6);
				serial_send("\n",  1);
				
        delay_ms(1000);
      }
        
}



