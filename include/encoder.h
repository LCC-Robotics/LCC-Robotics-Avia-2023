#ifndef LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
#define LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_

#include <CrcLib.h>

#include "utils.h"

using Crc::CrcLib;

template <typename T = int>
class RotaryEncoder {
public:
    explicit RotaryEncoder(uint8_t pinA, uint8_t pinB, T stepSize = 1, T initialValue = 0);

    void update(); // call every loop
    inline T getVal() const noexcept { return m_val; }

private:
    uint8_t m_pinA, m_pinB; // pins
    T m_val; // value
    const T m_stepSize; // value to add per step

    uint8_t m_aState = 0;
    uint8_t m_aLastState;
};

// Implementations for member functions

template <typename T>
RotaryEncoder<T>::RotaryEncoder(uint8_t pinA, uint8_t pinB, const T stepSize, T initialValue)
    : m_pinA { pinA }
    , m_pinB { pinB }
    , m_val { initialValue }
    , m_stepSize { stepSize }
    , m_aLastState { CrcLib::GetDigitalInput(pinA) }
{
}

template <typename T>
void RotaryEncoder<T>::update()
{
    m_aState = CrcLib::GetDigitalInput(m_pinA);
    if (m_aState != m_aLastState)
        m_val = CrcLib::GetDigitalInput(m_pinB) != m_aState
            ? m_val + m_stepSize
            : m_val - m_stepSize;

    m_aLastState = m_aState;
}

#endif // LCC_ROBOTICS_22_23_INCLUDE_SENSOR_H_
