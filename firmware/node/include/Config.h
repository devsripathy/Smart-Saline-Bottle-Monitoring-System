#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/*
============================================================
Project Information
============================================================
*/

#define PROJECT_NAME            "Smart Saline Monitoring System"
#define FIRMWARE_VERSION        "1.0.0"

/*
============================================================
Device Information
============================================================
*/

#define DEVICE_ID               "WARDA_BED03"
#define HOSPITAL_NAME           "Smart Hospital"

/*
============================================================
Serial Configuration
============================================================
*/

#define SERIAL_BAUD             115200

/*
============================================================
ESP32 Pin Configuration
============================================================
*/

/* HX711 */
#define HX711_DOUT_PIN          4
#define HX711_SCK_PIN           5

/* IR Sensor */
#define IR_SENSOR_PIN           15

/* Status LED */
#define STATUS_LED_PIN          2

/****************************************************
 LoRa SX1278 Pins
****************************************************/

#define LORA_SS_PIN             18
#define LORA_RST_PIN            14
#define LORA_DIO0_PIN           26

/*
============================================================
LoRa Configuration
============================================================
*/

#define LORA_FREQUENCY          433E6

#define LORA_SYNC_WORD          0x34

#define LORA_TX_POWER           17

/*
============================================================
Load Cell Calibration
============================================================
*/

#define CALIBRATION_FACTOR      -7050.0f

/*
Bottle Weights (grams)

Adjust according to your bottle.
*/

#define EMPTY_BOTTLE_WEIGHT     480.0f

#define FULL_BOTTLE_WEIGHT      980.0f

/*
============================================================
Alert Thresholds
============================================================
*/

#define LOW_LEVEL_PERCENT       20.0f

#define CRITICAL_LEVEL_PERCENT  5.0f

/*
============================================================
IR Sensor Parameters
============================================================
*/

#define DRIP_DEBOUNCE_MS        40

#define NO_DRIP_TIMEOUT_MS      10000

#define FLOW_WINDOW_MS          60000

/*
============================================================
Sensor Timing
============================================================
*/

#define SENSOR_READ_INTERVAL    1000

#define LORA_SEND_INTERVAL      5000

#define DEBUG_PRINT_INTERVAL    1000

/*
============================================================
Moving Average Filter
============================================================
*/

#define HX711_FILTER_SIZE       10

/*
============================================================
Safety Limits
============================================================
*/

#define MAX_VALID_WEIGHT        5000.0f

#define MIN_VALID_WEIGHT        (-100.0f)

/*
============================================================
Future OTA Support
============================================================
*/

#define OTA_ENABLED             false

/*
============================================================
Debug Configuration
============================================================
*/

#define ENABLE_SERIAL_DEBUG     true

#define ENABLE_SENSOR_DEBUG     true

#define ENABLE_LORA_DEBUG       true

#endif
