#ifndef HX711_SENSOR_H
#define HX711_SENSOR_H

class HX711Sensor
{
public:

    bool begin();

    float getWeight();

    float getPercentage();

};

#endif
