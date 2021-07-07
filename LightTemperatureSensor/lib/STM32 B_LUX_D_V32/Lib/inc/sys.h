#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h"
#include "DataType.h"

/*-------------------------系统时钟参数配置----------------------------*/
#define		HES_VALUE						8000000
#define		PLL_M_VALUE					0x08					//VCO_IN = HES_VALUE/PLL_M_VALUE = 8Mhz/8 = 1Mhz
#define		PLL_N_VALUE					0x0150				//VCO = VCO_IN*PLL_N_VALUE = 1Mz*336 = 336Mhz
#define		PLL_P_VALUE					0x00					//FCLK = VCO/PLL_P_VALUE(2、4、6、8) = 336Mhz/2 = 168Mhz
#define		PLL_Q_VALUE					0x07					//USBCLK = VCO/PLL_Q_VALUE = 336Mhz/7 = 48Mhz

#define		AHB_VALUE						0x00					//AHB = FCLK/AHB_VALUE = 168Mhz/1 = 168Mhz
#define		APB1_VALUE					0x05					//APB1 = AHB/APB1_VALUE = 168Mhz/4 = 42Mhz
#define		APB2_VALUE					0x04					//APB2 = AHB/APB2_VALUE = 168Mhz/2 = 84Mhz

#define		PLLSAI_N_VALUE			0xC0					//VCO = VCO_IN*PLLSAI_N_VALUE = 1Mz*192  = 192Mhz
#define		PLLSAI_R_VALUE			0x04					//RCLK = VCO/PLLSAI_R_VALUE  = 192Mhz/4 = 48Mhz
#define		PLLDCKSAI_R_VALUE		0x02					//LCDCLK = RCLK/PLLDCKSAI_R_VALUE(2,4,8,16) = 48Mhz/8 = 6Mhz

#define		PLLI2S_N_VALUE			0xC0					//VCO = VCO_IN*PLLI2S_N_VALUE = 1Mz*192  = 192Mhz
#define		PLLI2S_R_VALUE			0x02					//RCLK = VCO/PLLI2S_R_VALUE  = 192Mhz/2 = 96Mhz


//系统时钟初始化		  
//加入了JTAG管理,避免了禁用JTAG及SWD,导致某些情况使用JLINK/ULINK无法下载程序.
//JTAG模式设置定义
//#define JTAG_SWD_DISABLE   0X02
//#define SWD_ENABLE         0X01
//#define JTAG_SWD_ENABLE    0X00	

/////////////////////////////////////////////////////////////////  
//void BKP_Write(u8 reg,u16 dat);																//写入后备寄存器
void Stm32_Clock_Init(void);  																	//时钟初始化  
void NVIC_PriorityGroup(uint8 vGroup);													//优先级分组
//void Sys_Soft_Reset(void);      															//系统软复位
//void Sys_Standby(void);         															//待机模式 	
//void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);		//设置偏移地址
//void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);							//设置NVIC分组
//void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);	//设置中断
//void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);								//外部中断配置函数(只对GPIOA~G)
//void JTAG_Set(u8 mode);

#endif











