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

// initial states
unsigned char steeringState = STATE_OFF;
unsigned char motorState = STATE_OFF;

inline void right()
{
  steeringState = STATE_RIGHT;
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH); 
}


inline void left()
{
  steeringState = STATE_LEFT;
  digitalWrite(IN3, HIGH); 
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

inline void forward()
{
  motorState = STATE_FORWARD;
  digitalWrite(IN2,LOW);
  digitalWrite(IN1,HIGH);
}

inline void backward()
{
  motorState = STATE_BACKWARD;
  digitalWrite(IN2,HIGH);
  digitalWrite(IN1,LOW);
}

inline void motorOff()
{
motorState = STATE_OFF;
digitalWrite(IN2, HIGH); 
digitalWrite(IN1, HIGH);   
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
      Serial.println("backward");
      break;
      
      case 'w':
      forward();
      Serial.println("forward");
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
      Serial.println("f");
      motorOff();
      break;
      
    }   
  } 
}
