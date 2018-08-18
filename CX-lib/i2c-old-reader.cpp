#include <i2c-old-reader.h>

bool i2cOldReader::reading_ = false;

i2cOldReader::i2cOldReader(int update_interval, 
  std::function<void (const float value)> f, MuxPCA9544* mux, AdcADS1015* adc,
  int mux_channel, int adc_channel):
    mux_channel_(mux_channel),
    adc_channel_(adc_channel), state_(IDLE)
{
  cb = f;
  mux_ = mux;
  adc_ = adc;
  update_interval_ = update_interval;
  setPriority(0);
}

i2cOldReader::i2cOldReader(){}

int i2cOldReader::getPriority(unsigned long now){
  return nextRunTime_ - now;
}

void i2cOldReader::setPriority(int t){
  if (t == 0) nextRunTime_ = millis() + update_interval_;
  else nextRunTime_ = millis() + t;
}

bool i2cOldReader::run(){
  if (i2cOldReader::reading_ && state_ == IDLE) { setPriority(0); return false;}
  if (state_ == IDLE){
    mux_->openChannel(mux_channel_);
    setPriority(adc_->getDelay());
    state_ = READING;
    i2cOldReader::reading_ = true;
    return false;
  }
  if (state_ == READING){
    value_ = adc_->beginRead(adc_channel_);
    setPriority(adc_->getDelay());
    state_ = FINISHED;
    i2cOldReader::reading_ = true;
    return false;
  }
  if (state_ == FINISHED){
    value_ = adc_->finishRead(adc_channel_);      
    mux_->closeChannel(mux_channel_);
    cb(value_);
    state_ = IDLE;
    setPriority(0);
    i2cOldReader::reading_ = false;
    return true;
  }
}