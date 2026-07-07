#ifndef SENSOR_FUSION_H
#define SENSOR_FUSION_H

#include <Arduino.h>

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
