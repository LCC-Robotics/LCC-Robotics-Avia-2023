#include <Arduino.h>
#include <CrcLib.h>

#include "limit.h"
#include "motor.h"

using namespace Crc;

void PWM_Motor::update()
{
    if (!slewRate)
        return;

    auto thisTick = millis(); // calculate delta time

    if (_command != _output)
    {
        auto dt = thisTick - _lastUpdate;
        _output = limitSlew(flipped ? -_command : _command, _output, (int)(slewRate * dt));
        CrcLib::SetPwmOutput(pin, _output);
    }

    _lastUpdate = thisTick;
}

void ArcadeDriveTrain::update() // Calls update function all motors. Should be ran every MOTOR_UPDATE_INTERVAL.
{
    LMotor.update();
    RMotor.update();
};
void ArcadeDriveTrain::stop() // sets all motors to 0
{
    LMotor.set(0);
    RMotor.set(0);
};
void ArcadeDriveTrain::move(int8_t forwardChannel, int8_t yawChannel)
{
    LMotor.set(constrain(forwardChannel - yawChannel, -128, 127)); // Determines the power of the left wheels
    RMotor.set(constrain(forwardChannel + yawChannel, -128, 127)); // Determines the power of the right wheels
}
