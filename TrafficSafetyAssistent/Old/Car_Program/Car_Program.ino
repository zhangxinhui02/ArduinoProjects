//引脚定义： 继电器(Relay)：7.左 8.右
//          尾灯(LED)：左：11 右：12
//          蜂鸣器(Buzzer)：6
//          超声波(CH)：echo：5 trig：4
//          GPS：rx：2 tx：3
//          蓝牙：tx:0 rx:1

#include <SCoop.h>
#include <SparkFun_ADXL345.h>
#include <SoftwareSerial.h>
#define Relay1 7
#define Relay2 8
#define LED1 11
#define LED2 12
#define Buzzer 6
#define CHECHO 5
#define CHTRIG 4

SoftwareSerial GPS(3,2);
ADXL345 adxl = ADXL345();

defineTask(Receive);
defineTask(Send);

void setup(){
  Serial.begin(9600);
  GPS.begin(9600);
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(CHTRIG,OUTPUT);
  pinMode(CHECHO,INPUT);

mySCoop.start();

adxl.powerOn();                     // Power on the ADXL345

adxl.setRangeSetting(16);           // Give the range settings
                                    // Accepted values are 2g, 4g, 8g or 16g
                                    // Higher Values = Wider Measurement Range
                                    // Lower Values = Greater Sensitivity

adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                    // Default: Set to 1
                                    // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 

adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

// Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
adxl.setTapThreshold(50);           // 62.5 mg per increment
adxl.setTapDuration(15);            // 625 μs per increment
adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

// Set values for what is considered FREE FALL (0-255)
adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment

// Setting all interupts to take place on INT1 pin
//adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.

// Turn on Interrupts for each mode (1 == ON, 0 == OFF)
adxl.InactivityINT(1);
adxl.ActivityINT(1);
adxl.FreeFallINT(1);
adxl.doubleTapINT(1);
adxl.singleTapINT(1);

//attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);   // Attach Interrupt
  }

String getDistance() {
  digitalWrite(CHTRIG, LOW);
  sleep(1);
  digitalWrite(CHTRIG, HIGH);
  sleep(1);
  digitalWrite(CHTRIG, LOW);
  float distance = pulseIn(CHECHO, HIGH) / 58.00;
  sleep(10);
  char s[15];
  dtostrf(distance,3,2,s);
  String str=s;
  return str;
}

void Warn(){
    Run(0);
    for(int i=0;i<10;i++){
          digitalWrite(Buzzer,HIGH);
          digitalWrite(LED1,HIGH);
          digitalWrite(LED2,HIGH);
          sleep(250);
          digitalWrite(Buzzer,LOW);
          digitalWrite(LED1,LOW);
          digitalWrite(LED2,LOW);
          sleep(250);
        }
  }

String getGPS(){
  //GPS.listen();
  long timeout=1500+millis();
  String GPSstr="";
  while(millis()<timeout){
    while(GPS.available()){
      GPSstr+=(char)GPS.read();
      }
    }

  String weidu="None";
  String weidubq="None";
  String jingdu="None";
  String jingdubq="None";
  
  GPSstr.replace("$GNGGA,","%");
  GPSstr=GPSstr.substring(GPSstr.indexOf('%'));

  
  GPSstr=GPSstr.substring(GPSstr.indexOf(',')+1);
  if(GPSstr.charAt(0)!=','){
      weidu=GPSstr.substring(0,GPSstr.indexOf(',')-1);
    }
  GPSstr=GPSstr.substring(GPSstr.indexOf(',')+1);
  if(GPSstr.charAt(0)!=','){
      weidubq=GPSstr.substring(0,GPSstr.indexOf(',')-1);
    }
  GPSstr=GPSstr.substring(GPSstr.indexOf(',')+1);
  if(GPSstr.charAt(0)!=','){
      jingdu=GPSstr.substring(0,GPSstr.indexOf(',')-1);
    }
  GPSstr=GPSstr.substring(GPSstr.indexOf(',')+1);
  if(GPSstr.charAt(0)!=','){
      jingdubq=GPSstr.substring(0,GPSstr.indexOf(',')-1);
    }

  
  return weidu+"#"+weidubq+"#"+jingdu+"#"+jingdubq;
  }

String getACC(){
  int x,y,z;  
  adxl.readAccel(&x, &y, &z);
  char s[20];
  itoa(x,s,10);
  String str=s;
  return str;
  }

void Run(int direct){  //0.停止 1.前进 2.左转 3.右转   继电器引脚：7.左 8.右
  switch(direct){
    case 0:
      digitalWrite(Relay1,LOW);
      digitalWrite(Relay2,LOW);
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
      break;
    case 1:
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,HIGH);
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,LOW);
      break;
    case 2:
      digitalWrite(Relay1,LOW);
      digitalWrite(Relay2,HIGH);
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
      break;
    case 3:
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,LOW);
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
      break;
    default:
      break;
    }
  }


void Receive::setup()
{

}
void Receive::loop()
{
  while(Serial.available()){
    char c=Serial.read();
    int a=atoi(c);
    switch(a){  //1.前进 2.左转 3.右转 4.警报
      case 1:
        Run(1);
        break;
      case 2:
        Run(2);
        break;
      case 3:
        Run(3);
        break;
      case 4:
        Run(0);
        Warn();
        break;
      default:
        Run(0);
        break;
      }
    }
  Run(0);
}


void Send::setup()
{

}
void Send::loop()
{
  String str=getGPS()+"@"+getACC()+"&"+getDistance()+"!";
  Serial.println(str);
}


void loop(){
  yield();
  }
