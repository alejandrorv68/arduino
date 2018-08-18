#ifndef I2COLDREADER_H
#define I2COLDREADER_H

#include <muxPCA9544.h>
#include <adcADS1015.h>
#include <atask.h>

class i2cOldReader : public ATask
{
private: 
  enum State {IDLE = 0, READING = 1, FINISHED = 2};
  State state_;
  MuxPCA9544 * mux_;
  AdcADS1015 * adc_;
  int mux_channel_;
  int adc_channel_;
  float value_;

  std::function<void (const float value)> cb;
  
  static bool reading_;
public:
  i2cOldReader(int update_interval, 
    std::function<void (const float value)> f, MuxPCA9544* mux, AdcADS1015* adc,
    int mux_channel, int adc_channel);
  i2cOldReader();

  int getPriority(unsigned long now);
  
  void setPriority(int t);

  bool run();
};

#endif // I2COLDREADER_H