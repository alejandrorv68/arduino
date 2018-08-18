#ifndef VALVE_H
#define VALVE_H

#include <ArduinoJson.h>
#include <Adafruit_PWMServoDriver.h>
// ----- scheduling ----------
#include <scheduler.h>
#include <i2c-output.h>

#define VALVE_UPDATE_INTERVAL 2000

class Valve 
{
private:
  i2cOutput* out;

public:
	Valve();
	Valve(Adafruit_PWMServoDriver* pwm, int index);
	~Valve();
	void initialization(JsonObject& data);
	void turnOn();
	void turnOff();
	ATask* getTask();
};

#endif // VALVE_H
