#ifndef LCC_ROBOTICS_22_23_REMOTESTATE_H
#define LCC_ROBOTICS_22_23_REMOTESTATE_H

#include <CrcLib.h>

using CrcUtility::ANALOG;
using CrcUtility::BUTTON;

class RState { // dumb name to avoid confusion with CrcUtility::RemoteState
public:
    static RState Next(); // get next remote state
    static RState Convert(CrcUtility::RemoteState crcRemoteState) noexcept;

    int8_t operator[](ANALOG channel) const noexcept;
    bool operator[](BUTTON button) const noexcept;

private:
    int8_t joystick1X = 0;
    int8_t joystick1Y = 0;
    int8_t joystick2X = 0;
    int8_t joystick2Y = 0;
    int8_t gachetteG = 0;
    int8_t gachetteD = 0;

    bool arrowRight = false;
    bool arrowUp = false;
    bool arrowLeft = false;
    bool arrowDown = false;
    bool colorRight = false;
    bool colorUp = false;
    bool colorLeft = false;
    bool colorDown = false;
    bool L1 = false;
    bool R1 = false;
    bool select = false;
    bool start = false;
    bool logo = false; // PS button on PS3/4, XBox button on XBox One, etc.
    bool hatL = false; // hat = pression sur joystick
    bool hatR = false;
};

#endif // LCC_ROBOTICS_22_23_REMOTESTATE_H
