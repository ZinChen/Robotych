#include "Robotych.h"

Robotych::Robotych(MotorPins motorPins, HeadPins headPins)
{
  roboMotors = new RoboMotors(motorPins);
  roboMotors->applyMotorState();

  roboHead = new RoboHead(headPins);

  roboAction = new RoboAction(roboMotors);

  lastDistanceCheckTime = 0;
  checkDistanceTimePeriod = 100;
}

void Robotych::trackDistance()
{
  unsigned long currentTime = millis();
  if (currentTime - lastDistanceCheckTime > checkDistanceTimePeriod)
  {
    roboHead->averageDistance(3);
    if (roboHead->distanceLessThan(40) && roboMotors->isMovingForward())
    {
      roboMotors->motorState.controlState = ControlState::SelfControl;
      roboMotors->stop();
      Serial.print("Stopped on distance: ");
      Serial.println(roboHead->headState.distance);
    }
    // measureTimeInterval("measure distance");
    // Serial.print("Current distance: ");
    // Serial.println(robotych->headState.distance);
    lastDistanceCheckTime = currentTime;
  }
}

void Robotych::measureTimeInterval(String name)
{
  unsigned long currentTime = millis();
  unsigned long interval = currentTime - time;
  time = currentTime;
  Serial.println("Timer since last" + name + ": " + interval);
}
