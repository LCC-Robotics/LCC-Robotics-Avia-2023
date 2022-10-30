#include <CrcLib.h>

#include "limit.h"
#include "motor.h"

using namespace Crc;

void PWM_Motor::init()
{
    CrcLib::InitializePwmOutput(pin);
}

void PWM_Motor::update() 
{   
    if (slewRate == 0) // slew of 0 disables motor
        return;

    auto dt = millis() - _last_tick; // calculate delta time
    _output = limitSlew(flipped ? -command : command, _output, (int)(slewRate * dt));

    if (command != _output) {
        CrcLib::SetPwmOutput(pin, _output);
    }
}

void DriveTrain::init()
{
    FLMotor.init();
    FRMotor.init();
    BLMotor.init();
    BRMotor.init();
}

void DriveTrain::update()
{
    FLMotor.update();
    FRMotor.update();
    BLMotor.update();
    BRMotor.update();
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
