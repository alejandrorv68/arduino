#include "valve.h"

Valve::Valve()
{
}

Valve::Valve(Adafruit_PWMServoDriver* pwm, int index)
{
	out = new i2cOutput(pwm, index, VALVE_UPDATE_INTERVAL);
}

Valve::~Valve()
{
}

void Valve::initialization(JsonObject& data){
  //TODO: read from json and initilize
}

void Valve::turnOn(){
	//TODO: activate the colors with the correct factors
    out->setDuty(1); // turns pin fully on
}

void Valve::turnOff(){
	//TODO: activate the colors with the correct factors
    out->setDuty(0); // turns pin fully off
}


ATask* Valve::getTask(){
	return out;
}
