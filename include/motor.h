#ifndef __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_
#define __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_

#include <CrcLib.h>

#define MOTOR_UPDATE_INTERVAL 20 // ms
#define DEFAULT_SLEW_RATE 0.75

/**
 * @brief Used for PWM motors such as the ones used for the drivetrain and servos.
 * https://github.com/purduesigbots/libblrs/blob/master/libmtrmgr/src/mtrmgr.c
 */
struct PWM_Motor {
    unsigned char pin; // PWM pin
    int command = 0; // Value that we want to be set
    bool flipped = false; // Programmatically flips motor
    float slewRate = DEFAULT_SLEW_RATE; // Limit slew

    int _output = 0; // Adjusted value that motor will be set to
    unsigned long _lastTick; // Millis

    /**
     * @brief Init pwm pin
     */
    void init();

    /**
     * @brief Updates output based on slewRate. Should be ran every MOTOR_UPDATE_INTERVAL
     */
    void update();
};

/**
 * @brief Manages motors of the drivetrain.
 */
struct DriveTrain {
    PWM_Motor FLMotor;
    PWM_Motor FRMotor;
    PWM_Motor BLMotor;
    PWM_Motor BRMotor;

    /**
     * @brief Calls init function of all motors.
     */
    void init();

    /**
     * @brief Calls update function all motors. Should be ran every MOTOR_UPDATE_INTERVAL.
     */
    void update();

    /**
     * @brief sets all motors to 0
     */
    void stop();

    /**
     * @brief Based on parameters updates motor command values.
     *
     * @param forwardChannel -127 to 127
     * @param yawChannel -127 to 127
     * @param strafeChannel -127 to 127
     */
    void moveHolonomic(int8_t forwardChannel, int8_t yawChannel, int8_t strafeChannel);
};

#endif // __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_
