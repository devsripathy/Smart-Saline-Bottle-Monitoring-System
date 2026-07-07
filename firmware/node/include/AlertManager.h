#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include <Arduino.h>
#include "SensorFusion.h"

class AlertManager
{
public:

    explicit AlertManager(uint8_t ledPin);

    bool begin();

    void update(SalineStatus status);

    SalineStatus getCurrentAlert() const;

private:

    uint8_t _ledPin;

    SalineStatus _currentAlert;

    unsigned long _lastBlink;

    bool _ledState;

    void normal();

    void lowSaline();

    void emptyBottle();

    void bottleRemoved();

    void noDrip();

    void tubeBlocked();

    void sensorFailure();

    void blink(uint16_t interval);
};

#endif
