#include "SensorFusion.h"

/*
============================================================
Constructor
============================================================
*/

SensorFusion::SensorFusion()
{
    _currentStatus = SalineStatus::UNKNOWN;
}

/*
============================================================
Evaluate Sensor Data

Decision Priority:

1. Sensor Failure
2. Bottle Removed
3. Empty Bottle
4. Low Saline
5. Tube Blocked
6. No Drip
7. Normal
============================================================
*/

SalineStatus SensorFusion::evaluate(
    bool sensorHealthy,
    bool bottlePresent,
    bool bottleLow,
    bool bottleEmpty,
    bool noDrip,
    float salinePercentage)
{
    // Sensor malfunction
    if (!sensorHealthy)
    {
        _currentStatus = SalineStatus::SENSOR_FAILURE;
        return _currentStatus;
    }

    // Bottle removed
    if (!bottlePresent)
    {
        _currentStatus = SalineStatus::BOTTLE_REMOVED;
        return _currentStatus;
    }

    // Empty bottle
    if (bottleEmpty)
    {
        _currentStatus = SalineStatus::EMPTY_BOTTLE;
        return _currentStatus;
    }

    // Low saline
    if (bottleLow)
    {
        _currentStatus = SalineStatus::LOW_SALINE;
        return _currentStatus;
    }

    /*
        If bottle still contains saline
        but drip has stopped,
        tube is likely blocked.
    */
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

    // Everything normal
    _currentStatus = SalineStatus::NORMAL;

    return _currentStatus;
}

/*
============================================================
Return Current Status
============================================================
*/

SalineStatus SensorFusion::getStatus() const
{
    return _currentStatus;
}

/*
============================================================
Convert Status Enum to String
============================================================
*/

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

        case SalineStatus::UNKNOWN:
        default:
            return "UNKNOWN";
    }
}
