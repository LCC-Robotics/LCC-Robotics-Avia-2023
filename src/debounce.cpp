#include "debounce.h"

Debounce::Debounce(bool initialState, unsigned int debounceInterval)
    : m_debouncedState { initialState }
    , m_debounceInterval { debounceInterval }
{
}

void Debounce::addSample(bool newState, unsigned int millis)
{
    if (m_changed)
        m_changed = false; // state changed last update, unset

    if (m_debouncedState != newState) {
        m_timeSinceLastChange = 0;

        if (m_timeSinceLastChange >= m_debounceInterval) {
            m_debouncedState = newState;
            m_changed = true;
        }
    } else {
        m_timeSinceLastChange += millis;
    }
}
