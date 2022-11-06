#include <CrcLib.h>

#include "limit.h"
#include "motor.h"

using namespace Crc;

void PWM_Motor::update()
{
    if (!slewRate)
        return;

    auto thisTick = millis(); // calculate delta time

    if (command != _output) {
        auto dt = thisTick - _lastUpdate;
        _output = limitSlew(flipped ? -command : command, _output, (int)(slewRate * dt));
        CrcLib::SetPwmOutput(pin, _output);
    }

    _lastUpdate = thisTick;
}


void DriveTrain::update()
{
    FLMotor.update();
    FRMotor.update();
    BLMotor.update();
    BRMotor.update();
}

void DriveTrain::stop()
{
    FLMotor.command = 0;
    FRMotor.command = 0;
    BLMotor.command = 0;
    BRMotor.command = 0;
}

void DriveTrain::moveHolonomic(int8_t forwardChannel, int8_t yawChannel, int8_t strafeChannel)
{
    FLMotor.command = constrain(forwardChannel - yawChannel - strafeChannel,
        -128, 127); // Determines the power of the front left wheel
    BLMotor.command = constrain(forwardChannel - yawChannel + strafeChannel,
        -128, 127); // Determines the power of the front left wheel
    FRMotor.command = constrain(forwardChannel + yawChannel + strafeChannel,
        -128, 127); // Determines the power of the front left wheel
    BRMotor.command = constrain(forwardChannel + yawChannel - strafeChannel,
        -128, 127); // Determines the power of the right wheels
}
