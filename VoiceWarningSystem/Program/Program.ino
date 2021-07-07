/********************
 * 智能声控报警程序
 * Reedit by zhangxinhui02
 * 2021.6.22
 * 
 * 由于不需要调试，删除了串口功能
 ********************/

#include <Wire.h> 
#include "LiquidCrystal_I2C.h"
#include "QhVoice.h"

#define LED_PIN 7                 //指示灯引脚，可自行修改至其他数字输出管脚（不建议修改至0,1）

#define MODE 1                    //设备工作模式，可自行修改。 0:声控灯  1：防盗  2：地震报警

#define SAMPLING_TIME 5
#define SAMPING_NUM 80
#define SAMPLING_TH   150
         
LiquidCrystal_I2C lcd(0x27,16,2);  

bool isBootVoice = true;          //默认打开欢迎语音，如果不想要请把 true 改成 false 
int voicePin = A0;                //音量检测模块管脚，可自行更改至其他模拟输入管脚
long int voiceValue = 0; 

QhVoice qhvoice;
   
void setup(){
    pinMode(LED_PIN,OUTPUT);
    
    lcd.init();                      // initialize the lcd
    lcd.backlight();
    lcd.setCursor(2,0); // go to start of 2nd line
    lcd.print("Hello, world!");                         //此处的欢迎语可自行更改为其他的每行不大于16个的英文字符
    lcd.setCursor(2,1); // go to start of 2nd line
    lcd.print("VOICE ALARM");

    qhvoice.QhVoiceInit();
    qhvoice.QhVolume_control(30);

    //指示灯测试
    digitalWrite(LED_PIN,HIGH);
    delay(1000);
    digitalWrite(LED_PIN,LOW);
    
    if(isBootVoice){
      if( MODE == 0 ){ qhvoice.QhPlayVoice(1);}         //欢迎使用智能声控灯系统
        else { qhvoice.QhPlayVoice(2); }                //欢迎使用智能报警系统-沁和专用库提供
        delay(3000);
      }
    
    //测试完毕
} 

int flag_voice = 0;

void loop(){
    getSensor();
    update_lcd1602();
    if( voiceValue > SAMPLING_TH && flag_voice == 0 ){
       flag_voice = 1;
       digitalWrite(LED_PIN,HIGH);
       if( MODE == 0 ){ qhvoice.QhPlayVoice(3);}   //灯已打开
       if( MODE == 1 ){ qhvoice.QhPlayVoice(4);}   //警告，有非法入侵
       if( MODE == 2 ){ qhvoice.QhPlayVoice(5);}   //地震来啦，快逃命呀！！！
       delay(3000);
    }
    if( voiceValue < SAMPLING_TH && flag_voice == 1 ){
       flag_voice = 0;
       digitalWrite(LED_PIN,LOW);
       if( MODE == 0 ){ qhvoice.QhPlayVoice(6); delay(2000); }   //灯已关闭
    }
}

void getSensor(){
  long int tmp_value[SAMPING_NUM] = {0};
  long int tmpV = 0;
  
  for( int i = 0; i < SAMPING_NUM; i++ ){
    tmp_value[i] = analogRead(voicePin);
//    tmp_value[i] *= 100;
//    tmp_value[i] = tmp_value[i]/1023;    
    delay( SAMPLING_TIME );
  }

  for( int i = 0; i < SAMPING_NUM; i++ ){
    if( tmpV < tmp_value[i] ){
      tmpV = tmp_value[i];
    }
  }
  
  voiceValue = tmpV;
  Serial.println(voiceValue);
}

void update_lcd1602(){
    lcd.setCursor(2,0); 
    lcd.print("VOICE ALARM  ");
    lcd.setCursor(3,1);
    lcd.print(" Value ");    
    lcd.print(voiceValue);   
    lcd.print("  ");    
}
 
