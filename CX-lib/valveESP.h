#ifndef VALVEESP_H
#define VALVEESP_H

#define VALVE_UPDATE_INTERVAL 2000

class ValveESP
{
private:
	int index_;
public:
	ValveESP();
	ValveESP(int index):index_(index){ 

  		Serial.print("                  Pin  ");
  		Serial.println(index);
		pinMode(index_, OUTPUT);
	}
	~ValveESP();
	void turnOn(){digitalWrite(index_, HIGH);} //on
	void turnOff(){digitalWrite(index_, LOW);} //off
};

#endif // VALVEESP_H
