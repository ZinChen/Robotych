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
Robotych * robotych;

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
  robotych->roboMotors->defaultSpeed(150, 150);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  // TODO:
  //  - add optical sensor, think about merging Ultrasonic and Optical sensor results
  //  - if is moving, then left and right increases speed of related wheelpair +
  //  - add scanning as Guyver fan, if there is no free way in range, turn on 90 degrees approximately

  // think about use of black line detectors, if it's founded, then use it
  // add button switch to state for customize speed of each movement +

  delay(100);

  robotych->roboAction->checkAndUpdateCurrentAction();

  robotych->trackDistance();

  if (Serial.available())
  {
    serialValue = Serial.read();
    bool isUserControl = true;
    switch (serialValue)
    {
    case 'u':
      if (robotych->roboMotors->isSpeedControlState()) {
        robotych->roboMotors->leftFaster();
        robotych->roboMotors->rightFaster();
        isUserControl = false;
      } else {
        robotych->roboMotors->forward();
      }
      break;
    case 'd':
      if (robotych->roboMotors->isSpeedControlState()) {
        robotych->roboMotors->leftSlower();
        robotych->roboMotors->rightSlower();
        isUserControl = false;
      } else {
        robotych->roboMotors->back();
      }
      break;
    case 'l':
      if (robotych->roboMotors->isSpeedControlState()) {
        robotych->roboMotors->leftFaster();
        isUserControl = false;
      } else if ( robotych->roboMotors->isMovingForward() || robotych->roboMotors->isMovingBackward()) {
        robotych->roboMotors->steerLeft();
      } else {
        robotych->roboMotors->turnLeft();
      }
      break;
    case 'r':
      if (robotych->roboMotors->isSpeedControlState()) {
        robotych->roboMotors->rightFaster();
        isUserControl = false;
      } else if (robotych->roboMotors->isMovingForward() || robotych->roboMotors->isMovingBackward()) {
        robotych->roboMotors->steerRight();
      } else {
       robotych->roboMotors->turnRight();
      }
      break;
    case 's':
      robotych->roboMotors->stop();
      break;
    case 'w':
      Serial.println("Starting sequence");
      robotych->roboAction->startAction("test");
      isUserControl = false;
      break;
    case 'p':
      robotych->roboMotors->toggleSpeedControlState();
      isUserControl = false;
    default:
      isUserControl = false;
      break;
    }
    if (isUserControl) {
      robotych->roboMotors->motorState.controlState = ControlState::UserControl;
    }
    Serial.println((char) serialValue);
  }
}
