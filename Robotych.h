#ifndef Robotych_h
#define Robotych_h

#include "Arduino.h"
#include "RoboMotors.h"
#include "RoboHead.h"

struct ActionState
{
  int actionDuration;
  MotorPairState leftMotorPairState;
  MotorPairState rightMotorPairState;
  int leftMotorPower;
  int rightMotorPower;
  char actionName[30];
};

struct CurrentState
{
  ActionState currentAction;
  unsigned long actionStartTime;
  int actionIndex;
  int actionsCount;
};

class Robotych
{
  public:
    Robotych(MotorPins, HeadPins);
    void checkAndUpdateCurrentAction();
    void startActionSequence(ActionState[], int);
    void initCurrentAction();
    CurrentState currentState;
    ActionState * actionSequence;
    RoboMotors * roboMotors;
    RoboHead * roboHead;
  private:
    MotorPins _motorPins;
    HeadPins _headPins;
};

#endif
