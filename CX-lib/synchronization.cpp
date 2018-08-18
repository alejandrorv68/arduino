#include "synchronization.h"

Synchronization::Synchronization()
{
  HR_start_time_ = millis();
  offset_ = 0;
}

Synchronization::~Synchronization()
{
}

void Synchronization::setServerTime(const JsonObject& jo){
  long current_time = millis();
  long start_time  = jo["client_time"].as<double>();
  long server_time  = jo["server_time"].as<double>() * 1000;
  float delta_time = current_time - start_time;
  offset_ = server_time - (delta_time / 2 + start_time);
  
}

void Synchronization::setHRStartTime(unsigned long synch){
  if ((synch - offset_) > 0 ) HR_start_time_ = synch;
  else Serial.println("ERROR: getHRStartTime in synchronization.cpp -- Wrong input");
}