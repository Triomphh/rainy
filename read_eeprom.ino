#include <EEPROM.h>

const int DAILY_COUNT_ADDR = 10;     // Starting EEPROM address for daily counts
const int MAX_DAYS = 30;

void setup() 
{
    Serial.begin(9600);
    while (!Serial);  // wait for Serial connection
}

void loop() 
{
    if (Serial.available() > 0) 
    {
        char cmd = Serial.read();
        
        if (cmd == 'G' || cmd == 'g') 
        {
            // Read total count from address 0
            int total_count;
            EEPROM.get(0, total_count);
            Serial.print("Total count: ");
            Serial.println(total_count);

            Serial.println("\nDaily counts :");
            Serial.println("Day\tCount");
            Serial.println("---\t-----");
            
            for (int i = 0; i < MAX_DAYS; i++) 
            {
                int count;
                EEPROM.get(DAILY_COUNT_ADDR + (i * sizeof(int)), count);
                
                Serial.print(i + 1);
                Serial.print("\t");
                Serial.println(count);
            }
            Serial.println();
        }
    }
} 