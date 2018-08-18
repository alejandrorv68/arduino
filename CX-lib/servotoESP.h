#ifndef SERVOTOESP_H
#define SERVOTOESP_H

#include <servomotor-interface.h>

#define PEMSERVOMAX 1023
#define PEMSERVOMIN 0
#define SERVOMIN  0 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  180 // this is the 'maximum' pulse length count (out of 4096)

class ServoToESP: public ServoMotor
{
  private:
    int index_;
    int current_position_;
    int open_position_;
    int close_position_;
  public:

    ServoToESP(int index):index_(index){}
    ServoToESP(){}
    ~ServoToESP(){}
    
    void move(int target_position) {
      current_position_ = target_position;
    }

    void open(uint16_t duration){
      analogWrite(index_, map(open_position_, SERVOMIN, SERVOMAX, PEMSERVOMIN, PEMSERVOMAX));
      if (current_position_ == open_position_) return;
      move(open_position_);
    }

    void close(uint16_t duration){
      analogWrite(index_, map(close_position_, SERVOMIN, SERVOMAX, PEMSERVOMIN, PEMSERVOMAX));
      if (current_position_ == close_position_) return;
      move(close_position_);
    }

    void setMinPosition(int min_position) {
      close_position_ = min_position;
    }

    void setMaxPosition(int max_position) {
      open_position_ = max_position;
    }

    void setMaxPosition(){
      open_position_ = current_position_;
    }

    ATask* getTask(){
      return NULL;
    }
    
};

#endif // SERVOTOESP_H