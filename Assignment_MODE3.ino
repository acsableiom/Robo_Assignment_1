

#include <Wire.h>
#include <Zumo32U4.h>

// This is the threshold for the line and proximity sensors
#define QTR_THRESHOLD     800  // microseconds
#define PRO_THRESHOLD     6    // distance from ZUMO

// motor speed settings
#define REVERSE_SPEED     100  // 0 is stopped, 400 is full speed
#define TURN_SPEED        110
#define FORWARD_SPEED     80
#define REVERSE_DURATION  100  // ms
#define TURN_DURATION     200  // ms
#define ETURN_DURATION    400 // ms
#define ETURN_SPEED       300


Zumo32U4OLED display;

Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;

int turnCount = 0;

void waitForButtonAndCountDown()
{
  ledYellow(1);
  display.clear();
  display.print(F("Press A"));

  buttonA.waitForButton();

  ledYellow(0);
  display.clear();

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

  lineSensors.read(lineSensorValues);
  proxSensors.read();
  int left_sensor = proxSensors.countsLeftWithLeftLeds();
  int center_left_sensor = proxSensors.countsFrontWithLeftLeds();
  int center_right_sensor = proxSensors.countsFrontWithRightLeds();
  int right_sensor = proxSensors.countsRightWithRightLeds();

  if (lineSensorValues[0] > QTR_THRESHOLD)
  {
    // If left sensor detects line, reverse and turn to the
    // right.
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    turnCount ++;
  }
  else if (lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD)
  {
    // If right sensor detects line, reverse and turn to the
    // left.
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    // add one to the turn counter
    turnCount ++;
  }
  
  else
  {
    // Otherwise, go straight.
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  if (turnCount == 20)
  {
    // make emergency fast turn if stuck bouncing in corner
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-ETURN_SPEED, ETURN_SPEED);
    delay(ETURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    // resets turn counter
    turnCount = 0;
  }
  if (left_sensor >= PRO_THRESHOLD || center_left_sensor >= PRO_THRESHOLD)
  {
    motors.setSpeeds(0,0);
    Serial.print("PERSON FOUND: ");
    //play tune
    for (int i = 0; i < 3; i++)
    {
      delay(500);
      buzzer.playNote(NOTE_G(4), 200, 15);
    }
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(ETURN_SPEED, -ETURN_SPEED);
    delay(ETURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (center_right_sensor >= PRO_THRESHOLD || right_sensor >= PRO_THRESHOLD)
  {
    motors.setSpeeds(0,0);
    Serial1.println("PERSON FOUND: ");
    //play tune
    for (int i = 0; i < 3; i++)
    {
      delay(500);
      buzzer.playNote(NOTE_G(4), 200, 15);
    }
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-ETURN_SPEED, ETURN_SPEED);
    delay(ETURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
}
