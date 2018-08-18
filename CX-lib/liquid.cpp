#include "liquid.h"

Liquid::Liquid()
{
	status_open_ = false;
}

Liquid::~Liquid()
{
}


Liquid::Liquid(Adafruit_PWMServoDriver* pwm, int index_open)
{
	status_open_ = false;
	open_ = new Valve(pwm, index_open);
}

void Liquid::setLiquidStatusOpen(bool open){
	status_open_ = open; 
	if (status_open_)
  	{
    	open_->turnOn();
  	}
  	else
  	{    
    	open_->turnOff(); 
  	}
}

bool Liquid::getLiquidStatusOpen(){
  return status_open_;
}

ATask* Liquid::getTask(){
  return open_->getTask();
}