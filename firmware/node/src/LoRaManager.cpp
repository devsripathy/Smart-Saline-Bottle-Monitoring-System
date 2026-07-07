#include "LoRaManager.h"

#include <SPI.h>
#include <LoRa.h>

LoRaManager::LoRaManager(
    long frequency,
    uint8_t ssPin,
    uint8_t rstPin,
    uint8_t dio0Pin)
{
    _frequency = frequency;

    _ssPin = ssPin;
    _rstPin = rstPin;
    _dio0Pin = dio0Pin;

    _packetCounter = 0;
}

bool LoRaManager::begin()
{
    LoRa.setPins(
        _ssPin,
        _rstPin,
        _dio0Pin);

    if (!LoRa.begin(_frequency))
    {
        return false;
    }

    LoRa.enableCrc();

    return true;
}

String LoRaManager::createPacket(
    const String& deviceId,
    float weight,
    float percentage,
    uint32_t dripCount,
    float flowRate,
    const String& status)
{
    _packetCounter++;

    String packet = "{";

    packet += "\"id\":\"" + deviceId + "\",";
    packet += "\"seq\":" + String(_packetCounter) + ",";
    packet += "\"weight\":" + String(weight, 2) + ",";
    packet += "\"percent\":" + String(percentage, 1) + ",";
    packet += "\"drips\":" + String(dripCount) + ",";
    packet += "\"flow\":" + String(flowRate, 1) + ",";
    packet += "\"status\":\"" + status + "\"";
    packet += "}";

    return packet;
}

bool LoRaManager::sendPacket(
    const String& deviceId,
    float weight,
    float percentage,
    uint32_t dripCount,
    float flowRate,
    const String& status)
{
    String packet =
        createPacket(
            deviceId,
            weight,
            percentage,
            dripCount,
            flowRate,
            status);

    const uint8_t MAX_RETRIES = 3;

    for (uint8_t retry = 0; retry < MAX_RETRIES; retry++)
    {
        LoRa.beginPacket();

        LoRa.print(packet);

        LoRa.endPacket();

        if (waitForAck(1000))
        {
            return true;
        }
    }

    return false;
}

bool LoRaManager::available()
{
    return LoRa.parsePacket() > 0;
}

String LoRaManager::receivePacket()
{
    String incoming;

    while (LoRa.available())
    {
        incoming += (char)LoRa.read();
    }

    return incoming;
}

bool LoRaManager::waitForAck(uint32_t timeout)
{
    unsigned long start = millis();

    while (millis() - start < timeout)
    {
        int packetSize = LoRa.parsePacket();

        if (packetSize)
        {
            String ack;

            while (LoRa.available())
            {
                ack += (char)LoRa.read();
            }

            if (ack == "ACK")
            {
                return true;
            }
        }
    }

    return false;
}
