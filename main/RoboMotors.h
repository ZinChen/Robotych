#ifndef RoboMotors_h
#define RoboMotors_h

struct MotorPins
{
  int leftFront;
  int leftBack;
  int rightFront;
  int rightBack;
  int leftPower;
  int rightPower;
};

enum class ControlState
{
  SelfControl,
  UserControl,
  MoveControl,
};

enum class MotorPairState
{
  Stop,
  Forward,
  Backward,
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

class RoboMotors
{
  public:
    RoboMotors(MotorPins motorPins);
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

    MotorState motorState;
  private:
    MotorPins _motorPins;
};

#endif