//@yuguantoumx@icloud.com
//This driver is based on Magnetic Encoder,attach 4 A pins to interrupt pins and others to digital pins
//encoders and motor driver sheild are not attached to same Arduino
//In this case,we use two Arduino mega 2560 to prevent exhausting the CPU
#include <ros.h>
#include <ros/time.h>
#include <encoders_msgs/Encoders.h>
#include <math.h>
#include"encoder4WD.h"
#define BAUD 57600
encoders_msgs::Encoders encoders_msg;
int encoder_rate = 500; // [Hz]
uint32_t last_encoders_time;  // [milliseconds]
ros::NodeHandle_<ArduinoHardware, 10, 10, 1024, 1024> nh;
char frame_id[] = "base_link";

ros::Publisher pub_encoders("Encoders", &encoders_msg);

void setup()
{
  nh.getHardware()->setBaud(BAUD);
  nh.initNode();
  encoders_msg.header.frame_id = frame_id;
  nh.advertise(pub_encoders);
  EncoderInit();
  while (!nh.connected())
  {
    nh.spinOnce();
  }
  nh.loginfo(" 成功连接到控制器，这个控制器用来发布编码器数据"); 
}                         
void loop()
{
  if ((millis() - last_encoders_time) >= (1000 / encoder_rate))
  {
    encoders_msg.encoderA = read(1);
    encoders_msg.encoderB = read(2);
    encoders_msg.encoderC = read(3);
    encoders_msg.encoderD = read(4);
    encoders_msg.header.stamp = nh.now();
    pub_encoders.publish(&encoders_msg);
    last_encoders_time = millis();
  }
  nh.spinOnce();
}


