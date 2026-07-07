#include <Arduino.h>

#include "Config.h"
#include "HX711Sensor.h"
#include "IRSensor.h"
#include "SensorFusion.h"
#include "AlertManager.h"
#include "LoRaManager.h"

/*
============================================================
Objects
============================================================
*/

HX711Sensor hx711(
    HX711_DOUT_PIN,
    HX711_SCK_PIN,
    CALIBRATION_FACTOR,
    EMPTY_BOTTLE_WEIGHT,
    FULL_BOTTLE_WEIGHT);

IRSensor irSensor(IR_SENSOR_PIN);

SensorFusion fusion;

AlertManager alerts(STATUS_LED);

LoRaManager lora(
    LORA_FREQUENCY,
    LORA_SS,
    LORA_RST,
    LORA_DIO0);

/*
============================================================
Timers
============================================================
*/

unsigned long lastSensorRead = 0;
unsigned long lastLoRaSend = 0;

/*
============================================================
Latest Sensor Values
============================================================
*/

float weight = 0.0f;
float percentage = 0.0f;
float flowRate = 0.0f;

uint32_t dripCount = 0;

bool bottlePresent = false;
bool bottleLow = false;
bool bottleEmpty = false;
bool noDrip = false;

bool sensorHealthy = false;

SalineStatus status = SalineStatus::UNKNOWN;

/*
============================================================
Setup
============================================================
*/

void setup()
{
    Serial.begin(SERIAL_BAUD);

    while (!Serial)
    {
        delay(10);
    }

    Serial.println();
    Serial.println("======================================");
    Serial.println(" Smart Saline Monitoring System");
    Serial.println(" Firmware Version: " FIRMWARE_VERSION);
    Serial.println("======================================");

    if (!hx711.begin())
    {
        Serial.println("HX711 initialization failed.");
    }

    if (!irSensor.begin())
    {
        Serial.println("IR Sensor initialization failed.");
    }

    if (!alerts.begin())
    {
        Serial.println("Alert Manager initialization failed.");
    }

    if (!lora.begin())
    {
        Serial.println("LoRa initialization failed.");
    }

    Serial.println("System initialized.");
}

/*
============================================================
Read Sensors
============================================================
*/

void readSensors()
{
    irSensor.update();

    sensorHealthy =
        hx711.sensorHealthy() &&
        irSensor.sensorHealthy();

    weight = hx711.getFilteredWeight();

    percentage = hx711.getPercentage();

    bottlePresent = hx711.bottlePresent();

    bottleLow = hx711.isBottleLow();

    bottleEmpty = hx711.isBottleEmpty();

    dripCount = irSensor.getDripCount();

    flowRate = irSensor.getFlowRate();

    noDrip = irSensor.noDrip();
}

/*
============================================================
Process Sensor Fusion
============================================================
*/

void processFusion()
{
    status = fusion.evaluate(
        sensorHealthy,
        bottlePresent,
        bottleLow,
        bottleEmpty,
        noDrip,
        percentage);

    alerts.update(status);
}

/*
============================================================
Transmit Data
============================================================
*/

void sendTelemetry()
{
    bool success =
        lora.sendPacket(
            DEVICE_ID,
            weight,
            percentage,
            dripCount,
            flowRate,
            fusion.statusToString(status));

    Serial.print("LoRa Transmission: ");

    if (success)
    {
        Serial.println("SUCCESS");
    }
    else
    {
        Serial.println("FAILED");
    }
}

/*
============================================================
Debug Output
============================================================
*/

void printDebug()
{
    Serial.println();

    Serial.println("---------------");

    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" g");

    Serial.print("Saline: ");
    Serial.print(percentage);
    Serial.println(" %");

    Serial.print("Bottle Present: ");
    Serial.println(bottlePresent);

    Serial.print("Bottle Low: ");
    Serial.println(bottleLow);

    Serial.print("Bottle Empty: ");
    Serial.println(bottleEmpty);

    Serial.print("Drips: ");
    Serial.println(dripCount);

    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.println(" drops/min");

    Serial.print("No Drip: ");
    Serial.println(noDrip);

    Serial.print("Status: ");
    Serial.println(fusion.statusToString(status));

    Serial.println("---------------");
}

/*
============================================================
Loop
============================================================
*/

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - lastSensorRead >= SENSOR_INTERVAL)
    {
        lastSensorRead = currentMillis;

        readSensors();

        processFusion();

        printDebug();
    }

    if (currentMillis - lastLoRaSend >= LORA_INTERVAL)
    {
        lastLoRaSend = currentMillis;

        sendTelemetry();
    }
}
