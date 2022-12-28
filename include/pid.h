#ifndef LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_
#define LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_

#include <Arduino.h>

#include "utils.h"

// https://www.teachmemicro.com/arduino-pid-control-tutorial/
class PID {
public:
    explicit PID(float kp, float ki, float kd, Range<float> bounds);

    float calculate(float setPoint, float millis); // millis in millis
    inline float getTarget() const { return m_target; }
    inline void setTarget(float v) { m_target = v; }

private:
    const float m_kp; // proportional constant
    const float m_ki; // integral constant (s^-1)
    const float m_kd; // derivative constant (s)

    float m_prevError = 0;
    float m_cumError = 0; // (haha) error

    float m_target = 0; // target value
    const Range<float> m_bounds;
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_