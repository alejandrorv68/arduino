#ifndef ARMR_H
#define ARMR_H

// ----- Body parts ----------
#include <limb.h>
#include <throb.h>
#include <PN532_I2C.h>
#include <PN532.h>

#define DEBUG 0

class ArmR : public Limb
{

private:
  int flowmeter_sensor_;    // pin
  unsigned long time_card_read_;
  Throb* pulse_brachia_;
  Throb* pulse_radial_;
  MicroclientProtocol* protocol_;

public:
  ArmR( //constructor with ALL the info for the right arm !!!! WITHOUT i2c !!!!
    MicroclientProtocol * protocol, Synchronization* synch, 
    int flowmeter_sensor, int seizure_motor,                                      // SENSOR PINS
    int force_sensor_brachia, int index_pulse_brachia,                            // PULSE
    int force_sensor_radial, int index_pulse_radial,                              // PULSE
    int index_cyanosis_leds                                                       // SKIN
    ): Limb (protocol, synch, seizure_motor, index_cyanosis_leds){ 
      flowmeter_sensor_ = flowmeter_sensor;
      protocol_ = protocol; 
      pulse_brachia_ = new Throb(synch, force_sensor_brachia, index_pulse_brachia);
      pulse_radial_ = new Throb(synch, force_sensor_radial, index_pulse_radial);
    }

  ArmR( //constructor with ALL the info for the right arm !!!! WITH i2c !!!
    MicroclientProtocol * protocol, Synchronization* synch, 
    Scheduler* scheduler, Adafruit_PWMServoDriver* pwm, MuxPCA9544* mux, AdcADS1015* adc, 
    int flowmeter_sensor, int seizure_motor,                                                // SENSOR PINS
    int mux_channel_force_brachia, int adc_channel_force_brachia, int index_pulse_brachia,  // PULSE 
    int mux_channel_force_radial, int adc_channel_force_radial, int index_pulse_radial,     // PULSE 
    int index_cyanosis_leds                                                                 // SKIN
    ): Limb (protocol, synch, seizure_motor, index_cyanosis_leds){ 
      flowmeter_sensor_ = flowmeter_sensor;
      protocol_ = protocol; 
      pulse_brachia_ = new Throb(synch, scheduler, pwm,mux,adc,
        mux_channel_force_brachia, adc_channel_force_brachia, index_pulse_brachia);
      pulse_radial_ = new Throb(synch, scheduler, pwm,mux,adc,
        mux_channel_force_radial, adc_channel_force_radial, index_pulse_radial);
    }
  void process(){
    pulse_brachia_->process();
    pulse_radial_->process();
  }
};

#endif // ARMR_H