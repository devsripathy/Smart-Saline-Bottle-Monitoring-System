#include "IRSensor.h"

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

void IRSensor::update()
{
    _dripEvent = false;

    bool currentState = digitalRead(_pin);

    unsigned long now = millis();

    // Detect falling edge (HIGH -> LOW)
    if (_lastState == HIGH && currentState == LOW)
    {
        // Debounce filtering
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

bool IRSensor::dripDetected()
{
    return _dripEvent;
}

uint32_t IRSensor::getDripCount() const
{
    return _dripCount;
}

float IRSensor::getFlowRate()
{
    unsigned long now = millis();

    unsigned long elapsed = now - _windowStart;

    if (elapsed == 0)
    {
        return 0.0f;
    }

    float elapsedMinutes = elapsed / 60000.0f;

    float flowRate = _windowCount / elapsedMinutes;

    // Reset calculation window every minute
    if (elapsed >= FLOW_WINDOW_MS)
    {
        _windowStart = now;
        _windowCount = 0;
    }

    return flowRate;
}

bool IRSensor::noDrip()
{
    return (millis() - _lastDripTime) >= NO_DRIP_TIMEOUT_MS;
}

bool IRSensor::sensorHealthy()
{
    int state = digitalRead(_pin);

    return (state == HIGH || state == LOW);
}

void IRSensor::resetCounter()
{
    _dripCount = 0;
    _windowCount = 0;

    unsigned long now = millis();

    _windowStart = now;
    _lastDripTime = now;
    _lastPulseTime = now;

    _dripEvent = false;
}
