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

void RoboMotors::turnLeft(void)
{
  applyCustomSpeed(motorState.leftMotorPower * 0.85, motorState.rightMotorPower * 0.85);
  leftBackward();
  rightForward();
}

void RoboMotors::turnRight(void)
{
  applyCustomSpeed(motorState.leftMotorPower * 0.85, motorState.rightMotorPower * 0.85);
  leftForward();
  rightBackward();
}

void RoboMotors::steerLeft()
{
  applyCustomSpeed(_minSpeed(_currentLeftSpeed * 0.6), _maxSpeed(_currentRightSpeed * 1.5));
}

void RoboMotors::steerRight()
{
  applyCustomSpeed(_maxSpeed(_currentLeftSpeed * 1.5), _minSpeed(_currentRightSpeed * 0.6));
}

void RoboMotors::stop(void)
{
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
  _currentLeftSpeed = motorState.leftMotorPower;
  _currentRightSpeed = motorState.rightMotorPower;
}

void RoboMotors::applyCustomSpeed(int left, int right)
{
  analogWrite(_motorPins.leftPower, left);
  analogWrite(_motorPins.rightPower, right);
  _currentLeftSpeed = left;
  _currentRightSpeed = right;
}

void RoboMotors::leftFaster()
{
  motorState.leftMotorPower += motorState.motorPowerStep;
  motorState.leftMotorPower = _maxSpeed(motorState.leftMotorPower);
  applySpeed();

  Serial.print("Left motors get fast with value: ");
  Serial.println(motorState.leftMotorPower);
}

void RoboMotors::rightFaster()
{
  motorState.rightMotorPower += motorState.motorPowerStep;
  motorState.rightMotorPower = _maxSpeed(motorState.rightMotorPower);
  applySpeed();

  Serial.print("Right motors get fast with value: ");
  Serial.println(motorState.rightMotorPower);
}

void RoboMotors::leftSlower()
{
  motorState.leftMotorPower -= motorState.motorPowerStep;
  motorState.leftMotorPower = _minSpeed(motorState.leftMotorPower);
  applySpeed();

  Serial.print("Left motors get slow with value: ");
  Serial.println(motorState.leftMotorPower);
}

void RoboMotors::rightSlower()
{
  motorState.rightMotorPower -= motorState.motorPowerStep;
  motorState.rightMotorPower = _minSpeed(motorState.rightMotorPower);
  applySpeed();

  Serial.print("Right motors get slow with value: ");
  Serial.println(motorState.rightMotorPower);
}

bool RoboMotors::isMovingForward()
{
  return motorState.leftMotorState == MotorPairState::Forward
    && motorState.rightMotorState == MotorPairState::Forward;
}

bool RoboMotors::isMovingBackward()
{
  return motorState.leftMotorState == MotorPairState::Backward
    && motorState.rightMotorState == MotorPairState::Backward;
}

int RoboMotors::_minSpeed(int speed)
{
  return speed > LOWEST_SPEED ? speed : LOWEST_SPEED;
}

int RoboMotors::_maxSpeed(int speed)
{
  return speed > HIGHEST_SPEED ? HIGHEST_SPEED : speed;
}
