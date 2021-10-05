#include "sys.h"

/*

//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5�� 
//CHECK OK
//091209
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}
//����NVIC 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����
//CHECK OK
//100329
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	
	u8 IPRADDR=NVIC_Channel/4;  //ÿ��ֻ�ܴ�4��,�õ����ַ 
	u8 IPROFFSET=NVIC_Channel%4;//�����ڵ�ƫ��
	IPROFFSET=IPROFFSET*8+4;    //�õ�ƫ�Ƶ�ȷ��λ��
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//ȡ����λ

	if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel;//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
	else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
	NVIC->IPR[IPRADDR]|=temp<<IPROFFSET;//������Ӧ���ȼ����������ȼ�   	    	  				   
}

//�ⲿ�ж����ú���
//ֻ���GPIOA~G;������PVD,RTC��USB����������
//����:GPIOx:0~6,����GPIOA~G;BITx:��Ҫʹ�ܵ�λ;TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
//�ú���һ��ֻ������1��IO��,���IO��,���ε���
//�ú������Զ�������Ӧ�ж�,�Լ�������   
//������...
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;//�õ��жϼĴ�����ı��
	EXTOFFSET=(BITx%4)*4;

	RCC->APB2ENR|=0x01;//ʹ��io����ʱ��

	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//���ԭ�����ã�����
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITxӳ�䵽GPIOx.BITx
	
	//�Զ�����
	EXTI->IMR|=1<<BITx;//  ����line BITx�ϵ��ж�
	//EXTI->EMR|=1<<BITx;//������line BITx�ϵ��¼� (������������,��Ӳ�����ǿ��Ե�,��������������ʱ���޷������ж�!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx���¼��½��ش���
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx���¼��������ش���
} 


//����������ִ���������踴λ!�����������𴮿ڲ�����.		    
//������ʱ�ӼĴ�����λ
//CHECK OK
//091209
void MYRCC_DeInit(void)
{										  					   
	RCC->APB1RSTR = 0x00000000;//��λ����			 
	RCC->APB2RSTR = 0x00000000; 
	  
  	RCC->AHBENR = 0x00000014;  //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.	  
  	RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //�ر������ж�
	//����������				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else   
	MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI
//CHECK OK
//091209
__asm void WFI_SET(void)
{
	WFI;    
}
//�������ģʽ	 
//check ok 
//091202
void Sys_Standby(void)
{
	SCB->SCR|=1<<2;//ʹ��SLEEPDEEPλ (SYS->CTRL)	   
  	RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	    
 	PWR->CSR|=1<<8;          //����WKUP���ڻ���
	PWR->CR|=1<<2;           //���Wake-up ��־
	PWR->CR|=1<<1;           //PDDS��λ		  
	WFI_SET();				 //ִ��WFIָ��		 
}	  
//�󱸼Ĵ���д�����
//reg:�Ĵ������
//reg:Ҫд�����ֵ 
////check ok
////091202
//void BKP_Write(u8 reg,u16 dat)
//{
//  RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	    
//	RCC->APB1ENR|=1<<27;     //ʹ�ܱ���ʱ��	    
//	PWR->CR|=1<<8;           //ȡ��������д���� 
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
//ϵͳ��λ
//CHECK OK
//091209
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 

//JTAGģʽ����,��������JTAG��ģʽ
//mode:jtag,swdģʽ����;00,ȫʹ��;01,ʹ��SWD;10,ȫ�ر�;
//CHECK OK	
//100818		  
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
	AFIO->MAPR|=temp;       //����jtagģʽ
}
*/

void SysDeInit()
{
	
}

//ϵͳʱ�ӳ�ʼ������
void Stm32_Clock_Init()
{
	unsigned char temp = 0;
	
	RCC->CR |= (0x01<<16);  								//�ⲿ����ʱ��ʹ��HSEON ����
	while( !(RCC->CR&(0x01<<17)) );					//�ȴ��ⲿʱ�Ӿ���
	
	RCC->PLLCFGR &= ~(0x01<<22);
	RCC->PLLCFGR |=  (0x01<<22);						//����HSE�ⲿʱ��λPLLʱ��Դ
	
	RCC->PLLCFGR &= ~(0x1F<<0);
	RCC->PLLCFGR |=  (PLL_M_VALUE<<0);			//PLLN VCO����ʱ�ӷ�Ƶϵ��2-63  1Mhz-2Mhz 
	
	RCC->PLLCFGR &= ~(0x1FF<<6);
	RCC->PLLCFGR |=  (PLL_N_VALUE<<6);			//PLLN VCO����ʱ�ӱ�Ƶϵ��192-432
	
	RCC->PLLCFGR &= ~(0x03<<16);
 	RCC->PLLCFGR |=  (PLL_P_VALUE<<16);			//PLLP��ϵͳʱ�������Ƶϵ��
	
	RCC->CFGR &= ~(0x0F<<4);
	RCC->CFGR |=  (AHB_VALUE<<4);						//AHBʱ�Ӳ���Ƶ = FCLKϵͳʱ��
	
	RCC->CFGR &= ~(0x07<<10);
	RCC->CFGR |=  (APB1_VALUE<<10);			//APB1ʱ�� = AHB/APB1_VALUE = 42Mhz
	
	RCC->CFGR &= ~(0x07<<13);
	RCC->CFGR |=  (APB2_VALUE<<13);			//APB2ʱ�� = AHB/APB2_VALUE = 84Mhz
	
	FLASH->ACR |= (0x01<<8);	  				//ָ��Ԥȡ
	FLASH->ACR |= (0x01<<9);	  				//ָ��Ԥ��
	
	FLASH->ACR &= ~(0x07<<0);
	FLASH->ACR |=  (0x05<<0);	  				//FLASH 2����ʱ����
	
	RCC->CR |= (0x01<<24);  						//PLLON
	while( !(RCC->CR&(0x01<<25)) );			//�ȴ�PLL����

	RCC->CFGR &= ~(0x03<<0);
	RCC->CFGR |=  (0x02<<0);						//PLL��Ϊϵͳʱ��	 
	while(temp != 0x02)     						//�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
		temp = RCC->CFGR>>2;
		temp&= 0x03;
	}
	
	//-----------------------------------------------------------------------------------PLLSAIʱ������
	RCC->CR &= ~(0x01<<28);  											//�ر�PLLSA
	while( (RCC->CR&(0x01<<29)) );								//�ȴ�PLL����
	
	RCC->PLLSAICFGR &= ~(0x1FF<<6);								//PLLSAIN	����
	RCC->PLLSAICFGR |=  (PLLSAI_N_VALUE<<6);
	
	RCC->PLLSAICFGR &= ~(0x07<<28);								//PLLSAIR	����
	RCC->PLLSAICFGR |=  (PLLSAI_R_VALUE<<28);
	
	RCC->DCKCFGR		|=  (PLLDCKSAI_R_VALUE<<16);	//PLLSAIDIVR ����
	
	RCC->CR |= (0x01<<28);  											//�ȴ�PLLSA
	while( !(RCC->CR&(0x01<<29)) );								//�ȴ�PLL����
	
	//-----------------------------------------------------------------------------------PLLI2Sʱ������
	RCC->CR &= ~(0x01<<26);  											//�ر�PLLI2S
	while( (RCC->CR&(0x01<<27)) );								//�ȴ�PLL����
	
	RCC->PLLI2SCFGR &= ~(0x1FF<<6);								//PLLSAIN	����
	RCC->PLLI2SCFGR |=  (PLLI2S_N_VALUE<<6);
	
	RCC->PLLI2SCFGR &= ~(0x07<<28);								//PLLSAIR	����
	RCC->PLLI2SCFGR |=  (PLLI2S_R_VALUE<<28);
	
	RCC->CR |= (0x01<<26);  											//�ȴ�PLLI2S
	while( !(RCC->CR&(0x01<<27)) );								//�ȴ�PLL����
	
	//-----------------------------------------------------------------------------------MCO2 ���24Mhzʱ��
	RCC->AHB1ENR  |=  (0x01<<2);
	RCC->CFGR			&= ~0x38000000;					//MCO2	4��Ƶ
	RCC->CFGR 		|=  0x30000000;
	
	RCC->CFGR 		&= ~0xC0000000;					//ѡ��PLLI2S��Ϊʱ��Դ
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
�����������ж����ȼ����麯�� ϵͳ��λ��Ĭ��0 ������
����˵����vGroup -
										0 ������ ����4λ������ռʽ���ȼ�
										1 ������ ���3λ������ռʽ���ȼ�����1λ����ָ����Ӧ���ȼ�
										2 �ڶ��� ���2λ������ռʽ���ȼ�����2λ����ָ����Ӧ���ȼ�
										3 ��һ�� ���3λ������ռʽ���ȼ�����1λ����ָ����Ӧ���ȼ�
										4 ��0 �� ����λ����ָ����Ӧ���ȼ�
�������أ���
 ---------------------------------------------------------------------*/
void NVIC_PriorityGroup(uint8 vGroup)
{
	uint32 val;
	
	SCB->AIRCR &= ~(0x07<<8);
	val = vGroup&0x07;
	SCB->AIRCR |= (val<<8);
}



























