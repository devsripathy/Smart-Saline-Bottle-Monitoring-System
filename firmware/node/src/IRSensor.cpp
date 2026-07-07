#include "IRSensor.h"

/*
============================================================
Constructor
============================================================
*/

IRSensor::IRSensor(uint8_t pin)
    : _pin(pin),
      _lastState(HIGH),
      _dripEvent(false),
      _dripCount(0),
      _lastDripTime(0),
      _lastPulseTime(0),
      _windowStart(0),
      _windowCount(0)
{
}

/*
============================================================
Initialize IR Sensor
============================================================
*/

bool IRSensor::begin()
{
    pinMode(_pin, INPUT_PULLUP);

    _lastState = digitalRead(_pin);

    unsigned long now = millis();

    _lastDripTime = now;
    _lastPulseTime = now;
    _windowStart = now;

    return true;
}

/*
============================================================
Update Sensor State

Detects falling edge:
HIGH -> LOW
============================================================
*/

void IRSensor::update()
{
    _dripEvent = false;

    bool currentState = digitalRead(_pin);

    unsigned long now = millis();

    if (_lastState == HIGH &&
        currentState == LOW)
    {
        if ((now - _lastPulseTime) >= DEBOUNCE_MS)
        {
            _dripCount++;
            _windowCount++;

            _lastPulseTime = now;
            _lastDripTime = now;

            _dripEvent = true;
        }
    }

    _lastState = currentState;
}

/*
============================================================
Returns true only once per detected drip
============================================================
*/

bool IRSensor::dripDetected()
{
    return _dripEvent;
}

/*
============================================================
Total Drip Counter
============================================================
*/

uint32_t IRSensor::getDripCount() const
{
    return _dripCount;
}

/*
============================================================
Flow Rate

Returns:
Drops / Minute
============================================================
*/

float IRSensor::getFlowRate()
{
    unsigned long now = millis();

    unsigned long elapsed = now - _windowStart;

    if (elapsed == 0)
    {
        return 0.0f;
    }

    float elapsedMinutes =
        elapsed / 60000.0f;

    float flowRate =
        _windowCount / elapsedMinutes;

    if (elapsed >= FLOW_WINDOW_MS)
    {
        _windowStart = now;
        _windowCount = 0;
    }

    return flowRate;
}

/*
============================================================
Returns true when no drip
has been detected for 10 seconds.
============================================================
*/

bool IRSensor::noDrip()
{
    return (millis() - _lastDripTime) >=
            NO_DRIP_TIMEOUT_MS;
}

/*
============================================================
Basic Sensor Health Check
============================================================
*/

bool IRSensor::sensorHealthy()
{
    int state = digitalRead(_pin);

    return (state == HIGH ||
            state == LOW);
}

/*
============================================================
Reset Counters
============================================================
*/

void IRSensor::resetCounter()
{
    _dripCount = 0;

    _windowCount = 0;

    unsigned long now = millis();

    _windowStart = now;

    _lastPulseTime = now;

    _lastDripTime = now;

    _dripEvent = false;
}
