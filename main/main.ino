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
  robotych->roboMotors->defaultSpeed(180, 180);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  delay(100);

  // TODO: move to Robotych.cpp as method
  if (robotych->roboAction->currentState.actionsCount > 0
    && robotych->roboMotors->motorState.controlState == ControlState::SelfControl
  ) {
    robotych->roboAction->checkAndUpdateCurrentAction();
  }

  // TODO:
  // replace with robotych->roboHead->trackDistance()
  // move trackDistance to Robotych.cpp

  unsigned long currentTime = millis();
  if (currentTime - lastDistanceCheckTime > checkDistanceTimePeriod)
  {
    robotych->roboHead->averageDistance(3);
    if (robotych->roboHead->distanceLessThan(40) && robotych->roboMotors->isMovingForward())
    {
      robotych->roboMotors->motorState.controlState = ControlState::SelfControl;
      robotych->roboMotors->stop();
      Serial.print("Stopped on distance: ");
      Serial.println(robotych->roboHead->headState.distance);
    }
    // measureTimeInterval("measure distance");
    // Serial.print("Current distance: ");
    // Serial.println(robotych->headState.distance);
    lastDistanceCheckTime = currentTime;
  }

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
      robotych->roboAction->startTestAction(); // this is should work
      // robotych->roboAction->actionSets->getTestActionSequence();
      // robotych->roboAction->startActionSequence(testsActionSequence, sizeof(testActionSequence)/sizeof(*testActionSequence));
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
