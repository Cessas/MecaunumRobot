//@yuguantoumx@icloud.com
//This driver is based on Magnetic Encoder,attach 4 A pins to interrupt pins and others to digital pins
//encoders and motor driver sheild are not attached to same Arduino
//In this case,we use two Arduino mega 2560 to prevent exhausting the CPU

const byte encoder1pinA = 18;                              //A pin -> the interrupt pin 18
const byte encoder1pinB = 22;                              //B pin -> the digital pin 21
const byte encoder2pinA = 19;                              //A pin -> the interrupt pin 19
const byte encoder2pinB = 23;                              //B pin -> the digital pin 22
const byte encoder3pinA = 20;                              //A pin -> the interrupt pin 20
const byte encoder3pinB = 24;                              //B pin -> the digital pin 23
const byte encoder4pinA = 21;
const byte encoder4pinB = 25;
 long duration1=0;                                  //the number of the pulses of Moter1 in the interval
 long duration2=0;                                  //the number of the pulses of Moter2 in the interval
 long duration3=0;                                  //the number of the pulses of Moter3 in the interval
 long duration4=0;

void wheelSpeed1()  //motor1 speed count
{
  if (digitalRead(encoder1pinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder1pinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      duration1 = duration1 + 1;         // CCW
    } 
    else {
      duration1 = duration1 - 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder1pinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      duration1 = duration1 - 1;          // CW
    } 
    else {
      duration1 = duration1 + 1;          // CCW
    }

  }
}

void wheelSpeed2()  //motor2 speed count
{
  if (digitalRead(encoder2pinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder2pinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      duration2 = duration2 - 1;         // CCW
    } 
    else {
      duration2 = duration2 +1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder2pinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      duration2 = duration2 + 1;          // CW
    } 
    else {
      duration2 = duration2 - 1;          // CCW
    }

  }
}

void wheelSpeed3()  //motor3 speed count
{
  if (digitalRead(encoder3pinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder3pinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      duration3 = duration3 +1;         // CCW
    } 
    else {
      duration3 = duration3 -1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder3pinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      duration3 = duration3 - 1;          // CW
    } 
    else {
      duration3 = duration3 + 1;          // CCW
    }

  }
}

void wheelSpeed4()  //motor4 speed count
{
  if (digitalRead(encoder4pinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder4pinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      duration4 = duration4 - 1;         // CCW
    } 
    else {
      duration4 = duration4 + 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder4pinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      duration4 = duration4 + 1;          // CW
    } 
    else {
      duration4 = duration4 - 1;          // CCW
    }

  }
}

void EncoderInit() //Initialize encoder interruption
{
  pinMode(encoder1pinA,INPUT);
  digitalWrite(encoder1pinA,HIGH);
  pinMode(encoder2pinA,INPUT);
  digitalWrite(encoder2pinA,HIGH);
  pinMode(encoder3pinA,INPUT);
  digitalWrite(encoder3pinA,HIGH);
  pinMode(encoder4pinA,INPUT);
  digitalWrite(encoder4pinA,HIGH);
  pinMode(encoder1pinB,INPUT);
  digitalWrite(encoder1pinB,HIGH); 
  pinMode(encoder2pinB,INPUT);
  digitalWrite(encoder2pinB,HIGH);
  pinMode(encoder3pinB,INPUT);
  digitalWrite(encoder3pinB,HIGH);
  pinMode(encoder4pinB,INPUT);
  digitalWrite(encoder4pinB,HIGH);
   attachInterrupt(5, wheelSpeed1, CHANGE);
   attachInterrupt(4, wheelSpeed2, CHANGE);
   attachInterrupt(3, wheelSpeed3, CHANGE);
   attachInterrupt(2, wheelSpeed4, CHANGE);
}
 long read(int a){
if(a==1) return duration1;
else if(a==2) return duration2;
else if(a==3) return duration3;
else if(a==4) return duration4;
else ; 
 }

