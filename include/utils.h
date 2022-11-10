#ifndef __ROBOTICS_LCC_ROBOTICS_22_23_INC_UTILS_H_
#define __ROBOTICS_LCC_ROBOTICS_22_23_INC_UTILS_H_

// Stripped down and simple implementation of std::pair
template <class T1, class T2>
struct Pair
{
    T1 first;
    T2 second;
};

// Stores a range (min, max)
template <class T = int>
using Range = Pair<T, T>;

#endif // __ROBOTICS_LCC_ROBOTICS_22_23_INC_UTILS_H_
