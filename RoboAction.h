#ifndef RoboAction_h
#define RoboAction_h

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


class RoboAction
{
  public:
    RoboAction(RoboMotors *);
    void checkAndUpdateCurrentAction();
    void startActionSequence(ActionState[], int);
    void initCurrentAction();
    CurrentState currentState;
    ActionState * actionSequence;
  private:
    RoboMotors * _roboMotors;
};

#endif
