#include <EEPROM.h>

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  
  // Reset people count to 0
  int reset_value = 0;
  EEPROM.put(0, reset_value);
  
  Serial.println("EEPROM counter has been reset to 0");
}

void loop() 
{}
