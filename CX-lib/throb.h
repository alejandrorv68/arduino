#ifndef THROB_H
#define THROB_H

#include <Adafruit_PWMServoDriver.h>
#include <adcADS1015.h>
#include <muxPCA9544.h>
#include <i2c-old-reader.h>
#include <i2c-output.h>
#include <pulse.h>

#define MILLISEC_IN_MIN 60000
#define PULSE_PERIOD_MS 10 // in milliseconds  
#define FORCE_UPDATE_INTERVAL 200 
#define PULSE_UPDATE_INTERVAL 15
#define FORCE_THRESHOLD 500

class Throb 
{
private:
  i2cOldReader* force_sensor_;                  // force sensor reader
  i2cOutput* vibrator_;                         // vibrator for pulses
  float force_;
  int force_pin_;
  int pulse_pin_;
  Pulse* pulse_;
  int mux_channel_force_;
  int adc_channel_force_;

  inline float rest(float a, float b){ int c = a/b; return a - (b*c);}

public:
  Throb();
  Throb(Pulse* pulse, int force_sensor, int index_pulse);
  Throb(Pulse* pulse, Scheduler* scheduler, Adafruit_PWMServoDriver* pwm, MuxPCA9544* mux, AdcADS1015* adc, 
    int mux_channel_force, int adc_channel_force, int index_pulse);
  ~Throb();
  void  process();
};

#endif // THROB_H
