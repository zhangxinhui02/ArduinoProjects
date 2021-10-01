/* FingerprintLocker/指纹门锁  
 * Author: zhangxinhui02  
 * 2021.10.1
 * Some code comes from the Internet.  
 */

#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <Servo.h>

SoftwareSerial ss(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&ss);
Servo myservo;

void setup(){
    Serial.begin(9600);
    finger.begin(57600);
    ss.begin(57600);
    myservo.attach(5);
    myservo.write(0);
    delay(100);
    myservo.detach();
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
        tone(11,494,1000);
    } 
    else {
        Serial.println("Error! Did not find fingerprint sensor :(");
    while (1) {}
 }
}

void loop(){
    while(getFingerprintIDez() != -1){
    myservo.attach(5);
    myservo.write(180);
    tone(11,700,300);
    delay(2000);
    myservo.write(0);
    delay(100);
    myservo.detach();
    }
    delay(100);
}

// Check fingerprint. Returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    tone(11,494,500);
    delay(700);
    tone(11,494,500);
    delay(700);
    tone(11,494,500);
    delay(700);
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    tone(11,494,500);
    delay(700);
    tone(11,494,500);
    delay(700);
    tone(11,494,500);
    delay(700);
    return -1;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
