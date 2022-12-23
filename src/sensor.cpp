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