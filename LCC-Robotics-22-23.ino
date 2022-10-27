#include <CrcLib/CrcLib.h>

#include "inc/motor.h"
#include "inc/limits.h"
#include "inc/utils.h"

using namespace Crc;

void setup()
{
    CrcLib::Initialize();
    CrcLib::SetupPins();

    CrcLib::SetDigitalPinMode(CRC_DIG_1, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_2, INPUT);

    CrcLib::InitializePwmOutput(CRC_PWM_5);

    Serial.begin(9600);
}

void loop()
{
    CrcLib::Update();

    if (!CrcLib::IsCommValid())
        return;

    CrcLib::MoveHolonomic();

    if (CRC_DIG_4 == 1) {
        if (BUTTON::ARROW_UP == 1) {
            CrcLib::SetPwmOutput(CRC_PWM_5, 0);
        }
    }
}
