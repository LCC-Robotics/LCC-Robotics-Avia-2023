#include <Arduino.h>
#include <etl/utility.h>

#include "linearSlide.h"
#include "utils.h"

LinearSlide::LinearSlide(const uint8_t motorPin, PID pid, const Range<float>& bounds, float (*feedback)())
    : m_motorPin { motorPin }
    , m_pid { pid }
    , m_feedback { feedback }
    , m_bounds { etl::move(bounds) }
{
}

void LinearSlide::update(unsigned int millis)
{
    if (!isManualMode()) {
        auto command = m_pid.calculate((*m_feedback)(), millis);
        set(static_cast<int8_t>(command));
    }
}

bool LinearSlide::setManualMode()
{
    if (!isManualMode()) {
        m_manualMode = true;
        set(0);
        return true;
    }
    return false;
}

bool LinearSlide::setAutoMode()
{
    if (isManualMode()) {
        m_manualMode = false;
        set(0);
        m_pid.setTarget((*m_feedback)()); // keep at current position
        return true;
    }
    return false;
}
