#ifndef I2COUPUT_H
#define I2COUPUT_H

#include <Adafruit_PWMServoDriver.h>
#include <atask.h>

class i2cOutput : public ATask
{
private: 
	enum State {DUTY = 0, STARTSTOP = 1};
	State state_;
	int pulselen_;
  int currentposition_;
	int duty_;
	Adafruit_PWMServoDriver* pwm_;
	int index_;
  unsigned long nextRunTime_ = 0;

public:
  i2cOutput(Adafruit_PWMServoDriver* pwm, int index, int update_interval)
  {
		pwm_ = pwm;
		index_ = index;
    update_interval_ = update_interval;
    currentposition_ = 400;
    setPriority(0);
	}
  
  i2cOutput(){}

  int getPriority(unsigned long now){
    return nextRunTime_ - now;
  }
  
  void setPriority(int t){
  	if (t == 0) nextRunTime_ = millis() + update_interval_;
    else nextRunTime_ = millis() + t;
  }

  void setDuty(int duty){
    state_ = DUTY;
  	duty_ = duty;
  }

  void setStartStop(int pulselen){
    state_ = STARTSTOP;
  	pulselen_ = pulselen;
  }

  bool run(){
  // Drive each servo one at a time
    if (state_ == DUTY) {
      pwm_->setPWM(index_, 4096 * duty_, 4096 * (1 - duty_));   
    }
  	if (state_ == STARTSTOP){
      if (currentposition_ < pulselen_){
        for (uint16_t pulselen = currentposition_; pulselen < pulselen_; pulselen++) {
          pwm_->setPWM(index_, 0, pulselen);
        }          
      }
      else{
        for (uint16_t pulselen = currentposition_; pulselen > pulselen_; pulselen--) {
          pwm_->setPWM(index_, 0, pulselen);
        }
      }
    }
    currentposition_ = pulselen_;
    setPriority(0);
    return true;
  }
};

#endif // I2COUPUT_H