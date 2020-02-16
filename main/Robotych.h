#ifndef Robotych_h
#define Robotych_h

#include "Arduino.h"
#include "RoboMotors.h"
#include "RoboHead.h"
#include "RoboAction.h"

class Robotych
{
  public:
    Robotych(MotorPins, HeadPins);
    void measureTimeInterval(String);
    void trackDistance();
    RoboMotors * roboMotors;
    RoboHead * roboHead;
    RoboAction * roboAction;
  private:
    MotorPins _motorPins;
    HeadPins _headPins;
    unsigned long lastDistanceCheckTime;
    unsigned long checkDistanceTimePeriod;
    unsigned long time;
};

#endif
