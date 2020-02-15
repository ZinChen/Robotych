#ifndef roboaction_h
#define roboaction_h

#include "RoboMotors.h"
#include "ActionState.h"
#include "RoboActionSets.h"

class RoboAction
{
  public:
    RoboAction(RoboMotors *);
    void checkAndUpdateCurrentAction();
    void startActionSequence(ActionState[], int);
    void initCurrentAction();
    void startTestAction();
    CurrentState currentState;
    ActionState * actionSequence;
    RoboActionSets * actionSets;
  private:
    RoboMotors * _roboMotors;
};

#endif
