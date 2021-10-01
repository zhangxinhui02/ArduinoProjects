#ifndef __KEY_H
#define __KEY_H

#define uchar unsigned char 
#define uint unsigned int

//���尴������˿�
sbit KEY1=P0^4;
sbit KEY2=P0^3;
sbit KEY3=P0^2;
sbit KEY4=P0^1;
sbit KEY5=P0^0;

uchar key_num;
//��ʱ
void Delay_Ms(uint ms)               
{
	uint a,b,c;
	for(a=ms;a>0;a--)
		for(b=10;b>0;b--)
			for(c=85;c>0;c--);
}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4>KEY5!!
uchar KEY_Scan(uchar mode)
{	 
	static uchar key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0))
	{
		Delay_Ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
		else if(KEY5==0)return 5;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1)key_up=1; 	    
 	return 0;// �ް�������
}


#endif