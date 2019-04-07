#include "Robotych.h"

Robotych::Robotych(MotorPins motorPins, HeadPins headPins)
{
  motorState.motorPowerStep = 20;
  motorState.leftMotorState = MotorPairState::Stop;
  motorState.rightMotorState = MotorPairState::Stop;
  motorState.controlState = ControlState::UserControl;
  headState.ultrasonicUnknownLimit = 2000;
  currentState.actionsCount = 0;

  _motorPins = motorPins;
  pinMode(_motorPins.leftFront, OUTPUT);
  pinMode(_motorPins.leftBack, OUTPUT);
  pinMode(_motorPins.leftPower, OUTPUT);
  pinMode(_motorPins.rightFront, OUTPUT);
  pinMode(_motorPins.rightBack, OUTPUT);
  pinMode(_motorPins.rightPower, OUTPUT);
  pinMode(_headPins.distanceInput, INPUT);
  pinMode(_headPins.distanceOutput, OUTPUT);

  _headPins = headPins;

  applyMotorState();
}

// Basic movements
void Robotych::forward(void)
{
  applySpeed();
  leftForward();
  rightForward();
}

void Robotych::back(void)
{
  applySpeed();
  leftBackward();
  rightBackward();
}

void Robotych::left(void)
{
  applyCustomSpeed(motorState.leftMotorPower * 0.75, motorState.rightMotorPower * 0.75);
  leftBackward();
  rightForward();
}

void Robotych::right(void)
{
  applyCustomSpeed(motorState.leftMotorPower * 0.75, motorState.rightMotorPower * 0.75);
  leftForward();
  rightBackward();
}

void Robotych::stop(void)
{
  // applySpeed();
  leftStop();
  rightStop();
}

// Motor Control

void Robotych::leftForward(void)
{
  digitalWrite(_motorPins.leftFront, HIGH);
  digitalWrite(_motorPins.leftBack, LOW);
  motorState.leftMotorState = MotorPairState::Forward;
}

void Robotych::leftBackward(void)
{
  digitalWrite(_motorPins.leftFront, LOW);
  digitalWrite(_motorPins.leftBack, HIGH);
  motorState.leftMotorState = MotorPairState::Backward;
}

void Robotych::rightForward(void)
{
  digitalWrite(_motorPins.rightFront, HIGH);
  digitalWrite(_motorPins.rightBack, LOW);
  motorState.rightMotorState = MotorPairState::Forward;
}

void Robotych::rightBackward(void)
{
  digitalWrite(_motorPins.rightFront, LOW);
  digitalWrite(_motorPins.rightBack, HIGH);
  motorState.rightMotorState = MotorPairState::Backward;
}

void Robotych::leftStop(void)
{
  digitalWrite(_motorPins.leftFront, LOW);
  digitalWrite(_motorPins.leftBack, LOW);
  motorState.leftMotorState = MotorPairState::Stop;
}

void Robotych::rightStop(void)
{
  digitalWrite(_motorPins.rightFront, LOW);
  digitalWrite(_motorPins.rightBack, LOW);
  motorState.rightMotorState = MotorPairState::Stop;
}

void Robotych::applyMotorState(void)
{
  switch (motorState.rightMotorState)
  {
    case MotorPairState::Forward:
      rightForward();
      break;
    case MotorPairState::Backward:
      rightBackward();
      break;
    case MotorPairState::Stop:
      rightStop();
      break;
  }

  switch (motorState.leftMotorState)
  {
    case MotorPairState::Forward:
      leftForward();
      break;
    case MotorPairState::Backward:
      leftBackward();
      break;
    case MotorPairState::Stop:
      leftStop();
      break;
  }
}

// Speed Control

void Robotych::defaultSpeed(int left, int right)
{
  motorState.leftMotorPower = left;
  motorState.rightMotorPower = right;
  applySpeed();
}

void Robotych::applySpeed(void)
{
  analogWrite(_motorPins.leftPower, motorState.leftMotorPower);
  analogWrite(_motorPins.rightPower, motorState.rightMotorPower);
}

void Robotych::applyCustomSpeed(int left, int right)
{
  analogWrite(_motorPins.leftPower, left);
  analogWrite(_motorPins.rightPower, right);
}

void Robotych::leftFaster()
{
  motorState.leftMotorPower += motorState.motorPowerStep;
  motorState.leftMotorPower = motorState.leftMotorPower > 255 ? 255 : motorState.leftMotorPower;
  applySpeed();
}

void Robotych::rightFaster()
{
  motorState.rightMotorPower += motorState.motorPowerStep;
  motorState.rightMotorPower = motorState.rightMotorPower > 255 ? 255 : motorState.rightMotorPower;
  applySpeed();
}

void Robotych::leftSlower()
{
  motorState.leftMotorPower -= motorState.motorPowerStep;
  motorState.leftMotorPower = motorState.leftMotorPower > 0 ? motorState.leftMotorPower : 0;
  applySpeed();
}

void Robotych::rightSlower()
{
  motorState.rightMotorPower -= motorState.motorPowerStep;
  motorState.rightMotorPower = motorState.rightMotorPower > 0 ? motorState.rightMotorPower : 0;
  applySpeed();
}

/*
	* Ultrasonic sensor SR04, four connections pins
	* VCC, ECHO, TRIGGER, GND
	* <br>
	* distanceInput - digital INPUT-Pin for measuring distance
	* distanceOutput - trigger signal is generated from SR04
*/
long Robotych::measureDistance()
{
  digitalWrite(_headPins.distanceOutput, LOW);
  delayMicroseconds(2);
  digitalWrite(_headPins.distanceOutput, HIGH);
  delayMicroseconds(10);
  digitalWrite(_headPins.distanceOutput, LOW);
  delayMicroseconds(2);
  long duration = pulseIn(_headPins.distanceInput, HIGH, 20000L);
  headState.distance = duration / 58.82;
  return headState.distance;
}

long Robotych::averageDistance(int times)
{
  int result = 0;
  for (int i = 0; i < times; i++)
  {
    measureDistance();
    if (!distanceUnknown()) {
      result += headState.distance;
    }
  }

  headState.distance = result / times;

  if (headState.distance == 0)
  {
    headState.distance = headState.ultrasonicUnknownLimit + 1;
  }

  return headState.distance;
}

bool Robotych::distanceLessThan(int distance)
{
  return headState.distance < distance;
}

bool Robotych::distanceUnknown()
{
  return headState.distance > headState.ultrasonicUnknownLimit;
}

bool Robotych::isMovingForward()
{
  return motorState.leftMotorState == MotorPairState::Forward
    && motorState.rightMotorState == MotorPairState::Forward;
}

void Robotych::startActionSequence(ActionState *actionState, int length)
{
  actionSequence = actionState;
  currentState.actionIndex = 0;
  currentState.actionsCount = length;
  motorState.controlState = ControlState::SelfControl;
  initCurrentAction();
  Serial.print("Actions count: ");
  Serial.println(currentState.actionsCount);
}

void Robotych::checkAndUpdateCurrentAction()
{
  unsigned long passedTime = millis() - currentState.actionStartTime;
  unsigned long duration = currentState.currentAction.actionDuration;

  if (passedTime >= duration) {
    if (currentState.actionIndex + 1 < currentState.actionsCount) {
      currentState.actionIndex++;
      initCurrentAction();
    } else {
      currentState.actionIndex = -1;
      currentState.actionStartTime = -1;
      currentState.actionsCount = 0;
      motorState.controlState = ControlState::UserControl;
      motorState.leftMotorState = MotorPairState::Stop;
      motorState.rightMotorState = MotorPairState::Stop;
      applySpeed();
      applyMotorState();
      Serial.println("Finish action sequence");
    }
  }
}

void Robotych::initCurrentAction()
{
  currentState.actionStartTime = millis();
  currentState.currentAction = actionSequence[currentState.actionIndex];
  motorState.leftMotorState = currentState.currentAction.leftMotorPairState;
  motorState.rightMotorState = currentState.currentAction.rightMotorPairState;
  applyCustomSpeed(currentState.currentAction.leftMotorPower, currentState.currentAction.rightMotorPower);
  applyMotorState();

  Serial.print("New action: ");
  Serial.print("currentActionName: ");
  Serial.println(currentState.currentAction.actionName);
  Serial.print("currentState.actionIndex: ");
  Serial.println(currentState.actionIndex);
  Serial.print("currentState.currentActionDuration: ");
  Serial.println(currentState.currentAction.actionDuration);
  Serial.println("");
}
