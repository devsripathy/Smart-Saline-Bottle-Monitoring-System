#ifndef SENSOR_FUSION_H
#define SENSOR_FUSION_H

#include <Arduino.h>

/*
============================================================
Saline Status Enumeration
============================================================
*/

enum class SalineStatus
{
    UNKNOWN = 0,
    NORMAL,
    LOW_SALINE,
    EMPTY_BOTTLE,
    BOTTLE_REMOVED,
    NO_DRIP,
    TUBE_BLOCKED,
    SENSOR_FAILURE
};

/*
============================================================
Sensor Fusion Engine

Combines HX711 weight sensor and IR drip sensor
to determine the actual saline bottle state.

Priority:

1. Sensor Failure
2. Bottle Removed
3. Empty Bottle
4. Low Saline
5. Tube Blocked
6. No Drip
7. Normal
============================================================
*/

class SensorFusion
{
public:

    SensorFusion();

    SalineStatus evaluate(
        bool sensorHealthy,
        bool bottlePresent,
        bool bottleLow,
        bool bottleEmpty,
        bool noDrip,
        float salinePercentage);

    SalineStatus getStatus() const;

    const char* statusToString(SalineStatus status) const;

private:

    SalineStatus _currentStatus;
};

#endif
