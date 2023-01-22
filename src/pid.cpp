#include <etl/utility.h>

#include "pid.h"

template <typename T>
PID<T>::PID(const float kp, const float ki, const float kd, const Range<T>& bounds)
    : m_kp { kp }
    , m_ki { ki }
    , m_kd { kd }
    , m_bounds { etl::move(bounds) }
{
}

template <typename T>
T PID<T>::calculate(float currPoint, float millis)
{
    float error = m_targetPoint - currPoint;

    float integral = m_cumError + error * millis;
    float derivative = (error - m_prevError) / millis;

    T output = m_kp * error + m_ki * m_cumError + m_kd * derivative;

    if (output < m_bounds.lower)
        output = m_bounds.lower;
    else if (output > m_bounds.upper)
        output = m_bounds.upper;
    else
        m_cumError = integral; // anti-windup

    m_prevError = error;

    return output;
}
