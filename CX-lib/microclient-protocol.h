#ifndef MICROCLIENTPROTOCOL_H
#define MICROCLIENTPROTOCOL_H


#include <cxeth.h>
#include <cxwifi.h>
#include <subscription.h>
#include <pulse.h>
#include <synchronization.h>

// Available requests
#define R_SUBSCRIBE 's' // Listen to server events
#define R_TRIGGER 'R'   // Event triggered
#define R_IDENTIFY 'Y' // Client lets server know who and what it is
#define R_SYNCH_TIME 't'

#define MAX_DATA_LENGTH 3000
#define SYNCHRONIZATION_INTERVAL_MS 5000

#define PROTOCOL_DEBUG 1
class MicroclientProtocol
{
  enum ReceiveState
  {
    Idle, GotMessageId, DataLength, Data, BadState, IgnoreState
  };

  private:

    //std::map<String, std::map<String, JsonObject>> subscriptions;
    MicroclientInterface *mc_;
    Synchronization* synchronization_;
    Subscription* subscriptions_;

    char receiveState_, receiveMessageChar_;
    char *receiveBuffer_ = NULL;
    unsigned long int receiveTotal_ = 0;
    unsigned int receiveDone_, receiveBufferSize_ = 0;
    unsigned int last_update_5sec_ = 0;
    String acceptedRequests_;
    StaticJsonBuffer<10000> jsonbuffer_;
    bool subscribed_;

    void addEnabledFeature(JsonObject& root, String key);

  public:
    MicroclientProtocol(){}
    MicroclientProtocol(MicroclientInterface *mc, Synchronization *synch);
    ~MicroclientProtocol();
    bool identification();
    bool synchronization();
    bool addSubscription(const String topic, void(*f)(const JsonObject & jo));
    bool subscribe();
    bool trigger(const String topic, const JsonObject &jsData);
    void reset();
    void tryReceive();
    void update();
    virtual void handleMessage(char id, const JsonObject &jsData);
};

#endif // MICROCLIENTPROTOCOL_H
