#include "Robotych.h"

int IN1 = A1;
int IN2 = A2;
int IN3 = A3;
int IN4 = A4;
int RPW = 6;
int LPW = 5;
unsigned char serial_input_value;

Robotych robotych;

void setup()
{
  robotych.setRBPin(IN1);
  robotych.setRFPin(IN2);
  robotych.setLFPin(IN3);
  robotych.setLBPin(IN4);
  robotych.setRPWPin(RPW);
  robotych.setLPWPin(LPW);
  robotych.speed(130, 130);
  Serial.begin(9600);
}

// TODO:
// - create states for robotych
// - add checker function - if too close(or sensor value is too high) - stop
// - add dance function

void loop()
{
  if (Serial.available())
  {
    serial_input_value = Serial.read();
    switch (serial_input_value)
    {
    case 'U':
      robotych.forward();
      break;
    case 'D':
      robotych.back();
      break;
    case 'L':
      robotych.left();
      break;
    case 'R':
      robotych.right();
      break;
    case 'S':
      robotych.stop();
      break;
    case 'F':
      robotych.faster();
      break;
    case 'Z':
      robotych.slower();
      break;
    case 'A':
      robotych.leftForward();
      break;
    case 'B':
      robotych.leftBack();
      break;
    case 'C':
      robotych.rightForward();
      break;
    case 'V':
      robotych.rightBack();
      break;
    }
    Serial.println((char) serial_input_value);
  }
}