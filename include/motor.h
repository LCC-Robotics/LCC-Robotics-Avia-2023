#ifndef __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_
#define __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_

#include <Arduino.h>
#include <CrcLib.h>

#define MOTOR_UPDATE_INTERVAL 20 // ms
#define DEFAULT_SLEW_RATE 0.75


 // https://github.com/purduesigbots/libblrs/blob/master/libmtrmgr/src/mtrmgr.c
 // Used for PWM motors such as the ones used for the drivetrain and servos.
struct PWM_Motor {
    unsigned char pin; // PWM pin
    int command = 0; // Value that we want to be set
    bool flipped = false; // Programmatically flips motor
    float slewRate = DEFAULT_SLEW_RATE; // Limit slew

    int _output = 0; // Adjusted value that motor will be set to
    unsigned long _lastUpdate = millis(); // Millis

    void init(); // Init pwm pin
    void update(); // Updates output based on slewRate. Should be ran every MOTOR_UPDATE_INTERVAL
};

//  Manages motors of the drivetrain.
struct DriveTrain {
    PWM_Motor FLMotor;
    PWM_Motor FRMotor;
    PWM_Motor BLMotor;
    PWM_Motor BRMotor;

    void init(); // Calls init function of all motors.
    void update(); // Calls update function all motors. Should be ran every MOTOR_UPDATE_INTERVAL.
    void stop(); // sets all motors to 0
    void moveHolonomic(int8_t forwardChannel, int8_t yawChannel, int8_t strafeChannel);
};

#endif // __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_
