#include <EEPROM.h>

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  
  // Reset people count to 0
  int reset_value = 0;
  EEPROM.put(0, reset_value);
  
  // Reset all 30 daily counts to 0
  for(int i = 0; i < 30; i++) {
    EEPROM.put(10 + (i * sizeof(int)), reset_value);
  }
  
  // Reset last day counter to 0
  unsigned long reset_day = 0;
  EEPROM.put(134, reset_day);
  
  Serial.println("EEPROM has been reset: main counter, daily counts, and day counter all set to 0");
}

void loop() 
{}
