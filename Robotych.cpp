#include "Arduino.h"
#include "Robotych.h"

Robotych::Robotych(void)
{
  // nothing to do
}

// Setting pins
void Robotych::setLFPin(int pin)
{
  pinMode(pin, OUTPUT);
  _LFPin = pin;
}

void Robotych::setLBPin(int pin)
{
  pinMode(pin, OUTPUT);
  _LBPin = pin;
}

void Robotych::setRFPin(int pin)
{
  pinMode(pin, OUTPUT);
  _RFPin = pin;
}

void Robotych::setRBPin(int pin)
{
  pinMode(pin, OUTPUT);
  _RBPin = pin;
}

void Robotych::setRPWPin(int pin)
{
  pinMode(pin, OUTPUT);
  _RPWPin = pin;
}

void Robotych::setLPWPin(int pin)
{
  pinMode(pin, OUTPUT);
  _LPWPin = pin;
}

// Basic movements
void Robotych::forward(void)
{
  digitalWrite(_LFPin, HIGH);
  digitalWrite(_LBPin, LOW);
  digitalWrite(_RFPin, HIGH);
  digitalWrite(_RBPin, LOW);
}

void Robotych::back(void)
{
  digitalWrite(_LFPin, LOW);
  digitalWrite(_LBPin, HIGH);
  digitalWrite(_RFPin, LOW);
  digitalWrite(_RBPin, HIGH);
}

void Robotych::left(void)
{
  digitalWrite(_LFPin, LOW);
  digitalWrite(_LBPin, HIGH);
  digitalWrite(_RFPin, HIGH);
  digitalWrite(_RBPin, LOW);
}

void Robotych::right(void)
{
  digitalWrite(_LFPin, HIGH);
  digitalWrite(_LBPin, LOW);
  digitalWrite(_RFPin, LOW);
  digitalWrite(_RBPin, HIGH);
}

void Robotych::stop(void)
{
  digitalWrite(_LFPin, LOW);
  digitalWrite(_LBPin, LOW);
  digitalWrite(_RFPin, LOW);
  digitalWrite(_RBPin, LOW);
}

void Robotych::leftForward(void)
{
  digitalWrite(_LFPin, HIGH);
}

void Robotych::leftBack(void)
{
  digitalWrite(_LBPin, HIGH);
}

void Robotych::rightForward(void)
{
  digitalWrite(_RFPin, HIGH);
}

void Robotych::rightBack(void)
{
  digitalWrite(_RBPin, HIGH);
}

void Robotych::speed(int left, int right)
{
  _LPWM = left;
  _RPWM = right;
  analogWrite(_LPWPin, _LPWM);
  analogWrite(_RPWPin, _RPWM);
}

void Robotych::faster()
{
  _LPWM += 30;
  _RPWM += 30;
  analogWrite(_LPWPin, _LPWM);
  analogWrite(_RPWPin, _RPWM);
}

void Robotych::slower()
{
  _LPWM -= 30;
  _RPWM -= 30;
  analogWrite(_LPWPin, _LPWM);
  analogWrite(_RPWPin, _RPWM);
}

