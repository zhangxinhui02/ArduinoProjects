#include <SoftwareSerial.h>
SoftwareSerial GY(8,9);

#define RE 7
#define DE 2

void setup(){
    Serial.begin(9600);
    pinMode(DE,OUTPUT); 
    pinMode(RE,OUTPUT); 
        digitalWrite(RE,HIGH);
        digitalWrite(DE,HIGH);
    GY.begin(9600);
    GY.write(0xA5);
    GY.write(0x09);
    GY.write(0xAE);
}

void loop(){
    digitalWrite(RE,LOW);
    digitalWrite(DE,LOW);
    while(Serial.available()>0){
        digitalWrite(RE,HIGH);
        digitalWrite(DE,HIGH);
        GY.write(Serial.read());
    }
    digitalWrite(RE,LOW);
    digitalWrite(DE,LOW);
    while(GY.available()>0){
        Serial.write(GY.read());
    }
}