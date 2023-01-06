#ifndef LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_

#include <stdint.h>

#include "utils.h"

using utils::Range;

constexpr float DEFAULT_SLEW_RATE = 0.75;

template <typename T = int8_t>
constexpr Range<T> PWM_MOTOR_BOUNDS { -127, 127 };

// https://github.com/purduesigbots/libblrs/blob/master/libmtrmgr/src/mtrmgr.c
// Used for PWM motors such as the ones used for the drivetrain and servos.

class Motor {
public:
    explicit Motor(uint8_t pin, bool flipped = false, float slewRate = DEFAULT_SLEW_RATE);

    void update(unsigned int millis); // call per interval
    inline void set(int8_t v) noexcept { m_command = v; } // set motor speed: -127 to 127

private:
    uint8_t m_pin; // PWM pin
    bool m_flipped; // Programmatically flips motor
    float m_slewRate; // Limit slew
    int8_t m_command = 0; // Adjusted value that the motor will be set to
    int8_t m_output = 0; // Value that we want to be set
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
