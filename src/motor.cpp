#include <CrcLib.h>

#include "motor.h"
#include "safety.h"

using namespace Crc;

Motor::Motor(uint8_t pin, bool flipped, float slewRate)
    : m_pin { pin }
    , m_flipped { flipped }
    , m_slewRate { slewRate }
{
}

void Motor::update(unsigned int millis)
{
    if (m_slewRate == 0.0)
        return;

    if (m_command != m_output) {
        m_output = limitSlew<>(
            static_cast<int8_t>(m_flipped ? -m_command : m_command),
            m_output,
            static_cast<int8_t>(m_slewRate * millis));
        CrcLib::SetPwmOutput(m_pin, m_output);
    }
}
