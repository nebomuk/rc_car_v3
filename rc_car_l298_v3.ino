// use a current limiter for the motor

// Pin mapping L298 breakout board to MSP430 Launchpad

// IN1 - P2_1
// IN2 - P2_0
// IN3 - P1_4
// IN4 - P1_5

// +5V, GND, ENB and ENA are left floating on the L298 Board

#define STATE_LEFT 0
#define STATE_RIGHT 1
#define STATE_FORWARD 2
#define STATE_BACKWARD 3
#define STATE_OFF 4

// initial states
unsigned char steeringState = STATE_OFF;
unsigned char motorState = STATE_OFF;

inline void right()
{
  steeringState = STATE_RIGHT;
  digitalWrite(P1_4, LOW); 
  digitalWrite(P1_5, HIGH); 
}


inline void left()
{
  steeringState = STATE_LEFT;
  digitalWrite(P1_4, HIGH); 
  digitalWrite(P1_5, LOW); 
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

inline void forward()
{
  motorState = STATE_FORWARD;
  digitalWrite(P2_0,LOW);
  digitalWrite(P2_1,HIGH);
}

inline void backward()
{
  motorState = STATE_BACKWARD;
  digitalWrite(P2_0,HIGH);
  digitalWrite(P2_1,LOW);
}

inline void motorOff()
{
motorState = STATE_OFF;
digitalWrite(P2_0, HIGH); 
digitalWrite(P2_1, HIGH);   
}


inline void steeringCoilOff()
{
steeringState = STATE_OFF;
digitalWrite(P1_4, HIGH); 
digitalWrite(P1_5, HIGH); 
}

void setup() {                


pinMode(P1_4, OUTPUT);
pinMode(P1_5, OUTPUT); 

pinMode(P2_0, OUTPUT); 
pinMode(P2_1, OUTPUT); 




Serial.begin(9600); 
Serial.println("Begin_rc_car_program");
}

int incomingByte = 0;


void loop() {
  
  
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();

    
    switch(incomingByte)
    {
      case 's': 
      backward();
      break;
      
      case 'w':
      forward();
      break;
      
      case 'm':
      motorOff();
      break;
      
      case 'a':
      left();
      break;
        
      case 'd':
      right();
      break;
      
      case 'n':
      {
        if(motorState == STATE_FORWARD)
        {
          motorOff(); // motorState = STATE_OFF
          straightOn();
          forward(); // motorState = STATE_FORWARD
        }
        else if(motorState == STATE_BACKWARD)
        {
          motorOff(); // motorState = STATE_OFF
          straightOn();
          backward();  // motorState = STATE_BACKWARD
        }
        else if(motorState == STATE_OFF)
        {
          straightOn();
        }
        break;
      }
      
      case 'f':
      motorOff();
      break;
      
    }   
  } 
}
