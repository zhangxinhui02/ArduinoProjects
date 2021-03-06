/******************
 * 智能加湿器/AutomaticHumidifier
 * Some code comes from the Internet.
 * Author: zhangxinhui02
 * 2021.8.27
 * 
 * Only after importing UTFT and DHT libraries in Libraries folder can you run this program.
 * Adafruit_Sensor(https://github.com/adafruit/Adafruit_Sensor) had added to ../Libraries/DHT folder.
 * You have to use Arduino 1.0.6 IDE, or you will get an error about UTFT library.
 * Arduino 1.0.6 IDE is included in Docs folder.
 * ****************/

#include <UTFT.h>
#include <DHT.h>
#include <DHT_U.h>
#include "hx711.h"

#define RELAY 5
#define S1 6
#define S2 7
#define BUZZER 13

int TEMP = 0;
int HUMI = 0;
int WEIGHT = 0;
bool isStart = false;
bool LightMode = false;
#define MIN_WEIGHT 50     //停止加湿器的水的临界重量（单位：克）
#define MIN_HUMI 70       //开启加湿器的临界湿度
#define MAX_HUMI 80       //关闭加湿器的临界湿度

DHT dht(4, 11);
UTFT myGLCD(YYROBOT_TFT144,A2,A1,A5,A4,A3);  // Remember to change the model parameter to suit your display module!
//YYROBOT_TFT144  屏幕型号，不用修改
//SDA----A2
//SCL----A1
//CS-----A5
//RST----A4
//RS----A3
//LED---A0  UTFT库里面设定的，如果需要修改需要修改库文件

extern uint8_t SmallFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t BigFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t SevenSegNumFont[];//原始文件在库文件的DefaultFonts.c中




void setup()
{
  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(RELAY,OUTPUT);
  randomSeed(analogRead(0));
  SetupLCD();
  Init_Hx711();
  delay(3000);
  dht.begin();
  Get_Maopi();
  UpdateLCD(1);
}

void loop()
{
  //更新数据
  TEMP = dht.readTemperature();
  HUMI = dht.readHumidity();
  WEIGHT = Get_Weight();
  
  //监测重量，若过少则直接进入死循环停止设备以保证安全
  while(WEIGHT < -1 * MIN_WEIGHT)
  {
    WEIGHT = Get_Weight();
    if(WEIGHT >= -1 * MIN_WEIGHT){
      UpdateLCD(1);
      break;
    }
    isStart = false;
    digitalWrite(RELAY,LOW);
    myGLCD.clrScr(); 
    myGLCD.print("Warning", CENTER, 49);
    myGLCD.print("Water Needed !", CENTER, 64);
    tone(BUZZER,494,1000);
    delay(1000);
  }

  //监测湿度并自动开启加湿器
  if (HUMI < MIN_HUMI)
  {
    isStart = true;
  }else if (HUMI > MAX_HUMI)
  {
    isStart = false;
  }
  
  //监测A键(S1)调整加湿器模式
  while(digitalRead(S1)){
    tone(BUZZER,494,200);
    delay(300);
    LightMode = !LightMode;
  }
  //监测B键(S2)启动定时功能
  while(digitalRead(S2))
    SetTimeChecker();
  
  //循环调用
  TimeChecker();
  UpdateLCD(0);
  Driver();
}

//记录缓和模式运行的时间标记。注意：存储长时间的微秒数务必用long型！！！
long TimeLightMode = 0;
//硬件驱动函数
void Driver(){
  if (isStart){                                   //当设备状态设置为运行时

    if (LightMode){                               //当启动了缓和模式时
      if (TimeLightMode == 0){
        TimeLightMode = millis();
        digitalWrite(RELAY,HIGH);
      }
      if (millis() - TimeLightMode >= 10000 && millis() - TimeLightMode < 20000)      //每10s转换一次启动状态，交替开关，达到缓和加湿的目的
        digitalWrite(RELAY,LOW);
      if (millis() - TimeLightMode >= 20000)
        TimeLightMode = 0;

    }else digitalWrite(RELAY,HIGH);               //没有启动缓和模式则直接打开加湿器

  }else                                           //当设备状态设置为停止时暂停加湿器
    digitalWrite(RELAY,LOW);
}

bool isTimeSet = false;                           //表示定时模式的开启状态
int Time = 0;                                     //定时模式设置的分钟数
long TimeStartMillis = 0;                         //定时模式计算分钟使用的时间标记。注意：务必用long型！！！

//设置定时器的函数
void SetTimeChecker(){
  digitalWrite(RELAY,LOW);//中断雾化器防止水位过低
  tone(BUZZER,494,100);
  Time = 0;
  TimeStartMillis = 0;
  //显示提示信息
  myGLCD.clrScr(); //清屏
  myGLCD.print("Set Stop Time", CENTER, 2);
  myGLCD.print("Press A to add 1", LEFT, 20);   
  myGLCD.print("min for stopping.", RIGHT, 35);
  myGLCD.print("Press B to con", LEFT, 50);
  myGLCD.print("-firm.", RIGHT, 65);  
  myGLCD.print("Hold B to exit.", LEFT, 80); 

  //处理用户输入操作
  while(1){
    myGLCD.print(String(Time)+" min(s)", CENTER, 103); 
    if(digitalRead(S1)){
      delay(200);
      while(digitalRead(S1)){
        Time += 10;
        myGLCD.print(String(Time)+" min(s)", CENTER, 103); 
        delay(30);
      }
      Time++;
    }else if(digitalRead(S2)){
      delay(500);
      if(digitalRead(S2)){
        isTimeSet = false;
        Time = 0;
        TimeStartMillis = 0;
        tone(BUZZER,494,200);
        delay(200);
        break;
      }
      isTimeSet = true;
      TimeStartMillis = millis();
      tone(BUZZER,494,200);
      delay(200);
      break;
    }
  }
  UpdateLCD(1);
}

//检查定时器的状态并执行定时关闭
void TimeChecker(){

  if(isTimeSet == true){    //如果处于定时关闭模式，进行加湿操作
    isStart = true;
    if (millis() - TimeStartMillis >= 60000){
      Time--;
      TimeStartMillis = millis();
    }
    if (Time <= 0){
      isTimeSet = false;
      Time = -1;
    }


  }else if (Time == -1){    //如果已经定时关闭（Time设为-1），除非再次操作定时器，否则不再加湿
    isStart = false;
  }

                            //如果未开启过定时关闭功能，则此函数不执行任何功能
}

//初始化LCD
void SetupLCD(){
  myGLCD.InitLCD();//初始化液晶
  myGLCD.InitLCD();//初始化两次有利于系统稳定
  myGLCD.setFont(SmallFont);//设置字体为SmallFont格式
  myGLCD.fillScr(255,255,255);//填充BLACK
  delay (500);
  
  //En_8X12 Test
  myGLCD.setColor(0, 0, 0);//设置字体颜色WHITE
  myGLCD.setBackColor(255, 255, 255);//设置背景颜色BLACK
  myGLCD.clrScr(); //清屏
  myGLCD.setFont(SmallFont);//设置字体为SmallFont格式(8*12字符)
  myGLCD.print("Automatic", CENTER, 49);
  myGLCD.print("Humidifier", CENTER, 64);
  delay (2000);
}

//更新屏幕内容。参数：屏幕更新模式(int)(1:全部更新 0:仅更新关键数据加快速度)
void UpdateLCD(int AllUpdate){
  if(AllUpdate){
    //打印基础文本
    myGLCD.clrScr();
    myGLCD.print("Humidifier", CENTER, 5);
    myGLCD.print("Status:         ", LEFT, 65);  
    myGLCD.print("Mode (Press A): ", LEFT, 95);  
    myGLCD.print("Temp.:          ", LEFT, 20);   
    myGLCD.print("Humi.:          ", LEFT, 35);
    myGLCD.print("G.:             ", LEFT, 50);
    myGLCD.print("                ", LEFT, 80); 
    myGLCD.print("                ",LEFT, 110); 
  }
  //打印关键数据
  myGLCD.print("Temp.: "+String(TEMP)+"C ", LEFT, 20); 
  myGLCD.print("Humi.:"+String(HUMI)+" ", LEFT, 35);
  myGLCD.print("G.:"+String(WEIGHT)+" g  ", LEFT, 50);
  myGLCD.print(" "+CheckStatus()+"        ", LEFT, 80); 
  myGLCD.print(" "+CheckMode(),LEFT, 110); 
}

//返回加湿器运行状态
String CheckStatus(){
  if (isTimeSet)
    return "Timer "+String(Time)+"mins";
  else if(isStart)
    return "Running";
  else
    return "Stopped";
}

//返回加湿器模式
String CheckMode(){
  if (LightMode == true)
    return " Light Mode";
  else
    return "Normal Mode";
}
