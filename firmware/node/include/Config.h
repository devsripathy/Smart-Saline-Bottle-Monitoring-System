#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/*
==========================================
Device Information
==========================================
*/

#define DEVICE_ID           "WARDA_BED03"
#define HOSPITAL_NAME       "Smart Hospital"

#define FIRMWARE_VERSION    "1.0.0"

/*
==========================================
HX711
==========================================
*/

#define HX711_DOUT_PIN      4
#define HX711_SCK_PIN       5

#define CALIBRATION_FACTOR  -7050.0f

#define EMPTY_BOTTLE_WEIGHT 480.0f
#define FULL_BOTTLE_WEIGHT  980.0f

/*
==========================================
IR Sensor
==========================================
*/

#define IR_SENSOR_PIN       15

/*
==========================================
LED
==========================================
*/

#define STATUS_LED          2

/*
==========================================
LoRa
==========================================
*/

#define LORA_SS             18
#define LORA_RST            14
#define LORA_DIO0           26

#define LORA_FREQUENCY      433E6

/*
==========================================
Alert Levels
==========================================
*/

#define LOW_LEVEL_PERCENT       20
#define CRITICAL_PERCENT        5

/*
==========================================
Timing
==========================================
*/

#define SENSOR_INTERVAL     1000
#define LORA_INTERVAL       5000

/*
==========================================
Debug
==========================================
*/

#define SERIAL_BAUD         115200

#endif
