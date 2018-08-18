#ifndef EYELID_H
#define EYELID_H

#include <ArduinoJson.h>
#include <servotoi2c.h>
#include <servotoESP.h>

// ----- scheduling ----------
#include <i2c-chip-library.h>
#include <scheduler.h>
#include <atask.h>
#include <muxPCA9544.h>
#include <adcADS1015.h>
#include <i2c-old-reader.h>


//############### Push on eyelid Settings ####################
#define FORCE_THRESHOLD 0.15
#define FORCE_THRESHOLD_ERROR 1.5
#define PUSH_EYELID_TIME 1000 // the eye stay open for one second after it receives the push signal, after this time it restart with its normal behaviour
#define NUM_HALL_SAMPLES 2
#define NUM_CALIBRATION_SAMPLES 10
#define TIMEOUT 2
//######################################################

//############### Update interval for i2c inputs ####################
#define HALL_UPDATE_INTERVAL 200
#define CALIBRATION_UPDATE_INTERVAL -10000 
//######################################################

class Eyelid
{
private:

	double smooth_force_;
	unsigned long pushed_time_;
  unsigned long start_;
	bool pushed_ = false;
	double hall_avarage_;
  int samples_;
  float value_;
  bool new_value_ = false;
  bool calibrated_ = false;
  float total_;
  i2cOldReader* hall_sensor_;

  Scheduler * scheduler_;

public:
  Eyelid(Scheduler* scheduler, MuxPCA9544* mux, AdcADS1015* adc, int mux_channel, int adc_channel){
    calibrated_ = false;

    reset();

    hall_sensor_ = new i2cOldReader(HALL_UPDATE_INTERVAL, [this](const float   v){
      total_ += v;
      samples_ ++;
    }, mux, adc, mux_channel, adc_channel);

    //################## scheduling ###################
    scheduler_ = scheduler;
    scheduler->addTask(hall_sensor_);
    //################## calibrating ###################
    calibrate(0);

    while(!getCalibrated()){
      scheduler->update();
      calibrate(getSamples());    
    } 

  }

  ~Eyelid(){}

  void reset(){
    samples_ = 0;
    total_ = 0;
  } 

  void calibrate(int samples){ 
    samples_ = samples;
    hall_sensor_->setPriority(CALIBRATION_UPDATE_INTERVAL);
    if (samples_ < NUM_CALIBRATION_SAMPLES) return;
    hall_sensor_->setPriority(HALL_UPDATE_INTERVAL);
  	hall_avarage_ = total_ / samples_;
    reset();
    calibrated_ = true;
  }

  int getSamples(){
    return samples_;
  }

  int read(){
    int _return = 0;
    start_ = millis();
    while(samples_ < NUM_CALIBRATION_SAMPLES && !timeout()) scheduler_->update();
    if (timeout()) {
      _return = 2;
      return _return; // if stopped because of TIMEOUT get out and come back in the next cycle to continue
    }
    double force = (double)(total_ / samples_);
    double diff = force - hall_avarage_;
    if (FORCE_THRESHOLD < diff)         _return =   1;
    else if (- FORCE_THRESHOLD > diff)  _return = - 1;
    // NOTE: next IF should possibly never happen
    if (_return != 0 && FORCE_THRESHOLD_ERROR < abs(diff)) { // FORCE_THRESHOLD_ERROR in case the difference is too high, there migth be a mechanical problem
      Serial.println("ERROR: Hall sensor in eye is out of position");
      calibrate(0);
      while(!getCalibrated()){
        scheduler_->update();
        calibrate(getSamples());    
      } 
    }
    reset();
    return _return;
  }

  void pressed() {  	
  	pushed_time_ = millis();
  	pushed_ = true;
  } 

  bool timeout(){
    return millis() > start_ + TIMEOUT;
  }

  void released() {  	
   	pushed_ = false;
  }

  bool getPressing(){
  	return pushed_;
  }
  void setPressing(const bool pushed){
  	pushed_ = pushed;
  	if (pushed_) pushed_time_ = millis();
  }

  bool pushingTime(){
  	return (millis() - pushed_time_ < PUSH_EYELID_TIME && pushed_);
  }

  bool getCalibrated(){
    return calibrated_;
  }
};
#endif // EYELID_H
