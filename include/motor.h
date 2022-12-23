#ifndef LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_

#include <Arduino.h>
#include <CrcLib.h>

#define MOTOR_UPDATE_INTERVAL 20 // ms
#define DEFAULT_SLEW_RATE 0.75

// https://github.com/purduesigbots/libblrs/blob/master/libmtrmgr/src/mtrmgr.c
// Used for PWM motors such as the ones used for the drivetrain and servos.

class Motor {
public:
    explicit Motor(uint8_t pin, bool flipped = false, float slewRate = DEFAULT_SLEW_RATE);
    void update(); // Updates output based on slewRate. Should be ran every MOTOR_UPDATE_INTERVAL
    inline void set(int v) { command = v; };

private:
    uint8_t pin; // PWM pin
    bool flipped; // Programmatically flips motor
    float slewRate; // Limit slew

    int command = 0; // Value that we want to be set
    int output = 0; // Adjusted value that motor will be set to
    unsigned long lastUpdate = millis(); // Millis
};

// ============================

//  Manages motors of the drivetrain.
class ArcadeDriveTrain {
public:
    explicit ArcadeDriveTrain(Motor LMotor, Motor RMotor);
    void update(); // Calls update function all motors. Should be ran every MOTOR_UPDATE_INTERVAL.
    void stop(); // sets all motors to 0
    void move(uint8_t forwardChannel, uint8_t yawChannel);

private:
    Motor LMotor, RMotor;
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
