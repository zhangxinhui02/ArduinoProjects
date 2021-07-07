#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h"
#include "DataType.h"

/*-------------------------ϵͳʱ�Ӳ�������----------------------------*/
#define		HES_VALUE						8000000
#define		PLL_M_VALUE					0x08					//VCO_IN = HES_VALUE/PLL_M_VALUE = 8Mhz/8 = 1Mhz
#define		PLL_N_VALUE					0x0150				//VCO = VCO_IN*PLL_N_VALUE = 1Mz*336 = 336Mhz
#define		PLL_P_VALUE					0x00					//FCLK = VCO/PLL_P_VALUE(2��4��6��8) = 336Mhz/2 = 168Mhz
#define		PLL_Q_VALUE					0x07					//USBCLK = VCO/PLL_Q_VALUE = 336Mhz/7 = 48Mhz

#define		AHB_VALUE						0x00					//AHB = FCLK/AHB_VALUE = 168Mhz/1 = 168Mhz
#define		APB1_VALUE					0x05					//APB1 = AHB/APB1_VALUE = 168Mhz/4 = 42Mhz
#define		APB2_VALUE					0x04					//APB2 = AHB/APB2_VALUE = 168Mhz/2 = 84Mhz

#define		PLLSAI_N_VALUE			0xC0					//VCO = VCO_IN*PLLSAI_N_VALUE = 1Mz*192  = 192Mhz
#define		PLLSAI_R_VALUE			0x04					//RCLK = VCO/PLLSAI_R_VALUE  = 192Mhz/4 = 48Mhz
#define		PLLDCKSAI_R_VALUE		0x02					//LCDCLK = RCLK/PLLDCKSAI_R_VALUE(2,4,8,16) = 48Mhz/8 = 6Mhz

#define		PLLI2S_N_VALUE			0xC0					//VCO = VCO_IN*PLLI2S_N_VALUE = 1Mz*192  = 192Mhz
#define		PLLI2S_R_VALUE			0x02					//RCLK = VCO/PLLI2S_R_VALUE  = 192Mhz/2 = 96Mhz


//ϵͳʱ�ӳ�ʼ��		  
//������JTAG����,�����˽���JTAG��SWD,����ĳЩ���ʹ��JLINK/ULINK�޷����س���.
//JTAGģʽ���ö���
//#define JTAG_SWD_DISABLE   0X02
//#define SWD_ENABLE         0X01
//#define JTAG_SWD_ENABLE    0X00	

/////////////////////////////////////////////////////////////////  
//void BKP_Write(u8 reg,u16 dat);																//д��󱸼Ĵ���
void Stm32_Clock_Init(void);  																	//ʱ�ӳ�ʼ��  
void NVIC_PriorityGroup(uint8 vGroup);													//���ȼ�����
//void Sys_Soft_Reset(void);      															//ϵͳ��λ
//void Sys_Standby(void);         															//����ģʽ 	
//void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);		//����ƫ�Ƶ�ַ
//void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);							//����NVIC����
//void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);	//�����ж�
//void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);								//�ⲿ�ж����ú���(ֻ��GPIOA~G)
//void JTAG_Set(u8 mode);

#endif











