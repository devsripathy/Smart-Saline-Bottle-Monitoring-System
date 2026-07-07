#include "HX711Sensor.h"

HX711Sensor::HX711Sensor(
    uint8_t doutPin,
    uint8_t sckPin,
    float calibrationFactor,
    float emptyWeight,
    float fullWeight)
{
    _doutPin = doutPin;
    _sckPin = sckPin;

    _calibrationFactor = calibrationFactor;

    _emptyWeight = emptyWeight;
    _fullWeight = fullWeight;

    index = 0;
    filled = false;

    for (uint8_t i = 0; i < FILTER_SIZE; i++)
    {
        samples[i] = 0.0f;
    }
}

bool HX711Sensor::begin()
{
    scale.begin(_doutPin, _sckPin);

    if (!scale.is_ready())
    {
        Serial.println("[HX711] ERROR : Module not detected.");
        return false;
    }

    Serial.println("[HX711] Module detected.");

    scale.set_scale(_calibrationFactor);

    Serial.println("[HX711] Taring...");

    scale.tare(20);

    Serial.println("[HX711] Calibration Complete.");

    return true;
}

void HX711Sensor::tare()
{
    if (scale.is_ready())
    {
        scale.tare(20);

        Serial.println("[HX711] Tare Complete.");
    }
}

void HX711Sensor::setCalibrationFactor(float factor)
{
    _calibrationFactor = factor;

    scale.set_scale(_calibrationFactor);
}

float HX711Sensor::getCalibrationFactor() const
{
    return _calibrationFactor;
}

bool HX711Sensor::sensorHealthy()
{
    return scale.is_ready();
}

float HX711Sensor::getWeight()
{
    if (!sensorHealthy())
    {
        Serial.println("[HX711] Sensor Offline.");

        return -1.0f;
    }

    /*
        Read average of 10 samples.
        Removes electrical noise.
    */

    float weight = scale.get_units(10);

    /*
        Prevent tiny negative values caused
        by sensor drift.
    */

    if (weight < 0)
        weight = 0;

    return weight;
}
