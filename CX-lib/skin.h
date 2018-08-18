#ifndef SKIN_H
#define SKIN_H

#include <ArduinoJson.h>

/// LED settings------------
#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS 8 // TODO make it dynamic in init

#define SKIN_DEBUG 1
class Skin 
{
private:
  struct pixel{
    int color[3]; // in the order RGB
    int index;
    double factor_l;
    double factor_r;
    String area;
  };
  pixel pixels_[NUM_PIXELS];
  Adafruit_NeoPixel ap_;
  int pin_;

  String getAreaFromTopic(String str);
  int StringToHex(String a, String b);

public:
  Skin();
  Skin(int pin);
  ~Skin();
  void initialization(JsonObject& data);
  bool setColor(String area, int r, int g, int b);
  bool setFactor(String area, double factor, bool left);
  void update(const JsonObject& data);
  void updateAllTopic(const JsonObject& data);
};

#endif // SKIN_H
