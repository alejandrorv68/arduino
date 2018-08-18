// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define  ADDR 0x18

#define  NEW 0x05
#define  SAME 0x00

#define CH0 0b10000
#define CH1 0b11000
#define CH2 0b10001
#define CH3 0b11001
#define CH4 0b10010
#define CH5 0b11010
#define CH6 0b10011
#define CH7 0b11011


void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  Serial.print("ready!");
  
  Serial.println("");


}

void Check_i2c(byte address){
  Wire.beginTransmission(address);
  byte  error = Wire.endTransmission(false);

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  else{
    Serial.println("No I2C devices found\n");
  }
  
}

void loop() {

   
  Serial.println("");
  Check_i2c(ADDR);
  Serial.print("CH0: ");
  readAdc(CH0);
  Serial.print("CH1: ");
  readAdc(CH1);
  Serial.print("CH3: ");
  readAdc(CH3);
  Serial.println("");
  delay(2000);
}

void readAdc(byte CH){
  
  Wire.beginTransmission(ADDR);
  //Wire.write(ADDR);
  Wire.write(CH<<2);
  //Serial.println(((NEW<<5)|CH1),BIN);
  
  Wire.endTransmission(false);
  //delay(1);
  
  Wire.requestFrom(ADDR, 2);    // request 2 bytes from slave device #9
  
  
  int a= 0, i=0; 
  byte c[2]; 

 

 
  while (Wire.available()) { // slave may send less than requested
    c[i] = Wire.read(); // receive a byte as character
    i++;
   //Serial.println(c);         // print the character
    
  }

   a= (c[0]<<8|c[1])>>4;
   
  Serial.println(a);

}
