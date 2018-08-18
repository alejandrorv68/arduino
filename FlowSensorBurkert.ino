/* Code for the flow sensor 8031. http://www.wjf.ca/5021.pdf 
Connect a 1600R resistor between the signal and the ground wires. 
Wiring:
Green -> Signal
Brown -> Ground
White -> 5V-24V
*/
void setup() {
  Serial.begin(9600);
}
void loop() {
  // Read
  double sensorValue = pulseIn(A5,HIGH);
  double convert2flow = (100*1e6)/(4*780*sensorValue );
  //Serial.println(sensorValue);
  Serial.println(convert2flow);  
  delay(400);
  //Frequency ranges 0 780 Hz 100 l/h  
}
