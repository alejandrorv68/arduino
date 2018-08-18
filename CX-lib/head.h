#ifndef HEAD_H
#define HEAD_H

// ----- Body parts ----------
#include <eye.h>
#include <skin.h>
#include <throb.h>
#include <pneumatic.h>
#include <liquid.h>
#include <pulse.h>

#define DEBUG 1 // 1 yes, 0 no

class Head
{

private:
  Eye* eye_l_;
  Eye* eye_r_;
  Liquid * tears_;
  Liquid * ears_;
  Liquid * forehead_;
  Liquid * nose_;
  Liquid * drooling_;
  Pneumatic* cheek_l_;
  Pneumatic* cheek_r_;
  Pneumatic* mouth_;
  Throb * neck_l_;
  Throb * neck_r_;
  Pneumatic * neck_;
  Skin* skin_;
  i2cOldReader* light_l_; // NOTE: need to be changed if the i2c board change --> Need a refator of the logic.
  i2cOldReader* light_r_;

  float light_value_l_;
  float light_value_r_;

  Pulse* pulse_;

public:
  Head();
  Head( //constructor with ALL the info for the head
    Scheduler* scheduler,  Synchronization* synch,                                    // GENERAL
    Adafruit_PWMServoDriver* pwm,                                                     // GENERAL
    MuxPCA9544* mux, AdcADS1015* adc,                                                 // GENERAL
    int index_eyelid_motor_l, int index_pupil_motor_l,                                // EYE_L
    int mux_channel_button_sensor_l, int adc_channel_button_sensor_l,                 // EYE_L
    int mux_channel_light_sensor_l, int adc_channel_light_sensor_l,                   // EYE_L
    int mux_channel_hall_sensor_l, int adc_channel_hall_sensor_l,                     // EYE_L
    int min_pupil_l, int max_pupil_l,                                                 // EYE_L
    int min_eyelid_l, int max_eyelid_l,                                               // EYE_L
    int index_eyelid_motor_r, int index_pupil_motor_r,                                // EYE_R
    int mux_channel_button_sensor_r, int adc_channel_button_sensor_r,                 // EYE_R
    int mux_channel_light_sensor_r, int adc_channel_light_sensor_r,                   // EYE_R
    int mux_channel_hall_sensor_r, int adc_channel_hall_sensor_r,                     // EYE_R
    int min_pupil_r, int max_pupil_r,                                                 // EYE_R
    int min_eyelid_r, int max_eyelid_r,                                               // EYE_R
    int index_tears,                                                                  // TEARS
    int index_ears,                                                                   // EARS
    int index_nose,                                                                   // NOSE
    int index_drooling,                                                               // MOUTH - DROOLING 
    int index_open_mouth, int index_flush_mouth,                                      // MOUTH
    int index_open_forehead,                                                          // FOREHEAD
    int index_open_cheek_l, int index_flush_cheek_l,                                  // CHEEK L
    int index_open_cheek_r, int index_flush_cheek_r,                                  // CHEEK R
    int index_open_neck, int index_flush_neck,                                        // NECK
    int mux_channel_force_l, int adc_channel_force_l, int index_pulse_l,              // NECK
    int mux_channel_force_r, int adc_channel_force_r, int index_pulse_r,              // NECK
    int index_skin_,                                                                  // SKIN
    int mux_channel_light_nose_sensor_l, int adc_channel_light_nose_sensor_l,         // LIGHT SENSOR NOSE
    int mux_channel_light_nose_sensor_r, int adc_channel_light_nose_sensor_r          // LIGHT SENSOR NOSE
    )
  {
#if DEBUG
    Serial.println("-) 1. HEAD constructor STARTED");
#endif
    pulse_      = new Pulse(synch);
#if DEBUG
    Serial.println("-) 2. Pulse finished");
#endif

    eye_l_      = new Eye(scheduler, pwm, mux, adc, 
                  index_eyelid_motor_l, index_pupil_motor_l, 
                  mux_channel_button_sensor_l, adc_channel_button_sensor_l,
                  mux_channel_light_sensor_l, adc_channel_light_sensor_l,
                  mux_channel_hall_sensor_l, adc_channel_hall_sensor_l,
                  min_pupil_l, max_pupil_l,                                                 
                  min_eyelid_l, max_eyelid_l);
#if DEBUG
    Serial.println("-) 3. Eye left finished");
#endif
    eye_r_      = new Eye(scheduler, pwm, mux, adc, 
                  index_eyelid_motor_r, index_pupil_motor_r, 
                  mux_channel_button_sensor_r, adc_channel_button_sensor_r,
                  mux_channel_light_sensor_r, adc_channel_light_sensor_r,
                  mux_channel_hall_sensor_r, adc_channel_hall_sensor_r,
                  min_pupil_r, max_pupil_r,                                                 
                  min_eyelid_r, max_eyelid_r);

#if DEBUG
    Serial.println("-) 4. Eye right finished");
#endif
    forehead_     = new Liquid(pwm, index_open_forehead);

    tears_        = new Liquid(pwm, index_tears); 

    ears_         = new Liquid(pwm, index_ears); 

    nose_         = new Liquid(pwm, index_nose); 

    drooling_     = new Liquid(pwm, index_drooling); 

#if DEBUG
    Serial.println("-) 5. Liquid finished");
#endif
    mouth_        = new Pneumatic(pwm, index_open_mouth, index_flush_mouth);

    cheek_l_      = new Pneumatic(pwm, index_open_cheek_l, index_flush_cheek_l);

    cheek_r_      = new Pneumatic(pwm, index_open_cheek_r, index_flush_cheek_r);

    neck_         = new Pneumatic(pwm, index_open_neck, index_flush_neck);

#if DEBUG
    Serial.println("-) 6. Pneumatic finished");
#endif
    neck_l_       = new Throb(pulse_, scheduler, pwm, mux, adc,
      mux_channel_force_l, adc_channel_force_l, index_pulse_l);
    
    neck_r_       = new Throb(pulse_, scheduler, pwm, mux, adc,
      mux_channel_force_r, adc_channel_force_r, index_pulse_r);

#if DEBUG
    Serial.println("-) 7. Throb finished");
#endif
    skin_         = new Skin(index_skin_);

#if DEBUG
    Serial.println("-) 8. Skin finished");
#endif
    light_l_      = new i2cOldReader(LDR_UPDATE_INTERVAL, [this](const float   v){
        
    Serial.print("LL ");
    Serial.println(v);
        light_value_l_ = v;  
       }, mux, adc, mux_channel_light_nose_sensor_l, adc_channel_light_nose_sensor_l);

    light_r_      = new i2cOldReader(LDR_UPDATE_INTERVAL, [this](const float   v){
        Serial.print("LR ");
        Serial.println(v);
        light_value_r_ = v;  
       }, mux, adc, mux_channel_light_nose_sensor_r, adc_channel_light_nose_sensor_r);

#if DEBUG
    Serial.println("-) 9. i2cOldReader finished");
#endif
/*
    scheduler->addTask(tears_->getTask());        // Liquid
    scheduler->addTask(ears_->getTask());         // Liquid
    scheduler->addTask(nose_->getTask());         // Liquid
    scheduler->addTask(drooling_->getTask());     // Liquid
    scheduler->addTask(forehead_->getTask());     // Liquid
    scheduler->addTask(cheek_l_->getOpenTask());  // Pneumatics
    scheduler->addTask(cheek_r_->getOpenTask());  // Pneumatics
    scheduler->addTask(mouth_->getOpenTask());    // Pneumatics
*/
#if DEBUG
    Serial.println("-) 10. Scheduling finished --> HEAD FINISHED");
#endif
  }

  ~Head();  

  void callbackPulseBeat(const JsonObject& data){
    pulse_->callbackPulses(data);
  }

  void callbackCheeckL(bool open)  {
    cheek_l_->setPneumaticStatusOpen(open);
  }
  void callbackCheeckR(bool open)  {
    cheek_r_->setPneumaticStatusOpen(open);
  }
  void callbackMouth(bool open)  {
    mouth_->setPneumaticStatusOpen(open);
  }
  void callbackNeckLaryngea(bool open)  {
    neck_->setPneumaticStatusOpen(open);
  }
  void callbackEars(bool open)  {
    ears_->setLiquidStatusOpen(open);
  }
  void callbackTears(bool open)  {
    tears_->setLiquidStatusOpen(open);
  }
  void callbackDrooling(bool open)  {
    drooling_->setLiquidStatusOpen(open);
  }
  void callbackForehead(bool open){
    forehead_->setLiquidStatusOpen(open);
  }
  void callbackNose(bool open){
    nose_->setLiquidStatusOpen(open);
  }

  void callbackSkin(const JsonObject& data)  {
    skin_->updateAllTopic(data);
  }

  void callbackPulse(int HR)  {
    pulse_->setHR(HR); // HR is static so all the pulse instances are updated at the same time
  }

  void process(){
    eye_l_->process();
    eye_r_->process();
    neck_l_->process();
    neck_r_->process();
  }

  void initSkin(JsonObject& data){    
    skin_->initialization(data);
  }
};



#endif // HEAD_H
