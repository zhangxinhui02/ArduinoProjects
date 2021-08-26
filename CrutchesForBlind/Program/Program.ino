/* 盲人拐杖程序/CrutchesForBlind  
 * Auther: Li Shuo , zhangxinhui02  
 *
 * 针脚说明
 * 蜂鸣器：6
 * LED：5
 * 触摸模块：9
 * 水位模块：3
 * 超声波：4,7
 * 按钮：2
 */

#include <SoftwareSerial.h>
SoftwareSerial GPS(11,12);                      //创建SoftwareSerial对象GPS，用于卫星定位模块的串口通信
SoftwareSerial gsm(8, 10); // RX, TX            //创建SoftwareSerial对象gsm，用于连接中国移动蜂窝网络模块的串口通信
bool isConnect = false;                         //表示拐杖的服务器连接状态
String GPSstr="";                               //存储从GPS模块接收的的串口数据
String gsmstr="";                               //存储从GSM模块接收的的串口数据
volatile float up;                              //存储超声波模块测得的数据

//调用此函数对GSM模块发送AT指令，然后等待给定的延时(ms)。返回：GSM的串口返回值。参数：指令(String),延时(unsigned int)(ms)。
String cmd(String cmd,unsigned int time){
  String response = "";
  cmd+="\r\n";
  gsm.print(cmd);
  unsigned long timeout = time + millis();
  while(gsm.available()||millis()<timeout){     //延时期间收集GSM模块发送的串口数据
    while(gsm.available()){
      char c = gsm.read();
      response+=c;
      }
    }
    Serial.println(response);                   //打印至串口并将其返回
    return response;
  }

//调用此函数以开始连接服务器。
void startConnect(){
  String str = cmd("AT+CIPSTART=\"TCP\",\"此处为服务器\",\"19000\"",3000);
  cmd("AT+CIPSEND",100);
  }

//调用此函数以停止连接服务器。
void stopConnect(){
  cmd("AT+CIPCLOSE=1",300);
  cmd("AT+CIPSHUT",300);
  }

//调用此函数以对GSM模块进行初始化设置。
void initgsm(){
  delay(8000);                                  //等待8000毫秒以保证GSM模块已经正常启动
                                                //此处的AT指令详情参见模块文档
  cmd("AT+CGCLASS=\"B\"",300);
  cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"",300);
  cmd("AT+CGATT=1",300);
  cmd("AT+CIPCSGP=1,\"CMNET\"",300);
  cmd("AT+CLPORT=\"TCP\",\"2000\"",300);
  String str = cmd("AT+CIPSTART=\"TCP\",\"此处为服务器\",\"19000\"",3000);
  if(str.indexOf("OK") != -1){                  //判断是否成功连接服务器
    isConnect=true;
    }
  stopConnect();
  }

//调用此函数以获取GPS数据，并将数据发送至服务器。大约执行0.8秒。
void getGPS(){
  GPSstr="";                                    //清空上次的数据
  GPS.listen();
  long timeout=300+millis();
  while(millis()<timeout){                      //保存300ms内GPS模块发送至串口的所有信息
    while(GPS.available()){
      GPSstr+=(char)GPS.read();
      }
  }
  gsm.listen();
  startConnect();                               //连接服务器
  cmd(GPSstr,500);                              //发送数据至服务器
  gsm.write(0x1A);
  delay(1000);
  //Serial.println(GPSstr);
  stopConnect();                                //停止连接以节省电量
  }

//调用此函数以发送包含位置信息的求助短信到设定好的手机号码。
void SOS(){
  cmd("AT+CMGF=1",100);
  cmd("AT+CSCS=\"GSM\"",100);
  cmd("AT+CMGS=\"18600000000\"",100);           //这里填预定的手机号码
  String message="SOS!My Location:\n"+GPSstr;
  cmd(message,1000);
  gsm.write(0x1A);

  long timeout=60000+millis();                  //使蜂鸣器发出警报60秒
  while(millis()<timeout){
    digitalWrite(6,HIGH);
    delay(1000);
    digitalWrite(6,LOW);
    delay(300);
    }
  }

//调用此函数以获取超声波模块测得的距离(cm)。
float checkdistance_4_7() {
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  up = pulseIn(7, HIGH) / 58.00;
  delay(10);
}

//调用此函数以执行拐杖的主任务，即测距并发出警报。大约执行57秒。
void mainTask(){
  long timeout = 57000 + millis();              //在57秒内
  while(millis()<timeout){
    checkdistance_4_7();
    if (digitalRead(9)) {                       //触摸模块按下时才会工作(执行其他程序)

    if (up < 250) {                             //如果小于250cmLED灯亮起警报
      digitalWrite(5,HIGH);
      delay(up);
    }else{
      digitalWrite(5,LOW);
      }

    if (up < 100) {                             //如果小于100cm蜂鸣器警报
      digitalWrite(6,HIGH);
      delay(up);
      digitalWrite(6,LOW);
      delay(up);
    }

    if (digitalRead(3)) {                       //如果检测到积水LED亮，蜂鸣器警报
      digitalWrite(6,HIGH);
    } else {
      digitalWrite(6,LOW);
    }
    digitalWrite(5,LOW);
  } else {
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
  }

  if(!digitalRead(2)){                          
    int isLong=0;
    cmd("ATA",100);                             //发送接听指令
    delay(500);
    while(digitalRead(2)){}
    cmd("ATH",100);                             //发送挂断指令
    delay(500);
    if(!digitalRead(2)){
      digitalWrite(6,HIGH);
      delay(200);
      digitalWrite(6,LOW);
      isLong=0;
      delay(5000);
      if(!digitalRead(2)){
      digitalWrite(6,HIGH);
      delay(1000);
      digitalWrite(6,LOW);
        isLong=1;
        }
      if(isLong==0){                            //如果短按按钮则向预设号码打电话
        cmd("ATD18995434635;",100);             //包含预设号码的打电话指令
          while(digitalRead(2)){                //按下按钮后
          }
          cmd("ATH",100);                       //发送挂断指令
        }else{                                  //如果长按按钮则调用SOS()函数向预设号码发送SOS信息
          SOS();
          }
      }
    }
    }
  }

 
//Arduino开机调用的初始化函数，会初始化变量，管脚和串口设备。执行此函数约花费12秒(即正常开机需等待12秒)。
void setup() {
  up = 0;
  pinMode(6,OUTPUT);
  pinMode(2,INPUT);
  pinMode(5,OUTPUT);
  pinMode(9,INPUT);
  pinMode(3,INPUT);
  pinMode(7,INPUT);
  pinMode(4,OUTPUT);
  Serial.begin(9600);
  GPS.begin(9600);
  gsm.begin(9600);
  gsm.listen();
  delay(300);
  Serial.println("电子导盲犬程序开始运行\n作者：李硕\n");
  digitalWrite(5,HIGH);
  delay(100);
  digitalWrite(5,LOW);
  delay(100);
  digitalWrite(5,HIGH);
  delay(100);
  digitalWrite(5,LOW);
  initgsm();
  GPS.listen();
  if(!isConnect){
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
    delay(1000);
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
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

//Arduino调用完setup()函数后会循环调用的函数。此函数中包含拐杖的两个任务，即获取定位(getGPS)和测距警报(mainTask)，会反复循环执行。执行一个周期约花费58秒。
void loop() {
  getGPS();
  mainTask();
}
