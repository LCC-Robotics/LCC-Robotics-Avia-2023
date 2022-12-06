#ifndef LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_

#include <Arduino.h>
#include <CrcLib.h>

#define MOTOR_UPDATE_INTERVAL 20 // ms
#define DEFAULT_SLEW_RATE 0.75

// https://github.com/purduesigbots/libblrs/blob/master/libmtrmgr/src/mtrmgr.c
// Used for PWM motors such as the ones used for the drivetrain and servos.

// stupid proof wooo
struct Motor
{
    using Value = int;

    uint8_t pin;                        // PWM pin
    bool flipped = false;               // Programmatically flips motor
    float slewRate = DEFAULT_SLEW_RATE; // Limit slew

    Value _command = 0;                  // Value that we want to be set
    Value _output = 0;                    // Adjusted value that motor will be set to
    unsigned long _lastUpdate = millis(); // Millis

    void update(); // Updates output based on slewRate. Should be ran every MOTOR_UPDATE_INTERVAL
    inline void set(Value v) { _command = v; };
};

//  Manages motors of the drivetrain.
struct ArcadeDriveTrain
{
    Motor LMotor;
    Motor RMotor;

    void update(); // Calls update function all motors. Should be ran every MOTOR_UPDATE_INTERVAL.
    void stop(); // sets all motors to 0
    void move(int8_t forwardChannel, int8_t yawChannel);
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
