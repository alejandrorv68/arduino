#ifndef LEG_H
#define LEG_H

// ----- Body parts ----------
#include <limb.h>
#include <throb.h>

#define DEBUG 0

class Leg : public Limb
{

private:
  Throb* pulse_pedis_;
  Throb* pulse_popliteal_;
  Throb* pulse_posterior_;

public:
  Leg( //constructor with ALL the info for the left arm
    int force_sensor_pedis,       int index_pulse_pedis,                                // PULSE
    int force_sensor_popliteal,   int index_pulse_popliteal,                            // PULSE
    int force_sensor_posterior,   int index_pulse_posterior                            // PULSE
    ): Limb (seizure_motor){    
    pulse_pedis_      =  new Throb(synch, force_sensor_pedis, index_pulse_pedis); 
    pulse_popliteal_  =  new Throb(synch, force_sensor_popliteal, index_pulse_popliteal);
    pulse_posterior_  =  new Throb(synch, force_sensor_posterior, index_pulse_posterior);
  }

  void process(){ // loop
    pulse_pedis_->process();
    pulse_popliteal_->process();
    pulse_posterior_->process();
  }
};



#endif // HEAD_H