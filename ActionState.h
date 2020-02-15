#ifndef actionstate_h
#define actionstate_h

#include "RoboMotors.h"

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

#endif
