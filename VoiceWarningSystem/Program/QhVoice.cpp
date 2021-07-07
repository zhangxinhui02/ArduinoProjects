#include "QhVoice.h"
#include "qhvoice_base.h"


QhVoice::QhVoice(){
//    SoftwareSerial voiceSer(VOICE_SERIAL_RX_PIN, VOICE_SERIAL_TX_PIN);
//    voiceSer.begin(9600); 
}

void QhVoice::QhVoiceInit(){
//   SoftwareSerial voiceSer(VOICE_SERIAL_RX_PIN, VOICE_SERIAL_TX_PIN);
//   voiceSer.begin(9600);
   Serial.begin(9600);
   qhvoice_set_serial (Serial); 
   QhVolume_control(qhVolume);
}

void QhVoice::QhPlayVoice(unsigned char selData){
   qhvoice_play (selData);
}

void QhVoice::QhVolume_control(unsigned char volumeData){
   qhvoice_set_volume (volumeData);
}

void QhVoice::QhVolume_add(){
  qhVolume += 2;
  if(qhVolume > 30) qhVolume = 30;
  QhVolume_control(qhVolume);
}
void QhVoice::QhVolume_sub(){
  qhVolume -= 2;
  if(qhVolume < 0) qhVolume = 0;
  QhVolume_control(qhVolume);
}
