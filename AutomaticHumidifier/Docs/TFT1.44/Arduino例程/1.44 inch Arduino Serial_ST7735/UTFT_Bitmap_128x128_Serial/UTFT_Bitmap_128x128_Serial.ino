#include <UTFT.h>
#include <avr/pgmspace.h>

UTFT myGLCD(YYROBOT_TFT144,A2,A1,A5,A4,A3);  // Remember to change the model parameter to suit your display module!
//YYROBOT_TFT144  屏幕型号，不用修改
//SDA----A2
//SCL----A1
//CS-----A5
//RST----A4
//RS----A3
//LED---A0  UTFT库里面设定的，如果需要修改需要修改库文件

extern unsigned int icon1[0x400];
extern unsigned int icon2[0x400];
extern unsigned int tux[0x1000];
extern unsigned int lbxxPNG[16128];

void setup()
{
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.fillScr(255, 255, 255);
}

void loop()
{


// Draw a 4 by 4 grid of a 32x32 icon.
myGLCD.fillScr(255, 255, 255);
for (int x=0; x<4; x++)
  for (int y=0; y<4; y++)
    myGLCD.drawBitmap (x*32, y*32, 32, 32, icon1);

delay(5000);

// Draw a 2 by 2 grid of a 64x64 icon.
myGLCD.fillScr(255, 255, 255);
for (int x=0; x<2; x++)
  for (int y=0; y<2; y++)
myGLCD.drawBitmap (x*64, y*64, 64, 64, tux);

delay(5000);

// Draw a 2 by 2 grid of a 32x32 icon.
myGLCD.fillScr(255, 255, 255);
for (int x=0; x<2; x++)
  for (int y=0; y<2; y++)
    myGLCD.drawBitmap (x*64, y*64, 32, 32, icon2, 2);

delay(5000);

//  //显示大图片只能在ARDUINO MEGA2560上用，Arduino UNO的FLASH空间不够存放
//  myGLCD.drawBitmap (0, 0, 128, 126, lbxxPNG);
//  delay(5000);
}

