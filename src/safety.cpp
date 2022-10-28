#include "safety.h"
#include "utils.h"

// min and max are inclusive
int safety(bool activated, int value, int min, int max)
{
    if (!activated) return value;
    return clamp(value, min, max);
}   
