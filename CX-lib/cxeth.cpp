#include "cxeth.h"

CXEth::CXEth(IPAddress server_ip, unsigned int port,
    String idType, String idVersion, String idIdentification){
  server_ = server_ip; // server IP
  port_  = port;
  idType_ = idType;
  idVersion_ = idVersion;
  idIdentification_ = idIdentification;
}

CXEth::CXEth(){}

CXEth::~CXEth(){}

bool CXEth::tryConnect(){
#if CXETH_DEBUG
  Serial.println("Try connecting...");
#endif
  if (!client_.connected())
  {
#if CXETH_DEBUG
        Serial.print("With  ");Serial.print(server_);Serial.print("   ");Serial.println(port_);
#endif
    if (!client_.connect(server_, port_)) {
#if CXETH_DEBUG
      Serial.println("disconnecting.");
#endif      
      client_.stop();
      return false;
    }
  }
#if CXETH_DEBUG
  Serial.println("ETH: Connected!");
#endif
  return true;
}

bool CXEth::clientAvailable() {
  return client_.available();
}

char CXEth::clientRead() {
  return client_.read();
}

bool CXEth::sendMessage(char id, const JsonObject &jsData){
  if (!client_.connected()) return false;
  String msg;
  jsData.printTo(msg);
  client_.write((String(id) + ";" + String(msg.length()) + ";" + msg + "\n").c_str());
  return true;
}

String CXEth::getIdType(){
  return idType_;
}

String CXEth::getIdVersion(){
  return idVersion_;
}

String CXEth::getIdIdentification(){
  return idIdentification_;
}
