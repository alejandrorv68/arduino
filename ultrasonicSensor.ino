/*
 * Ultrasonic sensor HC-SR04 works on 5V 
 * 
 * https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-an-ultrasonic-sensor-181cee
*/
int trig = 2; 
int echo = 3; 
long cm;

class Ultrasensor{
  char units; int reaDelay;
  public:
  //Constructors
  Ultrasensor();
  Ultrasensor(int);
  Ultrasensor(int, char);
  //Functions
  long readRaw();
  void defaultPrint();
  void values();
  void printRaw();
  
  };


void setup() 
{ 
  pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  Serial.begin(9600); 
  
}
void loop() 
{ 
  Ultrasensor sensor;
  sensor.defaultPrint();
  }



Ultrasensor::Ultrasensor(int dely){
  units = "cm";
  reaDelay = dely;
  }
Ultrasensor::Ultrasensor(int dely, char meas){
  units = meas;
  reaDelay = dely;
  }
Ultrasensor::Ultrasensor(){
  units = "cm";
  reaDelay = 1000;
  }
  
long Ultrasensor::readRaw(){
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  return pulseIn(echo, HIGH); 
  }

void Ultrasensor::defaultPrint(){
  long lecture_echo = readRaw();
  cm = lecture_echo / 58; 
  Serial.print("Distance in cm : "); 
  Serial.println(cm); 
  delay(reaDelay); 
  }
void Ultrasensor::printRaw(void){Serial.println(readRaw()); delay(reaDelay);}
