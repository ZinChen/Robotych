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
  robotych->roboMotors->defaultSpeed(180, 180);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
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
      robotych->roboMotors->forward();
      break;
    case 'd':
      robotych->roboMotors->back();
      break;
    case 'l':
      robotych->roboMotors->left();
      break;
    case 'r':
      robotych->roboMotors->right();
      break;
    case 's':
      robotych->roboMotors->stop();
      break;
    case 'q':
      robotych->roboMotors->leftFaster();
      break;
    case 'z':
      robotych->roboMotors->leftSlower();
      break;
    case 'e':
      robotych->roboMotors->rightFaster();
      break;
    case 'c':
      robotych->roboMotors->rightSlower();
      break;
    case 'w':
      Serial.println("Starting sequence");
      robotych->roboAction->startAction("test");
      isUserControl = false;
      break;
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
