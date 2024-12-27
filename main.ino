#define DEBUG                               // Comment out to disable debug prints

#include <EEPROM.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

volatile uint8_t WDT_flag = 0;

const int TRIG_PIN = 14;
const int ECHO_PIN = 15;

const int EEPROM_ADDR = 0;                  // Address in EEPROM to store count
const float DISTANCE_THRESHOLD = 100.0;     // Distance threshold in cm

float last_distance  = 0;
int people_count = 0;


void WDT_interrupt_enable(void) 
{
    // Reset Watchdog timer
    MCUSR &= ~(1<<WDRF);
    
    // Timed sequence for WDT changes
    WDTCSR |= (1<<WDCE)|(1<<WDE);
    // Set interrupt mode and around 250ms timeout (WDP2 only)
    WDTCSR = (1<<WDIE)|(1<<WDP2);
}

void interrupt_wait(void) 
{
    WDT_flag = 0;
    while (!WDT_flag);
}

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    while (!Serial);                // wait for Serial to initialize (important for Pro Micro)
#endif

    // Read stored count from EEPROM
    EEPROM.get(EEPROM_ADDR, people_count);
    
    // Configure pins
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);    // ensure trigger pin starts on LOW
    pinMode(ECHO_PIN, INPUT);

    WDT_interrupt_enable();
    sei();                          // Enable interrupts
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

#ifdef DEBUG
    Serial.print(distance);
    Serial.println(" cm");
#endif

    // Person detection logic
    if (last_distance > DISTANCE_THRESHOLD && distance < DISTANCE_THRESHOLD)
    {
        people_count++;
        EEPROM.put(EEPROM_ADDR, people_count);      // Store new count in EEPROM
#ifdef DEBUG
        Serial.print("Person detected! Count: ");
        Serial.println(people_count);
        delay(10);                                  // Assure prints (in deep sleep mode) 
#endif
    }

    last_distance = distance;
    
    // Go to sleep until next measurement (around 250ms)
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
    interrupt_wait();
}

ISR(WDT_vect) 
{
    WDT_flag = 1;
}