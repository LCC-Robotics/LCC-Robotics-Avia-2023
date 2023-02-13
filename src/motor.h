#ifndef LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_

#include <CrcLib.h>

#include "utils.h"

using Crc::CrcLib;

constexpr float DEFAULT_SLEW_RATE = 0.75f;

constexpr Range<int8_t> PWM_MOTOR_BOUNDS { -128, 127 };
constexpr Range<int8_t> PWM_LIMIT_POSITIVE { 0, 127 };
constexpr Range<int8_t> PWM_LIMIT_NEGATIVE { -128, 0 };

// https://github.com/purduesigbots/libblrs/blob/master/libmtrmgr/src/mtrmgr.c
// Used for PWM motors such as the ones used for the drivetrain and servos.

class SlewLimitingMotor {
public:
    explicit SlewLimitingMotor(uint8_t pin, bool reversed = false, float slewRate = DEFAULT_SLEW_RATE)
        : m_pin { pin }
        , m_reversed { reversed }
        , m_slewRate { slewRate }
    {
        CrcLib::InitializePwmOutput(pin, false);
    }

    void update(float millis)
    {
        if (m_slewRate == 0.0)
            return;

        if (m_command != m_output) {
            m_output = static_cast<int8_t>(
                limitSlew(
                    m_reversed ? -m_command : m_command,
                    m_output,
                    static_cast<int>(m_slewRate * millis)));
            CrcLib::SetPwmOutput(m_pin, m_output);
        }
    }

    inline void set(int8_t v) { m_command = v; } // set motor speed: -127 to 127

private:
    uint8_t m_pin; // PWM pin
    bool m_reversed; // Programmatically flips motor
    float m_slewRate; // Limit slew
    int8_t m_command = 0; // Adjusted value that the motor will be set to
    int8_t m_output = 0; // Value that we want to be set
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_MOTOR_H_
