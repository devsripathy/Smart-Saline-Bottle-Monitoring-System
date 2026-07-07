#include "HX711Sensor.h"

HX711Sensor::HX711Sensor(
    uint8_t doutPin,
    uint8_t sckPin,
    float calibrationFactor,
    float emptyWeight,
    float fullWeight)
    : _doutPin(doutPin),
      _sckPin(sckPin),
      _calibrationFactor(calibrationFactor),
      _emptyWeight(emptyWeight),
      _fullWeight(fullWeight),
      index(0),
      filled(false)
{
    for (uint8_t i = 0; i < FILTER_SIZE; i++)
    {
        samples[i] = 0.0f;
    }
}

bool HX711Sensor::begin()
{
    scale.begin(_doutPin, _sckPin);

    delay(500);

    if (!scale.is_ready())
    {
        return false;
    }

    scale.set_scale(_calibrationFactor);

    tare();

    return true;
}

void HX711Sensor::tare()
{
    if (scale.is_ready())
    {
        scale.tare();
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
        return -1.0f;
    }

    return scale.get_units(10);
}

float HX711Sensor::applyMovingAverage(float value)
{
    samples[index] = value;

    index++;

    if (index >= FILTER_SIZE)
    {
        index = 0;
        filled = true;
    }

    uint8_t count = filled ? FILTER_SIZE : index;

    if (count == 0)
    {
        return value;
    }

    float sum = 0.0f;

    for (uint8_t i = 0; i < count; i++)
    {
        sum += samples[i];
    }

    return sum / static_cast<float>(count);
}

float HX711Sensor::getFilteredWeight()
{
    float weight = getWeight();

    if (weight < 0.0f)
    {
        return weight;
    }

    return applyMovingAverage(weight);
}

float HX711Sensor::constrainPercentage(float percent)
{
    if (percent < 0.0f)
    {
        return 0.0f;
    }

    if (percent > 100.0f)
    {
        return 100.0f;
    }

    return percent;
}

float HX711Sensor::getPercentage()
{
    float weight = getFilteredWeight();

    if (weight < 0.0f)
    {
        return 0.0f;
    }

    float usableWeight = _fullWeight - _emptyWeight;

    if (usableWeight <= 0.0f)
    {
        return 0.0f;
    }

    float percentage =
        ((weight - _emptyWeight) / usableWeight) * 100.0f;

    return constrainPercentage(percentage);
}

bool HX711Sensor::bottlePresent()
{
    constexpr float PRESENT_THRESHOLD = 50.0f;

    float weight = getFilteredWeight();

    if (weight < 0.0f)
    {
        return false;
    }

    return weight >= PRESENT_THRESHOLD;
}

bool HX711Sensor::isBottleEmpty()
{
    constexpr float EMPTY_THRESHOLD = 5.0f;

    if (!bottlePresent())
    {
        return false;
    }

    return getPercentage() <= EMPTY_THRESHOLD;
}

bool HX711Sensor::isBottleLow()
{
    constexpr float LOW_THRESHOLD = 20.0f;
    constexpr float EMPTY_THRESHOLD = 5.0f;

    if (!bottlePresent())
    {
        return false;
    }

    float percentage = getPercentage();

    return (percentage > EMPTY_THRESHOLD) &&
           (percentage <= LOW_THRESHOLD);
}
