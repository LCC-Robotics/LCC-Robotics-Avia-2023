#ifndef LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_
#define LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_

#include <Arduino.h>

// https://www.teachmemicro.com/arduino-pid-control-tutorial/
struct PID {
    const float& Kp; // proportional constant
    const float& Ki; // Tegral constant (s^-1)
    const float& Kd; // derivative constant (s)

    unsigned long _lastUpdate = millis(); // millis
    float _prevError = 0;
    float _cumError = 0; // (haha) error

    float calculate(float input, float setPoint);
};

float PID::calculate(float input, float setPoint)
{
    unsigned long thisTick = millis();
    float dt = _lastUpdate - thisTick;

    float error = setPoint - input;
    _cumError += error * dt;
    float rateError = (error - _prevError) / dt;

    _lastUpdate = thisTick;
    _prevError = error;

    return Kp * error + Ki * _cumError + Kd * rateError;
}

#endif // LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_
