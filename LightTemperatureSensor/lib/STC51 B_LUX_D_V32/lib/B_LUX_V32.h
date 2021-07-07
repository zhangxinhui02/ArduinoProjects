#ifndef __B_LUX_V30_H
#define __B_LUX_V30_H

/*--------------------------ͷ�ļ�����--------------------------------*/
#include <reg52.h>
#include <intrins.h>
#include "DataType.h"


/*-----------------------------�ṹ�嶨��-----------------------------*/

/*-----------------------------�궨��---------------------------------*/
sbit B_LUX_INT0		= 	P1^1;    				//�����ж�
sbit B_LUX_SCL0	=	P1^2;  					//I2C  SCLK
sbit B_LUX_SDA0	=	P1^3;  					//I2C  SDAT			

//���Ŷ���
#define B_LUX_SCL0_O       _nop_()
#define B_LUX_SCL0_H 			B_LUX_SCL0 = 1
#define B_LUX_SCL0_L				B_LUX_SCL0 = 0

#define B_LUX_SDA0_O    	_nop_()  
#define B_LUX_SDA0_H       B_LUX_SDA0 = 1
#define B_LUX_SDA0_L        B_LUX_SDA0 = 0

#define B_LUX_SDA0_I         _nop_()
#define B_LUX_SDA0_DAT    ( B_LUX_SDA0&0x01)

#define B_LUX_INT0_I    	     _nop_()
#define B_LUX_INT0_DAT      ( B_LUX_INT0&0x01)


#define	B_LUX_SlaveAddress	  0x94                                                  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
#define B_LUX_CONFIGURATION       0x00                                                  //Ĭ��ģʽÿ800MS�ɼ�һ�Σ� �Զ�ģʽ

/*-----------------------------��������-------------------------------*/
vid    B_LUX_delay_nms(uint16 k);
vid    B_LUX_Init(vid);
vid    B_Lux_RegCfg(uint8 val);

vid    B_LUX_Single_Write(uint8 REG_Address, uint8 REG_data);                            //����д������
uint8  B_LUX_Single_Read(uint8 REG_Address);                                             //������ȡ�ڲ��Ĵ�������
uint8  B_LUX_read(uint8 REG_Address);                                       //��ȡ�ڲ��Ĵ�������
vid    B_LUX_Multi_read(uint8 REG_Address1, uint8 REG_Address2, uint8 *vBuf);            //�����Ķ�ȡ�ڲ��Ĵ�������
//------------------------------------
vid    B_LUX_Delay5us(vid);
vid    B_LUX_Delay5ms(vid);
vid    B_LUX_Start(vid);                                                                 //��ʼ�ź�
vid    B_LUX_Stop(vid);                                                                  //ֹͣ�ź�
vid    B_LUX_SendACK(uint8 ack);                                                         //Ӧ��ACK
uint8  B_LUX_RecvACK(vid);                                                               //��ack
vid    B_LUX_SendByte(uint8 dat);                                                        //IIC�����ֽ�д
uint8  B_LUX_RecvByte(vid);                                                              //IIC�����ֽڶ�

vid B_LUX_GetLux(uint32 *vLux);

#endif