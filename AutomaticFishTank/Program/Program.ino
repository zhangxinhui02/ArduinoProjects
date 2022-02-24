/**
 * @file Program.ino
 * @author zhangxinhui02
 * @brief AutomaticFishTank/鱼缸自动化 Arduino主程序
 * @date 2022-02-23
 * 
 * 警告：存在一些未解决的bug
 * 
 */

//todo 获取时间 每天定时开灯关灯，投放鱼食，每周定时抽水放水
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//管脚定义
#define CHECK_INPUT 2   //水源的水量检测管脚
#define CHECK_TANK 4    //鱼缸内的水量检测管脚
#define BUTTON 12       //控制按钮管脚
#define ESP_EN 5        //ESP模块使能管脚
#define BUZZER 6        //蜂鸣器管脚
#define INPUT_PUMP 7    //向鱼缸内加水的水泵
#define OUTPUT_PUMP 8   //从鱼缸里向外抽水的水泵
#define CHECK_WARNING 9 //水位过高的预警监测引脚
#define LIGHT 10        //紫外线灯引脚d
//WiFi信息
#define SSID ssid
#define PASSWD password
//时间变量
unsigned int year = 2022;
unsigned int month = 2;
unsigned int day = 22;
unsigned int hour = 0;
unsigned int minu = 0;
unsigned int sec = 0;
unsigned int week = 2;    //星期
//硬件对象
LiquidCrystal_I2C mylcd(0x27, 16, 2);
//此优先级标志会令屏幕更新函数无效化从而保证特殊任务中屏幕的独占
unsigned short _task_special_task_flag = 0;


#pragma region ESP相关函数

//向ESP发送AT指令并等待指定延时，返回ESP的输出值
String _esp_cmd(String cmd,unsigned int time){
  String response = "";
  cmd+="\r\n";
  Serial.print(cmd);
  unsigned long timeout = time + millis();
  while(Serial.available()||millis()<timeout){
    while(Serial.available()){
      char c = Serial.read();
      response+=c;
      }
    }
    return response;
  }

//由年月日计算出星期值并返回
int _esp_get_week(unsigned int _year, unsigned int _month, unsigned int _day )
{
  int c, y, _week;
  if (_month == 1 || _month == 2)
    _year--, _month += 12;
  c = _year / 100;
  y = _year - c * 100;
  _week = y + y / 4 + c / 4 - 2 * c + 26 * (_month + 1) / 10 + _day - 1;
  while (week < 0)
    _week += 7;
  _week %= 7;
  return _week;
}

//使用ESP模块更新时间
bool esp_update_time(){
    digitalWrite(ESP_EN, HIGH);
    delay(5000);
    _esp_cmd("AT", 500);
    _esp_cmd("AT+CWMODE=3", 500);
    _esp_cmd("AT+CWJAP=\"SSID\",\"PASSWD\"", 10000);
    _esp_cmd("AT+CIPMUX=0", 500);
    _esp_cmd("AT+CIPSTART=\"TCP\",\"quan.suning.com\",80", 1000);
    _esp_cmd("AT+CIPMODE=1", 500);
    _esp_cmd("AT+CIPSEND", 500);
    for(int i = 0; i<5; i++){
        String response = _esp_cmd("GET http://quan.suning.com/getSysTime.do", 2000);
        if(response.length() > 100 || response.length() < 10)
            continue;
        else{
            year = response.substring(13, 17).toInt();
            month = response.substring(18, 20).toInt();
            day = response.substring(21, 23).toInt();
            hour = response.substring(24, 26).toInt();
            minu = response.substring(27, 29).toInt();
            sec = response.substring(30, 32).toInt();
            week = _esp_get_week(year, month, day);
            digitalWrite(ESP_EN, LOW);
            return true;
        }
    }
    digitalWrite(ESP_EN, LOW);
    return false;
}

#pragma endregion

#pragma region 水控相关函数

//检查水源的水量
bool _pump_check_input(){
    if(digitalRead(CHECK_INPUT))
        return true;
    else
        return false;
}

//检查鱼缸的水量
bool _pump_check_tank(){
    if(digitalRead(CHECK_TANK))
        return true;
    else
        return false;
}

//向鱼缸内加水
void pump_input(){
    if(!_pump_check_input()){
        mylcd.clear();
        lcd_warning("No enough water!", 5);
    }
    while(_pump_check_input())
        if(digitalRead(CHECK_WARNING))
            break;
        digitalWrite(INPUT_PUMP, HIGH);
    digitalWrite(INPUT_PUMP, LOW);
}

//从鱼缸向外抽水
void pump_output(){
    while(_pump_check_tank())
        digitalWrite(INPUT_PUMP, HIGH);
    digitalWrite(INPUT_PUMP, LOW);
}

#pragma endregion

#pragma region 屏幕相关函数

//显示指定秒数的警告信息
void lcd_warning(String msg, unsigned short sec){
    mylcd.clear();
    mylcd.backlight();
    mylcd.setCursor(0, 0);
    mylcd.print("Warning!");
    mylcd.setCursor(0, 1);
    mylcd.print(msg);
    for(unsigned short i=0; i<sec; i++){
        mylcd.backlight();
        tone(BUZZER, 262);
        delay(500);
        mylcd.noBacklight();
        noTone(BUZZER);
        delay(500);
    }
    mylcd.backlight();
    while(!digitalRead(BUTTON));
    delay(500);
    mylcd.clear();
    mylcd.setCursor(0, 0);
    mylcd.print("Processed!");
    mylcd.setCursor(0, 1);
    mylcd.print("Updating time...");
    for(unsigned int i=0; i<5; i++){
        if(esp_update_time())
            break;
        delay(10000);
    }
    mylcd.noBacklight();
}

//将时间转换为规整的字符串并返回
String _lcd_format_str(unsigned int time_) {
  String time_str = String(time_);
  if (time_str.length() < 2)
    return "0" + time_str;
  else return time_str;
}

//更新通常的屏幕信息
void lcd_update_common_screen(){
    if(!_task_special_task_flag){
        //String year_str = _lcd_format_str(year);
        String month_str = _lcd_format_str(month);
        String day_str = _lcd_format_str(day);
        String hour_str = _lcd_format_str(hour);
        String minu_str = _lcd_format_str(minu);
        //String sec_str = format_str(sec);
        mylcd.clear();
        mylcd.setCursor(0, 0);
        mylcd.print(String(year)+"."+month_str+"."+day_str+" "+hour_str+":"+minu_str);
        mylcd.setCursor(0, 1);
        mylcd.print("Execute at 12:00");
    }

}

#pragma endregion

#pragma region 子任务函数

//时间标志
unsigned long _task_time_flag = 0;

//更新本地时间信息
void task_update_time(){
    unsigned long _time_now = millis();
    if(_time_now - _task_time_flag < 0){     //溢出
        esp_update_time();
        _task_time_flag = millis();
    }
    else{
        unsigned int _time_passed_sec = (_time_now - _task_time_flag) / 1000;
        _task_time_flag = millis();
        sec += _time_passed_sec;
        minu += sec / 60;
        sec %= 60;
        hour += minu / 60;
        minu %= 60;
        if(hour == 24){ //0点更新网络时间
            day += 1;   //为了避免出现网络问题而预先本地更新
            hour = 0;
            mylcd.clear();
            mylcd.setCursor(0, 0);
            mylcd.print("One day passed!");
            mylcd.setCursor(0, 1);
            mylcd.print("Updating time...");
            for(unsigned int i=0; i<5; i++){
                if(esp_update_time())
                    break;
                delay(10000);
            }
        }
    }
}

//12点打开紫外线灯1小时 特殊标志 1
void task_control_light(){
    if(hour == 12){
        if(_task_special_task_flag <= 1){
            _task_special_task_flag = 1;
            //String year_str = _lcd_format_str(year);
            String month_str = _lcd_format_str(month);
            String day_str = _lcd_format_str(day);
            String hour_str = _lcd_format_str(hour);
            String minu_str = _lcd_format_str(minu);
            mylcd.clear();
            mylcd.setCursor(0, 0);
            mylcd.print(String(year)+"."+month_str+"."+day_str+" "+hour_str+":"+minu_str);
            mylcd.setCursor(0, 1);
            mylcd.print("Lighting...");
        }
        digitalWrite(LIGHT, HIGH);
    }

    else{
        if(_task_special_task_flag <= 1)
            _task_special_task_flag = 0;
        digitalWrite(LIGHT, LOW);
    }

}

//监测按键操作
unsigned int _task_button_time_flag = 0;
bool _task_button_pushed = false;
void task_check_button(){
    if(digitalRead(BUTTON)){
        tone(BUZZER, 262);
        delay(500);
        noTone(BUZZER);
        _task_button_time_flag = millis();
        _task_button_pushed = true;
    }
}

//控制按键后的屏幕背光
void task_control_lcdbacklight(){
    if(_task_button_pushed && millis() - _task_button_time_flag < 10000)
        mylcd.backlight();
    else{
        mylcd.noBacklight();
        _task_button_pushed = false;
    }
}

//换水任务 特殊标志 2
bool _task_changed_flag = false;
void task_change_water(){
    if(week == 6 && hour == 12 && !_task_changed_flag){
        if(_task_special_task_flag <= 2){
            _task_special_task_flag = 2;
        }
        tone(BUZZER, 262);
        mylcd.backlight();
        mylcd.clear();
        mylcd.setCursor(0, 0);
        mylcd.print("Now change water");
        noTone(BUZZER);
        mylcd.setCursor(0, 1);
        mylcd.print("Outputing...");
        pump_output();
        mylcd.setCursor(0, 1);
        mylcd.print("Inputing... ");
        pump_input();
        _task_changed_flag = true;
        mylcd.clear();
        mylcd.setCursor(0, 0);
        mylcd.print("Water changed");
        mylcd.setCursor(0, 1);
        mylcd.print("   successfully!");
        delay(10000);
    }
    else{
        if(_task_special_task_flag <= 2){
            _task_special_task_flag = 0;
        }
    }
    if(week != 6 && hour != 12)
        _task_changed_flag = false;
}

#pragma endregion


void setup(){
    pinMode(CHECK_INPUT, INPUT);
    pinMode(CHECK_TANK, INPUT);
    pinMode(BUTTON, INPUT);
    pinMode(ESP_EN, OUTPUT);
    //pinMode(BUZZER, OUTPUT);
    pinMode(INPUT_PUMP, OUTPUT);
    pinMode(OUTPUT_PUMP, OUTPUT);
    pinMode(CHECK_WARNING, INPUT);
    pinMode(LIGHT, OUTPUT);
    mylcd.init();
    mylcd.clear();
    mylcd.backlight();
    mylcd.setCursor(0, 0);
    mylcd.print("    Welcome!");
    mylcd.setCursor(0, 1);
    mylcd.print("Getting time...");
    Serial.begin(9600);
    for(int i = 0;i<5;i++){
        if(esp_update_time())
            break;
        delay(10000);
    }
}

void loop(){
    task_update_time();
    lcd_update_common_screen();
    task_control_light();
    task_change_water();
    task_check_button();
    task_control_lcdbacklight();
    delay(100);
}
