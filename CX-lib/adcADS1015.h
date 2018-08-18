#ifndef ADCADS1015_H
#define ADCADS1015_H

#include <Wire.h>
#include <Adafruit_ADS1015.h>

static void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}

static uint8_t i2cread(void) {
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}

// Writes 16-bits to the specified destination register
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
  Wire.beginTransmission(i2cAddress);
  i2cwrite((uint8_t)reg);
  i2cwrite((uint8_t)(value>>8));
  i2cwrite((uint8_t)(value & 0xFF));
  Wire.endTransmission();
}

//Reads 16-bits from the specified destination register
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) {
  Wire.beginTransmission(i2cAddress);
  i2cwrite(ADS1015_REG_POINTER_CONVERT);
  Wire.endTransmission();
  Wire.requestFrom(i2cAddress, (uint8_t)2);
  return ((i2cread() << 8) | i2cread());  
}

class AdcADS1015
{
  private:
    Adafruit_ADS1015* ads;     /* Use thi for the 12-bit version */
    uint16_t m_bitShift_;
    uint8_t i2cAddress_;
  public:
    AdcADS1015(float max_volt, uint8_t i2cAddress){ 
        i2cAddress_ = i2cAddress;
        m_bitShift_ = 4;
        ads = new Adafruit_ADS1015 (i2cAddress_);
        ads->begin();
        if (max_volt <= 0.000){}
        else if (max_volt <= 0.256){ads->setGain(GAIN_SIXTEEN);}
        else if (max_volt <= 0.512){ads->setGain(GAIN_EIGHT);}
        else if (max_volt <= 1.024){ads->setGain(GAIN_FOUR);}
        else if (max_volt <= 2.048){ads->setGain(GAIN_TWO);}
        else if (max_volt <= 4.096){ads->setGain(GAIN_ONE);}
        else if (max_volt <= 6.144){ads->setGain(GAIN_TWOTHIRDS);}
        else Serial.println("Error Max volt in Adc constructor");
    }

    float read(int channel) {
      if (channel > 3 || channel < 0) return 0;
      return ads->readADC_SingleEnded(channel);
    }
    float beginRead(int channel) {
      if (channel > 3)
      {
        return 0;
      }
      
      // Start with default values
      uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                        ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                        ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                        ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                        ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

      // Set PGA/voltage range
      config |= ads->getGain();

      // Set single-ended input channel
      switch (channel)
      {
        case (0):
          config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
          break;
        case (1):
          config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
          break;
        case (2):
          config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
          break;
        case (3):
          config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
          break;
      }

      // Set 'start single-conversion' bit
      config |= ADS1015_REG_CONFIG_OS_SINGLE;

      // Write config register to the ADC
      writeRegister(i2cAddress_, ADS1015_REG_POINTER_CONFIG, config);

    }

    int getDelay(){
      return ADS1015_CONVERSIONDELAY;
    }

    uint16_t finishRead(int channel) {
      return readRegister(i2cAddress_, ADS1015_REG_POINTER_CONVERT) >> m_bitShift_;  // m_bitShift = 4
    }
};



#endif // ADCADS1015_H