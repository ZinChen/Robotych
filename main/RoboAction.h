#ifndef roboaction_h
#define roboaction_h

#include "RoboMotors.h"
#include "ActionState.h"

class RoboAction
{
  public:
    RoboAction(RoboMotors *);
    void checkAndUpdateCurrentAction();
    void startActionSequence(ActionState[], int);
    void initCurrentAction();
    void startAction(const char *);
    CurrentState currentState;
    ActionState * actionSequence;
  private:
    RoboMotors * _roboMotors;
};

#endif
