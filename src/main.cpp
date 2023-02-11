#include <Arduino.h>
#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview

#include <etl/array.h>
#include <etl/debounce.h>

#include "pid.h"
#include "remoteState.h"
#include "utils.h"

using namespace Crc;

// ====================
// Controller Input
// ====================

constexpr ANALOG FORWARD_CHANNEL = ANALOG::JOYSTICK1_Y;
constexpr ANALOG YAW_CHANNEL = ANALOG::JOYSTICK1_X;

constexpr ANALOG LINEAR_SLIDE_MANUAL_CHANNEL = ANALOG::JOYSTICK2_Y;
constexpr BUTTON LINEAR_SLIDE_NEXT_BUTTON = BUTTON::ARROW_UP;
constexpr BUTTON LINEAR_SLIDE_PREV_BUTTON = BUTTON::ARROW_DOWN;

// ====================
// Pins
// ====================

constexpr uint8_t LEFT_MOTOR_PIN = CRC_PWM_5;
constexpr uint8_t RIGHT_MOTOR_PIN = CRC_PWM_6;
constexpr uint8_t ELEVATOR_MOTOR_PIN = CRC_PWM_7;

constexpr uint8_t ELEVATOR_ENCODER_PIN_A = CRC_DIG_2;
constexpr uint8_t ELEVATOR_ENCODER_PIN_B = CRC_DIG_3;

// ====================
// Constants
// ====================

constexpr float DEFAULT_SLEW_RATE = 0.75;

// ====================

RState lastRemoteState;
RState remoteState; // custom remote state that uses the forbidden arts

etl::debounce<> linearSlideNextButton;
etl::debounce<> linearSlidePrevButton;

volatile int linearSlideLevel = 0;

void setup()
{
    CrcLib::Initialize();

    // Initialize pins
    CrcLib::SetDigitalPinMode(ELEVATOR_ENCODER_PIN_A, INPUT);
    CrcLib::SetDigitalPinMode(ELEVATOR_ENCODER_PIN_B, INPUT);

    CrcLib::InitializePwmOutput(LEFT_MOTOR_PIN, false);
    CrcLib::InitializePwmOutput(RIGHT_MOTOR_PIN, false);
    CrcLib::InitializePwmOutput(ELEVATOR_MOTOR_PIN, false);

#ifdef DEBUG // only start serial if in debug mode (serial can affect performance)
    Serial.begin(BAUD); // macro defined in platformio.ini
#endif
}

void loop()
{
    CrcLib::Update();

    const unsigned int dt = CrcLib::GetDeltaTimeMillis();

    // Check if commands are valid
    if (!CrcLib::IsCommValid()) // controller not connected, don't run loop
    {
        CrcLib::MoveArcade(0, 0, LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN); // should fix jerking problem from last year?
        return;
    }

    // Update remote state
    lastRemoteState = remoteState;
    remoteState = RState::Next();

    linearSlideNextButton.add(remoteState[LINEAR_SLIDE_NEXT_BUTTON]);
    linearSlidePrevButton.add(remoteState[LINEAR_SLIDE_PREV_BUTTON]);

    // max amount that input can deviate by
    const auto maxSlew = static_cast<int8_t>(DEFAULT_SLEW_RATE * dt);

    // move robot
    CrcLib::MoveArcade(
        limitSlew<int8_t>(remoteState[FORWARD_CHANNEL], lastRemoteState[FORWARD_CHANNEL], maxSlew),
        limitSlew<int8_t>(remoteState[YAW_CHANNEL], lastRemoteState[YAW_CHANNEL], maxSlew),
        LEFT_MOTOR_PIN,
        RIGHT_MOTOR_PIN);
}
