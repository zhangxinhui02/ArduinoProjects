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
#include <SCoop.h>
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

defineTask(DataProcessing);
defineTask(Control);

bool BLState=false;
bool CO2State=false;
bool TCPState=false;

String Information="";
String Situation="";

String toString(int i){
  char s[30];
  dtostrf(i,3,2,s);
  String str=s;
  return str;
  }

void Print(String text1,String text2){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print(text1);
  LCD.setCursor(0, 1);
  LCD.print(text2);
  }

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

bool initESP(){
  ESP.print("+++");
  delay(100);
  cmd("\r\n",100);
  cmd("AT+CIPMUX=0",100);                                           //设置ESP8266为单连接
  cmd("AT+CWMODE=1",100);                                           //设置ESP8266为Cilent模式
  cmd("AT+CWJAP=\"Network\",\"88888888\"",7000);                    //连接网络
  String str=cmd("AT+CIPSTART=\"TCP\",\"此处为服务器IP\",9999",7000); //连接TCP服务器
  cmd("AT+CIPMODE=1",100);                                          //设置ESP8266为透传模式
  cmd("AT+CIPSEND",100);
  if(str.indexOf("CONNECT") != -1){
    return true;
    }else{
      return false;
      }
  }
int GetCO2(){
  return sgp.ECO2;
  }

int GetALC(){
  return analogRead(ALC);
  }

String GetGPSWeidu(){
  if(Information!=""){
    String temp1=Information.substring(0,Information.indexOf('#'));   //截取纬度+"#"
    temp1.replace("#",",");
    String temp2=Information.substring(Information.indexOf('#')+1);    //截取第一个"#"后的内容
    String temp3=temp2.substring(0,temp2.indexOf('#')-1);             //从temp2截取纬度半球数据（没有"#"）
    return temp1+temp3;
    }else{
        return "None";
      }
  
  }

String GetGPSJingdu(){
  if(Information!=""){
    String temp1=Information.substring(Information.indexOf('#')+1);    //截取第一个"#"后的内容
    String temp2=temp1.substring(temp1.indexOf('#')+1);                //从temp1再截取一个"#"后的内容
    
    String temp3=temp2.substring(0,temp2.indexOf('#'));               //截取经度+"#"
    temp3.replace("#",",");
    String temp4=temp2.substring(Information.indexOf('#')+1);          //截取第一个"#"后的内容
    String temp5=temp4.substring(0,temp4.indexOf('#')-1);             //从temp4截取经度半球数据（没有"#"）
    return temp3+temp5;
    }else{
      return "None";
      }

  }

int GetACC(){
  if(Information.indexOf('#')==-91){
      String temp1=Information.substring(Information.indexOf('@'));
    String temp2=temp1.substring(0,Information.indexOf('&')-1);
    if(temp2!=""){
      return temp2.toInt();
      }else{
        return 0;
        }
    }else{
      return 0;
      }

  }

int GetDistance(){
  if(Information!=""){
    String temp=Information.substring(Information.indexOf('&')+1);
    if(temp!=""){
      return (int)temp.toFloat();
      }else{
        return 0;
        }
    }else{
      return 0;
      }
  }

void setup() {
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
  Print(" Car Controller"," is Starting...");
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
  mySCoop.start();
}



void Send(int data){  //0.停止 1.前进 2.左转 3.右转 4.警报
  switch(data){
    case 0:
      Serial.print("0");
      break;
    case 1:
      Serial.print("1");
      break;
    case 2:
      Serial.print("2");
      break;
    case 3:
      Serial.print("3");
      break;
    case 4:
      Serial.print("4");
      break;
    default:
      Serial.print("0");
      break;
    }
  }

void Warn(String text1,String text2){
  Send(4);
  Print(text1,text2);
  digitalWrite(LED,HIGH);
  delay(250);
  digitalWrite(LED,HIGH);
  }



void DataProcessing::setup()
{

}
void DataProcessing::loop()
{
  long timeout=millis()+3500;
  while(millis()<timeout){
    if (Serial.available()){
        char c=Serial.read();
        Information+=c;
        }
      }
  if(GetACC()>200){   //todo:修改临界值
    Warn("ACC:"+toString(GetACC()),"   Dangerous!");
    Situation+="ACC:"+toString(GetACC());
    }
  if(GetALC()>500)
{
	Warn("ALC:"+toString(GetALC()),"   Dangerous!");
    Situation+="ALC:"+toString(GetALC());
}

if(GetCO2()<700 && sgp.ETHANOL_RAW>12000)
{
	Warn("CO2:"+toString(GetCO2()),"   Dangerous!");
  Situation+="CO2:"+toString(GetCO2());
}

  ESP.print(GetGPSJingdu()+"#"+GetGPSWeidu()+"@"+Situation);

  Information="";
  Situation="";
}


String line="";
int num=7;          //1.状态 2.经度 3.纬度 4.加速度 5.测距 6.酒精 7.CO2

void Control::setup()
{
  
}
void Control::loop()
{

  LCD.clear();
  if(digitalRead(BUTTONF)){
    Send(1);
    LCD.setCursor(0, 1);
    LCD.print("Going Forward...");
  }else if(digitalRead(BUTTONL)){
    Send(2);
    LCD.setCursor(0, 1);
    LCD.print("Turning Left...");
  }else if(digitalRead(BUTTONR)){
    Send(3);
    LCD.setCursor(0, 1);
    LCD.print("Turning Right...");
    int dis=GetDistance();
    if(dis<15){
      Warn("Dis(cm):"+dis,"   Dangerous!");
    }
  }else{
    Send(0);
    LCD.setCursor(0, 1);
    LCD.print("      Safe");
  }

  if(digitalRead(TOUCH)){
    num++;
  }
  if(num>7)
    num=1;

  //num=1 状态
  if(num==1){
    if(digitalRead(BL_STATE))
{
	BLState=true;
}
  line="BL:";
  if(BLState){
	line+="A CO2:";
  }else{
    line+="X CO2:";
  }
  if(CO2State){
    line+="A TCP:";
  }else{
    line+="X TCP:";
  }
  if(TCPState){
    line+="A";
  }else{
    line+="X";
  }
  }

  //num=2 经度
  else if(num==2){
    line=GetGPSJingdu();
  }

  //num=3 纬度
  else if(num==3){
    line=GetGPSWeidu();
  }

  //num=4 加速度
  else if(num==4){
    line="ACC:"+toString(GetACC());
  }

  //num=5 测距
  else if(num==5){
    line="Dis:"+toString(GetDistance());
    line+="cm";
  }

  //num=6 酒精
  else if(num==6){
    line="ALC:"+toString(GetALC());
  }

  //num=7 CO2
  else if(num==7){
    line="CO2:"+toString(GetCO2());
  }

  LCD.setCursor(0, 0);
  LCD.print(line);
}


void loop() {
  yield();
}
