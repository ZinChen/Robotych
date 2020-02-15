#ifndef RoboActionSets_h
#define RoboActionSets_h

#include "Robotych.h"
#include "ActionState.h"

ActionState testActionState = {
  500,
  MotorPairState::Forward,
  MotorPairState::Backward,
  150,
  150,
  "Test"
};

ActionState testActionState2 = {
  500,
  MotorPairState::Backward,
  MotorPairState::Forward,
  150,
  150,
  "Test2"
};

ActionState testActionSequence[4];


class RoboActionSets
{
  public:
    RoboActionSets();
    ActionState testActionState;
    ActionState testActionState2;
};

#endif
