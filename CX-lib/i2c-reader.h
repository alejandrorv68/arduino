// Activity to add when a read from i2c is necessary

#ifndef I2CREADER_H
#define I2CREADER_H

#include <i2c-chip-library.h>
#include <atask.h>

//############### I2C read possibilities ####################
#define I2C_22 0
#define I2C_16 1
#define DELAY_22 80
#define DELAY_16 150
//######################################################


class i2cReader : public ATask
{
private: 
  bool converting_ = false;
  int index_ = 0;
  float value_;

  std::function<void (const float value)> cb;

public:
  i2cReader(int index, int update_interval, std::function<void (const float value)> f):
    index_(index){
    	cb = f;
      update_interval_ = update_interval;
      setPriority(0);
	}
  i2cReader(){}

  int getPriority(unsigned long now){
    return nextRunTime_ - now;
  }
  void setPriority(int t){
  	if (t == 0) nextRunTime_ = millis() + update_interval_;
    else nextRunTime_ = millis() + t;
  }

  bool run(){
    if (!converting_){
      readI2C::setChannel(index_); 
      converting_ = true;
      if (I2C_22) setPriority(DELAY_22);
      if (I2C_16) setPriority(DELAY_16);
      return false;
    }
    if (converting_) {
      if (I2C_22) value_ = readI2C::read_22(index_); 
      if (I2C_16) value_ = readI2C::read_16(index_); 
      setPriority(0);
      cb(value_);
      converting_ = false;
      return true;
    }
  }
};



#endif // I2CREADER_H