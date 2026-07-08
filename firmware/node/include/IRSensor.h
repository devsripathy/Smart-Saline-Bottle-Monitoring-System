#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <Arduino.h>

class IRSensor
{
public:

    explicit IRSensor(uint8_t pin);

    bool begin();

    void update();

    bool dripDetected();

    uint32_t getDripCount() const;

    float getFlowRate();

    bool noDrip();

    bool sensorHealthy();

    void resetCounter();

private:

    uint8_t _pin;

    bool _lastState;
    bool _dripEvent;

    uint32_t _dripCount;

    unsigned long _lastDripTime;
    unsigned long _lastPulseTime;

    unsigned long _windowStart;
    uint32_t _windowCount;

    static constexpr uint16_t DEBOUNCE_MS = 40;
    
};

#endif