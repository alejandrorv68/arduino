#include "microclient-protocol.h"


MicroclientProtocol::MicroclientProtocol(MicroclientInterface *mc, Synchronization *synch)
{
  mc_ = mc;
  synchronization_ = synch;
  subscribed_ = false;
  receiveState_ = Idle;
  subscriptions_ = new Subscription();
}

MicroclientProtocol::~MicroclientProtocol()
{
}

void MicroclientProtocol::update()
{
  if (!mc_->isConnected())
  {
    Serial.println("Not connected!");
    if (!mc_->tryConnect())
    {
      Serial.println("Connection error");
      return;
    }
    subscribed_ = false;
    identification();
    return;
  }
  if(!subscribed_)
  {
    subscribed_ = subscribe();
  }
  if (millis() - last_update_5sec_ > SYNCHRONIZATION_INTERVAL_MS)
  {
    if (!synchronization()) Serial.println("Synchronization failed");
    last_update_5sec_ = millis();
  }
  tryReceive();
}

bool MicroclientProtocol::identification()
{
  // Make known who we are
  JsonObject& jsData = jsonbuffer_.createObject();
  jsData["description"] = mc_->getIdType();
  jsData["version"] = mc_->getIdVersion();
  JsonObject& feature = jsData.createNestedObject("features");
  addEnabledFeature(feature, "status/parameters/environment/light_sensor_eye_l");
  addEnabledFeature(feature, "status/parameters/environment/light_sensor_eye_r");  
  addEnabledFeature(feature, "status/parameters/environment/light_sensor_cheek_l");  
  addEnabledFeature(feature, "status/parameters/environment/light_sensor_cheek_r"); 
  addEnabledFeature(feature, "status/presets/skin/colors/cheek_l");  
  addEnabledFeature(feature, "status/presets/skin/colors/cheek_r");  
  addEnabledFeature(feature, "status/presets/skin/colors/chin");  
  addEnabledFeature(feature, "status/parameters/swollen_cheek_l");  
  addEnabledFeature(feature, "status/parameters/swollen_cheek_r");  
  addEnabledFeature(feature, "status/presets/respiration/paraglossia");    
  addEnabledFeature(feature, "status/presets/fluids/tears");     
  addEnabledFeature(feature, "status/presets/fluids/ears");     
  addEnabledFeature(feature, "status/presets/fluids/nose");  
  addEnabledFeature(feature, "status/presets/fluids/mouth");  
  addEnabledFeature(feature, "status/presets/fluids/forehead");  
  //if (mc_->getIdIdentification().length() > 0) jsData["id"] = mc_->getIdIdentification();
  jsData.prettyPrintTo(Serial);
  mc_->sendMessage(R_IDENTIFY, jsData);
  jsonbuffer_.clear();
}

bool MicroclientProtocol::addSubscription(const String topic, void(*f)(const JsonObject& jo))
{
  subscriptions_->add(topic, *f);
  return true;
}

bool MicroclientProtocol::subscribe()
{
  JsonObject& data = jsonbuffer_.createObject();
  for (int i = 0; i < subscriptions_->getNumSubscriptions(); i++)
  {
    data["topic"] = subscriptions_->getTopic(i);
    if (!mc_->sendMessage(R_SUBSCRIBE, data))
    {
      jsonbuffer_.clear();
      return false;
    }
  }
  jsonbuffer_.clear();
  return true;
}

bool MicroclientProtocol::trigger(const String topic, const JsonObject &jsData)
{
  JsonObject& data = jsonbuffer_.createObject();
  data["topic"] = topic;
  data["arg"] = jsData;
  mc_->sendMessage(R_TRIGGER, data);
  jsonbuffer_.clear();
  return true;
}


bool MicroclientProtocol::synchronization()
{
  if (!mc_->isConnected()) {return false;}
  Serial.println("synchronization...");
  JsonObject& data= jsonbuffer_.createObject();
  data["client_time"] = millis();
  mc_->sendMessage(R_SYNCH_TIME, data);
  jsonbuffer_.clear();
  return true;
}

void MicroclientProtocol::reset()
{
    subscriptions_->clear();
}

void MicroclientProtocol::tryReceive()
{
  enum ReceiveState
  {
    Idle, GotMessageId, DataLength, Data, BadState, IgnoreState
  };

  unsigned int now = millis();
  if (!mc_->isConnected()) { receiveState_ = Idle; return; }
  while(mc_->clientAvailable() && ((millis() - now) <= 3) )
  {
    char c = mc_->clientRead();
    switch(c)
    {
    case 10:
    case 13:
      // Newline: process any active message & get ready for the next
      if (receiveState_ == Data)
      {
        receiveBuffer_[receiveTotal_] = '\0';
        JsonObject& jo = jsonbuffer_.parseObject(receiveBuffer_);
        handleMessage(receiveMessageChar_, jo);
        jsonbuffer_.clear();
      }

    case 0:
      receiveState_ = Idle;
      break;

    default:

      // Expect messages like for ex. "z;3;abc" where the 'z' is one character
      // identifying the message, "3" tells the data length and "abc" is our data.
      // Will read 1 byte at a time so that the function blocks for a minimal time

      switch (receiveState_)
      {
        case Idle:
          // First char identifies the message type
          receiveMessageChar_ = c;
          receiveTotal_ = 0;
          receiveDone_ = 0;
          receiveState_ = GotMessageId;
          break;

        case GotMessageId:
          if (c != ';') {receiveState_ = BadState; Serial.println("Badstate");}
          else { receiveState_ = DataLength; }
          break;

        case DataLength:
          if (c == ';')
          {
            receiveState_ = Data;
            // Note one extra byte for null terminator
            if (receiveTotal_+1 > receiveBufferSize_)
            {
              receiveBufferSize_ = receiveTotal_+1;
              if (receiveBuffer_) delete[] receiveBuffer_;
              receiveBuffer_ = new char[receiveBufferSize_];
            }
          }
          else if (c < '0' || c > '9') receiveState_ = BadState;
          else
          {
            receiveTotal_ = (receiveTotal_ * 10) + c - '0';
            if (receiveTotal_ > MAX_DATA_LENGTH){receiveState_ = BadState; Serial.println("Badstate");}
          }
          break;

        case Data:
          if (receiveDone_ >= receiveTotal_) {receiveState_ = BadState; Serial.println("Badstate");}
          else {receiveBuffer_[receiveDone_++] = c; }
          break;
      }
      break;
    }
  }
}

void MicroclientProtocol::handleMessage(char id, const JsonObject &jsData)
{
  //jsData.printTo(Serial);
  if (id == R_TRIGGER)
  {
    subscriptions_->execute(jsData);
  }
  if (id == R_SYNCH_TIME)
  {
    synchronization_->setServerTime(jsData);
  }
}


void MicroclientProtocol::addEnabledFeature(JsonObject& root, String key)
{
    JsonObject& new_line = root.createNestedObject(key);
    new_line["enabled"] = 1;
}
