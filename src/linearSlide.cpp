#include <Arduino.h>

#include "linearSlide.h"

LinearSlide::LinearSlide(Motor&& motor, PID&& pid, const utils::Range<float>& bounds, float (*feedback)())
    : m_motor { utils::move(motor) }
    , m_pid { utils::move(pid) }
    , m_feedback { feedback }
    , m_bounds { utils::move(bounds) }
{
}

void LinearSlide::update(float millis)
{
    if (!isManualMode())
        m_motor.set((int8_t)m_pid.calculate((*m_feedback)(), millis));

    m_motor.update(millis);
}

bool LinearSlide::setManualMode()
{
    if (!isManualMode()) {
        m_manualMode = true;
        m_motor.set(0);
        return true;
    }
    return false;
}

bool LinearSlide::setAutoMode()
{
    if (isManualMode()) {
        m_manualMode = false;
        m_motor.set(0);
        m_pid.setTarget((*m_feedback)()); // keep at current position
        return true;
    }
    return false;
}
