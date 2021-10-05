#include "font.h"
#include <U8g2lib.h>
#include <Adafruit_Fingerprint.h>

//需要添加  u8g2  Adafruit_Fingerprint 库


#define x_coordinate   40


#define KEY1 4
#define KEY2 12
#define KEY3 6
#define KEY4 8
#define KEY5 9


#define mySerial Serial1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 2, /* data=*/ 11, /* reset=*/ U8X8_PIN_NONE);

u8 key_num=0;

void key_init()
{
  pinMode(KEY1 , INPUT_PULLUP);
  pinMode(KEY2 , INPUT_PULLUP);
  pinMode(KEY3 , INPUT_PULLUP);
  pinMode(KEY4 , INPUT_PULLUP);
  pinMode(KEY5 , INPUT_PULLUP);
}

u8 key_scan(u8 mode)
{
  static u8 key_up=1;//按键按松开标志
  if(mode)key_up=1;  //支持连按     
  if(key_up&&(digitalRead(KEY1)==0||digitalRead(KEY2)==0||digitalRead(KEY3)==0||digitalRead(KEY4)==0||digitalRead(KEY5)==0))
  {
    delay(10);
    key_up=0;
    if(digitalRead(KEY1)==0)return 1;
    else if(digitalRead(KEY2)==0)return 2;
    else if(digitalRead(KEY3)==0)return 3;
    else if(digitalRead(KEY4)==0)return 4;
    else if(digitalRead(KEY5)==0)return 5;
  }else if(digitalRead(KEY1)==1&&digitalRead(KEY2)==1&&digitalRead(KEY3)==1&&digitalRead(KEY4)==1&&digitalRead(KEY5)==1)key_up=1;       
  return 0;// 无按键按下
}

void Add_FR()
{
  u8 i,ensure,processnum=0;
  u8 ID_NUM=0;
  char str2[10];
  while(1)
  {
    switch (processnum)
    {
      case 0:
        i++;
        u8g2.clearBuffer();         // 清屏
        u8g2.drawXBMP(32,24,64,16,State5);  /* 字串 请按手指   64x16  */
        u8g2.sendBuffer();
        ensure=finger.getImage();
        if(ensure==FINGERPRINT_OK) 
        {
          ensure=finger.image2Tz(1);//生成特征
          if(ensure==FINGERPRINT_OK)
          {
            u8g2.clearBuffer();         // 清屏
            u8g2.drawXBMP(32,24,64,16,State6);  /* 字串 指纹正常   64x16  */ 
            u8g2.sendBuffer();
            i=0;
            processnum=1;//跳到第二步            
          }else {};       
        }else {};           
        break;
      
      case 1:
        i++;
        u8g2.clearBuffer();         // 清屏
        u8g2.drawXBMP(32,24,64,16,State7); /* 字串 再按一次   64x16  */
        u8g2.sendBuffer();
        ensure=finger.getImage();
        if(ensure==FINGERPRINT_OK) 
        {
          ensure=finger.image2Tz(2);//生成特征
          if(ensure==FINGERPRINT_OK)
          {
            u8g2.clearBuffer();         // 清屏
            u8g2.drawXBMP(32,24,64,16,State6);/* 字串 指纹正常   64x16  */
            u8g2.sendBuffer();
            i=0;
            processnum=2;//跳到第三步
          }else {}; 
        }else {};   
        break;

      case 2:
        u8g2.clearBuffer();         // 清屏
        u8g2.drawXBMP(32,24,64,16,State8);/* 字串 创建模板   64x16  */   
        u8g2.sendBuffer();
        ensure=finger.createModel();
        if(ensure==FINGERPRINT_OK) 
        {
          u8g2.clearBuffer();         // 清屏
          u8g2.drawXBMP(16,24,96,16,State9);  /* 字串 模板创建成功   96x16  */   
          u8g2.sendBuffer();
          processnum=3;//跳到第四步
        }
        else
        {         
          u8g2.clearBuffer();         // 清屏
          u8g2.drawXBMP(16,24,96,16,State10);  //显示字模汉字   
          u8g2.sendBuffer();
          i=0;
          processnum=0;//跳回第一步    
        }
        delay(500);
        break;        
      case 3: 
        u8g2.clearBuffer();         // 清屏
        u8g2.drawXBMP(1,0,128,48,State11);  //显示字模汉字 
        u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
        u8g2.drawStr(40,62,"ID=00");  
        u8g2.sendBuffer();
        
        while(key_num!=3)
        {
          key_num=key_scan(0);
          if(key_num==2)
          {
            key_num=0;
            if(ID_NUM>0)
              ID_NUM--;
            if(ID_NUM<10)
              sprintf(str2,"ID=0%d",ID_NUM);
            else
              sprintf(str2,"ID=%d",ID_NUM);
            u8g2.drawStr(40,62,str2);
            u8g2.sendBuffer();
          }
          if(key_num==4)
          {
            key_num=0;
            if(ID_NUM<99)
              ID_NUM++;
            if(ID_NUM<10)
              sprintf(str2,"ID=0%d",ID_NUM);
            else
              sprintf(str2,"ID=%d",ID_NUM);
            u8g2.drawStr(40,62,str2);
            u8g2.sendBuffer();
          }
        }
        key_num=0;
        ensure=finger.storeModel(ID_NUM);//储存模板
        if(ensure==0x00) 
        {   
          u8g2.clearBuffer();         // 清屏
          u8g2.drawXBMP(16,24,96,16,State12);  //显示字模汉字   
          u8g2.sendBuffer();
          delay(1500);
          return ;
        }else {processnum=0;}         
        break;        
    }
    delay(400);
    if(i==10)//超过5次没有按手指则退出
    {
      break;  
    }
  } 
}

void Press_FR()
{
  u8 ensure,i;
  char str[20];
  u8g2.clearBuffer();         // 清屏
  u8g2.drawXBMP(32,24,64,16,State5);/* 字串 指纹正常   64x16  */
  u8g2.sendBuffer();
  for(i=0;i<15;i++)
  {
    ensure=finger.getImage();
    if(ensure==0x00)//获取图像成功 
    {   
      ensure=finger.image2Tz();
      if(ensure==0x00) //生成特征成功
      {     
        ensure=finger.fingerFastSearch();
        if(ensure==0x00)//搜索成功
        {       
          u8g2.clearBuffer();         // 清屏
          u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
          u8g2.drawXBMP(16,16,96,16,State13);  //显示字模汉字   
                  
          sprintf(str,"ID:%d Score:%d",finger.fingerID,finger.confidence);
          u8g2.drawStr(1,46,str);
          u8g2.sendBuffer();
          delay(2000);
          break;
        }
        else 
        {
          u8g2.clearBuffer();         // 清屏
          u8g2.drawXBMP(16,16,96,16,State14);  //显示字模汉字 
          //ShowErrMessage(ensure);
          u8g2.sendBuffer();
          delay(2000);
          break;
        }                 
      }
      else
        {
          //ShowErrMessage(ensure);
        }
    }
    delay(200);
  }
}

void Del_FR()
{
  u8  ensure;
  u8 ID_NUM=0;
  char str2[10];
  u8g2.clearBuffer();         // 清屏
  u8g2.drawXBMP(1,0,128,48,State15);  //显示字模汉字
  sprintf(str2,"ID=0%d",ID_NUM);
  u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体 
  u8g2.drawStr(44,62,str2);  
  u8g2.sendBuffer();
  
  while(key_num!=3)
  {
    key_num=key_scan(0);
    if(key_num==2)
    {
      key_num=0;
      if(ID_NUM>0)
        ID_NUM--;
      if(ID_NUM<10)
        sprintf(str2,"ID=0%d",ID_NUM);
      else
        sprintf(str2,"ID=%d",ID_NUM);
      u8g2.drawStr(44,62,str2);
      u8g2.sendBuffer();
    }
    if(key_num==4)
    {
      key_num=0;
      if(ID_NUM<99)
        ID_NUM++;
      if(ID_NUM<10)
        sprintf(str2,"ID=0%d",ID_NUM);
      else
        sprintf(str2,"ID=%d",ID_NUM);
      u8g2.drawStr(44,62,str2);
      u8g2.sendBuffer();
    }
    if(key_num==1)
      goto MENU ; //返回主页面
    if(key_num==5)
    {
      key_num=0;
      ensure=finger.emptyDatabase();//清空指纹库
      if(ensure==0)
      {
        u8g2.clearBuffer();         // 清屏
        u8g2.drawXBMP(8,16,80,16,State17);  //显示字模汉字  
        u8g2.drawXBMP(88,16,32,16,State19);  //显示字模汉字 
        u8g2.sendBuffer();    
      }
      else
      {
        u8g2.clearBuffer();         // 清屏
        u8g2.drawXBMP(8,16,80,16,State17);  //显示字模汉字 
        u8g2.drawXBMP(88,16,32,16,State20);  //显示字模汉字  
        u8g2.sendBuffer();
      }
      delay(1500);
      goto MENU ; //返回主页面
    }   
  }
  ensure=finger.deleteModel(ID_NUM);//删除单个指纹
  if(ensure==0)
  {
    u8g2.clearBuffer();         // 清屏
    u8g2.drawXBMP(16,16,64,16,State18);  //显示字模汉字
    u8g2.drawXBMP(80,16,32,16,State19);  //显示字模汉字  
    u8g2.sendBuffer();  
  }
    else
  {
    u8g2.clearBuffer();         // 清屏
    u8g2.drawXBMP(16,16,64,16,State18);  //显示字模汉字
    u8g2.drawXBMP(80,16,32,16,State20);  //显示字模汉字  
    u8g2.sendBuffer();  
  }
  delay(1500);
MENU: 
  key_num=0;
}


void setup() {
  // put your setup code here, to run once:
  key_init();
  u8g2.begin();
  finger.begin(57600);
  u8g2.clearBuffer();         // 清屏
  u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
  u8g2.drawXBMP(1,0,128,16,State1);  //显示字模汉字   /* 指纹模块测试程序  128x16  */
  u8g2.drawXBMP(40,16,64,16,State2); 
  u8g2.drawXBMP(40,32,64,16,State3); 
  u8g2.drawXBMP(40,48,64,16,State4);
  u8g2.drawStr(22,30,"K1"); // write something to the internal memory
  u8g2.drawStr(22,46,"K3"); // write something to the internal memory
  u8g2.drawStr(22,62,"K5"); // write something to the internal memory

  u8g2.sendBuffer();          // 发送显示
  
}

void loop() {

  key_num = key_scan(0);
  if(key_num == 1)
  {
    key_num = 0;
    Add_FR();
    u8g2.clearBuffer();         // 清屏
    u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
    u8g2.drawXBMP(1,0,128,16,State1);  //显示字模汉字   /* 指纹模块测试程序  128x16  */
    u8g2.drawXBMP(40,16,64,16,State2); 
    u8g2.drawXBMP(40,32,64,16,State3); 
    u8g2.drawXBMP(40,48,64,16,State4);
    u8g2.drawStr(22,30,"K1"); // write something to the internal memory
    u8g2.drawStr(22,46,"K3"); // write something to the internal memory
    u8g2.drawStr(22,62,"K5"); // write something to the internal memory

    u8g2.sendBuffer();          // 发送显示
  }
  if(key_num == 3)
  {
    key_num = 0;
    Del_FR();
    u8g2.clearBuffer();         // 清屏
    u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
    u8g2.drawXBMP(1,0,128,16,State1);  //显示字模汉字   /* 指纹模块测试程序  128x16  */
    u8g2.drawXBMP(40,16,64,16,State2); 
    u8g2.drawXBMP(40,32,64,16,State3); 
    u8g2.drawXBMP(40,48,64,16,State4);
    u8g2.drawStr(22,30,"K1"); // write something to the internal memory
    u8g2.drawStr(22,46,"K3"); // write something to the internal memory
    u8g2.drawStr(22,62,"K5"); // write something to the internal memory

    u8g2.sendBuffer();          // 发送显示
  }
  if(key_num == 5)
  {
    key_num = 0;
    Press_FR();
    u8g2.clearBuffer();         // 清屏
    u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
    u8g2.drawXBMP(1,0,128,16,State1);  //显示字模汉字   /* 指纹模块测试程序  128x16  */
    u8g2.drawXBMP(40,16,64,16,State2); 
    u8g2.drawXBMP(40,32,64,16,State3); 
    u8g2.drawXBMP(40,48,64,16,State4);
    u8g2.drawStr(22,30,"K1"); // write something to the internal memory
    u8g2.drawStr(22,46,"K3"); // write something to the internal memory
    u8g2.drawStr(22,62,"K5"); // write something to the internal memory

    u8g2.sendBuffer();          // 发送显示
  }
}
