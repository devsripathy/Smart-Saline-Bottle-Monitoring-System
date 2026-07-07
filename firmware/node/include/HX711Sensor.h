#ifndef HX711_SENSOR_H
#define HX711_SENSOR_H

#include <Arduino.h>
#include <HX711.h>

class HX711Sensor
{
public:

    /*
    ============================================================
    Constructor
    ============================================================
    */

    HX711Sensor(
        uint8_t doutPin,
        uint8_t sckPin,
        float calibrationFactor,
        float emptyWeight,
        float fullWeight);

    /*
    ============================================================
    Initialization
    ============================================================
    */

    bool begin();

    /*
    ============================================================
    Sensor Operations
    ============================================================
    */

    void tare();

    float getWeight();

    float getFilteredWeight();

    /*
    ============================================================
    Bottle Information
    ============================================================
    */

    float getPercentage();

    bool bottlePresent();

    bool isBottleEmpty();

    bool isBottleLow();

    /*
    ============================================================
    Calibration
    ============================================================
    */

    void setCalibrationFactor(float factor);

    float getCalibrationFactor() const;

    /*
    ============================================================
    Diagnostics
    ============================================================
    */

    bool sensorHealthy();

private:

    /*
    ============================================================
    HX711 Object
    ============================================================
    */

    HX711 _scale;

    /*
    ============================================================
    Configuration
    ============================================================
    */

    uint8_t _doutPin;
    uint8_t _sckPin;

    float _calibrationFactor;

    float _emptyWeight;
    float _fullWeight;

    /*
    ============================================================
    Moving Average Filter
    ============================================================
    */

    static constexpr uint8_t FILTER_SIZE = 10;

    float _samples[FILTER_SIZE];

    uint8_t _sampleIndex;

    bool _bufferFilled;

    /*
    ============================================================
    Helper Functions
    ============================================================
    */

    float applyMovingAverage(float value);

    float constrainPercentage(float percent);
};

#endif
