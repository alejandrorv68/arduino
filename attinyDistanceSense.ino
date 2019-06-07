/*
 * UltrasonicSensor+LED+attiny85
 * https://create.arduino.cc/projecthub/arjun/programming-attiny85-with-arduino-uno-afb829
 * Ultrasonic sensor HC-SR04 works on 5V
 * If distance (cm-) is smaller than min_dist value, LED turns on
 * https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-an-ultrasonic-sensor-181cee
*/
int min_dist = 10
int trig = 1; 
int echo = 2; 
int led = 0;
long lecture_echo; 
long cm;
void setup() 
{ 
  pinMode(trig, OUTPUT); 
  pinMode(led, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  //Serial.begin(9600); 
}
void loop() 
{ 
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  //Serial.print("Distance in cm : "); 
  //Serial.println(cm); 
  if(cm<min_dist){digitalWrite(led,HIGH);}
  else{digitalWrite(led,LOW);}
  delay(1000); 
}
