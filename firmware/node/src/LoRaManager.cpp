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

    _sequenceNumber = 0;
}

bool LoRaManager::begin()
{
    LoRa.setPins(_ssPin, _rstPin, _dio0Pin);

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
    const char* status)
{
    _sequenceNumber++;

    String json;

    json.reserve(200);

    json += "{";
    json += "\"device\":\"";
    json += deviceId;
    json += "\",";
    json += "\"seq\":";
    json += String(_sequenceNumber);
    json += ",";
    json += "\"weight\":";
    json += String(weight,2);
    json += ",";
    json += "\"percentage\":";
    json += String(percentage,1);
    json += ",";
    json += "\"drips\":";
    json += String(dripCount);
    json += ",";
    json += "\"flowRate\":";
    json += String(flowRate,1);
    json += ",";
    json += "\"status\":\"";
    json += status;
    json += "\"";
    json += "}";

    return json;
}

bool LoRaManager::sendPacket(
    const String& deviceId,
    float weight,
    float percentage,
    uint32_t dripCount,
    float flowRate,
    const char* status)
{
    String packet = createPacket(
        deviceId,
        weight,
        percentage,
        dripCount,
        flowRate,
        status);

    const uint8_t MAX_RETRIES = 3;

    for(uint8_t i=0;i<MAX_RETRIES;i++)
    {
        LoRa.beginPacket();

        LoRa.print(packet);

        LoRa.endPacket();

        if(waitForAck(1000))
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
    String message;

    while(LoRa.available())
    {
        message += (char)LoRa.read();
    }

    return message;
}

bool LoRaManager::waitForAck(uint32_t timeout)
{
    unsigned long start = millis();

    while(millis() - start < timeout)
    {
        int packetSize = LoRa.parsePacket();

        if(packetSize)
        {
            String ack;

            while(LoRa.available())
            {
                ack += (char)LoRa.read();
            }

            if(ack == "ACK")
            {
                return true;
            }
        }
    }

    return false;
}
