#include <Arduino.h>

#include "CrcLib.h" // Crc dependency
#include "Bounce2.h" // debounce library

#include "motor.h"
#include "safety.h"
#include "utils.h"

using namespace Crc;

void setup()
{
    CrcLib::Initialize();

    initPWM();

    Serial.begin(BAUD); // macro defined in platformio.ini
}

void loop()
{
    CrcLib::Update();

    if (!CrcLib::IsCommValid())
        return;

    CrcLib::MoveHolonomic(
        ANALOG::JOYSTICK1_X,
        ANALOG::JOYSTICK1_Y,
        ANALOG::JOYSTICK2_X,
        DRIVE_FL,
        DRIVE_BL,
        DRIVE_FR,
        DRIVE_BR);
}
