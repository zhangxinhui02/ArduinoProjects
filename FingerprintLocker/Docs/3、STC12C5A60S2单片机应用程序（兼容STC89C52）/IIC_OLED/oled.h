/*********************************************绿深旗舰店************************************************/


/*                                    SGP30空气质量检测模块测试程序                                    */


/*********************************************绿深旗舰店************************************************/
#ifndef __OLED_H
#define __OLED_H	
#include "reg51.h"		  	  
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//-----------------OLED端口定义----------------  
sbit OLED_SCL=P2^4;//SCL
sbit OLED_SDIN=P2^3;//SDA

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	130
#define Max_Row		64
#define	Brightness	0xFF     						  
					   




//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 fill_Data);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(u8 x, u8 y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);
void Delay_50ms(u32 Del_50ms);
void Delay_1ms(u32 Del_1ms);
void fill_picture(u8 fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(u8 IIC_Command);
void Write_IIC_Data(u8 IIC_Data);
void Write_IIC_Byte(u8 IIC_Byte);
void IIC_Wait_Ack();
void OLED_ShowCH(u8 x, u8 y, const u8 *chs);
#endif  
	 



