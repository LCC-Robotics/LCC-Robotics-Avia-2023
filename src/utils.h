#ifndef LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
#define LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_

#include <Arduino.h>

#include <math.h>

template <typename T>
struct Range {
    T lower;
    T upper;
};

template <class T>
// Simple function which prevents robot from committing self die by clamping the raws between lower and upper when limit switch is activated
inline T safety(bool activated, T raw, const Range<T>& range)
{
    if (!activated)
        return raw;
    return constrain(raw, range.lower, range.upper);
}

// https://www.embeddedrelated.com/showarticle/646.php
// Smooths out sudden motions
inline int limitSlew(int raw, int prev_out, int max_deviation)
{
    int delta = raw - prev_out;
    if (delta > max_deviation)
        delta = max_deviation;
    else if (delta < -max_deviation)
        delta = -max_deviation;
    return prev_out + delta;
}

#endif // LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
