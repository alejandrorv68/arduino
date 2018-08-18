#ifndef EYE_H
#define EYE_H

#include <liquid.h>
#include <eyelid.h>


#define EYE_DEBUG 1
#define DEBUG 0

//############### Blinking Settings ####################
#define NUM_STATIC_POSITION 4 // hardcoded the number of possible static position: open, close, medium, manually moved by the user
#define NUM_BLINKING 3 // : normal blinking (full-open -> full close); half low (half open <-> full close); half high (half open <-> full open)
#define NO_BLINKING 0
#define FAST_MOVING_TIME 5 //ms
#define SLOW_MOVING_TIME 20 //ms
#define SLOW_PERIOD 5000
#define RANDOM_CLOSE_TIME 500
//------------------- Motor positions: ----------------
#define EYELID_MIN_POSITION 0
#define EYELID_MAX_POSITION 100
#define EYELID_HALF_POSITION 50
//######################################################

//############### Update interval for i2c inputs ####################
#define BUTTON_UPDATE_INTERVAL 300
#define LDR_UPDATE_INTERVAL 1000
//######################################################

//############### Click on eye Settings ####################
#define BUTTON_THRESHOLD 1.5
//######################################################

class Eye
{
private:
  struct Behaviour{
    String key;
    int blinking;   // blinking: 0 NOblinking 1 full 2 half-low 3 half-high
    int speed;      // speed: 1 slow -> 5 fast 
    int position;   // position: 1 open 2 middle 3 close 4 manual
  };
  
  enum State {READING = 2, MOVING_DOWN = - 1, MOVING_UP = 1, IDLE = 0};
  //#################### Scheduling task in a single eye ##################
  i2cOldReader* button_sensor_;
  i2cOldReader* light_sensor_;
  Liquid * tears_actuator_;
  ServoMotor * pupil_actuator_;
  ServoMotor * eyelid_actuator_;
  Eyelid* eyelid;
  //##################### Eye position #################
  int current_max_position_;

	Behaviour behaviours_[15];
	
  static float period_;
  static float closing_in_period_;
  static bool manual_static_;

  float moving_time_;
	float clicked_time_;
  float light_value_;
  int current_behaviour_;
  State eyelid_read_ ;
  bool eye_clicked_;

  int mux_channel_button_sensor_;
  int adc_channel_button_sensor_;

public:
  Eye();
  Eye(Scheduler* scheduler, Adafruit_PWMServoDriver* pwm, 
  MuxPCA9544* mux, AdcADS1015* adc, 
  int index_eyelid_motor, int index_pupil_motor, 
  int mux_channel_button_sensor, int adc_channel_button_sensor,
  int mux_channel_light_sensor, int adc_channel_light_sensor,
  int mux_channel_hall_sensor, int adc_channel_hall_sensor,
  int min_pupil, int max_pupil,
  int min_eyelid, int max_eyelid);
  ~Eye();

  void initialization(const JsonObject& jo);
  //------- CHECK --------
  void eyeClicked();
  void eyelidPushed();
  void eyelidReleased();
  void checkReadStatus();
  //------- SET --------
  void eyeBehaviour();
  void setBlinking();
  void setStatic();
  //------- GET --------
  bool getEyelidPushed();
  bool getEyeClick();
  float getLightValue();

  void process();
  void manualProcess();

  inline float rest(unsigned long now)	{
	  int c = now/period_;
	  return (now - (period_*c)) / period_;
	}  
};
#endif // EYE_H