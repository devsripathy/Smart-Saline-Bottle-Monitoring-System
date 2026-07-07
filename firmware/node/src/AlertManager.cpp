#include "AlertManager.h"

/*
============================================================
Constructor
============================================================
*/

AlertManager::AlertManager(uint8_t ledPin)
    : _ledPin(ledPin),
      _currentAlert(SalineStatus::UNKNOWN),
      _lastBlinkTime(0),
      _ledState(false)
{
}

/*
============================================================
Initialize Alert Manager
============================================================
*/

bool AlertManager::begin()
{
    pinMode(_ledPin, OUTPUT);

    digitalWrite(_ledPin, LOW);

    return true;
}

/*
============================================================
Update Alert State
============================================================
*/

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

/*
============================================================
Current Alert
============================================================
*/

SalineStatus AlertManager::getCurrentAlert() const
{
    return _currentAlert;
}

/*
============================================================
Normal
============================================================
*/

void AlertManager::normal()
{
    digitalWrite(_ledPin, HIGH);
}

/*
============================================================
Low Saline
============================================================
*/

void AlertManager::lowSaline()
{
    blink(1000);
}

/*
============================================================
Empty Bottle
============================================================
*/

void AlertManager::emptyBottle()
{
    blink(300);
}

/*
============================================================
Bottle Removed
============================================================
*/

void AlertManager::bottleRemoved()
{
    blink(150);
}

/*
============================================================
No Drip
============================================================
*/

void AlertManager::noDrip()
{
    blink(700);
}

/*
============================================================
Tube Blocked
============================================================
*/

void AlertManager::tubeBlocked()
{
    blink(500);
}

/*
============================================================
Sensor Failure
============================================================
*/

void AlertManager::sensorFailure()
{
    blink(100);
}

/*
============================================================
Generic Blink Function
============================================================
*/

void AlertManager::blink(uint16_t interval)
{
    unsigned long currentTime = millis();

    if (currentTime - _lastBlinkTime >= interval)
    {
        _lastBlinkTime = currentTime;

        _ledState = !_ledState;

        digitalWrite(_ledPin, _ledState);
    }
}
