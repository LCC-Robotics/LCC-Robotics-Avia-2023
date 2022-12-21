#ifndef LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_
#define LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_

#include <Arduino.h>

// https://www.teachmemicro.com/arduino-pid-control-tutorial/
template <typename T = int>
struct PID {
    const T& Kp; // proportional constant
    const T& Ki; // Tegral constant (s^-1)
    const T& Kd; // derivative constant (s)

    unsigned long _lastUpdate = millis(); // millis
    T _prevError = 0;
    T _cumError = 0; // (haha) error

    T calculate(T input, T setPoint);
};

template <class T>
T PID<T>::calculate(T input, T setPoint)
{
    unsigned long thisTick = millis();
    T dt = _lastUpdate - thisTick;

    T error = setPoint - input;
    _cumError += error * dt;
    T rateError = (error - _prevError) / dt;

    _lastUpdate = thisTick;
    _prevError = error;

    return Kp * error + Ki * _cumError + Kd * rateError;
}

#endif // LCC_ROBOTICS_22_23_INCLUDE_CORRECTION_H_
