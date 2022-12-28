#ifndef LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
#define LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_

#include <math.h>

template <typename T>
struct Range {
    T lower;
    T upper;
};

// implementation of arduino map function using float
inline float fmap(float v, float in_min, float in_max, float out_min, float out_max)
{
    return (v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
