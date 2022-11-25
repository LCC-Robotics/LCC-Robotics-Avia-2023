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

struct ColorSensor
{
    struct RGB_Freq
    {
        unsigned long r, g, b;

        static RGB_Freq fromRGB(uint8_t r, uint8_t g, uint8_t b);
    };

    unsigned long timeout = 200;
    uint8_t d0, d1, d2, d3;
    uint8_t out;

    void init();
    RGB_Freq readRawColor();
    unsigned long readRawRedChannel();
    unsigned long readRawGreenChannel();
    unsigned long readRawBlueChannel();
};

struct LightSensor
{

};

struct DistanceSensor
{
    

};

#endif // LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
