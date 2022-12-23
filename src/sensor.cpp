#include <Arduino.h>
#include <CrcLib.h>

#include "sensor.h"

using namespace Crc;

// =========================

DistanceSensor::DistanceSensor(uint8_t echoPin, uint8_t trigPin)
    : echoPin { echoPin }
    , trigPin { trigPin } {};

void DistanceSensor::update()
{
    CrcLib::SetDigitalOutput(trigPin, LOW);
    delayMicroseconds(2);
    CrcLib::SetDigitalOutput(trigPin, HIGH);
    delayMicroseconds(10);
    CrcLib::SetDigitalOutput(trigPin, LOW);

    // Calculating the distance
    noInterrupts();
    output = static_cast<double>(pulseIn(echoPin, HIGH, timeout)) * SPEED_OF_SOUND / 2;
    interrupts();
}

// =========================

using RGB_Freq = ColorSensor::RGB_Freq;

RGB_Freq RGB_Freq::fromRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    return RGB_Freq {
        (unsigned long)map(red, 0, 255, RED_FREQ_LOWER, RED_FREQ_UPPER),
        (unsigned long)map(green, 0, 255, GREEN_FREQ_LOWER, GREEN_FREQ_UPPER),
        (unsigned long)map(blue, 0, 255, BLUE_FREQ_LOWER, BLUE_FREQ_UPPER)
    };
}

// =========================

ColorSensor::ColorSensor(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t outPin, unsigned long timeout)
    : s0 { s0 }
    , s1 { s1 }
    , s2 { s2 }
    , s3 { s3 }
    , outPin { outPin }
    , timeout { timeout } {};

void ColorSensor::init() const
{
    CrcLib::SetDigitalOutput(s0, HIGH);
    CrcLib::SetDigitalOutput(s1, HIGH);
}

void ColorSensor::update()
{
    output.r = ColorSensor::getRawRedChannel();
    output.g = ColorSensor::getRawGreenChannel();
    output.b = ColorSensor::getRawBlueChannel();
}

unsigned long ColorSensor::getRawRedChannel() const
{
    CrcLib::SetDigitalOutput(s2, LOW);
    CrcLib::SetDigitalOutput(s3, LOW);
    delayMicroseconds(10);
    noInterrupts();
    unsigned long pulse_len = pulseIn(outPin, LOW, timeout);
    interrupts();
    return pulse_len;
}

unsigned long ColorSensor::getRawGreenChannel() const
{
    CrcLib::SetDigitalOutput(s2, HIGH);
    CrcLib::SetDigitalOutput(s3, HIGH);
    delayMicroseconds(10);
    noInterrupts();
    unsigned long pulse_len = pulseIn(outPin, LOW, timeout);
    interrupts();
    return pulse_len;
}

unsigned long ColorSensor::getRawBlueChannel() const
{
    CrcLib::SetDigitalOutput(s2, LOW);
    CrcLib::SetDigitalOutput(s3, HIGH);
    delayMicroseconds(10);
    noInterrupts();
    unsigned long pulse_len = pulseIn(outPin, LOW, timeout);
    interrupts();
    return pulse_len;
}
