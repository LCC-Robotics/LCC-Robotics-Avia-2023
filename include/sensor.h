#ifndef LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_

#include <Arduino.h>

#include "utils.h"

#define RED_FREQ_LOWER 24000UL
#define RED_FREQ_UPPER 14000UL

#define GREEN_FREQ_LOWER 24000UL
#define GREEN_FREQ_UPPER 8000UL

#define BLUE_FREQ_LOWER 21600UL
#define BLUE_FREQ_UPPER 11200UL

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

// ===========================

class ColorSensor {
public:
    struct RGB_Freq {
        unsigned long r = 0, g = 0, b = 0;

        static RGB_Freq fromRGB(uint8_t red, uint8_t green, uint8_t blue);
    };

    explicit ColorSensor(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t outPin, unsigned long timeout = 100);
    void init() const;
    void update();
    unsigned long getRawRedChannel() const;
    unsigned long getRawGreenChannel() const;
    unsigned long getRawBlueChannel() const;
    inline const RGB_Freq& get() const { return output; };

private:
    uint8_t s0, s1, s2, s3;
    uint8_t outPin;
    unsigned long timeout;
    RGB_Freq output {};
};

class LightSensor {
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
