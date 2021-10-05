#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];//原始文件在库文件的DefaultFonts.c中

UTFT myGLCD(YYROBOT_TFT144,A2,A1,A5,A4,A3);  // Remember to change the model parameter to suit your display module!
//YYROBOT_TFT144  屏幕型号，不用修改
//SDA----A2
//SCL----A1
//CS-----A5
//RST----A4
//RS----A3
//LED---A0  UTFT库里面设定的，如果需要修改需要修改库文件

void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD(PORTRAIT);//竖向显示
  myGLCD.setFont(SmallFont);//设置字体为SmallFont格式
}

void loop()
{
  byte buf[126];
  int x, x2;
  int y, y2;
  int r;
  
// Clear the screen and draw the frame
  myGLCD.clrScr();//清屏
  myGLCD.setColor(255, 0, 0);//设置颜色
  myGLCD.fillRect(0,0,127,12);//填充方形

  myGLCD.setColor(64, 64, 64);//设置颜色
  myGLCD.fillRect(0,117,127,127);//填充方形

  
  myGLCD.setColor(255, 255, 255);//设置画笔颜色
  myGLCD.setBackColor(255,0,0);//设置背景颜色
  myGLCD.print("Arduino Code", CENTER, 0);//写字符中间对齐，第0行

  myGLCD.setBackColor(64,64,64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("YYROBOT TFT 1.44", LEFT, 116);

  myGLCD.setColor(0,255,0);
  myGLCD.drawRect(0,13,127,116);//画方框


// Draw crosshairs
  myGLCD.setColor(0,0,255);
  myGLCD.drawLine(63,14,63,115);//画线
  myGLCD.drawLine(1,63,126,63);
  for (int i=3; i<128; i+=10)
    myGLCD.drawLine(i, 61, i, 65);
  for (int i=14; i<118; i+=10)
    myGLCD.drawLine(61, i, 65, i);

  
// Draw sin-, cos- and tan-lines  
  myGLCD.setColor(0,255,255);
  myGLCD.setBackColor(0,0,0);
  myGLCD.print("Sin", 2, 14);

  for (int i=1; i<126; i++)
  {
    myGLCD.drawPixel(i,63+(sin(((i*2.85)*3.14)/180)*45));//利用画点程序，画sin波形
  }
  
  myGLCD.setColor(255,0,0);
  myGLCD.print("Cos", 2, 26);
  for (int i=1; i<126; i++)
  {
    myGLCD.drawPixel(i,63+(cos(((i*2.85)*3.14)/180)*45));//利用画点程序，画cos波形
  }

  myGLCD.setColor(255,255,0);
  myGLCD.print("Tan", 2, 38);
  for (int i=1; i<126; i++)
  {
    myGLCD.drawPixel(i,63+(tan(((i*2.85)*3.14)/180)));//利用画点程序，画tan波形
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);
  myGLCD.setColor(0,0,255);
  myGLCD.drawLine(63,14,63,115);
  myGLCD.drawLine(1,63,126,63);

// Draw a moving sinewave
  x=1;
  for (int i=1; i<3654; i++)
  {
    x++;
    if (x==127)
      x=1;
    if (i>127)
    {
      if ((x==63)||(buf[x-1]==63))
        myGLCD.setColor(0,0,255);
      else
        myGLCD.setColor(0,0,0);
      myGLCD.drawPixel(x,buf[x-1]);
    }
    myGLCD.setColor(0,255,255);
    y=63+(sin(((i*1.3)*3.14)/180)*45);
    myGLCD.drawPixel(x,y);
    buf[x-1]=y;
//    delay(3);
  }


  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);
  
// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRect(10+(i*10),10+(i*10), 60+(i*10), 60+(i*10));
  }
  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115); 

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRoundRect(70-(i*10),10+(i*10), 120-(i*10), 60+(i*10));//填充带倒圆角的方框
  }
  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);
  
// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillCircle(30+(i*10),35+(i*10), 25);//填充圆
  }
    
  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);

  // Draw some lines in a pattern
  myGLCD.setColor (255,0,0);
  for (int i=11; i<115; i+=3)
  {
    myGLCD.drawLine(1, i, i-10, 115);//画线
  }
  myGLCD.setColor (255,0,0);
  for (int i=112; i>14; i-=3)
  {
    myGLCD.drawLine(126, i, i+14, 14);
  }
  myGLCD.setColor (0,255,255);
  for (int i=115; i>14; i-=3)
  {
    myGLCD.drawLine(1, i, 116-i, 14);
  }
  myGLCD.setColor (0,255,255);
  for (int i=14; i<115; i+=3)
  {
    myGLCD.drawLine(126, i, 140-i, 115);
  }
  
  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=22+random(85);
    y=35+random(59);
    r=random(20);
    myGLCD.drawCircle(x, y, r);//画圆
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(124);
    y=15+random(101);
    x2=2+random(124);
    y2=15+random(101);
    myGLCD.drawRect(x, y, x2, y2);//画方框
  }

  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);

// Draw some random rounded rectangles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(124);
    y=15+random(101);
    x2=2+random(124);
    y2=15+random(101);
    myGLCD.drawRoundRect(x, y, x2, y2);//画倒圆角的方框
  }
  
  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);

// Draw some random lines
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(124);
    y=15+random(101);
    x2=2+random(124);
    y2=15+random(101);
    myGLCD.drawLine(x, y, x2, y2);//画线
  }
  
  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,14,126,115);

// Draw some random pixels
  for (int i=0; i<5000; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    myGLCD.drawPixel(2+random(124), 15+random(101));
  }
  delay (2000);
  
  //Set up the "Finished"-screen
  myGLCD.setContrast(0);
  myGLCD.fillScr(0,0,255);
  myGLCD.setColor(255,0,0);
  myGLCD.fillRoundRect(2, 40, 125, 88);
  
  myGLCD.setColor(255,255,255);
  myGLCD.setBackColor(255,0,0);
  myGLCD.print("That's it!", CENTER, 46);
  myGLCD.print("Restarting in a", CENTER, 66);
  myGLCD.print("few seconds...", CENTER, 76);
  
  myGLCD.setColor(0,0,0);
  myGLCD.setBackColor(0,0,255);
  myGLCD.print("Runtime: (msecs)", CENTER, 108);
  myGLCD.printNumI(millis(), CENTER, 118);

  delay (2000);
  
}


