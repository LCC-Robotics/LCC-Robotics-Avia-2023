#ifndef LCC_ROBOTICS_22_23_DRIVETRAIN_H
#define LCC_ROBOTICS_22_23_DRIVETRAIN_H

#include <stdint.h>

#include "motor.h"

//  Manages motors of the drivetrain.
class ArcadeDriveTrain {
public:
    explicit ArcadeDriveTrain(Motor&& LMotor, Motor&& RMotor);
    ArcadeDriveTrain(ArcadeDriveTrain&&) = default;
    ArcadeDriveTrain(const ArcadeDriveTrain&) = delete;

    void update(unsigned int millis); // Calls update function all motors. Should run every MOTOR_UPDATE_INTERVAL.
    void stop(); // sets all motors to 0
    void move(uint8_t forwardChannel, uint8_t yawChannel);

private:
    Motor m_LMotor, m_RMotor;
};

#endif // LCC_ROBOTICS_22_23_DRIVETRAIN_H
