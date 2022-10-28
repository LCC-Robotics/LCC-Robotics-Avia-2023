#include "CrcLib.h"

#include "motor.h"

using namespace Crc;

void initPWM()
{
    CrcLib::SetDigitalPinMode(DRIVE_FL, INPUT);
    CrcLib::SetDigitalPinMode(DRIVE_FR, INPUT);
    CrcLib::SetDigitalPinMode(DRIVE_BL, INPUT);
    CrcLib::SetDigitalPinMode(DRIVE_BR, INPUT);
};

void initServos(){};

