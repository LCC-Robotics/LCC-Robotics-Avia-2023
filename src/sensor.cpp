#include <Arduino.h>
#include <CrcLib.h>

#include "sensor.h"

using namespace Crc;
using RGB_Freq = ColorSensor::RGB_Freq;


RGB_Freq RGB_Freq::fromRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return RGB_Freq{
        (unsigned long)map(r, 0, 255, RED_FREQ_LOWER, RED_FREQ_UPPER),
        (unsigned long)map(g, 0, 255, GREEN_FREQ_LOWER, GREEN_FREQ_UPPER),
        (unsigned long)map(b, 0, 255, BLUE_FREQ_LOWER, BLUE_FREQ_UPPER)};
}

void ColorSensor::init()
{
    CrcLib::SetDigitalOutput(d0, HIGH);
    CrcLib::SetDigitalOutput(d1, HIGH);
}

void ColorSensor::update()
{
    ColorSensor::getRawRedChannel(output.r);
    ColorSensor::getRawGreenChannel(output.g);
    ColorSensor::getRawBlueChannel(output.b);
}

 void ColorSensor::getRawRedChannel(unsigned long& output)
{
    CrcLib::SetDigitalOutput(d2, LOW);
    CrcLib::SetDigitalOutput(d3, LOW);
    output = pulseIn(outPin, LOW, timeout);
}

 void ColorSensor::getRawGreenChannel(unsigned long &output)
{
    CrcLib::SetDigitalOutput(d2, HIGH);
    CrcLib::SetDigitalOutput(d3, HIGH);
    output = pulseIn(outPin, LOW, timeout);
}

 void ColorSensor::getRawBlueChannel(unsigned long& output)
{
    CrcLib::SetDigitalOutput(d2, LOW);
    CrcLib::SetDigitalOutput(d3, HIGH);
    output = pulseIn(outPin, LOW, timeout);
}
