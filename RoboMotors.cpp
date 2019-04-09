#include "Arduino.h"
#include "RoboMotors.h"

RoboMotors::RoboMotors(MotorPins motorPins)
{
  motorState.motorPowerStep = 20;
  motorState.leftMotorState = MotorPairState::Stop;
  motorState.rightMotorState = MotorPairState::Stop;
  motorState.controlState = ControlState::UserControl;

  _motorPins = motorPins;
  pinMode(_motorPins.leftFront, OUTPUT);
  pinMode(_motorPins.leftBack, OUTPUT);
  pinMode(_motorPins.leftPower, OUTPUT);
  pinMode(_motorPins.rightFront, OUTPUT);
  pinMode(_motorPins.rightBack, OUTPUT);
  pinMode(_motorPins.rightPower, OUTPUT);
}


// Basic movements
void RoboMotors::forward(void)
{
  applySpeed();
  leftForward();
  rightForward();
}

void RoboMotors::back(void)
{
  applySpeed();
  leftBackward();
  rightBackward();
}

void RoboMotors::left(void)
{
  applyCustomSpeed(motorState.leftMotorPower * 0.75, motorState.rightMotorPower * 0.75);
  leftBackward();
  rightForward();
}

void RoboMotors::right(void)
{
  applyCustomSpeed(motorState.leftMotorPower * 0.75, motorState.rightMotorPower * 0.75);
  leftForward();
  rightBackward();
}

void RoboMotors::stop(void)
{
  // applySpeed();
  leftStop();
  rightStop();
}

// Motor Control

void RoboMotors::leftForward(void)
{
  digitalWrite(_motorPins.leftFront, HIGH);
  digitalWrite(_motorPins.leftBack, LOW);
  motorState.leftMotorState = MotorPairState::Forward;
}

void RoboMotors::leftBackward(void)
{
  digitalWrite(_motorPins.leftFront, LOW);
  digitalWrite(_motorPins.leftBack, HIGH);
  motorState.leftMotorState = MotorPairState::Backward;
}

void RoboMotors::rightForward(void)
{
  digitalWrite(_motorPins.rightFront, HIGH);
  digitalWrite(_motorPins.rightBack, LOW);
  motorState.rightMotorState = MotorPairState::Forward;
}

void RoboMotors::rightBackward(void)
{
  digitalWrite(_motorPins.rightFront, LOW);
  digitalWrite(_motorPins.rightBack, HIGH);
  motorState.rightMotorState = MotorPairState::Backward;
}

void RoboMotors::leftStop(void)
{
  digitalWrite(_motorPins.leftFront, LOW);
  digitalWrite(_motorPins.leftBack, LOW);
  motorState.leftMotorState = MotorPairState::Stop;
}

void RoboMotors::rightStop(void)
{
  digitalWrite(_motorPins.rightFront, LOW);
  digitalWrite(_motorPins.rightBack, LOW);
  motorState.rightMotorState = MotorPairState::Stop;
}

void RoboMotors::applyMotorState(void)
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

void RoboMotors::defaultSpeed(int left, int right)
{
  motorState.leftMotorPower = left;
  motorState.rightMotorPower = right;
  applySpeed();
}

void RoboMotors::applySpeed(void)
{
  analogWrite(_motorPins.leftPower, motorState.leftMotorPower);
  analogWrite(_motorPins.rightPower, motorState.rightMotorPower);
}

void RoboMotors::applyCustomSpeed(int left, int right)
{
  analogWrite(_motorPins.leftPower, left);
  analogWrite(_motorPins.rightPower, right);
}

void RoboMotors::leftFaster()
{
  motorState.leftMotorPower += motorState.motorPowerStep;
  motorState.leftMotorPower = motorState.leftMotorPower > 255 ? 255 : motorState.leftMotorPower;
  applySpeed();
}

void RoboMotors::rightFaster()
{
  motorState.rightMotorPower += motorState.motorPowerStep;
  motorState.rightMotorPower = motorState.rightMotorPower > 255 ? 255 : motorState.rightMotorPower;
  applySpeed();
}

void RoboMotors::leftSlower()
{
  motorState.leftMotorPower -= motorState.motorPowerStep;
  motorState.leftMotorPower = motorState.leftMotorPower > 0 ? motorState.leftMotorPower : 0;
  applySpeed();
}

void RoboMotors::rightSlower()
{
  motorState.rightMotorPower -= motorState.motorPowerStep;
  motorState.rightMotorPower = motorState.rightMotorPower > 0 ? motorState.rightMotorPower : 0;
  applySpeed();
}

bool RoboMotors::isMovingForward()
{
  return motorState.leftMotorState == MotorPairState::Forward
    && motorState.rightMotorState == MotorPairState::Forward;
}
