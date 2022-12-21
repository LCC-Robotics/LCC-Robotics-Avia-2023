#include "correction.h"

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
