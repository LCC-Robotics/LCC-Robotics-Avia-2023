#ifndef LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_

#include <stdint.h>

#include "utils.h"

#define RED_FREQ_LOWER 24000UL
#define RED_FREQ_UPPER 14000UL

#define GREEN_FREQ_LOWER 24000UL
#define GREEN_FREQ_UPPER 8000UL

#define BLUE_FREQ_LOWER 21600UL
#define BLUE_FREQ_UPPER 11200UL

#define SPEED_OF_SOUND 0.034 // m/s

struct ColorSensor {
    struct RGB_Freq {
        unsigned long r = 0, g = 0, b = 0;

        static RGB_Freq fromRGB(uint8_t r, uint8_t g, uint8_t b);
    };

    uint8_t d0, d1, d2, d3;
    uint8_t outPin;

    unsigned long timeout = 100;
    RGB_Freq output;

    void init();
    void update();
    void getRawRedChannel(unsigned long& output);
    void getRawGreenChannel(unsigned long& output);
    void getRawBlueChannel(unsigned long& output);
};

struct LightSensor {
};

struct DistanceSensor {
    uint8_t echoPin;
    uint8_t trigPin;

    unsigned long timeout = 100;
    float output; // cm

    void update();
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
