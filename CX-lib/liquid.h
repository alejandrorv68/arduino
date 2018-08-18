#ifndef LIQUID_H
#define LIQUID_H

#include <valve.h>
#include <scheduler.h>


class Liquid
{
private:

  bool status_open_; // T: when need to push air, F: when need to release air
  Valve* open_;

public:
  Liquid();
  Liquid(Adafruit_PWMServoDriver* pwm, int index_open);
  ~Liquid();
  void setLiquidStatusOpen(bool open);
  bool getLiquidStatusOpen();
  ATask* getTask();
};

#endif // MICROCLIENTINTERFACE_H
