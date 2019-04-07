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
  MoveControl,
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

struct ActionState
{
  int actionDuration;
  MotorPairState leftMotorPairState;
  MotorPairState rightMotorPairState;
  int leftMotorPower;
  int rightMotorPower;
  char actionName[30];
};

struct CurrentState
{
  ActionState currentAction;
  unsigned long actionStartTime;
  int actionIndex;
  int actionsCount;
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
    void leftBackward();
    void rightForward();
    void rightBackward();
    void leftStop();
    void rightStop();
    void applyMotorState();
    void defaultSpeed(int lpw, int rpw);
    void applySpeed();
    void applyCustomSpeed(int lpw, int rpw);
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
    void checkAndUpdateCurrentAction();
    void startActionSequence(ActionState[], int);
    void initCurrentAction();
    MotorState motorState;
    HeadState headState;
    CurrentState currentState;
    ActionState *actionSequence;
  private:
    MotorPins _motorPins;
    HeadPins _headPins;
};

#endif
