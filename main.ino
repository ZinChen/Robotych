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

ActionState testActionState = {
  500,
  MotorPairState::Forward,
  MotorPairState::Backward,
  150,
  150,
  "Test"
};

ActionState testActionState2 = {
  500,
  MotorPairState::Backward,
  MotorPairState::Forward,
  150,
  150,
  "Test2"
};

ActionState testActionSequence[4];

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
  robotych->roboMotors->defaultSpeed(130, 130);
  Serial.begin(9600);
  delay(1000);
  testActionSequence[0] = testActionState;
  testActionSequence[1] = testActionState2;
  testActionSequence[2] = testActionState;
  testActionSequence[3] = testActionState2;
}

void loop()
{
  delay(100);

  if (robotych->currentState.actionsCount > 0 && robotych->roboMotors->motorState.controlState == ControlState::SelfControl) {
    robotych->checkAndUpdateCurrentAction();
  }

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
    case 'U':
      robotych->roboMotors->forward();
      break;
    case 'D':
      robotych->roboMotors->back();
      break;
    case 'L':
      robotych->roboMotors->left();
      break;
    case 'R':
      robotych->roboMotors->right();
      break;
    case 'S':
      robotych->roboMotors->stop();
      break;
    case 'Q':
      robotych->roboMotors->leftFaster();
      break;
    case 'Z':
      robotych->roboMotors->leftSlower();
      break;
    case 'E':
      robotych->roboMotors->rightFaster();
      break;
    case 'C':
      robotych->roboMotors->rightSlower();
      break;
    case 'W':
      Serial.println("Starting sequence");
      robotych->startActionSequence(testActionSequence, sizeof(testActionSequence)/sizeof(*testActionSequence));
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
