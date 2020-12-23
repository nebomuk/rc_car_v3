#include <msp430.h>


// use a current limiter for the motor

// Pin mapping L298 breakout board to MSP430 Launchpad

// IN1 - P2_1
// IN2 - P2_0
// IN3 - P1_4
// IN4 - P1_5

// +5V, GND, ENB and ENA are left floating on the L298 Board

// TODO use analogWrite to create pwm, see purple pins

#define STATE_LEFT 0
#define STATE_RIGHT 1
#define STATE_FORWARD 2
#define STATE_BACKWARD 3
#define STATE_OFF 4

const int IN1 = P2_1;
const int IN2 = P2_0;
const int IN3 = P1_4;
const int IN4 = P1_5;

const int MOSFET_TOP = P2_3;
// P2_4 always 3.3v, probably damaged on this MSP430G2553
const int MOSFET_BOTTOM = P2_5;

// initial states
unsigned char steeringState = STATE_OFF;
unsigned char motorState = STATE_OFF;

int analogLevel = 255;


inline void right()
{
  steeringState = STATE_RIGHT;
  digitalWrite(IN3, LOW); 
  analogWrite(IN4, 180); //min possible value 150 for steering coil
}


inline void left()
{
  steeringState = STATE_LEFT;
  analogWrite(IN3, 180); 
  digitalWrite(IN4, LOW); 
}

inline void straightOn()
{
  // does nothing if steeringState == STATE_OFF
  
if(steeringState == STATE_RIGHT)
{
  left();
  delay(20);
  steeringCoilOff();
}
else if(steeringState == STATE_LEFT)
{
  right();
  delay(20);
  steeringCoilOff();
}
}

inline void forward(bool bypassResistor)
{
  motorState = STATE_FORWARD;
  digitalWrite(IN2,LOW);
  analogWrite(IN1,analogLevel);

  if(bypassResistor)
  {
    digitalWrite(MOSFET_BOTTOM,HIGH);
    digitalWrite(MOSFET_TOP,HIGH);
  }
  else
  {
    digitalWrite(MOSFET_BOTTOM,LOW);
    digitalWrite(MOSFET_TOP,LOW);
  }
  
}

inline void backward(bool bypassResistor)
{
  motorState = STATE_BACKWARD;
  analogWrite(IN2,analogLevel);
  digitalWrite(IN1,LOW);

   if(bypassResistor)
  {

    digitalWrite(MOSFET_BOTTOM,HIGH);
    digitalWrite(MOSFET_TOP,HIGH);
  }
  else
  {
     digitalWrite(MOSFET_BOTTOM,LOW);
    digitalWrite(MOSFET_TOP,LOW);
  }
}

inline void motorOff()
{
motorState = STATE_OFF;
digitalWrite(IN2, HIGH); 
digitalWrite(IN1, HIGH); 
steeringCoilOff();  
}


inline void steeringCoilOff()
{
steeringState = STATE_OFF;
digitalWrite(IN3, HIGH); 
digitalWrite(IN4, HIGH); 
}

void setup() {                


pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT); 

pinMode(IN2, OUTPUT); 
pinMode(IN1, OUTPUT); 


pinMode(MOSFET_BOTTOM, OUTPUT); 
pinMode(MOSFET_TOP, OUTPUT); 
digitalWrite(MOSFET_BOTTOM,LOW);
digitalWrite(MOSFET_TOP,LOW);

steeringCoilOff();
motorOff();

Serial.begin(9600); 
Serial.println("Begin_rc_car_program");
}

int incomingByte = 0;


void loop() {
  
//  if(millis()%100 == 0)
//  {
//    OnTimer();
//    
//
//    analogWrite(IN1,analogLevel);
//  }
  
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();

    
    switch(incomingByte)
    {
      case 's': 
      backward(true);
      Serial.println("backward");
      break;
      
      case 'w':
      forward(true);
      Serial.println("forward with resistor");
      break;

      case 't':
      forward(false);
      Serial.println("forward");
      break;

      case 'z':
      backward(false);
      Serial.println("backward with resistor");
      break;
      
      case 'm':
      Serial.println("motorOff");
      motorOff();
      break;
      
      case 'a':
      left();
      Serial.println("left");
      break;
        
      case 'd':
      right();
      Serial.println("right");
      break;
      
      case 'n':
      {
        Serial.println("n");
        if(motorState == STATE_FORWARD)
        {
          motorOff(); // motorState = STATE_OFF
          straightOn();
          forward(false); // motorState = STATE_FORWARD
        }
        else if(motorState == STATE_BACKWARD)
        {
          motorOff(); // motorState = STATE_OFF
          straightOn();
          backward(false);  // motorState = STATE_BACKWARD
        }
        else if(motorState == STATE_OFF)
        {
          straightOn();
        }
        break;
      }
      
      case 'f':
      Serial.println("f");
      motorOff();
      break;
      
    }   
  } 
}

//void OnTimer()
//{
//  if(analogLevel < 120)
//  {
//    analogLevel++;
//  }
//}

