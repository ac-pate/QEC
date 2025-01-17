/*
   -- Sliders_App_1 --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can Motor_1_Motor_2_ENle debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__WIFI_POINT

#include <WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "RemoteXY_Team_1"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_ACCESS_PASSWORD "123456789"


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 79 bytes
  { 255,4,0,0,0,72,0,19,0,0,0,69,83,80,56,50,54,54,0,0,
  2,106,200,200,84,1,1,4,0,4,14,50,7,86,12,255,16,87,48,12,
  26,4,11,14,9,176,109,51,89,16,176,12,26,1,255,107,57,57,40,45,
  24,24,0,12,31,0,1,14,107,57,57,69,45,24,24,0,12,31,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t slider_left; // from -100 to 100
  int8_t slider_right; // from -100 to 100
  uint8_t pushSwitch_01; // =1 if state is ON, else =0
  uint8_t pushSwitch_02; // =1 if state is ON, else =0

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////




#define Motor_1_EN  2  // D4
#define Motor_1_in1 4  // D2
#define Motor_1_in2 5  // D1


#define Motor_2_EN  14  // D5
#define Motor_2_in1 12  // D6
#define Motor_2_in2 13  // D7


#define Servo_in1 16   //D0
#define Servo_in2 15  //D8

void setup() 
{
  RemoteXY_Init (); 
  
  init_pinout();
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();

  Handler(RemoteXY.slider_right, RemoteXY.slider_left, RemoteXY.pushSwitch_01, RemoteXY.pushSwitch_02);
 
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 


}

void init_pinout() {
  // motor 1
  pinMode(Motor_1_EN,  OUTPUT);
  pinMode(Motor_1_in1, OUTPUT);
  pinMode(Motor_1_in2, OUTPUT);

  // motor 2
  pinMode(Motor_2_EN,  OUTPUT);
  pinMode(Motor_2_in1, OUTPUT);
  pinMode(Motor_2_in2, OUTPUT);
  pinMode(Servo_in1, OUTPUT);
  pinMode(Servo_in2, OUTPUT);

}


/////////////////////////////////////////////
//             The Driver                  //
/////////////////////////////////////////////

void Handler(int val2, int val1, int bnt1, int bnt2) {
  int speed1 = map(abs(val1), 0, 100, 0, 255); // Map val1 to PWM range (0-255)
  int speed2 = map(abs(val2), 0, 100, 0, 255); // Map val2 to PWM range (0-255)

  // Priority to val2 for turning
  if (val2 > 0) {  // Turn right
    forward_motor1(speed2);    // Motor 1 moves forward
    backward_motor2(speed2);   // Motor 2 moves backward
  } 
  else if (val2 < 0) {  // Turn left
    backward_motor1(speed2);   // Motor 1 moves backward
    forward_motor2(speed2);    // Motor 2 moves forward
  } 
  else {
    // If not turning, check val1 for forward/backward motion
    if (val1 > 0) {  // Move both motors forward
      forward_motor1(speed1);
      forward_motor2(speed1);
    } else if (val1 < 0) {  // Move both motors backward
      backward_motor1(speed1);
      backward_motor2(speed1);
    } else {
      // Stop motors if both val1 and val2 are 0
      stopMotors();
    }
  }
  
  if(!bnt1 && !bnt2){
    stop_servo();
  }else if(bnt1 && !bnt2){
    cw();

  }else if(!bnt1 && bnt2){
    ccw();

  }else {
    stop_servo();
  }
}


/////////////////////////////////////////////
//        Motor controller Funtions        //
/////////////////////////////////////////////
void forward_motor1(int speed) {
  analogWrite(Motor_1_EN, speed);    // Set PWM speed for motor 1
  digitalWrite(Motor_1_in1, LOW);     // Forward direction for motor 1
  digitalWrite(Motor_1_in2, HIGH);
}

void backward_motor1(int speed) {
  analogWrite(Motor_1_EN, speed);    // Set PWM speed for motor 1
  digitalWrite(Motor_1_in1, HIGH);    // Backward direction for motor 1
  digitalWrite(Motor_1_in2, LOW);
}

void forward_motor2(int speed) {
  analogWrite(Motor_2_EN, speed);    // Set PWM speed for motor 2
  digitalWrite(Motor_2_in1, LOW);    // Forward direction for motor 2
  digitalWrite(Motor_2_in2, HIGH);
}

void backward_motor2(int speed) {
  analogWrite(Motor_2_EN, speed);    // Set PWM speed for motor 2
  digitalWrite(Motor_2_in1, HIGH);   // Backward direction for motor 2
  digitalWrite(Motor_2_in2, LOW);
}

void stopMotors() {
  // Stop both motors
  digitalWrite(Motor_1_in1, LOW);
  digitalWrite(Motor_1_in2, LOW);
  analogWrite(Motor_1_EN, 0);

  digitalWrite(Motor_2_in1, LOW);
  digitalWrite(Motor_2_in2, LOW);
  analogWrite(Motor_2_EN, 0);
}

/////////////////////////////////////////////
//             Servo Funtions              //
/////////////////////////////////////////////

void lift_up() {
  
  digitalWrite(Servo_in1, HIGH);
  digitalWrite(Servo_in2, LOW);  

}

void put_down() {

  digitalWrite(Servo_in1, LOW);
  digitalWrite(Servo_in2, HIGH);  

}

void servo_stop() {
  
  digitalWrite(Servo_in1, LOW);
  digitalWrite(Servo_in2, LOW);  

}


void stop_servo() {
  digitalWrite(Servo_in1, LOW);
  digitalWrite(Servo_in2, LOW);
}
void cw() {
   // Set PWM speed for motor 2
  digitalWrite(Servo_in1, LOW);    // Forward direction for motor 2
  digitalWrite(Servo_in2, HIGH);
}

void ccw() {
    // Set PWM speed for motor 2
  digitalWrite(Servo_in1, HIGH);   // Backward direction for motor 2
  digitalWrite(Servo_in2, LOW);
}



