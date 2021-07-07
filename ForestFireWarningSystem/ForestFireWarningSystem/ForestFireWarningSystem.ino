/********************
 * 森林火警系统 Arduino下位机数据采集端 
 * Author : zhangxinhui02
 * 2021.6.23
 *********************/

 
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>



int TIMEOUT = 5;                            //每次程序检测温度并上传至服务器之间的延时，单位：秒，可自行修改秒数，不建议小于3秒
DHT dht(2,DHT11);                           //DHT11模块（接线2，可以自行修改到其他数字管脚，不建议换到0和1）

//初始化
void setup() {
  dht.begin();
  Serial.begin(115200);
}

//主程序
void loop() {
  delay(TIMEOUT * 1000);
  Serial.println("#"+String(dht.readTemperature())+"$");
}
