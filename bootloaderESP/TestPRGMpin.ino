/*
 * Test functionality of the Rasp PI 
 * automatic bootloader sequence for ESP. Use in 
 * combination with bootloader.py and esptool.py
 * The code runs a version of blink until a reset is
 * triggered. The versions of blink are associated to
 * cases where the prgm pin is up and low. Note value
 * is declared in setup and there is a 1s delay
*/

int rstPinValue;  // Value of the reset pin 0 or 1 (rst in bootloader.py)

void setup(){
  Serial.begin(115200);
  delay(1000);    // This delay is used to prevent the ESP from going to bootloader mode when setting the pins
  pinMode(0,INPUT); // Pin 0 is the PRGM pin in ESP8266
  pinMode(LED_BUILTIN, OUTPUT);
  rstPinValue = digitalRead(0); // Read value at pin

}

void loop() {

  // When the value of rst is low, the first case will be triggered, in a similar fashion to the config mode and final program
  if(rstPinValue==0){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
    }
    else if (rstPinValue==1){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
      
      }
  Serial.println(rstPinValue);
  delay(500);

  /**/

}
