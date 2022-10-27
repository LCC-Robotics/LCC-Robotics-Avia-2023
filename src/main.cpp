#include <Arduino.h>

#include "../lib/CrcLib/CrcLib.h"

#include "../include/motor.h"
#include "../include/limits.h"
#include "../include/utils.h"

using namespace Crc;

void setup()
{
    CrcLib::Initialize();

    CrcLib::SetDigitalPinMode(CRC_DIG_1, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_2, INPUT);

    CrcLib::InitializePwmOutput(CRC_PWM_5);

    Serial.begin(BAUD);
}

void loop()
{
    CrcLib::Update();

    if (!CrcLib::IsCommValid())
        return;
}
