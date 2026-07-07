#include <Arduino.h>
#include "Config.h"

void setup()
{
    Serial.begin(SERIAL_BAUD);

    pinMode(STATUS_LED, OUTPUT);

    pinMode(IR_SENSOR_PIN, INPUT);

    Serial.println("--------------------------------");
    Serial.println("Smart Saline Monitoring System");
    Serial.println(FIRMWARE_VERSION);
    Serial.println("--------------------------------");

    digitalWrite(STATUS_LED, HIGH);

    delay(1000);

    digitalWrite(STATUS_LED, LOW);
}

void loop()
{
    static unsigned long previous = 0;

    if (millis() - previous >= SENSOR_INTERVAL)
    {
        previous = millis();

        Serial.println("System Running...");
    }
}
