/*********************
 * 杜心雨生日礼物
 * by zhangxinhui02
 * 生日：2003.6.30
 * 
 * 管脚定义：
 * 主屏幕 A4 A5
 * 副屏幕 A2 A3
 * 检测播放生日歌 1
 * 蜂鸣器 2
 * 拆线低电位提供 3
 * 主超声波模块(T,E) 4,5(L) 6,7(R)
 * 副超声波模块 8,9
 * 泡泡机 10
 * 水泵 11
 * 舵机 12
 * 拆线电位监测 13
 ********************/

#include <LiquidCrystal_SoftI2C.h>
#include <SoftI2CMaster.h>
#include <Servo.h>

volatile int DistanceL;
volatile int DistanceR;
LiquidCrystal_SoftI2C mylcd(0x27,16,2,A3,A2);
LiquidCrystal_SoftI2C pianolcd(0x27,16,2,A5,A4);
Servo servo_12;



// ******************Music*********************


#define Do 262  
#define Re 294  
#define Mi 330  
#define Fa 349  
#define Sol 392  
#define La 440  
#define Si 494  
#define Do_h 523  
#define Re_h 587  
#define Mi_h 659  
#define Fa_h 698  
#define Sol_h 784  
#define La_h 880  
#define Si_h 988
int length;
int scale[]={Sol,Sol,La,Sol,Do_h,Si,
             Sol,Sol,La,Sol,Re_h,Do_h,
             Sol,Sol,Sol_h,Mi_h,Do_h,Si,La,
             Fa_h,Fa_h,Mi_h,Do_h,Re_h,Do_h}; 
float durt[]=
{
  0.5,0.5,1,1,1,1+1,
  0.5,0.5,1,1,1,1+1,
   0.5,0.5,1,1,1,1,1,
    0.5,0.5,1,1,1,1+1,
};                                                        //音长

// ******************Music*********************







void HappyBirthday() {

  //炸弹倒计时
  mylcd.clear();
  mylcd.setCursor(5-1, 1-1);
  mylcd.print("Warning!");
  mylcd.setCursor(5-1, 2-1);
  mylcd.print(" 00:");
  for (int i = 15; i >= 10; i = i + (-1)) {
    if (digitalRead(13)) {
      break;

    }
    mylcd.backlight();
    mylcd.setCursor(9-1, 2-1);
    mylcd.print(String(i, DEC));
    tone(2,262);
    delay(500);
    mylcd.noBacklight();
    noTone(2);
    delay(500);
  }
  mylcd.setCursor(9-1, 2-1);
  mylcd.print("0");
  for (int i = 9; i >= 6; i = i + (-1)) {
    if (digitalRead(13)) {
      break;

    }
    mylcd.backlight();
    mylcd.setCursor(10-1, 2-1);
    mylcd.print(String(i, DEC));
    tone(2,262);
    delay(500);
    mylcd.noBacklight();
    noTone(2);
    delay(500);
  }
  for (int i = 5; i >= 0; i = i + (-1)) {
    if (digitalRead(13)) {
      break;

    }
    mylcd.backlight();
    mylcd.setCursor(10-1, 2-1);
    mylcd.print(String(i, DEC));
    tone(2,262);
    delay(200);
    mylcd.noBacklight();
    noTone(2);
    delay(200);
    mylcd.backlight();
    tone(2,262);
    delay(200);
    mylcd.noBacklight();
    noTone(2);
    delay(200);
  }
  mylcd.clear();

  //检测距离是否大于40cm
  while (checkdistance_8_9() > 40) {
  }
  digitalWrite(11,HIGH);  //打开水泵
  delay(1000);
  digitalWrite(11,LOW);   //关闭水泵
  digitalWrite(10,HIGH);  //打开泡泡机
  delay(10000);
  servo_12.write(110);     //舵机推动盒壁
  delay(500);
  servo_12.write(20);
  delay(500);

  //显示信息
  mylcd.setCursor(5-1, 1-1);
  mylcd.print("After the song, ");
  mylcd.setCursor(5-1, 2-1);
  mylcd.print("read the note.");

  Sing();
}

void PlayPiano() {
  DistanceL = checkdistance_4_5();
  DistanceR = checkdistance_6_7();
  if(DistanceL < 13) {
  switch (DistanceL) {
   case 12:
    tone(2,330);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("E4  ");
    break;
   case 11:
    tone(2,294);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("D4  ");
    break;
   case 10:
    tone(2,262);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("C4  ");
    break;
   case 9:
    tone(2,247);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("B3  ");
    break;
   case 8:
    tone(2,220);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("A3  ");
    break;
   case 7:
    tone(2,196);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("G3  ");
    break;
   case 6:
    tone(2,175);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("F3  ");
    break;
   case 5:
    tone(2,165);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("E3  ");
    break;
   case 4:
    tone(2,147);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("D3  ");
    break;
   case 3:
    tone(2,131);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("C3  ");
    break;
   default:
    noTone(2);
    pianolcd.setCursor(1-1, 2-1);
    pianolcd.print("None");
    break;
  }}
  if (DistanceL >= 13) {
    switch (DistanceR) {
     case 13:
      tone(2,349);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("F4  ");
      break;
     case 12:
      tone(2,392);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("G4  ");
      break;
     case 11:
      tone(2,440);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("A4  ");
      break;
     case 10:
      tone(2,494);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("B4  ");
      break;
     case 9:
      tone(2,532);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("C5  ");
      break;
     case 8:
      tone(2,587);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("D5  ");
      break;
     case 7:
      tone(2,659);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("E5  ");
      break;
     case 6:
      tone(2,698);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("F5  ");
      break;
     case 5:
      tone(2,784);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("G5  ");
      break;
     case 4:
      tone(2,880);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("A5  ");
      break;
     default:
      noTone(2);
      pianolcd.setCursor(1-1, 2-1);
      pianolcd.print("None");
      break;
    }
  }
}

void Sing() {
  for(int x=0;x<length;x++)
  {
    tone(2,scale[x]);
    delay(400*durt[x]);   //这里用来根据节拍调节延时，500这个指数可以自己调整
    delay(100*durt[x]);
    noTone(2);
  }
  delay(3000);
}

float checkdistance_8_9() {
  digitalWrite(8, LOW);
  delayMicroseconds(2);
  digitalWrite(8, HIGH);
  delayMicroseconds(10);
  digitalWrite(8, LOW);
  float distance = pulseIn(9, HIGH) / 58.00;
  delay(10);
  return distance;
}

float checkdistance_4_5() {
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  float distance = pulseIn(5, HIGH) / 58.00;
  delay(10);
  return distance;
}

float checkdistance_6_7() {
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);
  float distance = pulseIn(7, HIGH) / 58.00;
  delay(10);
  return distance;
}

void setup(){
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
  digitalWrite(0,LOW);
  digitalWrite(3,LOW);
  length=sizeof(scale)/sizeof(scale[0]);   //计算长度
  DistanceL = 0;
  DistanceR = 0;
  servo_12.attach(12);
  servo_12.write(20);
  delay(500);
  mylcd.init();
  pianolcd.init();
  mylcd.backlight();
  pianolcd.backlight();
  if (!digitalRead(13)) {
    HappyBirthday();

  }
  if (!digitalRead(1)) {
    Sing();

  }
}

void loop(){

  pianolcd.clear();
  pianolcd.setCursor(0, 0);
  pianolcd.print(" Happy Birthday");
  pianolcd.setCursor(0, 1);
  pianolcd.print("     to You");
  delay(3000);
  pianolcd.clear();
  pianolcd.setCursor(0, 0);
  pianolcd.print("For a birth song");
  pianolcd.setCursor(0, 1);
  pianolcd.print("connect 1 to GND");
  delay(3000);
  pianolcd.clear();
  pianolcd.setCursor(0, 0);
  pianolcd.print("Then reboot it.");
  delay(3000);
  pianolcd.clear();
  pianolcd.setCursor(1-1, 1-1);
  pianolcd.print("Just Piano");
  while (true) {
    PlayPiano();
  }

}
