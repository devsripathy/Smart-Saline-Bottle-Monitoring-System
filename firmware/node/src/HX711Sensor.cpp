#include "HX711Sensor.h"
#include "Config.h"

/*
============================================================
Constructor
============================================================
*/

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
      _sampleIndex(0),
      _bufferFilled(false)
{
    for (uint8_t i = 0; i < FILTER_SIZE; i++)
    {
        _samples[i] = 0.0f;
    }
}

/*
============================================================
Initialize HX711
============================================================
*/

bool HX711Sensor::begin()
{
    _scale.begin(_doutPin, _sckPin);

    _scale.set_scale(_calibrationFactor);

    delay(500);

    tare();

    return _scale.wait_ready_timeout(1000);
}

/*
============================================================
Tare Scale
============================================================
*/

void HX711Sensor::tare()
{
    if (_scale.wait_ready_timeout(1000))
    {
        _scale.tare();
    }
}

/*
============================================================
Read Current Weight
============================================================
*/

float HX711Sensor::getWeight()
{
    if (!_scale.wait_ready_timeout(500))
    {
        return 0.0f;
    }

    float weight = _scale.get_units(5);

    if (weight < MIN_VALID_WEIGHT)
    {
        weight = 0.0f;
    }

    if (weight > MAX_VALID_WEIGHT)
    {
        weight = MAX_VALID_WEIGHT;
    }

    return weight;
}

/*
============================================================
Moving Average Filter
============================================================
*/

float HX711Sensor::applyMovingAverage(float value)
{
    _samples[_sampleIndex] = value;

    _sampleIndex++;

    if (_sampleIndex >= FILTER_SIZE)
    {
        _sampleIndex = 0;
        _bufferFilled = true;
    }

    uint8_t count = _bufferFilled ? FILTER_SIZE : _sampleIndex;

    if (count == 0)
    {
        return value;
    }

    float sum = 0.0f;

    for (uint8_t i = 0; i < count; i++)
    {
        sum += _samples[i];
    }

    return sum / count;
}

/*
============================================================
Filtered Weight
============================================================
*/

float HX711Sensor::getFilteredWeight()
{
    return applyMovingAverage(getWeight());
}

/*
============================================================
Calibration
============================================================
*/

void HX711Sensor::setCalibrationFactor(float factor)
{
    _calibrationFactor = factor;

    _scale.set_scale(_calibrationFactor);
}

float HX711Sensor::getCalibrationFactor() const
{
    return _calibrationFactor;
}
/*
============================================================
Sensor Health Check
============================================================
*/

bool HX711Sensor::sensorHealthy()
{
    return _scale.wait_ready_timeout(500);
}

/*
============================================================
Constrain Percentage
============================================================
*/

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

/*
============================================================
Calculate Saline Percentage

Formula:

          Current - Empty
% = ----------------------------- × 100
      Full - Empty

============================================================
*/

float HX711Sensor::getPercentage()
{
    float weight = getFilteredWeight();

    float percent =
        ((weight - _emptyWeight) /
        (_fullWeight - _emptyWeight)) * 100.0f;

    return constrainPercentage(percent);
}

/*
============================================================
Bottle Presence Detection

Bottle is considered present if measured
weight exceeds the empty bottle threshold
by a small tolerance.
============================================================
*/

bool HX711Sensor::bottlePresent()
{
    float weight = getFilteredWeight();

    return weight >= (_emptyWeight + 20.0f);
}

/*
============================================================
Bottle Empty Detection

Returns true when saline percentage
falls below the critical threshold.
============================================================
*/

bool HX711Sensor::isBottleEmpty()
{
    return getPercentage() <= CRITICAL_LEVEL_PERCENT;
}

/*
============================================================
Low Saline Detection

Returns true when saline percentage
falls below the configured low level.
============================================================
*/

bool HX711Sensor::isBottleLow()
{
    float percentage = getPercentage();

    return (percentage <= LOW_LEVEL_PERCENT) &&
           (percentage > CRITICAL_LEVEL_PERCENT);
}
