#ifndef ARML_H
#define ARML_H

// ----- Body parts ----------
#include <limb.h>
#include <throb.h>

#define STETHOSCOPE_PRESENT 2000
#define CUFF_PRESENT 2000
#define DEBUG 0

class ArmL : public Limb
{

private:
  int cuff_sensor_;        // pin
  int hall_sensor_;        // pin
  int pressure_sensor_;    // pin
  bool stethoscope_connected_;
  Throb* pulse_brachia_;
  Throb* pulse_radial_;
  MicroclientProtocol* protocol_;
  StaticJsonBuffer<1000> jsonbuffer_;
  Pulse* pulse_;

public:
  ArmL( //constructor with ALL the info for the left arm
    MicroclientProtocol * protocol, Synchronization* synch, 
    int cuff_sensor, int hall_sensor, int pressure_sensor, int seizure_motor,     // SENSOR PINS
    int force_sensor_brachia, int index_pulse_brachia,                            // PULSE
    int force_sensor_radial, int index_pulse_radial,                              // PULSE
    int index_cyanosis_leds                                                       // SKIN
    ): Limb (seizure_motor, index_cyanosis_leds){ 
    cuff_sensor_ = cuff_sensor;    
    hall_sensor_ = hall_sensor;        
    pressure_sensor_ = pressure_sensor;   
    pulse_      = new Pulse(synch); 
    pulse_brachia_ = new Throb(pulse_, synch, force_sensor_brachia, index_pulse_brachia); 
    pulse_radial_ = new Throb(pulse_, synch, force_sensor_radial, index_pulse_radial);
    protocol_ = protocol;  
    stethoscope_connected_ = false;
  }

  void process(){ // loop
  #if DEBUG
    Serial.println("READING:   ");
    Serial.print("Cuff sensor (LDR): ");
    Serial.println(analogRead(cuff_sensor_));
    Serial.print("Hall sensor (): ");
    Serial.println(analogRead(hall_sensor_));
    Serial.print("stethoscope_connected_ (): ");
    Serial.println(stethoscope_connected_);
    Serial.print("pressure_sensor_ (): ");
    Serial.println(analogRead(pressure_sensor_));
  #endif

    if (analogRead(cuff_sensor_) > CUFF_PRESENT){
      JsonObject& data_pressure = jsonbuffer_.createObject();
      data_pressure["value"] = analogRead(pressure_sensor_);
      protocol_->trigger("hps?/action_execution/pressure", data_pressure);
      jsonbuffer_.clear();
    }
    if (!stethoscope_connected_ && analogRead(hall_sensor_) > STETHOSCOPE_PRESENT){
      stethoscope_connected_ = true;
      JsonObject& data_stethoscope = jsonbuffer_.createObject();
      data_stethoscope["value"] = stethoscope_connected_;
      protocol_->trigger("hps?/action_execution/stethoscope", data_stethoscope);
      jsonbuffer_.clear();
    }
    else if (stethoscope_connected_){
      stethoscope_connected_ = false;
      JsonObject& data_stethoscope = jsonbuffer_.createObject();
      data_stethoscope["value"] = stethoscope_connected_;
      protocol_->trigger("hps?/action_execution/stethoscope", data_stethoscope);
      jsonbuffer_.clear();    
    }
    pulse_brachia_->process();
    pulse_radial_->process();
  }
  void callbackPulseBeat(const JsonObject& data){
    pulse_->callbackPulses(data);
  }
  void callbackPulse(int HR)  {
    pulse_->setHR(HR); // HR is static so all the pulse instances are updated at the same time
  }
};



#endif // HEAD_H