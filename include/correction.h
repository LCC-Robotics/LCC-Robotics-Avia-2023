#ifndef LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_
#define LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_

#include <Arduino.h>

// https://www.teachmemicro.com/arduino-pid-control-tutorial/
class PID {
public:
    explicit PID(const float& Kp, const float& Ki, const float& Kd);
    float calculate(float input, float setPoint);

private:
    const float& Kp; // proportional constant
    const float& Ki; // Tegral constant (s^-1)
    const float& Kd; // derivative constant (s)

    unsigned long lastUpdate = millis(); // millis
    float prevError = 0;
    float cumError = 0; // (haha) error
};

#endif // LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_
