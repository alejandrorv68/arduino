#ifndef LIMB_H
#define LIMB_H

// ----- Body parts ----------
#include <pulse.h>
#include <skin.h>
#include <microclient-protocol.h>
#define DEBUG 0

class Limb
{

private:  

public:
  Limb(){}
  Limb( //constructor with ALL the info for the limb
    int seizure_motor,                                                            // SENSOR PINS
    int index_cyanosis_leds                                                       // SKIN
    ){    
      seizure_motor_ = seizure_motor;
      cyanosis_leds_ = new Skin(index_cyanosis_leds);
  }
  Limb( //constructor with ALL the info for the limb
    int seizure_motor                                                            // SENSOR PINS
    ){    
      seizure_motor_ = seizure_motor;
      cyanosis_leds_ = NULL; // not valid value
  }

  ~Limb(){}  

  void initSkin(JsonObject& data){
    cyanosis_leds_->initialization(data);
  }

  void callbackSeizureMotor(bool open){
    if (open) {
      digitalWrite(seizure_motor_, HIGH);
    }
    else{
      digitalWrite(seizure_motor_, LOW);
    }
  }

  void callbackSkin(const JsonObject& data){
    if (cyanosis_leds_ != NULL) cyanosis_leds_->updateAllTopic(data);
  }

  int seizure_motor_;      // pin
  Skin * cyanosis_leds_;
};



#endif // LIMB_H
