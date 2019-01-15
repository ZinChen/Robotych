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

long outCounter = 0;

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
  robotych->speed(130, 130);
  Serial.begin(9600);
}

// TODO:
// - add dance function

void loop()
{
  if (outCounter > 1000)
  {
    robotych->averageDistance(5);
    if (robotych->distanceLessThan(20) && robotych->isMovingForward())
    {
      robotych->motorState.controlState = ControlState::SelfControl;
      robotych->stop();
      Serial.println(robotych->headState.distance);
    }
    outCounter = 0;
  }
  outCounter++;
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