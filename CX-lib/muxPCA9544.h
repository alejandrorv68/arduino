#ifndef MUXPCA9544_H
#define MUXPCA9544_H

#include <Arduino.h>
#include <Wire.h>
#define TCAADDR 0x70

class MuxPCA9544
{
  private:
  public:
    MuxPCA9544(){}
    void openChannel(int pin) {
      Wire.beginTransmission(TCAADDR);
      Wire.write( 0x04 | pin);
      Wire.endTransmission(); 
    }
    void closeChannel(int pin) {
      Wire.beginTransmission(TCAADDR);
      Wire.write(0);
      Wire.endTransmission(); 
    }
};

#endif // MUXPCA9544_H