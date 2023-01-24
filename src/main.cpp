#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview

#include <etl/array.h>
#include <etl/debounce.h>

#include "pid.h"
#include "remoteState.h"
#include "utils.h"

using Crc::CrcLib;

// ====================
// Controller Input
// ====================

constexpr ANALOG FORWARD_CHANNEL = ANALOG::JOYSTICK1_Y;
constexpr ANALOG YAW_CHANNEL = ANALOG::JOYSTICK1_X;

constexpr ANALOG ELEVATOR_MANUAL_CHANNEL = ANALOG::JOYSTICK2_Y;
// constexpr BUTTON ELEVATOR_NEXT_BUTTON = BUTTON::ARROW_UP;
// constexpr BUTTON ELEVATOR_PREV_BUTTON = BUTTON::ARROW_DOWN;

// ====================
// Pins
// ====================

constexpr uint8_t LEFT_MOTOR_PIN = CRC_PWM_5;
constexpr uint8_t RIGHT_MOTOR_PIN = CRC_PWM_6;
constexpr uint8_t ELEVATOR_MOTOR_PIN = CRC_PWM_7;

// constexpr uint8_t ELEVATOR_ENCODER_PIN_A = CRC_DIG_2;
// constexpr uint8_t ELEVATOR_ENCODER_PIN_B = CRC_DIG_3;

// ====================
// CONSTANTS
// ====================

constexpr float DEFAULT_SLEW_RATE = 0.75;
// constexpr int ENCODER_STEPS = 30;

// constexpr int ELEVATOR_STAGES = 3;
// constexpr int ELEVATOR_LEVELS = 8;
// constexpr float ELEVATOR_SPOOL_DIAMETER = 2.0; // cm
// constexpr float ELEVATOR_ENCODER_STEP_SIZE = ELEVATOR_SPOOL_DIAMETER * ELEVATOR_STAGES * PI / ENCODER_STEPS; // cm - distance travelled per step of rotary encoder

// constexpr Range<long> ELEVATOR_RANGE { 50, 2000 };
// constexpr etl::array<long, ELEVATOR_LEVELS> ELEVATOR_HEIGHTS { 0, 60, 170, 380, 660, 1020, 1460, 1900 }; // mm

// ====================

// PID<long> elevatorPID { 10, 1.0, 1.0, PWM_MOTOR_BOUNDS<long> };

RState lastRemoteState;
RState remoteState; // custom remote state that uses the forbidden arts

// int elevatorLevel = 0;
// long elevatorHeight = 0;

void setup()
{
    CrcLib::Initialize();

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
        remoteState = RState(); // empty remote set
    } else {
        // Update remote state
        lastRemoteState = remoteState;
        remoteState = RState::Next();
    }

    // static etl::debounce<> elevatorNextButton;
    // static etl::debounce<> elevatorPrevButton;

    // elevatorNextButton.add(remoteState[ELEVATOR_NEXT_BUTTON]);
    // elevatorPrevButton.add(remoteState[ELEVATOR_PREV_BUTTON]);

    // max amount that input can deviate by
    const auto maxMotorDeviation = static_cast<int8_t>(DEFAULT_SLEW_RATE * dt);

    // move robot
    CrcLib::MoveArcade(
        limitSlew<int8_t>(remoteState[FORWARD_CHANNEL], lastRemoteState[FORWARD_CHANNEL], maxMotorDeviation),
        limitSlew<int8_t>(remoteState[YAW_CHANNEL], lastRemoteState[YAW_CHANNEL], maxMotorDeviation),
        LEFT_MOTOR_PIN,
        RIGHT_MOTOR_PIN);

    // move elevator
    CrcLib::SetPwmOutput(
        ELEVATOR_MOTOR_PIN,
        limitSlew<int8_t>(remoteState[ELEVATOR_MANUAL_CHANNEL], lastRemoteState[ELEVATOR_MANUAL_CHANNEL], maxMotorDeviation));
}
