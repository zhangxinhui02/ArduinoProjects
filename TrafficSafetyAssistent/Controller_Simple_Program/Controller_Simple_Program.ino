//引脚定义：蓝牙：TX:0 RX:1 STATE:8
//        显示屏：SCL:3 SDA:2
//        触摸(TOUCH):4
//        LED:5
//        酒精(ALC):A0
//        二氧化碳:SCL:A5 SDA:A4
//        ESP:TX:6 RX:7
//        按钮(BUTTON):9(F),10(L),11(R)

#include <SoftwareSerial.h>
#include <LiquidCrystal_SoftI2C.h>
#include <SoftI2CMaster.h>
#include <SGP30.h>
#define TOUCH 4
#define LED 5
#define ALC A0
#define ESP_TX 6
#define ESP_RX 7
#define BL_STATE 8
#define BUTTONF 9
#define BUTTONL 10
#define BUTTONR 11

LiquidCrystal_SoftI2C LCD(0x27,16,2,3,2);
SGP30 sgp;
SoftwareSerial ESP(6,7);

bool BLState=false;
bool CO2State=false;
bool TCPState=false;
bool isALC=false;

//将整型转换为字符串
String toString(int i){
  char s[30];
  dtostrf(i,3,2,s);
  String str=s;
  return str;
  }

//向屏幕输出两行文字
void Print(String text1,String text2){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print(text1);
  LCD.setCursor(0, 1);
  LCD.print(text2);
  }

//遥控器自身警报
void Warn(String text){
  Print("Warning:",text);
  for(int i=0;i<10;i++){
    digitalWrite(LED,HIGH);
    delay(250);
    digitalWrite(LED,LOW);
    delay(250);
    }
  }

//向ESP的串口发送AT指令+延时
String cmd(String cmd,unsigned int time_){
  String response = "";
  ESP.println(cmd);
  unsigned long timeout = time_ + millis();
  while(ESP.available()||millis()<timeout){
    while(ESP.available()){
      char c = ESP.read();
      response+=c;
      }
    }
    return response;
  }

//初始化ESP
bool initESP(){
  ESP.print("+++");
  delay(100);
  cmd("\r\n",100);
  cmd("AT+CIPMUX=0",100);                                           //设置ESP8266为单连接
  cmd("AT+CWMODE=1",100);                                           //设置ESP8266为Cilent模式
  cmd("AT+CWJAP=\"Network\",\"88888888\"",7000);                    //连接网络
  String str=cmd("AT+CIPSTART=\"TCP\",\"服务器IP\",39",9999);  //连接TCP服务器
  cmd("AT+CIPMODE=1",100);                                          //设置ESP8266为透传模式
  cmd("AT+CIPSEND",100);
  if(str.indexOf("CONNECT") != -1){
    return true;
    }else{
      return false;
      }
  }

//获取CO2浓度值
int GetCO2(){
  sgp.measure_air_quality();
  return sgp.ECO2;
  }

//获取酒精浓度值
int GetALC(){
  return analogRead(ALC);
  }

//自动向车（蓝牙串口）发送控制指令
void Send(){
  if(!digitalRead(BUTTONF)){
    Serial.print("1");
    delay(50);
    }else if(!digitalRead(BUTTONL)){
      Serial.print("2");
      delay(50);
      }else if(!digitalRead(BUTTONR)){
        Serial.print("3");
        delay(50);
        }else{
          Serial.print("0");
          delay(50);
          }
  }

bool isWarning=false;
String warning="";

//接收车（蓝牙串口）发来的数据并处理
void Receive(){
  while(Serial.available()){
    char c=(char)Serial.read();
    ESP.print(c);                           //将数据发送至服务器
    
    if(c=='~' && isWarning==false){         //判断是否有警报数据并截取出警报信息，使遥控器自身警报
      isWarning=true;
      }else if(c=='~' && isWarning==true){
        isWarning=false;
        Warn(warning);
        warning="";
        }
    if(isWarning==true){
      warning+=c;
      }
    }
  }

void setup(){
  pinMode(BL_STATE,INPUT);
  pinMode(TOUCH,INPUT);
  pinMode(BUTTONF,INPUT);
  pinMode(BUTTONL,INPUT);
  pinMode(BUTTONR,INPUT);
  pinMode(LED,OUTPUT);

  Serial.begin(9600);
  ESP.begin(9600);
  LCD.init();
  LCD.backlight();
  Print(" Car Controller"," is Booting...");
  if(digitalRead(BL_STATE))
    BLState=true;
  TCPState=initESP();
  CO2State=sgp.begin();

  if(!BLState)
{
  LCD.clear();
  Print("     ERROR!","Bluetooth Broken");
    delay(2000);
}
  if(!CO2State)
{
  LCD.clear();
  Print("     ERROR!","   CO2 Broken");
    delay(2000);
}
  if(!TCPState)
{
  LCD.clear();
  Print("     ERROR!","   TCP Broken");
    delay(2000);
  LCD.clear();
}
  }

void loop(){
  Print("Car State:","      Safe");
  delay(100);
  Send();
  Receive();
  int co2=GetCO2();
  sgp.measure_raw_signals();
  if(sgp.ETHANOL_RAW<12000){
    isALC=true;
    }else{
      if(co2<700)
        isALC=false;
      }
  if(co2>700 && isALC==false){
    Serial.print("4");
    Warn("CO2:"+toString(co2));
    }
  int alc=GetALC();
  if(alc>550){
    Serial.print("4");
    Warn("ALC:"+toString(alc));
    }
  }
