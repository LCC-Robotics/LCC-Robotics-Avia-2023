#include <Arduino.h>

#include "drivetrain.h"

ArcadeDriveTrain::ArcadeDriveTrain(Motor LMotor, Motor RMotor)
    : m_LMotor { LMotor }
    , m_RMotor { RMotor }
{
}

void ArcadeDriveTrain::update(float millis) // Calls update function all motors. Should be ran every MOTOR_UPDATE_INTERVAL.
{
    m_LMotor.update(millis);
    m_RMotor.update(millis);
}

void ArcadeDriveTrain::stop() // sets all motors to 0
{
    m_LMotor.set(0);
    m_RMotor.set(0);
}

void ArcadeDriveTrain::move(uint8_t forwardChannel, uint8_t yawChannel)
{
    m_LMotor.set(constrain(forwardChannel - yawChannel, -127, 127)); // Determines the power of the left wheels
    m_RMotor.set(constrain(forwardChannel + yawChannel, -127, 127)); // Determines the power of the right wheels
}