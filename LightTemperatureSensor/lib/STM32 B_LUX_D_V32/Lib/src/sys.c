#include "sys.h"

/*

//设置NVIC分组
//NVIC_Group:NVIC分组 0~4 总共5组 
//CHECK OK
//091209
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB->AIRCR=temp;  //设置分组	    	  				   
}
//设置NVIC 
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Channel           :中断编号
//NVIC_Group             :中断分组 0~4
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
//组划分:
//组0:0位抢占优先级,4位响应优先级
//组1:1位抢占优先级,3位响应优先级
//组2:2位抢占优先级,2位响应优先级
//组3:3位抢占优先级,1位响应优先级
//组4:4位抢占优先级,0位响应优先级
//NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先
//CHECK OK
//100329
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	
	u8 IPRADDR=NVIC_Channel/4;  //每组只能存4个,得到组地址 
	u8 IPROFFSET=NVIC_Channel%4;//在组内的偏移
	IPROFFSET=IPROFFSET*8+4;    //得到偏移的确切位置
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//取低四位

	if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel;//使能中断位(要清除的话,相反操作就OK)
	else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
	NVIC->IPR[IPRADDR]|=temp<<IPROFFSET;//设置响应优先级和抢断优先级   	    	  				   
}

//外部中断配置函数
//只针对GPIOA~G;不包括PVD,RTC和USB唤醒这三个
//参数:GPIOx:0~6,代表GPIOA~G;BITx:需要使能的位;TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
//该函数一次只能配置1个IO口,多个IO口,需多次调用
//该函数会自动开启对应中断,以及屏蔽线   
//待测试...
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;//得到中断寄存器组的编号
	EXTOFFSET=(BITx%4)*4;

	RCC->APB2ENR|=0x01;//使能io复用时钟

	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITx映射到GPIOx.BITx
	
	//自动设置
	EXTI->IMR|=1<<BITx;//  开启line BITx上的中断
	//EXTI->EMR|=1<<BITx;//不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx上事件上升降沿触发
} 


//不能在这里执行所有外设复位!否则至少引起串口不工作.		    
//把所有时钟寄存器复位
//CHECK OK
//091209
void MYRCC_DeInit(void)
{										  					   
	RCC->APB1RSTR = 0x00000000;//复位结束			 
	RCC->APB2RSTR = 0x00000000; 
	  
  	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
  	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //关闭所有中断
	//配置向量表				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else   
	MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
//CHECK OK
//091209
__asm void WFI_SET(void)
{
	WFI;    
}
//进入待机模式	 
//check ok 
//091202
void Sys_Standby(void)
{
	SCB->SCR|=1<<2;//使能SLEEPDEEP位 (SYS->CTRL)	   
  	RCC->APB1ENR|=1<<28;     //使能电源时钟	    
 	PWR->CSR|=1<<8;          //设置WKUP用于唤醒
	PWR->CR|=1<<2;           //清除Wake-up 标志
	PWR->CR|=1<<1;           //PDDS置位		  
	WFI_SET();				 //执行WFI指令		 
}	  
//后备寄存器写入操作
//reg:寄存器编号
//reg:要写入的数值 
////check ok
////091202
//void BKP_Write(u8 reg,u16 dat)
//{
//  RCC->APB1ENR|=1<<28;     //使能电源时钟	    
//	RCC->APB1ENR|=1<<27;     //使能备份时钟	    
//	PWR->CR|=1<<8;           //取消备份区写保护 
//	switch(reg)
//	{
//		case 1:
//			BKP->DR1=dat;
//			break;
//		case 2:
//			BKP->DR2=dat;
//			break;
//		case 3:
//			BKP->DR3=dat;
//			break; 
//		case 4:
//			BKP->DR4=dat;
//			break;
//		case 5:
//			BKP->DR5=dat;
//			break;
//		case 6:
//			BKP->DR6=dat;
//			break;
//		case 7:
//			BKP->DR7=dat;
//			break;
//		case 8:
//			BKP->DR8=dat;
//			break;
//		case 9:
//			BKP->DR9=dat;
//			break;
//		case 10:
//			BKP->DR10=dat;
//			break;
//	} 
//}	    
//系统软复位
//CHECK OK
//091209
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 

//JTAG模式设置,用于设置JTAG的模式
//mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
//CHECK OK	
//100818		  
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=temp;       //设置jtag模式
}
*/

void SysDeInit()
{
	
}

//系统时钟初始化函数
void Stm32_Clock_Init()
{
	unsigned char temp = 0;
	
	RCC->CR |= (0x01<<16);  								//外部高速时钟使能HSEON （）
	while( !(RCC->CR&(0x01<<17)) );					//等待外部时钟就绪
	
	RCC->PLLCFGR &= ~(0x01<<22);
	RCC->PLLCFGR |=  (0x01<<22);						//配置HSE外部时钟位PLL时钟源
	
	RCC->PLLCFGR &= ~(0x1F<<0);
	RCC->PLLCFGR |=  (PLL_M_VALUE<<0);			//PLLN VCO输入时钟分频系数2-63  1Mhz-2Mhz 
	
	RCC->PLLCFGR &= ~(0x1FF<<6);
	RCC->PLLCFGR |=  (PLL_N_VALUE<<6);			//PLLN VCO输入时钟倍频系数192-432
	
	RCC->PLLCFGR &= ~(0x03<<16);
 	RCC->PLLCFGR |=  (PLL_P_VALUE<<16);			//PLLP主系统时钟输出分频系数
	
	RCC->CFGR &= ~(0x0F<<4);
	RCC->CFGR |=  (AHB_VALUE<<4);						//AHB时钟不分频 = FCLK系统时钟
	
	RCC->CFGR &= ~(0x07<<10);
	RCC->CFGR |=  (APB1_VALUE<<10);			//APB1时钟 = AHB/APB1_VALUE = 42Mhz
	
	RCC->CFGR &= ~(0x07<<13);
	RCC->CFGR |=  (APB2_VALUE<<13);			//APB2时钟 = AHB/APB2_VALUE = 84Mhz
	
	FLASH->ACR |= (0x01<<8);	  				//指令预取
	FLASH->ACR |= (0x01<<9);	  				//指令预存
	
	FLASH->ACR &= ~(0x07<<0);
	FLASH->ACR |=  (0x05<<0);	  				//FLASH 2个延时周期
	
	RCC->CR |= (0x01<<24);  						//PLLON
	while( !(RCC->CR&(0x01<<25)) );			//等待PLL锁定

	RCC->CFGR &= ~(0x03<<0);
	RCC->CFGR |=  (0x02<<0);						//PLL作为系统时钟	 
	while(temp != 0x02)     						//等待PLL作为系统时钟设置成功
	{   
		temp = RCC->CFGR>>2;
		temp&= 0x03;
	}
	
	//-----------------------------------------------------------------------------------PLLSAI时钟配置
	RCC->CR &= ~(0x01<<28);  											//关闭PLLSA
	while( (RCC->CR&(0x01<<29)) );								//等待PLL锁定
	
	RCC->PLLSAICFGR &= ~(0x1FF<<6);								//PLLSAIN	配置
	RCC->PLLSAICFGR |=  (PLLSAI_N_VALUE<<6);
	
	RCC->PLLSAICFGR &= ~(0x07<<28);								//PLLSAIR	配置
	RCC->PLLSAICFGR |=  (PLLSAI_R_VALUE<<28);
	
	RCC->DCKCFGR		|=  (PLLDCKSAI_R_VALUE<<16);	//PLLSAIDIVR 配置
	
	RCC->CR |= (0x01<<28);  											//等待PLLSA
	while( !(RCC->CR&(0x01<<29)) );								//等待PLL锁定
	
	//-----------------------------------------------------------------------------------PLLI2S时钟配置
	RCC->CR &= ~(0x01<<26);  											//关闭PLLI2S
	while( (RCC->CR&(0x01<<27)) );								//等待PLL锁定
	
	RCC->PLLI2SCFGR &= ~(0x1FF<<6);								//PLLSAIN	配置
	RCC->PLLI2SCFGR |=  (PLLI2S_N_VALUE<<6);
	
	RCC->PLLI2SCFGR &= ~(0x07<<28);								//PLLSAIR	配置
	RCC->PLLI2SCFGR |=  (PLLI2S_R_VALUE<<28);
	
	RCC->CR |= (0x01<<26);  											//等待PLLI2S
	while( !(RCC->CR&(0x01<<27)) );								//等待PLL锁定
	
	//-----------------------------------------------------------------------------------MCO2 输出24Mhz时钟
	RCC->AHB1ENR  |=  (0x01<<2);
	RCC->CFGR			&= ~0x38000000;					//MCO2	4分频
	RCC->CFGR 		|=  0x30000000;
	
	RCC->CFGR 		&= ~0xC0000000;					//选择PLLI2S作为时钟源
	RCC->CFGR 		|=  0x40000000;
	
	GPIOC->AFR[1] &= ~(0x0F<<4);
	
	GPIOC->MODER	&= ~(0x03<<18);
	GPIOC->MODER	|=  (0x02<<18);
	
	GPIOC->PUPDR	&= ~(0x03<<18);
	GPIOC->PUPDR	|=  (0x02<<18);
	
	GPIOC->OSPEEDR&= ~(0x03<<18);
	GPIOC->OSPEEDR|=  (0x01<<18);
	
}

/*---------------------------------------------------------------------
功能描述：中断优先级分组函数 系统复位后默认0 第四组
参数说明：vGroup -
										0 第四组 所有4位用于抢占式优先级
										1 第三组 最高3位用于抢占式优先级，最1位用于指定响应优先级
										2 第二组 最高2位用于抢占式优先级，最2位用于指定响应优先级
										3 第一组 最高3位用于抢占式优先级，最1位用于指定响应优先级
										4 第0 组 所有位用于指定响应优先级
函数返回：无
 ---------------------------------------------------------------------*/
void NVIC_PriorityGroup(uint8 vGroup)
{
	uint32 val;
	
	SCB->AIRCR &= ~(0x07<<8);
	val = vGroup&0x07;
	SCB->AIRCR |= (val<<8);
}



























