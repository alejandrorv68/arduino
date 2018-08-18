#ifndef SERVOMOTORINTERFACE_H
#define SERVOMOTORINTERFACE_H

/*
   first attach, after that set moveTo
   loop moveAsyncy every time
*/
// ----- scheduling ----------
#include <scheduler.h>
#include <i2c-output.h>



class ServoMotor
{

public:

  virtual  ~ServoMotor(){}
  virtual void move(int target_position) = 0;
  virtual void open(uint16_t duration) = 0;
  virtual void close(uint16_t duration) = 0;
  virtual void setMinPosition(int min_position) = 0;
  virtual void setMaxPosition(int max_position) = 0;
  virtual void setMaxPosition() = 0;
  virtual ATask* getTask() = 0;
};


#endif // SERVOMOTORINTERFACE_H