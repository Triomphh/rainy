#include <EEPROM.h>

const int TRIG_PIN = 14;
const int ECHO_PIN = 15;
const int EEPROM_ADDR = 0;  // Address in EEPROM to store count

const float DISTANCE_THRESHOLD = 100.0;     // distance threshold in cm

float last_distance = 0;
int people_count = 0;

void setup()
{
    Serial.begin(9600);
    while (!Serial);                // wait for Serial to initialize (important for Pro Micro)

    // Read stored count from EEPROM
    EEPROM.get(EEPROM_ADDR, people_count);
    
    // Configure pins
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);    // ensure trigger pin starts on LOW
    pinMode(ECHO_PIN, INPUT);
}

void loop()
{
    // Get distance reading
    float duration, distance;

    // Clear trigger pin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Send trigger pulse
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read echo pulse
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calculate distance in centimeters
    // Speed of sound in air is roughly 343m/s = 0.0343cm/microsec
    // distance = (duration * speed of sound (in air?) / 2)
    distance = (duration * 0.0343) / 2;

    Serial.print(distance);
    Serial.println(" cm");

    // Person detecion logic
    if (last_distance > DISTANCE_THRESHOLD && distance < DISTANCE_THRESHOLD)
    {
        people_count++;
        EEPROM.put(EEPROM_ADDR, people_count);  // Store new count in EEPROM
        Serial.print("Person detected! Count: ");
        Serial.println(people_count);
    }

    last_distance = distance;
    
    // wait before next reading
    delay(650); //ms
}