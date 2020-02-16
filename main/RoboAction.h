#ifndef roboaction_h
#define roboaction_h

#include "RoboMotors.h"
#include "ActionState.h"
// #include "RoboActionSets.h"

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
    // ActionState testAction[4] = {
    //   testActionState,
    //   testActionState2,
    //   testActionState,
    //   testActionState2
    // };
  private:
    RoboMotors * _roboMotors;
};

#endif
