
#include <Wire.h>
#include <Zumo32U4.h>
#include <Zumo32U4Encoders.h>

// This is the threshold for the line and proximity sensors
#define QTR_THRESHOLD     800  // microseconds
#define PRO_THRESHOLD     6    // distance from ZUMO

// motor speed settings
#define REVERSE_SPEED     100  // 0 is stopped, 400 is full speed
#define TURN_SPEED        300
#define FORWARD_SPEED     80
#define REVERSE_DURATION  100  // ms
#define TURN_DURATION     200  // ms
#define ETURN_DURATION    400 // ms
#define ETURN_SPEED       300

Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Encoders encoders;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

//user input variables
int input;

//distance travelled variables
int16_t distanceTravelled;
int16_t encoderTicks;
int16_t wheelRotations;

//proximity sensor variables
bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;

//timer variables
unsigned long timer;
unsigned long interval = 500;
unsigned long newTimer = 0;


void waitForButtonAndCountDown()
{
  ledYellow(1);


  buttonA.waitForButton();

  ledYellow(0);


  // Play Countdown notes
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}

void setup()
{

  Serial1.begin(9600);
  Serial.begin(9600);  

  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();

  waitForButtonAndCountDown();
}

void loop()
{
  if (buttonA.isPressed())
  {
    motors.setSpeeds(0, 0);
    buttonA.waitForRelease();
    waitForButtonAndCountDown();
  }
  // read serial input
  input = Serial.read(); 
  //read line sensors  
  lineSensors.read(lineSensorValues);
  //read proximity sensors
  proxSensors.read();
  int left_sensor = proxSensors.countsLeftWithLeftLeds();
  int center_left_sensor = proxSensors.countsFrontWithLeftLeds();
  int center_right_sensor = proxSensors.countsFrontWithRightLeds();
  int right_sensor = proxSensors.countsRightWithRightLeds();
  //find out distance travelled
  encoderTicks = encoders.getCountsLeft();
  wheelRotations = encoderTicks / 360;
  distanceTravelled = wheelRotations * 39;  
  
  if (lineSensorValues[0] > QTR_THRESHOLD)
    {
      // If left sensor detects line, reverse and turn to the
      // right.
      motors.setSpeeds(0,0);
      if (input == 'd')
      {
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_SPEED);
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
      }
      else if (input == 'a')
      {
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_SPEED);
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
      }  
    }
    else if (lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD)
    {
      // If right sensor detects line, reverse and turn to the
      // left.
      motors.setSpeeds(0,0);
      if (input == 'd')
      {
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_SPEED);
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
      }
      else if (input == 'a')
      {
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_SPEED);
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
      }
    }
    else if (input == 'd')
    {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_SPEED);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION);
    }
    else if (input == 'a')
    {    
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_SPEED);
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(TURN_DURATION);                   
    }
    else
    {
      // Otherwise, go straight.
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }  
  
  if (left_sensor >= PRO_THRESHOLD || center_left_sensor >= PRO_THRESHOLD)
  {
    
    motors.setSpeeds(0,0);
    Serial.println("PERSON FOUND: ");
    Serial.print(distanceTravelled);
    Serial.print("\n");
    //play tune
    for (int i = 0; i < 3; i++)
    {
      delay(500);
      buzzer.playNote(NOTE_G(4), 200, 15);
      
    }
    if (input == 'd')
    {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_SPEED);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION);
    }
    else if (input == 'a')
    {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_SPEED);
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(TURN_DURATION);
    }
  }
  else if (center_right_sensor >= PRO_THRESHOLD || right_sensor >= PRO_THRESHOLD)
  {      
    motors.setSpeeds(0,0);
    Serial.print("PERSON FOUND: ");
    Serial.print(distanceTravelled);
    Serial.print("\n");
    //play tune
    for (int i = 0; i < 3; i++)
    {
      delay(500);
      buzzer.playNote(NOTE_G(4), 200, 15);
    }
    if (input == 'd')
    {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_SPEED);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION);
    }
    else if (input == 'a')
    {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_SPEED);
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(TURN_DURATION);
    }    
  }
}
