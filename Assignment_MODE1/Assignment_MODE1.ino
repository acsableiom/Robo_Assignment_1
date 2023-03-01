

#include <Wire.h>
#include <Zumo32U4.h>

// This is the threshold for the proximity sensors
#define PRO_THRESHOLD     6    // distance from ZUMO

// motor speed settings
#define REVERSE_SPEED     100  // 0 is stopped, 400 is full speed
#define TURN_SPEED        110
#define FORWARD_SPEED     80
#define REVERSE_DURATION  100  // ms
#define TURN_DURATION     200  // ms
#define ETURN_DURATION    400 // ms
#define ETURN_SPEED       300

Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;

int input;
bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;

int turnCount = 0;

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

  input = Serial.read();  
  proxSensors.read();
  int left_sensor = proxSensors.countsLeftWithLeftLeds();
  int center_left_sensor = proxSensors.countsFrontWithLeftLeds();
  int center_right_sensor = proxSensors.countsFrontWithRightLeds();
  int right_sensor = proxSensors.countsRightWithRightLeds();  
  
  if (input == 'd')
  {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
  }
  else if (input == 'a')
  {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
  }
  else if (input == 'w')
  {
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(500);
  }
  else if (input == 's')
  {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(500);
  }
  else if (input == -1)
  {
    motors.setSpeeds(0,0);
  }
  else
  {
     Serial.print("ERROR: command not recognised \n");
  }
  

  if (left_sensor >= PRO_THRESHOLD || center_left_sensor >= PRO_THRESHOLD)
  {
    Serial.print("PERSON FOUND: ");
    //play tune
    for (int i = 0; i < 3; i++)
    {
      delay(500);
      buzzer.playNote(NOTE_G(4), 200, 15);
    }
  }
  else if (center_right_sensor >= PRO_THRESHOLD || right_sensor >= PRO_THRESHOLD)
  {
    Serial1.println("PERSON FOUND: ");
    //play tune
    for (int i = 0; i < 3; i++)
    {
      delay(500);
      buzzer.playNote(NOTE_G(4), 200, 15);
    }
  }
}

