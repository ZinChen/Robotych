#include "Robotych.h"

int IN1 = A1;
int IN2 = A2;
int IN3 = A3;
int IN4 = A4;
int RPW = 6;
int LPW = 5;
int USI = 7;
int USO = 8;
unsigned char serialValue;
MotorPins motorPins;
HeadPins headPins;
Robotych* robotych;

unsigned long checkDistanceTimePeriod = 100;
unsigned long lastDistanceCheckTime = 0;
unsigned long time;

void measureTimeInterval(String name)
{
  unsigned long currentTime = millis();
  unsigned long interval = currentTime - time;
  time = currentTime;
  Serial.println("Timer since last" + name + ": " + interval);
}

void setup()
{
  motorPins.rightBack = IN1;
  motorPins.rightFront = IN2;
  motorPins.leftFront = IN3;
  motorPins.leftBack = IN4;
  motorPins.rightPower = RPW;
  motorPins.leftPower = LPW;
  headPins.distanceInput = USI;
  headPins.distanceOutput = USO;
  robotych = new Robotych(motorPins, headPins);
  robotych->defaultSpeed(130, 130);
  Serial.begin(9600);
}

// TODO:
// - add dance function

void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - lastDistanceCheckTime > checkDistanceTimePeriod)
  {
    robotych->averageDistance(3);
    if (robotych->distanceLessThan(40) && robotych->isMovingForward())
    {
      robotych->motorState.controlState = ControlState::SelfControl;
      robotych->stop();
      Serial.print("Stopped on distance: ");
      Serial.println(robotych->headState.distance);
    }
    // measureTimeInterval("measure distance");
    // Serial.print("Current distance: ");
    // Serial.println(robotych->headState.distance);
    lastDistanceCheckTime = currentTime;
  }

  if (Serial.available())
  {
    serialValue = Serial.read();
    robotych->motorState.controlState = ControlState::UserControl;
    switch (serialValue)
    {
    case 'U':
      robotych->forward();
      break;
    case 'D':
      robotych->back();
      break;
    case 'L':
      robotych->left();
      break;
    case 'R':
      robotych->right();
      break;
    case 'S':
      robotych->stop();
      break;
    case 'Q':
      robotych->leftFaster();
      break;
    case 'Z':
      robotych->leftSlower();
      break;
    case 'E':
      robotych->rightFaster();
      break;
    case 'C':
      robotych->rightSlower();
      break;
    }
    // Serial.println((char) serialValue);
  }
}