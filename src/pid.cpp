#include <etl/utility.h>

#include "pid.h"

PID::PID(const float kp, const float ki, const float kd, const Range<float>& bounds)
    : m_kp { kp }
    , m_ki { ki }
    , m_kd { kd }
    , m_bounds { etl::move(bounds) }
{
}

float PID::calculate(float currPoint, float millis)
{
    float error = m_targetPoint - currPoint;

    float integral = m_cumError + error * millis;
    float derivative = (error - m_prevError) / millis;

    float output = m_kp * error + m_ki * m_cumError + m_kd * derivative;

    if (output < m_bounds.lower)
        output = m_bounds.lower;
    else if (output > m_bounds.upper)
        output = m_bounds.upper;
    else
        m_cumError = integral; // anti-windup

    m_prevError = error;

    return output;
}
