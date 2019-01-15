#ifndef Robotych_h
#define Robotych_h

#include "Arduino.h"

struct MotorPins
{
  int leftFront;
  int leftBack;
  int rightFront;
  int rightBack;
  int leftPower;
  int rightPower;
};

struct HeadPins
{
  int distanceInput;
  int distanceOutput;
  int servo;
};


enum class MotorPairState
{
  Stop,
  Forward,
  Backward,
};

enum class ControlState
{
  SelfControl,
  UserControl,
};

struct MotorState
{
  MotorPairState leftMotorState;
  MotorPairState rightMotorState;
  ControlState controlState;
  int leftMotorPower;
  int rightMotorPower;
  int motorPowerStep;
};


enum class UltrasonicState
{
  WaitingServo,
  MeasureInProgress,
  MeasureCompleted
};

struct HeadState
{
  int servoAngle;
  int ultrasonicUnknownLimit;
  long distance;
  UltrasonicState ultrasonicState;
};


struct CurrentMove
{
  int moveDuration;
  char moveName;
};

struct CurrentState
{
  CurrentMove currentMove;
  int moveStartTime;
  int moveIndex;
};


class Robotych
{
  public:
    Robotych(MotorPins, HeadPins);
    void forward();
    void back();
    void left();
    void right();
    void stop();
    void leftForward();
    void leftBack();
    void rightForward();
    void rightBack();
    void leftStop();
    void rightStop();
    void speed(int lpw, int rpw);
    void rightFaster();
    void leftFaster();
    void leftSlower();
    void rightSlower();
    void slower();
    bool isMovingForward();
    long measureDistance();
    long averageDistance(int);
    bool distanceLessThan(int);
    bool distanceUnknown();
    void updateCurrentMove();
    MotorState motorState;
    HeadState headState;
    CurrentState currentState;
  private:
    MotorPins _motorPins;
    HeadPins _headPins;
};

struct RobotychAction
{
  char action;
  int duration;
};

#endif
