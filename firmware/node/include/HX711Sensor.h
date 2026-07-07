#ifndef HX711_SENSOR_H
#define HX711_SENSOR_H

#include <Arduino.h>
#include <HX711.h>

class HX711Sensor
{
public:
    HX711Sensor(
        uint8_t doutPin,
        uint8_t sckPin,
        float calibrationFactor,
        float emptyWeight,
        float fullWeight);

    bool begin();

    float getWeight();

    float getFilteredWeight();

    float getPercentage();

    bool bottlePresent();

    bool isBottleEmpty();

    bool isBottleLow();

    bool sensorHealthy();

    void tare();

    void setCalibrationFactor(float factor);

    float getCalibrationFactor() const;

private:
    HX711 scale;

    uint8_t _doutPin;
    uint8_t _sckPin;

    float _calibrationFactor;

    float _emptyWeight;
    float _fullWeight;

    static const uint8_t FILTER_SIZE = 10;

    float samples[FILTER_SIZE];

    uint8_t index;

    bool filled;

    float applyMovingAverage(float value);

    float constrainPercentage(float percent);
};

#endif
