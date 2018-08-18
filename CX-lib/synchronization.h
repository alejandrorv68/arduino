#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <ArduinoJson.h>
#include <Arduino.h>

class Synchronization
{
private:
  unsigned long offset_ = 0;
  unsigned long HR_start_time_ = 0;

public:
	Synchronization();
	~Synchronization();
	void setServerTime(const JsonObject& jo);
	void setHRStartTime(unsigned long BPMsynch);
	unsigned long getOffset() {return offset_;}
	unsigned long getHRStartTime(){return HR_start_time_;}

	unsigned long getServerTime () {return millis() + offset_;}
};

#endif // MICROCLIENTINTERFACE_H
