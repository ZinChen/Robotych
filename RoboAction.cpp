#include "Arduino.h"
#include "RoboAction.h"

RoboAction::RoboAction(RoboMotors * roboMotors)
{
  _roboMotors = roboMotors;

  currentState.actionsCount = 0;
  actionSets = new RoboActionSets();
}

void RoboAction::startActionSequence(ActionState * actionState, int length)
{
  delete [] actionSequence;
  actionSequence = actionState;
  currentState.actionIndex = 0;
  currentState.actionsCount = length;
  _roboMotors->motorState.controlState = ControlState::SelfControl;
  initCurrentAction();
  Serial.print("Actions count: ");
  Serial.println(currentState.actionsCount);
}

void RoboAction::checkAndUpdateCurrentAction()
{
  unsigned long passedTime = millis() - currentState.actionStartTime;
  unsigned long duration = currentState.currentAction.actionDuration;

  if (passedTime >= duration) {
    if (currentState.actionIndex + 1 < currentState.actionsCount) {
      currentState.actionIndex++;
      initCurrentAction();
    } else {
      currentState.actionIndex = -1;
      currentState.actionStartTime = -1;
      currentState.actionsCount = 0;
      _roboMotors->motorState.controlState = ControlState::UserControl;
      _roboMotors->motorState.leftMotorState = MotorPairState::Stop;
      _roboMotors->motorState.rightMotorState = MotorPairState::Stop;
      _roboMotors->applySpeed();
      _roboMotors->applyMotorState();
      Serial.println("Finish action sequence");
    }
  }
}

void RoboAction::initCurrentAction()
{
  currentState.actionStartTime = millis();
  currentState.currentAction = actionSequence[currentState.actionIndex];
  _roboMotors->motorState.leftMotorState = currentState.currentAction.leftMotorPairState;
  _roboMotors->motorState.rightMotorState = currentState.currentAction.rightMotorPairState;
  _roboMotors->applyCustomSpeed(
    currentState.currentAction.leftMotorPower,
    currentState.currentAction.rightMotorPower
  );
  _roboMotors->applyMotorState();

  Serial.print("New action: ");
  Serial.print("currentActionName: ");
  Serial.println(currentState.currentAction.actionName);
  Serial.print("currentState.actionIndex: ");
  Serial.println(currentState.actionIndex);
  Serial.print("currentState.currentActionDuration: ");
  Serial.println(currentState.currentAction.actionDuration);
  Serial.println("");
}

void RoboAction::startTestAction()
{
  // actionSequence =
  // erase and create array
  // pass it here and its length
  // startActionSequence
  int size = 4;
  ActionState actionState[size];
  actionState[0] = actionSets->testActionState;
  actionState[1] = actionSets->testActionState2;
  actionState[2] = actionSets->testActionState;
  actionState[3] = actionSets->testActionState2;
  startActionSequence(actionState, size);
}
