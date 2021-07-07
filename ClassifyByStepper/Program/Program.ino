/***************
 * 物品分装程序
 * by zhangxinhui02
 * 2021.6.16
 * 
 * 程序参考了以下文章：
 * https://blog.csdn.net/sdlgq/article/details/50357703
 * https://blog.csdn.net/jh1513/article/details/90486397
 * 
 **************/


#include <Stepper.h>

//设置复位按钮的管脚，可自行修改
#define RESET 38
//设置光敏电阻检测的临界值（0-1024），可在测试后自行修改
#define LIGHT 512
//设置步进电机旋转一圈是多少步，可在计算后自行修改（100相当于每步3.6度）
#define STEPS 100
//设置步进电机转速（每分钟转多少步），可在计算后自行修改
#define SPEED 90
//设置电机A单次向前推动的步数（一共推6次（或者7次？）后反向旋转拉回来（如果不需要反向旋转请向下看）），可在计算后自行修改
#define ASTEP 9

//实例化7个电机，可在此修改电机管脚（5个参数的后四个对应驱动板上的IN1-IN4）
Stepper stepper1(STEPS,2,3,4,5);
Stepper stepper2(STEPS,6,7,8,9);
Stepper stepper3(STEPS,10,11,12,13);
Stepper stepper4(STEPS,22,23,24,25);
Stepper stepper5(STEPS,26,27,28,29);
Stepper stepper6(STEPS,30,31,32,33);
Stepper stepperA(STEPS,34,35,36,37);

void setup() {
  //初始化电机
  stepper1.setSpeed(SPEED);
  stepper2.setSpeed(SPEED);
  stepper3.setSpeed(SPEED);
  stepper4.setSpeed(SPEED);
  stepper5.setSpeed(SPEED);
  stepper6.setSpeed(SPEED);
  stepperA.setSpeed(SPEED);
  
  //初始化重置按钮
  pinMode(RESET,INPUT);
}

void loop() {
  //刚开始A电机可能需要转一下来往前推一下，如果不需要请删掉下面一行代码
  stepperA.step(ASTEP);
  

  //电机1
  while (analogRead(A0) > LIGHT) {
    stepper1.step(1);
  }
  delay(200);
  while (analogRead(A0) > LIGHT) {
    stepper1.step(1);
  }
  delay(200);
  stepperA.step(ASTEP);


  
  //电机2
  while (analogRead(A0) > LIGHT) {
    stepper2.step(1);
  }
  delay(200);
  while (analogRead(A0) > LIGHT) {
    stepper2.step(1);
  }
  delay(200);
  stepperA.step(ASTEP);


  
  //电机3
  while (analogRead(A0) > LIGHT) {
    stepper3.step(1);
  }
  delay(200);
  while (analogRead(A0) > LIGHT) {
    stepper3.step(1);
  }
  delay(200);
  stepperA.step(ASTEP);


  
  //电机4
  while (analogRead(A0) > LIGHT) {
    stepper4.step(1);
  }
  delay(200);
  while (analogRead(A0) > LIGHT) {
    stepper4.step(1);
  }
  delay(200);
  stepperA.step(ASTEP);


  
  //电机5
  while (analogRead(A0) > LIGHT) {
    stepper5.step(1);
  }
  delay(200);
  while (analogRead(A0) > LIGHT) {
    stepper5.step(1);
  }
  delay(200);
  stepperA.step(ASTEP);


  
  //电机6
  while (analogRead(A0) > LIGHT) {
    stepper6.step(1);
  }
  delay(200);
  while (analogRead(A0) > LIGHT) {
    stepper6.step(1);
  }
  delay(200);
  stepperA.step(ASTEP);


  //A电机反转（反转数是6或7请自行确认，此处为6），如果不需要反转请删掉下面的一行代码
  stepperA.step(- ASTEP * 6);


  //等待复位按钮按下（有的按钮在不按的时候会输出高电平，这里会反，如果这样的话请删掉下面的感叹号）
  while(!digitalRead(RESET)){
    }
}
