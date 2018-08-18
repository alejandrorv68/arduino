#ifndef CXETH_H
#define CXETH_H


// Base code for an auto-connecting CommandX peripheral driven by an ESP-12
// Arduino-compatible WiFi-enabled microcontroller.
// Note that while it has up to 1MB space for the program, it has just 80KB
// available RAM of which a lot is already used by global variables.

#include <microclient-interface.h>
#include <Ethernet2.h>

#define CXETH_DEBUG 1 // debug: 1, no-debug: 0

class CXEth : public MicroclientInterface
{
private:

  EthernetClient client_;
  IPAddress server_;
  unsigned int port_;
  String idType_, idVersion_, idIdentification_;

public:

  CXEth();
  CXEth(IPAddress server_ip, unsigned int port, String idType, String idVersion, String idIdentification);
  ~CXEth();

  inline bool isConnected () { return client_.connected(); }

  bool tryConnect();
  bool sendMessage(char id, const JsonObject &data);
  bool clientAvailable();
  char clientRead();
  String getIdType();
  String getIdVersion();
  String getIdIdentification();
};

#endif // CXETH_H
