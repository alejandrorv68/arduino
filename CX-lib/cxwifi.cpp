#include "cxwifi.h"


CXWifi::CXWifi(){}

CXWifi::CXWifi(IPAddress server_ip, unsigned int port,
    String idType, String idVersion, String idIdentification){
  server_ = server_ip; // server IP
  port_  = port;
  idType_ = idType;
  idVersion_ = idVersion;
  idIdentification_ = idIdentification;
  pinMode(ESP_LED_PIN, OUTPUT);
  statusLED(false);
}

CXWifi::~CXWifi(){}


bool CXWifi::tryConnect(){
#if CXESP_DEBUG
  Serial.println("New connection attempt.");
#endif

  statusLED(true);
  client_.stop();
  statusLED(false);

  // If connected to a WiFi, but can't connect to a server, we *might* want
  // to break the WiFi and find another manikin. So, maybe some criteria?
  bool keepWifi = true;

  bool ok = false;
  if (!keepWifi || WiFi.status() != WL_CONNECTED)
  {
    statusLED(true);

    WiFi.disconnect(1);
    int n = WiFi.scanNetworks();

    statusLED(false);

    String bestSsid;
    int bestStrength = -9999;
    for (int i = 0; i < n; i++)
    {
      String ssid = WiFi.SSID(i);
        #if CXESP_DEBUG
              Serial.print("Scanning SSID: ");
              Serial.println(ssid);
        #endif
      if (!matchSsid(ssid)) continue;
        #if CXESP_DEBUG
              Serial.print("Found: ");
              Serial.println(ssid);
        #endif

      int strength = WiFi.RSSI(i);
      if (strength > bestStrength)
      {
        bestStrength = strength;
        bestSsid = ssid;
      }
    }

    if (bestSsid.length() == 0)
    {
#if CXESP_DEBUG
      Serial.println("Cannot find any SSID.");
#endif
      return false;
    }

#if CXESP_DEBUG
    Serial.print("Detected ");
    Serial.println(bestSsid);
#endif

    WiFi.begin(bestSsid.c_str(), tryPassword(bestSsid).c_str());

    for (int i = 0; i < 300 && !(ok = WiFi.status() == WL_CONNECTED); i++)
    {
#if CXESP_DEBUG
      Serial.print(".");
#endif
      statusLED(true);
      delay(30);
      statusLED(false);
      delay(70);
    }

    if (!ok)
    {
#if CXESP_DEBUG
      Serial.println("WiFi connection failed");
#endif
      return false;
    }
  }

#if CXESP_DEBUG
  Serial.println("Connected to WiFi. Connecting to server...");
#endif

  statusLED(true);
  ok = client_.connect(server_, port_);
  statusLED(false);

  if (!ok)
  {
#if CXESP_DEBUG
    Serial.println("FAILED.");
#endif
    if (keepWifi) delay(1000);
    return false;
  }

#if CXESP_DEBUG
  Serial.println("Successful!");
#endif
  delay(1000);

  return true;
}

void CXWifi::statusLED(bool state){
  digitalWrite(ESP_LED_PIN, state != ESP_LED_INVERTED);
}

char CXWifi::clientRead() {
  return client_.read();
}

bool CXWifi::clientAvailable() {
  return client_.available();
}


bool CXWifi::sendMessage(char id, const JsonObject &jsData){
  if (!client_.connected()) return false;
  String msg;
  jsData.printTo(msg);
  client_.write((String(id) + ";" + String(msg.length()) + ";" + msg + "\n").c_str());
  return true;
}

String CXWifi::getIdType(){
  return idType_;
}

String CXWifi::getIdVersion(){
  return idVersion_;
}

String CXWifi::getIdIdentification(){
  return idIdentification_;
}
