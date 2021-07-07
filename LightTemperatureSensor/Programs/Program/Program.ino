/************
 * 亮度温度传感器   LightTemperatureSensor
 * Reedit by zhangxinhui02
 * 2021.7.7
 * 
 * 参考：
 * https://www.arduino.cn/thread-47862-1-3.html
 * https://www.yiboard.com/thread-1102-1-1.html
 ************/


#include <SPI.h>
#include <SD.h>
//声明文件对象
File myFile;

#include <MAX6675_Thermocouple.h>
#define SCK_PIN 3
#define CS_PIN  6
#define SO_PIN  5
MAX6675_Thermocouple* thermocouple = NULL;
#include <SoftwareSerial.h>
SoftwareSerial GY(8,9);

#define RE 7
#define DE 2


//向光敏模块发送信号并接收光敏模块传回的数据
double ValueReceive(){
    byte v1,v2,v3,v4;
    digitalWrite(RE,HIGH);
    digitalWrite(DE,HIGH);
    GY.write(0xA5);
    GY.write(0x09);
    GY.write(0xAE);
    digitalWrite(RE,LOW);
    digitalWrite(DE,LOW);


    while(GY.available()==0){   //等待发送数据
    }

    while(GY.available()>0){    //获取数据
        v1 = GY.read();
        v1 = GY.read();
        v1 = GY.read();
        v1 = GY.read();
        v1 = GY.read(); //读取到第五个字节
        v2 = GY.read();
        v3 = GY.read();
        v4 = GY.read();
    }

    while(GY.available()>0){    //读取完本次数据防止意外
        GY.read();
    }
    
    double lux = (v1<<24)|(v2<<16)|(v3<<8)|v4;
    lux = lux / 100;

    return lux;
}

void setup() 
{
  Serial.begin(9600);
  GY.begin(9600);
  thermocouple = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN);
  //接收光线数据时置低，发送数据时置高
  pinMode(DE,OUTPUT); 
  pinMode(RE,OUTPUT); 
  while (!Serial) 
  {
      ; // wait for serial port to connect. Needed for Leonardo only
  }
    while(Serial.available()>0){
        GY.read();
    }

    
    Serial.print("Initializing SD card...");//正在初始化
 
    //如果为非则初始化失败
    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");//初始化结束
    //打开文件
    myFile = SD.open("LSLAB.txt", FILE_WRITE);//打开文件LSLAB.txt，若无则自动创建，但必须加入FILE_WRITE函数
    
    while(GY.available()>0){
        GY.read();
    }
}
void loop() 
{
    double val = ValueReceive();
    const double celsius = thermocouple->readCelsius();

    Serial.print(String(val));
    //Serial.print(" umol.m-2.s-1 ");
    Serial.print(" lux ");
    Serial.print(String(celsius));
    Serial.println(" C ");


    if (myFile) 
    {
        myFile.print("温度1=");//写入数据
        myFile.print(celsius);
        myFile.print("C");   
                  
        myFile.print("      光敏电阻=");//写入数据
        myFile.print(val);
        //myFile.println(" umol.m-2.s-1 ");
        myFile.println(" lux ");
        
                  
        //myFile.close();//关闭文件
    }
    else 
    {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }  

    
    delay(500);
}