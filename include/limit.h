#ifndef __ROBOTICS_LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_
#define __ROBOTICS_LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_

#include <Arduino.h>
#include <CrcLib.h>

#include "utils.h"

#define PWM_LIMIT_POSITIVE 0, 127
#define PWM_LIMIT_NEGATIVE -127, 0

/**
 * @brief Simple function which prevents robot from committing self die by clamping the raws between min and max when limit switch is activated
 *
 * @tparam T number type
 * @param activated is safety switch activated
 * @param raw raw input
 * @param min inclusive
 * @param max inclusive
 * @return T evaluated output
 */
template <typename T>
inline T safety(bool activated, T raw, T min, T max)
{
    if (!activated)
        return raw;
    return constrain(raw, min, max);
}

/**
 * @brief Smooths out suddent motions
 * https://www.embeddedrelated.com/showarticle/646.php
 *
 * @tparam T number type
 * @param raw raw input
 * @param prev_out previous output value from function
 * @param max_change maxmimum allowed deviation of raw from prev_out
 * @return T evaluated output
 */
template <typename T>
inline T limitSlew(T raw, T prev_out, T max_change)
{
    T delta = raw - prev_out;
    if (delta > max_change)
        delta = max_change;
    else if (delta < -max_change)
        delta = -max_change;
    return prev_out + delta;
}

#endif // __ROBOTICS_LCC_ROBOTICS_22_23_INCLUDE_LIMIT_H_

