#include "eye.h"
float Eye::period_ = 1.0;
float Eye::closing_in_period_ = 1.0;
bool Eye::manual_static_ = false;
Eye::Eye(){}

Eye::~Eye(){}

Eye::Eye(Scheduler* scheduler, Adafruit_PWMServoDriver* pwm, 
    MuxPCA9544* mux, AdcADS1015* adc, 
    int index_eyelid_motor, int index_pupil_motor, 
    int mux_channel_button_sensor, int adc_channel_button_sensor,
    int mux_channel_light_sensor, int adc_channel_light_sensor,
    int mux_channel_hall_sensor, int adc_channel_hall_sensor,
    int min_pupil, int max_pupil,
    int min_eyelid, int max_eyelid){

  eyelid = new Eyelid(scheduler, mux, adc, mux_channel_hall_sensor, adc_channel_hall_sensor);
  mux_channel_button_sensor_ = mux_channel_button_sensor;
  adc_channel_button_sensor_ = adc_channel_button_sensor;
  button_sensor_ = new i2cOldReader(BUTTON_UPDATE_INTERVAL, [this](const float   v){
#if DEBUG
  Serial.print(" B ");
  Serial.print(" M ");
  Serial.print(mux_channel_button_sensor_);
  Serial.print(" A ");
  Serial.print(adc_channel_button_sensor_);
  Serial.print(" V: ");
  Serial.println(v);
#endif
      //eye_clicked_ = v > BUTTON_THRESHOLD ? true : false;
     }, mux, adc, mux_channel_button_sensor, adc_channel_button_sensor);
  
  light_sensor_ = new i2cOldReader(LDR_UPDATE_INTERVAL, [this](const float   v){
    light_value_ = v;  
     }, mux, adc, mux_channel_light_sensor, adc_channel_light_sensor);

  //################## scheduling ###################
  pupil_actuator_ = new ServoToI2C(pwm, index_pupil_motor, min_pupil, max_pupil);
  eyelid_actuator_ = new ServoToI2C(pwm, index_eyelid_motor, min_eyelid, max_eyelid);

  scheduler->addTask(button_sensor_);

  scheduler->addTask(light_sensor_);

  scheduler->addTask(pupil_actuator_->getTask());

  scheduler->addTask(eyelid_actuator_->getTask());

  // start blinking
  setBlinking(); // SetBlinking(int speed, int blinking) For testing 

  eye_clicked_ = false;
  light_value_ = 0;

}

void Eye::initialization(const JsonObject& jo){
  // Hard coded setting to determine the behaviour of the eye
  JsonArray& modes = jo["eyelids"];
  int b_counter = 0;
  for (int i = 0; i < 5; i++) //for (int i = 0; i < sizeOf(modes); i++)
  {
    behaviours_[b_counter].key = modes[i]["name"].as<String>();
    if (modes[i]["name"] == "blinking full slow")
    {
      behaviours_[b_counter].blinking = 1;
      behaviours_[b_counter].speed = 3;
      behaviours_[b_counter].position = 0;
    }
    else if (modes[i]["name"] == "blinking full medium")
    {
      behaviours_[b_counter].blinking = 1;
      behaviours_[b_counter].speed = 2;
      behaviours_[b_counter].position = 0;
    }
    else if (modes[i]["name"] == "blinking full fast")
    {
      behaviours_[b_counter].blinking = 1;
      behaviours_[b_counter].speed = 1;
      behaviours_[b_counter].position = 0;
    }
    else if (modes[i]["name"] == "blinking halflow slow")
    {
      behaviours_[b_counter].blinking = 2;
      behaviours_[b_counter].speed = 1;
      behaviours_[b_counter].position = 0;
    }
    else if (modes[i]["name"] == "blinking halflow medium")
    {
      behaviours_[b_counter].blinking = 2;
      behaviours_[b_counter].speed = 2;
      behaviours_[b_counter].position = 0;
    }
    else if (modes[i]["name"] == "blinking halfhigh slow")
    {
      behaviours_[b_counter].blinking = 3;
      behaviours_[b_counter].speed = 1;
      behaviours_[b_counter].position = 0;
    }
    else if (modes[i]["name"] == "blinking halfhigh medium")
    {
      behaviours_[b_counter].blinking = 3;
      behaviours_[b_counter].speed = 2;
      behaviours_[b_counter].position = 0;
    }
    else if (modes[i]["name"] == "static open")
    {
      behaviours_[b_counter].blinking = 0;
      behaviours_[b_counter].speed = 0;
      behaviours_[b_counter].position = 1;
    }
    else if (modes[i]["name"] == "static middle")
    {
      behaviours_[b_counter].blinking = 0;
      behaviours_[b_counter].speed = 0;
      behaviours_[b_counter].position = 2;
    }
    else if (modes[i]["name"] == "static close")
    {
      behaviours_[b_counter].blinking = 0;
      behaviours_[b_counter].speed = 0;
      behaviours_[b_counter].position = 3;
    }
    else if (modes[i]["name"] == "static manual")
    {
      behaviours_[b_counter].blinking = 0;
      behaviours_[b_counter].speed = 0;
      behaviours_[b_counter].position = 4;
    }
    b_counter++;
  }
}

void Eye::eyeClicked(){
  // triggered when the button is clicked. It moves fast to the target point
  eyelid_actuator_->close(moving_time_ < FAST_MOVING_TIME? moving_time_ : FAST_MOVING_TIME);
  clicked_time_ = millis();
}

void Eye::eyelidPushed(){
  // triggered when the eyelid is pushed. It moves slow to the target point
  eyelid_actuator_->open(moving_time_ > SLOW_MOVING_TIME ? moving_time_ : SLOW_MOVING_TIME);
  eyelid->setPressing(true);
}

void Eye::eyelidReleased(){
  eyelid->setPressing(false);
}

void Eye::checkReadStatus(){
  switch (eyelid->read()){
    case -1:
      eyelid_read_ = MOVING_DOWN;
      break;
    case  0:
      eyelid_read_ = IDLE;
      break;
    case  1:
      eyelid_read_ = MOVING_UP;
      break;
    case  2:
      eyelid_read_ = READING;
      break;
  }
  return;
}

void Eye::manualProcess(){
  // NOTE: eyelid->read() : 2 -> not finished to read; 1 -> push up 0->idle -1-> push down
  if (eyelid_read_  == READING) {
    checkReadStatus();
  }
  if (eyelid_read_  == READING) return;  // if not return
  
  if (eyelid_read_ == MOVING_UP && current_max_position_ < EYELID_MAX_POSITION - 10) // IF push UP and NOT max -> increase current max position a bit
    current_max_position_ = current_max_position_ *= 1.5 > EYELID_MAX_POSITION ? EYELID_MAX_POSITION : current_max_position_ * 1.5;
  
  if (eyelid_read_ == MOVING_DOWN && current_max_position_ > EYELID_MIN_POSITION + 10) // IF push DOWN and NOT min -> decrease current max position a bit
    current_max_position_ = (current_max_position_ /= 2 < EYELID_MIN_POSITION + 10) ? EYELID_MIN_POSITION : current_max_position_ / 2;
  
  eyelid_actuator_->setMaxPosition(current_max_position_);
  eyelid_actuator_->open(moving_time_ > SLOW_MOVING_TIME ? moving_time_ : SLOW_MOVING_TIME);
  eyelid_read_  = READING;
  return; 
}

void Eye::process(){
/*  
  if (Eye::manual_static_) {               // CASE: MANUAL, no blinking but the eyelid position can be adjust manually
    manualProcess();
    return;
  }

  if (getEyelidPushed()) { 
    if (!eyelid->getPressing()) eyelidPushed();
    return;
  }   

  // if the eyelid has been pushed recently, it keeps the eye open
  if (eyelid->pushingTime()){ 
      eyelid_actuator_->setMaxPosition(EYELID_MAX_POSITION);      // set target position to FULLY open 
      eyelid_actuator_->open(moving_time_);                       // open to target position
      eyelid_actuator_->setMaxPosition(current_max_position_);    // set target position back to current_max_position
      return;
    }    
  else eyelidReleased();

  // CASE static eye in position -> they only open if eyelid pushed
  if (Eye::closing_in_period_ == Eye::period_) return; // ASSUMPTION: if static, react only to pushing up the eye

  if (getEyeClick()) { eyeClicked(); return;} 
*/
  // open-close action in period
  if (rest(millis()) < Eye::closing_in_period_) {
    eyelid_actuator_->open(moving_time_);
  }
  else {
    eyelid_actuator_->close(moving_time_);
  }
}

void Eye::setBlinking() {
#if EYE_DEBUG
  // speed -> behaviours_[current_behaviour_].speed
  // blinking -> behaviours_[current_behaviour_].blinking
  // position -> behaviours_[current_behaviour_].position
  int speed = 1;
  int blinking = 1;
  Serial.println("        ) Set Blinking Started");
#endif
  Eye::period_ = SLOW_PERIOD / speed; // time to complete a single blink
  moving_time_ = SLOW_MOVING_TIME / speed; // NOTE: REVIEW NEDEED!! ms the time to move from open to close and viceversa
  // STRONG ASSUMPION: the eyes are closed only for about 10% of the time. In real life the eye are open almost always.
  Eye::closing_in_period_ = (float)(random(RANDOM_CLOSE_TIME) + (SLOW_PERIOD - RANDOM_CLOSE_TIME)) / SLOW_PERIOD; // randomly create the closing duration (0.9 to 1)
#if EYE_DEBUG
  Eye::closing_in_period_ -= 0.5; // for debug the time open-close is almost 50-50
#endif
  Eye::manual_static_ = false;
  switch (blinking){
    case 0: 
      setStatic();
      break;
    case 1: 
      current_max_position_ = EYELID_MAX_POSITION;
      eyelid_actuator_->setMaxPosition(EYELID_MAX_POSITION);
      eyelid_actuator_->setMinPosition(EYELID_MIN_POSITION);
      break;
    case 2: 
      current_max_position_ = EYELID_HALF_POSITION;
      eyelid_actuator_->setMaxPosition(EYELID_HALF_POSITION);
      eyelid_actuator_->setMinPosition(EYELID_MIN_POSITION);
      break;
    case 3: 
      current_max_position_ = EYELID_MAX_POSITION;
      eyelid_actuator_->setMaxPosition(EYELID_MAX_POSITION);
      eyelid_actuator_->setMinPosition(EYELID_HALF_POSITION);
      break;
  }

  Serial.println("        ) Set Blinking Completed");
}

void Eye::setStatic() {
  Eye::closing_in_period_ = Eye::period_;
  switch (behaviours_[current_behaviour_].position){
    case 1:       
      current_max_position_ = EYELID_MIN_POSITION;
      eyelid_actuator_->setMaxPosition(EYELID_MIN_POSITION);
      eyelid_actuator_->open(FAST_MOVING_TIME);
      break;
    case 2: 
      current_max_position_ = EYELID_HALF_POSITION;
      eyelid_actuator_->setMaxPosition(EYELID_HALF_POSITION);
      eyelid_actuator_->open(FAST_MOVING_TIME);
      break;
    case 3: 
      current_max_position_ = EYELID_MAX_POSITION;
      eyelid_actuator_->setMaxPosition(EYELID_MAX_POSITION);
      eyelid_actuator_->open(FAST_MOVING_TIME);
      break;
    case 4: 
      eyelid_actuator_->setMaxPosition();
      Eye::manual_static_ = true;
      break;
  }
}

bool Eye::getEyeClick(){
  return eye_clicked_;
}

bool Eye::getEyelidPushed(){
  if (eyelid_read_  == READING) {               // if in the previous cycle we didnt finish to read
    checkReadStatus();
  }
  if (eyelid_read_  == IDLE || eyelid_read_  == READING) {
    eyelid_read_ = READING;
    return false; // Status: idle or reading
  }
  // Status eyelid pushed up or down
  eyelid_read_ = READING;
  return true;
}

float Eye::getLightValue(){
  return light_value_;
}

