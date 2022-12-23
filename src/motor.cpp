#include <Arduino.h>
#include <CrcLib.h>

#include "limit.h"
#include "motor.h"

using namespace Crc;

Motor::Motor(uint8_t pin, bool flipped, float slewRate)
    : pin { pin }
    , flipped { flipped }
    , slewRate { slewRate } {};

void Motor::update()
{
    if (slewRate == 0.0)
        return;

    auto thisTick = millis(); // calculate delta time

    if (command != output) {
        int dt = static_cast<int>(thisTick - lastUpdate);
        output = limitSlew(flipped ? -command : command, output, (int)(slewRate * dt));
        CrcLib::SetPwmOutput(pin, static_cast<char>(output));
    }

    lastUpdate = thisTick;
}

// ================

ArcadeDriveTrain::ArcadeDriveTrain(Motor LMotor, Motor RMotor)
    : LMotor { LMotor }
    , RMotor { RMotor } {};

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
void ArcadeDriveTrain::move(uint8_t forwardChannel, uint8_t yawChannel)
{
    LMotor.set(constrain(forwardChannel - yawChannel, -127, 127)); // Determines the power of the left wheels
    RMotor.set(constrain(forwardChannel + yawChannel, -127, 127)); // Determines the power of the right wheels
}
