#ifndef LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_
#define LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_

#include <Arduino.h>

#include "utils.h"

using utils::Range;

template <typename T = int8_t>
constexpr Range<T> PWM_LIMIT_POSITIVE { 0, 127 };

template <typename T = int8_t>
constexpr Range<T> PWM_LIMIT_NEGATIVE { -127, 0 };

// Simple function which prevents robot from committing self die by clamping the raws between lower and upper when limit switch is activated
template <typename T = int8_t>
inline T safety(bool activated, T raw, const Range<T>& range)
{
    if (!activated)
        return raw;
    return constrain(raw, range.lower, range.upper);
}

// https://www.embeddedrelated.com/showarticle/646.php
// Smooths out sudden motions
template <typename T = int8_t>
inline T limitSlew(T raw, T prev_out, T max_deviation)
{
    T delta = raw - prev_out;
    if (delta > max_deviation)
        delta = max_deviation;
    else if (delta < -max_deviation)
        delta = -max_deviation;
    return prev_out + delta;
}

#endif // LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_
