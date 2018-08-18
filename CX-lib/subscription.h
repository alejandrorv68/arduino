#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H


// Base code for an auto-connecting CommandX peripheral driven by an ESP-12
// Arduino-compatible WiFi-enabled microcontroller.
// Note that while it has up to 1MB space for the program, it has just 80KB
// available RAM of which a lot is already used by global variables.

// Note: careful with long messages

#include <ArduinoJson.h>
#define MAX_SUBSCRIPTIONS 20
#define SUBSCRIPTION_DEBUG 0
class Subscription
{

  struct subs{
    String topic;
    void (*cb)(const JsonObject &jo);    //NOTE call subs.cb[x].function(*)
  };
  typedef struct subs Subs;

private:
  Subs list_[MAX_SUBSCRIPTIONS];
  int num_subscriptions_;

public:
  Subscription();
  ~Subscription();
  void clear();
  void add(String t, void(*f)(const JsonObject& jo));
  bool contains(String topic);
  bool execute(const JsonObject &data);
  int getNumSubscriptions();
  String getTopic(int i);

};

#endif // SUBSCRIPTION_H
