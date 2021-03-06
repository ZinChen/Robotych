#include "Arduino.h"
#include "RoboAction.h"
#include "RoboActionSets.h"

RoboAction::RoboAction(RoboMotors * roboMotors)
{
  _roboMotors = roboMotors;

  currentState.actionsCount = 0;
}

void RoboAction::startActionSequence(ActionState * actionState, int length)
{
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
  if (currentState.actionsCount <= 0
    || _roboMotors->motorState.controlState != ControlState::SelfControl
  ) {
    return;
  }

  unsigned long passedTime = millis() - currentState.actionStartTime;
  unsigned long duration = currentState.currentAction.actionDuration;

  Serial.print("Action passed time: ");
  Serial.println(passedTime);

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

  Serial.println("New action: ");
  Serial.println("actionSequence: ");
  Serial.println(currentState.actionIndex);
  Serial.print("currentActionName: ");
  Serial.println(currentState.currentAction.actionName);
  Serial.print("currentState.actionIndex: ");
  Serial.println(currentState.actionIndex);
  Serial.print("currentState.currentActionDuration: ");
  Serial.println(currentState.currentAction.actionDuration);
  Serial.println("");
}

void RoboAction::startAction(const char * sequenceName)
{
  int size;
  if (sequenceName == "test") {
    size = sizeof(actionStateSequence)/sizeof(*actionStateSequence);
    startActionSequence(actionStateSequence, size);
  }
}
