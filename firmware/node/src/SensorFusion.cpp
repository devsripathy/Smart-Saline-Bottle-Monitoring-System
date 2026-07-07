#include "SensorFusion.h"

SensorFusion::SensorFusion()
{
    _currentStatus = SalineStatus::UNKNOWN;
}

SalineStatus SensorFusion::evaluate(
    bool sensorHealthy,
    bool bottlePresent,
    bool bottleLow,
    bool bottleEmpty,
    bool noDrip,
    float salinePercentage)
{
    if (!sensorHealthy)
    {
        _currentStatus = SalineStatus::SENSOR_FAILURE;
        return _currentStatus;
    }

    if (!bottlePresent)
    {
        _currentStatus = SalineStatus::BOTTLE_REMOVED;
        return _currentStatus;
    }

    if (bottleEmpty)
    {
        _currentStatus = SalineStatus::EMPTY_BOTTLE;
        return _currentStatus;
    }

    if (bottleLow)
    {
        _currentStatus = SalineStatus::LOW_SALINE;
        return _currentStatus;
    }

    if (noDrip)
    {
        if (salinePercentage > 10.0f)
        {
            _currentStatus = SalineStatus::TUBE_BLOCKED;
        }
        else
        {
            _currentStatus = SalineStatus::NO_DRIP;
        }

        return _currentStatus;
    }

    _currentStatus = SalineStatus::NORMAL;

    return _currentStatus;
}

SalineStatus SensorFusion::getStatus() const
{
    return _currentStatus;
}

const char* SensorFusion::statusToString(SalineStatus status) const
{
    switch (status)
    {
        case SalineStatus::NORMAL:
            return "NORMAL";

        case SalineStatus::LOW_SALINE:
            return "LOW_SALINE";

        case SalineStatus::EMPTY_BOTTLE:
            return "EMPTY_BOTTLE";

        case SalineStatus::BOTTLE_REMOVED:
            return "BOTTLE_REMOVED";

        case SalineStatus::NO_DRIP:
            return "NO_DRIP";

        case SalineStatus::TUBE_BLOCKED:
            return "TUBE_BLOCKED";

        case SalineStatus::SENSOR_FAILURE:
            return "SENSOR_FAILURE";

        default:
            return "UNKNOWN";
    }
}
