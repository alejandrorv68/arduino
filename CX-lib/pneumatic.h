#ifndef PNEUMATIC_H
#define PNEUMATIC_H

#include <valve.h>
#include <valveESP.h>


class Pneumatic
{
private:

  bool status_open_; // T: when need to push air, F: when need to release air
  Valve* open_;
  ValveESP* flush_;

public:
  Pneumatic();
  Pneumatic(Adafruit_PWMServoDriver* pwm, int index_open, int index_flush);
  ~Pneumatic();
  void setPneumaticStatusOpen(bool open);
  bool getPneumaticStatusOpen();
  ATask* getOpenTask();
};

#endif // MICROCLIENTINTERFACE_H
