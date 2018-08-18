#ifndef CXWIFI_H
#define CXWIFI_H


// Base code for an auto-connecting CommandX peripheral driven by an ESP-12
// Arduino-compatible WiFi-enabled microcontroller.
// Note that while it has up to 1MB space for the program, it has just 80KB
// available RAM of which a lot is already used by global variables.

// Note: careful with long messages
#include <microclient-interface.h>
#include <ESP8266WiFi.h>

#define CXESP_DEBUG 1

#define ESP_LED_PIN 2
#define ESP_LED_INVERTED true

class CXWifi: public MicroclientInterface
{
private:

  WiFiClient client_;
  IPAddress server_;
  unsigned int port_;

  String idType_, idVersion_, idIdentification_;

public:

    CXWifi();
    CXWifi(IPAddress server_ip, unsigned int port, String idType, String idVersion, String idIdentification);
    ~CXWifi();

    inline bool isConnected () { return client_.connected(); }
    
    bool tryConnect();
    void statusLED(bool state);
    bool sendMessage(char id, const JsonObject &data);
    bool clientAvailable();
    char clientRead();
    String getIdType();
    String getIdVersion();
    String getIdIdentification();

    virtual bool matchSsid(const String &ssid)
    {
      //return ssid.indexOf("~") >= 0 &&
      return (ssid.startsWith("Arch-Box") || ssid.startsWith("NENA ") || ssid.startsWith("ADAM ") || ssid.startsWith("MAMA "));
    }
    virtual String tryPassword(const String &ssid)
    {
      //return "NENA_SIM"; //"COMMAND_X";
      return "123456789"; //"Arch-Box";
    }
};

#endif // CXWIFI_H
