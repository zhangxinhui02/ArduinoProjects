#include <SoftwareSerial.h>
SoftwareSerial GY(8,9);

#define RE 7
#define DE 2

void setup(){
    Serial.begin(9600);
    pinMode(DE,OUTPUT); 
    pinMode(RE,OUTPUT); 
    GY.begin(9600);
    digitalWrite(RE,HIGH);
    digitalWrite(DE,HIGH);
    GY.write(0xA5);
    GY.write(0x09);
    GY.write(0xAE);
    digitalWrite(RE,LOW);
    digitalWrite(DE,LOW);

   
}

void loop(){
 while(GY.available()>0){
        Serial.println("byte:");
        Serial.println(GY.read(),HEX);
    }

}