#include "skin.h"

Skin::Skin()
{
  ap_ = Adafruit_NeoPixel(NUM_PIXELS, pin_, NEO_GRB + NEO_KHZ800);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  ap_.begin(); 
}

Skin::Skin(int pin)
{
	pin_ = pin;
	ap_ = Adafruit_NeoPixel(NUM_PIXELS, pin_, NEO_GRB + NEO_KHZ800);
#if defined (__AVR_ATtiny85__)
	if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
	ap_.begin(); 
}

Skin::~Skin()
{
}

void Skin::initialization(JsonObject& data){
    for (int i = 0; i< data["Pixels"].size(); i++){ 
		pixels_[i].area = data["Pixels"][i].as<String>();
		//pixels_[i].area = data["Pixels"][i]["area"].as<String>();
		//pixels_[i].area = data["Pixels"][i]["factor_l"].as<String>();
		//pixels_[i].area = data["Pixels"][i]["factor_r"].as<String>();
		pixels_[i].index = i + 1;
	}
}

bool Skin::setColor(String area, int r, int g, int b){
  	for(int i = 0; i < NUM_PIXELS; i++)
	{
		if (area == pixels_[i].area) {
			pixels_[i].color[0] = r;
			pixels_[i].color[1] = g;
			pixels_[i].color[2] = b;
			ap_.setPixelColor(pixels_[i].index, ap_.Color(
				pixels_[i].color[0],
				pixels_[i].color[1],
				pixels_[i].color[2]));
			ap_.show();
		}
	}
}

bool Skin::setFactor(String area, double factor, bool left){
	for(int i = 0; i < NUM_PIXELS; i++)
	{
		if (pixels_[i].area == area){
			if (left) pixels_[i].factor_l = factor;
			else pixels_[i].factor_r = factor;
		}
	}
}

void Skin::update(const JsonObject& data){
	//TODO: activate the colors with the correct factors
	String hex = data["arg"].as<String>();
	int r = StringToHex(hex.substring(0,1), hex.substring(1,2));
	int g = StringToHex(hex.substring(2,3), hex.substring(3,4));
	int b = StringToHex(hex.substring(4,5), hex.substring(5,6));
	String msg_area = getAreaFromTopic(data["topic"].as<String>());
	setColor(msg_area, r, g, b);
}
void Skin::updateAllTopic(const JsonObject& data){
	//TODO: activate the colors with the correct factors
	data.printTo(Serial);
	for(int i = 0; i < NUM_PIXELS; i++)
	{
		// careful color uppercase
		int r = StringToHex(data["arg"][pixels_[i].area].as<String>().substring(0,1), data["arg"][pixels_[i].area].as<String>().substring(1,2));
		int g = StringToHex(data["arg"][pixels_[i].area].as<String>().substring(2,3), data["arg"][pixels_[i].area].as<String>().substring(3,4));
		int b = StringToHex(data["arg"][pixels_[i].area].as<String>().substring(4,5), data["arg"][pixels_[i].area].as<String>().substring(5,6));
		ap_.setPixelColor(pixels_[i].index, ap_.Color(
			pixels_[i].color[0],pixels_[i].color[1],pixels_[i].color[2]));
#if SKIN_DEBUG
	Serial.println("Index: ");
	Serial.println(pixels_[i].index);
	Serial.println("Color: ");
	Serial.println("R: ");
	Serial.println(r);
	Serial.println("G: ");
	Serial.println(g);
	Serial.println("B: ");
	Serial.println(b);
#endif
		ap_.setPixelColor(pixels_[i].index, ap_.Color(
			pixels_[i].color[0],pixels_[i].color[1],pixels_[i].color[2]));
		ap_.show();
	}
}

int Skin::StringToHex(String a, String b){
	char hexValue[] = {a.charAt(0),b.charAt(0)};
	byte tens = (hexValue[0] <= '9') ? hexValue[0] - '0' : (hexValue[0] <= 'F')  ? hexValue[0] - '7' : hexValue[0] - 'W' ;
	byte ones = (hexValue[1] <= '9') ? hexValue[0] - '0' : (hexValue[0] <= 'F')  ? hexValue[0] - '7' : hexValue[0] - 'W' ;

	return (16 * tens) + ones;
}

String Skin::getAreaFromTopic(String str){
	char buf[sizeof(str)*8];
    str.toCharArray(buf, sizeof(buf));
	char *p = buf;
    char *a;
    char *b;
	while ((a = strtok_r(p, "/", &p)) != NULL) {
#if SKIN_DEBUG
		Serial.println(a);
#endif
		b = a;
	}
	return String(b);
}

