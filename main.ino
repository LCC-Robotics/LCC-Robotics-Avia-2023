#define __AVR_ATmega1280__ // get around dumb header guard in CrcLib

#include <CrcLib/CrcLib.h>
#include "inc/robot.h"

using namespace Crc;

void setup() {
    Serial.begin(9600);
    CrcLib::Initialize();

    CrcLib::SetDigitalPinMode(CRC_DIG_1, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_2, INPUT);
    
    CrcLib::InitializePwmOutput(CRC_PWM_5);
}

void loop() {
    CrcLib::Update();

    if (CRC_DIG_4 == 1){
        if (BUTTON::ARROW_UP == 1){
            CrcLib::SetPwmOutput(CRC_PWM_5, 0);
        }
    }

}
