#include "AlertManager.h"

AlertManager::AlertManager(uint8_t ledPin)
{
    _ledPin = ledPin;

    _currentAlert = SalineStatus::UNKNOWN;

    _lastBlink = 0;

    _ledState = false;
}

bool AlertManager::begin()
{
    pinMode(_ledPin, OUTPUT);

    digitalWrite(_ledPin, LOW);

    return true;
}

void AlertManager::update(SalineStatus status)
{
    _currentAlert = status;

    switch (status)
    {
        case SalineStatus::NORMAL:
            normal();
            break;

        case SalineStatus::LOW_SALINE:
            lowSaline();
            break;

        case SalineStatus::EMPTY_BOTTLE:
            emptyBottle();
            break;

        case SalineStatus::BOTTLE_REMOVED:
            bottleRemoved();
            break;

        case SalineStatus::NO_DRIP:
            noDrip();
            break;

        case SalineStatus::TUBE_BLOCKED:
            tubeBlocked();
            break;

        case SalineStatus::SENSOR_FAILURE:
            sensorFailure();
            break;

        default:
            digitalWrite(_ledPin, LOW);
            break;
    }
}

SalineStatus AlertManager::getCurrentAlert() const
{
    return _currentAlert;
}

void AlertManager::normal()
{
    digitalWrite(_ledPin, HIGH);
}

void AlertManager::lowSaline()
{
    blink(1000);
}

void AlertManager::emptyBottle()
{
    blink(300);
}

void AlertManager::bottleRemoved()
{
    blink(150);
}

void AlertManager::noDrip()
{
    blink(700);
}

void AlertManager::tubeBlocked()
{
    blink(500);
}

void AlertManager::sensorFailure()
{
    blink(100);
}

void AlertManager::blink(uint16_t interval)
{
    unsigned long now = millis();

    if (now - _lastBlink >= interval)
    {
        _lastBlink = now;

        _ledState = !_ledState;

        digitalWrite(_ledPin, _ledState);
    }
}
