#include "Robotych.h"

Robotych::Robotych(MotorPins motorPins, HeadPins headPins)
{
  motorState.motorPowerStep = 20;
  motorState.leftMotorState = MotorPairState::Stop;
  motorState.rightMotorState = MotorPairState::Stop;
  motorState.controlState = ControlState::UserControl;
  headState.ultrasonicUnknownLimit = 2000;

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
}

// Basic movements
void Robotych::forward(void)
{
  speed(motorState.leftMotorPower, motorState.rightMotorPower);
  leftForward();
  rightForward();
}

void Robotych::back(void)
{
  speed(motorState.leftMotorPower, motorState.rightMotorPower);
  leftBack();
  rightBack();
}

void Robotych::left(void)
{
  speed(motorState.leftMotorPower * 0.75, motorState.rightMotorPower * 0.75);
  leftBack();
  rightForward();
}

void Robotych::right(void)
{
  speed(motorState.leftMotorPower * 0.75, motorState.rightMotorPower * 0.75);
  leftForward();
  rightBack();
}

void Robotych::stop(void)
{
  speed(motorState.leftMotorPower, motorState.rightMotorPower);
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

void Robotych::leftBack(void)
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

void Robotych::rightBack(void)
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

// Speed Control

void Robotych::defaultSpeed(int left, int right)
{
  motorState.leftMotorPower = left;
  motorState.rightMotorPower = right;
  speed(left, right);
}

void Robotych::speed(int left, int right)
{
  analogWrite(_motorPins.leftPower, motorState.leftMotorPower);
  analogWrite(_motorPins.rightPower, motorState.rightMotorPower);
}

void Robotych::leftFaster()
{
  motorState.leftMotorPower += motorState.motorPowerStep;
  motorState.leftMotorPower = motorState.leftMotorPower > 255 ? 255 : motorState.leftMotorPower;
  analogWrite(_motorPins.leftPower, motorState.leftMotorPower);
}

void Robotych::rightFaster()
{
  motorState.rightMotorPower += motorState.motorPowerStep;
  motorState.rightMotorPower = motorState.rightMotorPower > 255 ? 255 : motorState.rightMotorPower;
  analogWrite(_motorPins.rightPower, motorState.rightMotorPower);
}

void Robotych::leftSlower()
{
  motorState.leftMotorPower -= motorState.motorPowerStep;
  motorState.leftMotorPower = motorState.leftMotorPower > 0 ? motorState.leftMotorPower : 0;
  analogWrite(_motorPins.leftPower, motorState.leftMotorPower);
}

void Robotych::rightSlower()
{
  motorState.rightMotorPower -= motorState.motorPowerStep;
  motorState.rightMotorPower = motorState.rightMotorPower > 0 ? motorState.rightMotorPower : 0;
  analogWrite(_motorPins.rightPower, motorState.rightMotorPower);
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