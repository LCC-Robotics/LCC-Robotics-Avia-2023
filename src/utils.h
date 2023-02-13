#ifndef LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
#define LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_

#include <Arduino.h>

#include <math.h>

template <typename T>
struct Range {
    T lower;
    T upper;
};

constexpr Range<int8_t> PWM_MOTOR_BOUNDS { -128, 127 };
constexpr Range<int8_t> PWM_LIMIT_POSITIVE { 0, 127 };
constexpr Range<int8_t> PWM_LIMIT_NEGATIVE { -128, 0 };

// Simple function which prevents robot from committing self die by clamping the raws between lower and upper when limit switch is activated
template <typename T = int>
inline T safety(bool activated, T raw, const Range<T>& range)
{
    if (!activated)
        return raw;
    return constrain(raw, range.lower, range.upper);
}

// https://www.embeddedrelated.com/showarticle/646.php
// Smooths out sudden motions
template <typename T = int>
inline T limitSlew(T raw, T prev_out, T max_deviation)
{
    T delta = raw - prev_out;
    if (delta > max_deviation)
        delta = max_deviation;
    else if (delta < -max_deviation)
        delta = -max_deviation;
    return prev_out + delta;
}

#endif // LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
