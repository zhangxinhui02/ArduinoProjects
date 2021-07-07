/* Version 1
 *  针脚说明
 * 蜂鸣器：6
 * LED：5
 * 触摸模块：9
 * 水位模块：3
 * 超声波：4,7
 */
#include <SoftwareSerial.h>
SoftwareSerial GPS(11,12);
SoftwareSerial ESP(8, 10); // RX, TX
bool isConnect = false;
String GPSstr="";
volatile float up;

String cmd(String cmd,unsigned int time){
  String response = "";
  ESP.println(cmd);
  unsigned long timeout = time + millis();
  while(ESP.available()||millis()<timeout){
    while(ESP.available()){
      char c = ESP.read();
      response+=c;
      }
    }
    Serial.println(response);
    return response;
  }

void initESP(){
  ESP.print("+++");
  delay(100);
  cmd("\r\n",100);
  cmd("AT+RST",2000);
  Serial.println("ESP8266模块版本信息：");
  cmd("AT+GMR",200);
  Serial.println("设置ESP8266为单连接……");
  cmd("AT+CIPMUX=0",100);
  Serial.println("设置ESP8266为Cilent模式……");
  cmd("AT+CWMODE=1",100);
  Serial.println("连接热点中……");
  cmd("AT+CWJAP=\"Network\",\"88888888\"",7000);
  Serial.println("连接TCP服务器……");
  String str=cmd("AT+CIPSTART=\"TCP\",\"服务器IP\",9999",7000);
  Serial.println("设置ESP8266为透传模式……（输入\"+++\"关闭透传进入调试模式）");
  cmd("AT+CIPMODE=1",100);
  cmd("AT+CIPSEND",100);
  if(str.indexOf("CONNECT") != -1){
    isConnect=true;
    }
  }

void getGPS(){
  long timeout=500+millis();
  while(millis()<timeout){
    while(GPS.available()){
      GPSstr+=(char)GPS.read();
      }
    }
  ESP.println(GPSstr);
  Serial.println(GPSstr);
  GPSstr="";
  }

float checkdistance_4_7() {
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  up = pulseIn(7, HIGH) / 58.00;
  delay(10);
}

void mainTask(){
  long timeout = 29000 + millis();
  while(millis()<timeout){
    checkdistance_4_7();
    if (digitalRead(9)) {
    //Serial.println(up);
    if (up < 250) {
      digitalWrite(5,HIGH);
      delay(up);

    }else{
      digitalWrite(5,LOW);
      }
    if (up < 100) {
      digitalWrite(6,HIGH);
      delay(up);
      digitalWrite(6,LOW);
      delay(up);

    }
    if (digitalRead(3)) {
      digitalWrite(6,HIGH);

    } else {
      digitalWrite(6,LOW);

    }
    digitalWrite(5,LOW);

  } else {
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);

  }
    }
  }

void setup() {
  up = 0;
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(9,INPUT);
  pinMode(3,INPUT);
  pinMode(7,INPUT);
  pinMode(4,OUTPUT);
  Serial.begin(9600);
  GPS.begin(9600);
  ESP.begin(9600);
  ESP.listen();
  delay(300);
  Serial.println("电子导盲犬程序开始运行\n");
  digitalWrite(5,HIGH);
  delay(100);
  digitalWrite(5,LOW);
  delay(100);
  digitalWrite(5,HIGH);
  delay(100);
  digitalWrite(5,LOW);
  initESP();
  GPS.listen();
  
  if(!isConnect){
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
    delay(1000);
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
    //ESP.deepSleep(3600*1000000);//深度睡眠1小时
    }else{
      digitalWrite(5,HIGH);
      delay(100);
      digitalWrite(5,LOW);
      delay(100);
      digitalWrite(5,HIGH);
      delay(100);
      digitalWrite(5,LOW);
      }
}

void loop() {
  /*if(Serial.available()){
    GPS.write(Serial.read());
    }
    if(GPS.available()){
    Serial.write(GPS.read());
    }*/
  GPS.listen();
  getGPS();
  ESP.listen();
  mainTask();
}
