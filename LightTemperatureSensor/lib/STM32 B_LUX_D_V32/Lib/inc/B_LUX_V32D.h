#ifndef __B_LUX_V30_H
#define __B_LUX_V30_H

/*--------------------------头文件引用--------------------------------*/
#include "stm32f4xx.h"
#include "DataType.h"

/*-----------------------------结构体定义-----------------------------*/

/*-----------------------------宏定义---------------------------------*/

//引脚定义
#define B_LUX_SCL0_O    GPIOD->MODER    |=  (0x01<<12);  GPIOD->PUPDR |= (0x01<<12)       //配置PD6上拉输出 默认推免、2Mhz低速输出
#define B_LUX_SCL0_H    GPIOD->BSRRL    |=  (0x01<<6)																			//PD6置位
#define B_LUX_SCL0_L    GPIOD->BSRRH    |=  (0x01<<6)																			//PD6复位

#define B_LUX_SDA0_O    GPIOD->MODER    |=  (0x01<<14);  GPIOD->PUPDR |= (0x01<<14)       //配置PD6上拉输出 默认推免、2Mhz低速输出
#define B_LUX_SDA0_H    GPIOD->BSRRL    |=  (0x01<<7)																			//PD6置位
#define B_LUX_SDA0_L    GPIOD->BSRRH    |=  (0x01<<7)																			//PD6复位

#define B_LUX_SDA0_I    GPIOD->MODER    &=  ~(0x02<<14);  GPIOD->PUPDR &= ~(0x02<<14)     //悬浮输入
#define B_LUX_SDA0_DAT  ( (GPIOD->IDR>>7) & 0x01)

#define B_LUX_INT0_I    GPIOC->DDR    &=  ~(0x02<<10);  GPIOC->CR1 &= ~(0x02<<10)       	//悬浮输入
#define B_LUX_INT0_DAT  ( (GPIOC->IDR>>5) & 0x01)

#define	B_LUX_SlaveAddress	  		0x94                                                  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
#define B_LUX_CONFIGURATION       0x00                                                  //默认模式每800MS采集一次， 自动模式

/*-----------------------------函数声明-------------------------------*/
vid    B_LUX_delay_nms(uint16 k);
vid    B_LUX_Init(vid);
vid    B_Lux_RegCfg(uint8 val);

vid    B_LUX_Single_Write(uint8 REG_Address, uint8 REG_data);                            //单个写入数据
uint8  B_LUX_Single_Read(uint8 REG_Address);                                             //单个读取内部寄存器数据
uint8  B_LUX_read(uint8 REG_Address);                                       //读取内部寄存器数据
vid    B_LUX_Multi_read(uint8 REG_Address1, uint8 REG_Address2, uint8 *vBuf);            //连续的读取内部寄存器数据
//------------------------------------
vid    B_LUX_Delay5us(vid);
vid    B_LUX_Delay5ms(vid);
vid    B_LUX_Start(vid);                                                                 //起始信号
vid    B_LUX_Stop(vid);                                                                  //停止信号
vid    B_LUX_SendACK(uint8 ack);                                                         //应答ACK
uint8  B_LUX_RecvACK(vid);                                                               //读ack
vid    B_LUX_SendByte(uint8 dat);                                                        //IIC单个字节写
uint8  B_LUX_RecvByte(vid);                                                              //IIC单个字节读

vid B_LUX_GetLux(uint32 *vLux);

#endif
