#include "correction.h"

PID::PID(const float& Kp, const float& Ki, const float& Kd)
    : Kp { Kp }
    , Ki { Ki }
    , Kd { Kd } {};

float PID::calculate(float input, float setPoint)
{
    unsigned long thisTick = millis();
    auto dt = static_cast<float>(lastUpdate - thisTick);

    float error = setPoint - input;
    cumError += error * dt;
    float rateError = (error - prevError) / dt;

    lastUpdate = thisTick;
    prevError = error;

    return Kp * error + Ki * cumError + Kd * rateError;
}
