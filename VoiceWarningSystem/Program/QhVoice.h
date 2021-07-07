#ifndef QHVOICE_H
#define QHVOICE_H

#include <Wire.h> 
#include "arduino.h"

class QhVoice {

	public:
      int qhVolume = 30;
	    QhVoice();
      void QhVoiceInit();
      void QhPlayVoice(unsigned char selData);   //语音播放
      void QhVolume_control(unsigned char volumeData);      //音量控制
      void QhVolume_add();      //音量增加
      void QhVolume_sub();      //音量减小
};

#endif
