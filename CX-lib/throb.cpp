#include "throb.h"

Throb::Throb()
{
}

Throb::Throb(Pulse* pulse, int force_sensor, int index_pulse)
{
  force_pin_ = force_sensor;
  pulse_pin_ = index_pulse;
  pulse_ = pulse;
}

Throb::Throb(Pulse* pulse, Scheduler* scheduler, Adafruit_PWMServoDriver* pwm, MuxPCA9544* mux, AdcADS1015* adc, 
    int mux_channel_force, int adc_channel_force, int index_pulse)
{
  pulse_ = pulse;
  force_pin_ = -1;  // not valid value --> input output via i2c
  if (mux &&  adc && pwm)
  {
  mux_channel_force_ = mux_channel_force;
  adc_channel_force_ = adc_channel_force;
    force_sensor_ = new i2cOldReader(FORCE_UPDATE_INTERVAL, [this](const float   v){
      force_ = v;
    }, mux, adc, mux_channel_force, adc_channel_force);

    vibrator_ = new i2cOutput(pwm, index_pulse, PULSE_UPDATE_INTERVAL);
    
    //################## scheduling ###################

    scheduler->addTask(force_sensor_);  
    scheduler->update();
    scheduler->addTask(vibrator_); 
    scheduler->update();
  }
}

Throb::~Throb()
{
}

void Throb::process(){
  if (force_pin_ >= 0){ // if pulse attached to ESP directly...
    pulse_->timeToPulse() && analogRead(force_pin_) > FORCE_THRESHOLD ? digitalWrite(pulse_pin_, HIGH) : digitalWrite(pulse_pin_, LOW);
    return;
  }
  // if with i2c...
  if (pulse_->timeToPulse() && force_ > FORCE_THRESHOLD) vibrator_->setDuty(1);
  else vibrator_->setDuty(0);
}

