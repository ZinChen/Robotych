#ifndef Robotych_h
#define Robotych_h

#include "Arduino.h"

// TODO: add state constants

class Robotych
{
  public:
    Robotych();
    void setLBPin(int pin);
    void setLFPin(int pin);
    void setRBPin(int pin);
    void setRFPin(int pin);
    void setRPWPin(int pin);
    void setLPWPin(int pin);
    void forward();
    void back();
    void left();
    void right();
    void stop();
    void leftForward();
    void leftBack();
    void rightForward();
    void rightBack();
    void speed(int lpw, int rpw);
    void faster();
    void slower();
  private:
    int _LBPin;
    int _LFPin;
    int _RBPin;
    int _RFPin;
    int _RPWPin;
    int _LPWPin;
    int _LPWM;
    int _RPWM;
};

struct RobotychAction
{
  char action;
  int duration;
};

#endif
