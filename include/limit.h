#ifndef __ROBOTICS_LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_
#define __ROBOTICS_LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_

#include <Arduino.h>

#include "utils.h"

#define PWM_LIMIT_POSITIVE Range<uint8_t>{0, 127}
#define PWM_LIMIT_NEGATIVE Range<uint8_t>{-127, 0}

// Simple function which prevents robot from committing self die by clamping the raws between min and max when limit switch is activated
template <class T>
inline T safety(bool activated, T raw, const Range<T>& range)
{
    if (!activated)
        return raw;
    return constrain(raw, range.first, range.second);
}


// https://www.embeddedrelated.com/showarticle/646.php
// Smooths out suddent motions
template <class T>
inline T limitSlew(T raw, T prev_out, T max_deviation)
{
    T delta = raw - prev_out;
    if (delta > max_deviation)
        delta = max_deviation;
    else if (delta < -max_deviation)
        delta = -max_deviation;
    return prev_out + delta;
}

#endif // __ROBOTICS_LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_

