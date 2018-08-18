#include "subscription.h"

Subscription::Subscription()
{
  num_subscriptions_ = 0;
}
Subscription::~Subscription()
{
}

void Subscription::add(String topic, void(*f)(const JsonObject &jo))
{
  if (contains(topic))
  {
    for (int i = 0; i < num_subscriptions_; i++){
      if (list_[i].topic == topic){
        list_[i].cb = *f;
        return;
      }
    }
  }
  else
  {
    if (num_subscriptions_ < MAX_SUBSCRIPTIONS)
    {
      #if SUBSCRIPTION_DEBUG
            Serial.println("NEW TOPIC ");
            Serial.println(sizeof(list_));
            Serial.println(num_subscriptions_);
      #endif
      Subs newTopic;
      newTopic.topic = topic;
      newTopic.cb = *f;
      list_[num_subscriptions_] = newTopic;
      #if SUBSCRIPTION_DEBUG
            Serial.print("NEW TOPIC ADDED: ");
            Serial.println(list_[num_subscriptions_].topic);
      #endif
      num_subscriptions_++;
      #if SUBSCRIPTION_DEBUG
            Serial.print("Number subscriptions: ");
            Serial.println(num_subscriptions_);
      #endif
    }

  }
}

bool Subscription::contains(String topic)
{
  for (int i = 0; i < num_subscriptions_; i++){
    if (list_[i].topic == topic){
      return true;
    }
  }
  return false;
}

bool Subscription::execute(const JsonObject &data)
{
  String topic = data["topic"].as<String>();
  if (!contains(topic))
  {
    #if SUBSCRIPTION_DEBUG
    Serial.println("ERROR: message input error");
    data.printTo(Serial);
    Serial.print("TOPIC:    ");
    Serial.println(topic);
    #endif
    return false;
  }
  for (int i = 0; i < num_subscriptions_; i++){
    if (list_[i].topic == topic){
      list_[i].cb(data);
      return true;
    }
  }
  return false;
}

void Subscription::clear()
{
  num_subscriptions_ = 0;
}

int Subscription::getNumSubscriptions()
{
  return num_subscriptions_;
}

String Subscription::getTopic(int i)
{
  return list_[i].topic;
}
