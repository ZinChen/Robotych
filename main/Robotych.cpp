#include "Robotych.h"

Robotych::Robotych(MotorPins motorPins, HeadPins headPins)
{
  roboMotors = new RoboMotors(motorPins);
  roboMotors->applyMotorState();

  roboHead = new RoboHead(headPins);

  roboAction = new RoboAction(roboMotors);
}
