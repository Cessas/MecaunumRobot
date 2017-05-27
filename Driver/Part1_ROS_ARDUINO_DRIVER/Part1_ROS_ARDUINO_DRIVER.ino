//@yuguantoumx@icloud.com
//This driver is based on Magnetic Encoder,attach 4 A pins to interrupt pins and others to digital pins
//encoders and motor driver sheild are not attached to same Arduino
//In this case,we use two Arduino mega 2560 to prevent exhausting the CPU
//No PID controller,So it is not particularly accurate 
//I will give Position PID controller soon
#include <ros.h>
#include <ros/time.h>
#include<geometry_msgs/Twist.h>
#include <math.h>
#include "motor4WD.h"
#define BAUD 57600
//a simple implentation for velocity fitting
float vel2PWM(float speed){
  if(speed>0) return 361.42*speed+80.593;
  else return -(361.42*abs(speed)+80.593);
}

int control_rate=50;   // [Hz]
int no_cmd_timeout=1.5; // [seconds]
uint32_t last_cmd_time;       // [milliseconds]

ros::NodeHandle_<ArduinoHardware, 10, 10, 1024, 1024> nh;
//callback function
//we assum that sending velocity can only send one feild speed to lower base,for example
//sending twist with x:1 y:1 z:0 is not allowed
void cmdVelCallback( const geometry_msgs::Twist& vel_msg) 
{ float ControlMsg[3];
  float vx=vel_msg.linear.x;
  float vy=vel_msg.linear.y;
  float vz=vel_msg.angular.z;
  if(vx!=0&&(equalZero(vy)&&equalZero(vz))){ControlMsg[0]=vel2PWM(vx);ControlMsg[1]=0;ControlMsg[2]=0;} 
  else if(vy!=0&&(equalZero(vx)&&equalZero(vz))){ControlMsg[0]=0;ControlMsg[1]=vel2PWM(vy);ControlMsg[2]=0;} 
  else if(vz!=0&&(equalZero(vy)&&equalZero(vx))){ControlMsg[0]=0;ControlMsg[1]=0;ControlMsg[2]=vel2PWM(vz);} 
  else {ControlMsg[0]=0;ControlMsg[1]=0;ControlMsg[2]=0;}
  control4WD(ControlMsg);
  last_cmd_time = millis();
}


ros::Subscriber<geometry_msgs::Twist> sub_vel("smoother_cmd_vel", cmdVelCallback);

void setup()
{ 
  nh.getHardware()->setBaud(BAUD);
  nh.initNode();
  nh.subscribe(sub_vel);

  while (!nh.connected())
  {
    nh.spinOnce();
  }
  nh.loginfo("成功连接到控制器，这个控制器用来控制电机");
  motorInit();
}
void loop() 
{
  
  // Stop motors after a period of no commands
  if((millis() - last_cmd_time) >= (no_cmd_timeout * 1000))
  {
    motor_stop();
  }
  nh.spinOnce();
}




