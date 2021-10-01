#ifndef __KEY_H
#define __KEY_H

#define uchar unsigned char 
#define uint unsigned int

//定义按键输入端口
sbit KEY1=P0^4;
sbit KEY2=P0^3;
sbit KEY3=P0^2;
sbit KEY4=P0^1;
sbit KEY5=P0^0;

uchar key_num;
//延时
void Delay_Ms(uint ms)               
{
	uint a,b,c;
	for(a=ms;a>0;a--)
		for(b=10;b>0;b--)
			for(c=85;c>0;c--);
}


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4>KEY5!!
uchar KEY_Scan(uchar mode)
{	 
	static uchar key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0))
	{
		Delay_Ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
		else if(KEY5==0)return 5;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1)key_up=1; 	    
 	return 0;// 无按键按下
}


#endif