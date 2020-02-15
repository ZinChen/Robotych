#ifndef RoboHead_h
#define RoboHead_h

struct HeadPins
{
  int distanceInput;
  int distanceOutput;
  int servo;
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

class RoboHead
{
  public:
    RoboHead(HeadPins);
    long measureDistance();
    long averageDistance(int);
    bool distanceLessThan(int);
    bool distanceUnknown();
    void trackDistance();
    // TODO: void measure distance and stop here

    HeadState headState;
  private:
    HeadPins _headPins;
};

#endif
