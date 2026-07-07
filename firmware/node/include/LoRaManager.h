#ifndef LORA_MANAGER_H
#define LORA_MANAGER_H

#include <Arduino.h>

class LoRaManager
{
public:
    LoRaManager(
        long frequency,
        uint8_t ssPin,
        uint8_t rstPin,
        uint8_t dio0Pin);

    bool begin();

    bool sendPacket(
        const String& deviceId,
        float weight,
        float percentage,
        uint32_t dripCount,
        float flowRate,
        const String& status);

    bool available();

    String receivePacket();

private:
    long _frequency;

    uint8_t _ssPin;
    uint8_t _rstPin;
    uint8_t _dio0Pin;

    uint16_t _packetCounter;

    String createPacket(
        const String& deviceId,
        float weight,
        float percentage,
        uint32_t dripCount,
        float flowRate,
        const String& status);

    bool waitForAck(uint32_t timeout);
};

#endif
