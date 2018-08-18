#ifndef MICROCLIENTINTERFACE_H
#define MICROCLIENTINTERFACE_H


// Base code for an auto-connecting CommandX peripheral driven by an ESP-12
// Arduino-compatible WiFi-enabled microcontroller.
// Note that while it has up to 1MB space for the program, it has just 80KB
// available RAM of which a lot is already used by global variables.

// Note: careful with long messages
#include <ArduinoJson.h>
class MicroclientInterface
{

public:

  virtual  ~MicroclientInterface(){}
  virtual bool tryConnect() = 0;
  virtual bool isConnected() = 0;
  virtual bool sendMessage(char id, const JsonObject &data) = 0;
  virtual bool clientAvailable() = 0;
  virtual char clientRead() = 0;

  virtual String getIdType() = 0;
  virtual String getIdVersion() = 0;
  virtual String getIdIdentification() = 0;
};

#endif // MICROCLIENTINTERFACE_H
