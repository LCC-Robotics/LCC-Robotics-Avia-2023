#ifndef LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_

#include <Arduino.h>

#include "utils.h"

#define SPEED_OF_SOUND 0.034 // m/s

class DistanceSensor {
public:
    explicit DistanceSensor(uint8_t echoPin, uint8_t trigPin);
    void update();
    inline const double& get() const { return output; };

private:
    uint8_t echoPin, trigPin;
    unsigned long timeout = 100;
    double output {}; // cm
};


class LightSensor {
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
