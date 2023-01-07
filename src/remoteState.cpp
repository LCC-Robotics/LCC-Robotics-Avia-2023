#include <CrcLib.h>

#include "remoteState.h"

using Crc::CrcLib;

RState RState::Next()
{
    return Convert(CrcLib::RemoteState());
}

RState RState::Convert(CrcUtility::RemoteState crcRemoteState) noexcept
{
    crcRemoteState.joystick1X -= 128;
    crcRemoteState.joystick1Y -= 128;
    crcRemoteState.joystick2X -= 128;
    crcRemoteState.joystick2Y -= 128;
    crcRemoteState.gachetteG -= 128;
    crcRemoteState.gachetteD -= 128;
    return *reinterpret_cast<RState*>(&crcRemoteState); // forbidden c++ dark magic
}

int8_t RState::operator[](ANALOG channel) const noexcept
{
    switch (channel) {
    case ANALOG::JOYSTICK1_X: return joystick1X;
    case ANALOG::JOYSTICK1_Y: return joystick1Y;
    case ANALOG::JOYSTICK2_X: return joystick2X;
    case ANALOG::JOYSTICK2_Y: return joystick2Y;
    case ANALOG::GACHETTE_R: return gachetteD;
    case ANALOG::GACHETTE_L: return gachetteG;
    case ANALOG::NONE:
    default: return 0;
    }
}

bool RState::operator[](BUTTON button) const noexcept
{
    switch (button) {
    case BUTTON::ARROW_RIGHT: return arrowRight;
    case BUTTON::ARROW_UP: return arrowUp;
    case BUTTON::ARROW_LEFT: return arrowLeft;
    case BUTTON::ARROW_DOWN: return arrowDown;
    case BUTTON::COLORS_RIGHT: return colorRight;
    case BUTTON::COLORS_UP: return colorUp;
    case BUTTON::COLORS_LEFT: return colorLeft;
    case BUTTON::COLORS_DOWN: return colorDown;
    case BUTTON::L1: return L1;
    case BUTTON::R1: return R1;
    case BUTTON::SELECT: return select;
    case BUTTON::START: return start;
    case BUTTON::LOGO: return logo;
    case BUTTON::HATL: return hatL;
    case BUTTON::HATR: return hatR;
    case BUTTON::NONE:
    default: return false;
    }
}
