#include "Arduino.h"
#include "RoboHead.h"

RoboHead::RoboHead(HeadPins headPins)
{
  _headPins = headPins;
  pinMode(_headPins.distanceInput, INPUT);
  pinMode(_headPins.distanceOutput, OUTPUT);

  headState.ultrasonicUnknownLimit = 2000;

}

/*
	* Ultrasonic sensor SR04, four connections pins
	* VCC, ECHO, TRIGGER, GND
	* <br>
	* distanceInput - digital INPUT-Pin for measuring distance
	* distanceOutput - trigger signal is generated from SR04
*/
long RoboHead::measureDistance()
{
  digitalWrite(_headPins.distanceOutput, LOW);
  delayMicroseconds(2);
  digitalWrite(_headPins.distanceOutput, HIGH);
  delayMicroseconds(10);
  digitalWrite(_headPins.distanceOutput, LOW);
  delayMicroseconds(2);
  long duration = pulseIn(_headPins.distanceInput, HIGH, 20000L);
  headState.distance = duration / 58.82;
  return headState.distance;
}

long RoboHead::averageDistance(int times)
{
  int result = 0;
  for (int i = 0; i < times; i++)
  {
    measureDistance();
    if (!distanceUnknown()) {
      result += headState.distance;
    }
  }

  headState.distance = result / times;

  if (headState.distance == 0)
  {
    headState.distance = headState.ultrasonicUnknownLimit + 1;
  }

  return headState.distance;
}

bool RoboHead::distanceLessThan(int distance)
{
  return headState.distance < distance;
}

bool RoboHead::distanceUnknown()
{
  return headState.distance > headState.ultrasonicUnknownLimit;
}