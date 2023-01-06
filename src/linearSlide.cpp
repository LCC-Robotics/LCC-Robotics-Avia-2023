#include <Arduino.h>
#include <etl/utility.h>

#include "linearSlide.h"

using utils::Range;

LinearSlide::LinearSlide(Motor motor, PID pid, const Range<float>& bounds, float (*feedback)())
    : m_motor { motor }
    , m_pid { pid }
    , m_feedback { feedback }
    , m_bounds { etl::move(bounds) }
{
}

void LinearSlide::update(unsigned int millis)
{
    if (!isManualMode()) {
        auto command = m_pid.calculate((*m_feedback)(), millis);
        m_motor.set(static_cast<int8_t>(command));
    }
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
