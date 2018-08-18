#ifndef PULSE_H
#define PULSE_H

#include <synchronization.h>
#include <scheduler.h>

#define MILLISEC_IN_MIN 60000
#define PULSE_PERIOD_MS 10 // in milliseconds  
#define FORCE_UPDATE_INTERVAL 300 
#define PULSE_UPDATE_INTERVAL 300
#define FORCE_THRESHOLD 1000


class Beat{
public:
    int id;
    float duration;
    float intensity;
    unsigned long timestamp;
    Beat* next;

    Beat(){
      id = -1;
      duration = 0;
      intensity = 0;
      timestamp = 0;
      next = NULL;
    }
    ~Beat(){}
};

class Pulse 
{
private:  
  Beat* current_beat_;                          
  
  static int  HR_;                               // heart rate
  static int  active_rate_;
  unsigned long active_sync_time_ = 0;

  Synchronization* synchronization_;            // object to obtain information for synchronization
  
  // Private methods 

  inline float rest(float a, float b){ int c = a/b; return a - (b*c);}
  inline float getPulseInterval(){
    if (Pulse::HR_ != 0) return MILLISEC_IN_MIN / Pulse::HR_;
    else return 0; 
  }

  unsigned long getActiveSyncTime();

public:
  Pulse();
  Pulse(Synchronization* synch);
  ~Pulse();
  void  setHR(int HR);
  void  synchronize();
  int   getHR();
  int   getActiveRate();
  bool  timeToPulse();
  void  callbackPulses(const JsonObject& jo);
};

#endif // MICROCLIENTINTERFACE_H
