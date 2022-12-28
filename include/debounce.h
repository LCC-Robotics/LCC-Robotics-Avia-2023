#ifndef LCC_ROBOTICS_22_23_DEBOUNCE_H
#define LCC_ROBOTICS_22_23_DEBOUNCE_H

#include <stdint.h>

#define DEBOUNCE_INTERVAL 20 // ms

class Debounce {
public:
    explicit Debounce(bool initialState = false, unsigned int debounceInterval = DEBOUNCE_INTERVAL);

    void addSample(bool newState, unsigned int millis); // ms
    inline void setInterval(unsigned int debounceInterval) { m_debounceInterval = debounceInterval; }
    inline bool read() const { return m_debouncedState; }
    inline bool rose() const { return m_changed && m_debouncedState; }; // debounced state changed from 0 to 1
    inline bool fell() const { return m_changed && !m_debouncedState; }; // debounced state changed from 1 to 0

private:
    bool m_debouncedState;
    unsigned int m_debounceInterval;

    bool m_changed = false;
    unsigned int m_timeSinceLastChange = 0;
};

#endif // LCC_ROBOTICS_22_23_DEBOUNCE_H
