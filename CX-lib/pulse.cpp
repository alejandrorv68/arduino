#include "pulse.h"


int Pulse::HR_ = 60;
int Pulse::active_rate_ = 60;

Pulse::Pulse()
{
  current_beat_ = new Beat();
}

Pulse::Pulse( Synchronization* synch)
{
  synchronization_ = synch;
  current_beat_ = new Beat();
}


Pulse::~Pulse()
{
}

void Pulse::setHR(int HR)
{
  Pulse::HR_ = HR;
}

void Pulse::synchronize()
{
  if(millis() > synchronization_->getHRStartTime()) {
    active_sync_time_ = synchronization_->getHRStartTime();
    Pulse::active_rate_ = Pulse::HR_;
  }
}

int Pulse::getHR()
{
  return Pulse::HR_;
}

bool Pulse::timeToPulse()
{
  if (!current_beat_) {
    Serial.println("NO current_beat_");
    return false;
  }
  while (current_beat_->next != NULL && current_beat_->next->timestamp < synchronization_->getServerTime()) {
    current_beat_ = current_beat_->next;
  }
  if (Pulse::HR_ == 0) {
    Serial.println("Pulse::HR_");
    return false;
  }
  float delta_time =  synchronization_->getServerTime() - synchronization_->getHRStartTime();
  if (rest(delta_time,getPulseInterval()) < current_beat_->duration) {
    return true; 
  }
  else {
    return false; 
  }     
}

int Pulse::getActiveRate(){
  return Pulse::active_rate_;
}

unsigned long Pulse::getActiveSyncTime(){
  return active_sync_time_;
}

void  Pulse::callbackPulses(const JsonObject& jo)
{
  Beat* pp = current_beat_;
  
  JsonArray& pulses = jo["arg"];
  for (auto& p : pulses) {
    if (current_beat_->id < 0){
      current_beat_->duration  = p["duration"];
      current_beat_->intensity = p["intensity"];
      current_beat_->timestamp = p["timestamp"];
      current_beat_->id = p["beat_id"];
      continue;
    }
    while (p["beat_id"] != pp->id && pp->next) {pp = pp->next;}
    if (p["beat_id"] == pp->id){
      pp->duration  = p["duration"];
      pp->intensity = p["intensity"];
      pp->timestamp = p["timestamp"];
    }
    else
    {
      Beat* new_beat = new Beat();
      new_beat->duration  = p["duration"];
      new_beat->intensity = p["intensity"];
      new_beat->timestamp = p["timestamp"];
      new_beat->id        = p["beat_id"];
      new_beat->next      = NULL;
      pp->next = new_beat;
    }
  }
}

// ######### Comments OLD CODE ##################
/*



void  Pulse::callbackPulses(const JsonObject& jo)
{
  jo.prettyPrintTo(Serial);
  JsonObject& pulses = jo["arg"];
  pulses.prettyPrintTo(Serial);
  Beat* pp;
  for (i = current_beat_->id; i< current_beat_->id + 6; i ++) {
    if (!pulses[i]) return;
    pp = current_beat_;

    if (inList(i)){
      while (i != pp->id && pp->next) {pp = pp->next;}
      pp->duration  = pulses[i]["duration"];
      pp->intensity = pulses[i]["intensity"];
      pp->timestamp = pulses[i]["timestamp"];
    }
    else
    {
      while (pp->next) {pp = pp->next;}
      Beat* new_beat;
      pp->next = new_beat;
      new_beat->duration  = pulses[i]["duration"];
      new_beat->intensity = pulses[i]["intensity"];
      new_beat->timestamp = pulses[i]["timestamp"];
    }
  }
}

bool Pulse::inList(int id){
  Beat* pp;
  pp = current_beat_;
  if (pp->id == id){return true;}
  while (pp->next) if (pp->id == id){return true;}
  return false;
}




*/