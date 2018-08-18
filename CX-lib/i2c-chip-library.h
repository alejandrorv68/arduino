#ifndef READI2C_H
#define READI2C_H

#include <Arduino.h>
#include <Wire.h>

#define  ADDR 0x45

#define  NEW 0b10100000
#define  SAME 0x00
#define INPUT_VOLTAGE 5

#define READI2C_DEBUG 1

#define CH0 0b10000
#define CH1 0b11000
#define CH2 0b10001
#define CH3 0b11001
#define CH4 0b10010
#define CH5 0b11010
#define CH6 0b10011
#define CH7 0b11011
#define CH8 0b10100
#define CH9 0b11100
#define CH10 0b10101
#define CH11 0b11101
#define CH12 0b10110
#define CH13 0b11110
#define CH14 0b10111
#define CH15 0b11111

class readI2C
{
  private:
  public:
    static void setChannel(int pin) {
      
      Wire.beginTransmission(ADDR);
      switch(pin){
        case 0: 
          Wire.write(NEW|CH0);   
          break;    
        case 1: 
          Wire.write(NEW|CH1);   
          break;    
        case 2: 
          Wire.write(NEW|CH2);   
          break;    
        case 3: 
          Wire.write(NEW|CH3);   
          break;    
        case 4: 
          Wire.write(NEW|CH4);   
          break;    
        case 5: 
          Wire.write(NEW|CH5);   
          break;    
        case 6: 
          Wire.write(NEW|CH6);   
          break;    
        case 7: 
          Wire.write(NEW|CH7);   
          break;    
        case 8: 
          Wire.write(NEW|CH8);   
          break;    
        case 9: 
          Wire.write(NEW|CH9);   
          break;    
        case 10: 
          Wire.write(NEW|CH10);   
          break;    
        case 11: 
          Wire.write(NEW|CH11);   
          break;    
        case 12: 
          Wire.write(NEW|CH12);   
          break;    
        case 13: 
          Wire.write(NEW|CH13);   
          break;    
        case 14: 
          Wire.write(NEW|CH14);   
          break;   
        case 15: 
          Wire.write(NEW|CH15);   
          break;      
              
      }      
      Wire.endTransmission(true);
    }

    static float read_22(int pin)
    {   
#if READI2C_DEBUG
      Serial.print("PIN ");
      Serial.println(pin); 
#endif
      Wire.requestFrom(ADDR, 3 ,false);    // request 3 of the 4 bytes from the chip, stopmessage = false because otherwise the chip start a new conversion
      
      short count = 0;
      byte check = 0;
      uint32_t total = 0;
      while (Wire.available()) { // slave may send less than requested
        byte c = Wire.read(); // receive a byte as character

        if (count ==0){
           total = (c & 0b00111111);
           check = (c & 0b11000000) >> 6;
        }
        else {
          total = (total<<8) |c;
        }
        count = count+1; 
      }
      if(count!=3)//error check
      {
        return 0;
      }
      double T = double(total)/4194304L;//2^22 
      double result;
      switch (check){
        case 0b00:
          result = 0;
          break;
        case 0b01:
          result = T*(INPUT_VOLTAGE / 2);
          break;
        case 0b10:
          result =  T* (INPUT_VOLTAGE / 2)+(INPUT_VOLTAGE / 2);
          break;
         case 0b11:
          result = INPUT_VOLTAGE;
          break;  
      }

#if READI2C_DEBUG
      Serial.println("READ FROM i2c VALE: ");
      Serial.println(result);
#endif
      return result;
    }


    
    static float read_16(int pin) {  
#if READI2C_DEBUG
      Serial.print("PIN ");
      Serial.println(pin); 
#endif
      Wire.requestFrom(ADDR, 3);    // request 3 bytes from slave device #8
      
      short count = 0;
      byte check = 0;
      long int total = 0;

      while (Wire.available()) { // slave may send less than requested
        byte c = Wire.read(); // receive a byte as character

        if (count ==0){
           total = (c & 0b00111111);
           check = (c & 0b11000000) >> 6;
           
        }
        else {
          total = (total<<8) |c;
        }
        count = count+1; 
      }

      total = total>>6;
      double T = double(total)/65536L;
      double result;
      switch (check){
        case 0b00:
          result = 0;
          break;
        case 0b01:
          result = T*2.5/2;
          break;
        case 0b10:
          result =  T*2.5/2+2.5;
          break;
         case 0b11:
          result = 5;
          break;  
      }
      delay(300);
      return result;
    }
    
};

#endif // READI2C_H