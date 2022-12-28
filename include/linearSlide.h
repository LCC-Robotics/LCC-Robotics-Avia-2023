#ifndef LCC_ROBOTICS_22_23_LINEARSLIDE_H
#define LCC_ROBOTICS_22_23_LINEARSLIDE_H

#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "utils.h"

class LinearSlide {
public:
    explicit LinearSlide(Motor motor, PID pid, Range<float> bounds, float (*feedback)());

    void update(float millis); // millis
    inline void move(int8_t v) { m_motor.set(v); } // motor value, call setManualMode before calling this function
    bool setManualMode(); // must be called before setting height or moving
    bool setAutoMode(); // if not in mode, return true
    inline bool isManualMode() const { return m_manualMode; }
    inline float getHeight() const { return m_pid.getTarget(); }
    inline void setHeight(float v) { m_pid.setTarget(constrain(v, m_bounds.lower, m_bounds.upper)); } // height as float, call setAutoMode before calling this function

private:
    Motor m_motor; // motor with slew limiting
    PID m_pid;
    bool m_manualMode = false; // manual mode =  joystick, auto mode = preset heights

    float (*m_feedback)(); // callback to get current position
    const Range<float> m_bounds;
};

#endif // LCC_ROBOTICS_22_23_LINEARSLIDE_H
