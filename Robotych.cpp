#include "Robotych.h"

Robotych::Robotych(MotorPins motorPins, HeadPins headPins)
{
  currentState.actionsCount = 0;

  roboMotors = new RoboMotors(motorPins);
  roboMotors->applyMotorState();

  roboHead = new RoboHead(headPins);
}

void Robotych::startActionSequence(ActionState *actionState, int length)
{
  actionSequence = actionState;
  currentState.actionIndex = 0;
  currentState.actionsCount = length;
  roboMotors->motorState.controlState = ControlState::SelfControl;
  initCurrentAction();
  Serial.print("Actions count: ");
  Serial.println(currentState.actionsCount);
}

void Robotych::checkAndUpdateCurrentAction()
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
      roboMotors->motorState.controlState = ControlState::UserControl;
      roboMotors->motorState.leftMotorState = MotorPairState::Stop;
      roboMotors->motorState.rightMotorState = MotorPairState::Stop;
      roboMotors->applySpeed();
      roboMotors->applyMotorState();
      Serial.println("Finish action sequence");
    }
  }
}

void Robotych::initCurrentAction()
{
  currentState.actionStartTime = millis();
  currentState.currentAction = actionSequence[currentState.actionIndex];
  roboMotors->motorState.leftMotorState = currentState.currentAction.leftMotorPairState;
  roboMotors->motorState.rightMotorState = currentState.currentAction.rightMotorPairState;
  roboMotors->applyCustomSpeed(
    currentState.currentAction.leftMotorPower,
    currentState.currentAction.rightMotorPower
  );
  roboMotors->applyMotorState();

  Serial.print("New action: ");
  Serial.print("currentActionName: ");
  Serial.println(currentState.currentAction.actionName);
  Serial.print("currentState.actionIndex: ");
  Serial.println(currentState.actionIndex);
  Serial.print("currentState.currentActionDuration: ");
  Serial.println(currentState.currentAction.actionDuration);
  Serial.println("");
}
