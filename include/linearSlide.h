#ifndef LCC_ROBOTICS_22_23_LINEARSLIDE_H
#define LCC_ROBOTICS_22_23_LINEARSLIDE_H

#include <CrcLib.h>

#include "encoder.h"
#include "pid.h"
#include "utils.h"

using Crc::CrcLib;

class LinearSlide {
public:
    explicit LinearSlide(uint8_t motorPin, PID pid, const Range<float>& bounds, float (*feedback)());

    void update(unsigned int millis); // millis
    inline void set(int8_t v) const { CrcLib::SetPwmOutput(m_motorPin, v); } // motor value, call setManualMode before calling this function
    bool setManualMode(); // must be called before setting height or moving
    bool setAutoMode(); // if not in mode, return true
    inline bool isManualMode() const noexcept { return m_manualMode; }
    inline float getHeight() const noexcept { return m_pid.getTarget(); }
    inline void setHeight(float v) { m_pid.setTarget(constrain(v, m_bounds.lower, m_bounds.upper)); } // height as float, call setAutoMode before calling this function

private:
    const uint8_t m_motorPin; // motor with slew limiting
    PID m_pid;
    bool m_manualMode = false; // manual mode =  joystick, auto mode = preset heights

    float (*m_feedback)(); // callback to get current position
    const Range<float> m_bounds;
};

#endif // LCC_ROBOTICS_22_23_LINEARSLIDE_H
