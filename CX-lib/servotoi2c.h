#ifndef SERVOTOI2C_H
#define SERVOTOI2C_H

/*
   first attach, after that set moveTo
   loop moveAsyncy every time
*/
#include <Adafruit_PWMServoDriver.h>
#include <servomotor-interface.h>

#define FUTUBA_BLS175_1 192
#define FUTUBA_BLS175_2 517
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

#define SERVO_UPDATE_INTERVAL 10

class ServoToI2C : public ServoMotor
{
  private:
    int current_position_;
    int open_position_;
    int close_position_;
    i2cOutput* out;

    uint32_t start_;
    uint16_t duration_;

    int servomin_;
    int servomax_;
    
    bool moving_ = false;
  public:

    ServoToI2C(Adafruit_PWMServoDriver* pwm, int index, int min, int max)
      {        
        out = new i2cOutput(pwm, index, SERVO_UPDATE_INTERVAL);
        servomax_ = max;
        servomin_ = min;
      }
    ServoToI2C(){}
    ~ServoToI2C(){}
    
    void move(int target_position) {
      out->setStartStop(target_position);
      current_position_ = target_position;
    }

    void open(uint16_t duration){
      if (current_position_ == open_position_) return;
      move(open_position_);
    }

    void close(uint16_t duration){
      if (current_position_ == close_position_) return;
      move(close_position_);
    }

    void setMinPosition(int min_position) {
      close_position_ = map(min_position, 0, 100, servomin_, servomax_);
    }

    void setMaxPosition(int max_position) {
      open_position_ = map(max_position, 0, 100, servomin_, servomax_);
    }
    
    void setMaxPosition(){
      open_position_ = current_position_;
    }

    ATask* getTask(){
      return out;
    }
        
};


#endif // ASYNCHSERVOMOTOR_H