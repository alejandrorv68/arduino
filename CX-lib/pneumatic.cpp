#include "pneumatic.h"


// NOTE: flush commented because not connected at the moment!!

Pneumatic::Pneumatic()
{
	status_open_ = false;
}

Pneumatic::~Pneumatic()
{
}


Pneumatic::Pneumatic(Adafruit_PWMServoDriver* pwm, int index_open, int index_flush)
{
	status_open_ = false;
	open_ = new Valve(pwm, index_open);
  //flush_ = new ValveESP(index_flush);
}

void Pneumatic::setPneumaticStatusOpen(bool open){
	status_open_ = open; 
	if (status_open_)
  	{
  		//Serial.println("setPneumaticStatusOpen OPEN");
    	open_->turnOn();
    	//flush_->turnOff();
  	}
  	else
  	{    
  		//Serial.println("setPneumaticStatusOpen CLOSE");
    	open_->turnOff();
    	//flush_->turnOn();   
  	}
}

bool Pneumatic::getPneumaticStatusOpen(){
	return status_open_;
}

ATask* Pneumatic::getOpenTask(){
  return open_->getTask(); 
}