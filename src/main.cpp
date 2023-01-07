#include <Arduino.h>
#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview

#include <etl/array.h>
#include <etl/debounce.h>

#include "encoder.h"
#include "linearSlide.h"
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
constexpr int ENCODER_STEPS = 30;

constexpr int LINEAR_SLIDE_STAGES = 3;
constexpr int LINEAR_SLIDE_LEVELS = 8;
constexpr float LINEAR_SLIDE_SPOOL_DIAMETER = 2.0; // cm
constexpr float LINEAR_SLIDE_ENCODER_STEP_SIZE = LINEAR_SLIDE_SPOOL_DIAMETER * LINEAR_SLIDE_STAGES * PI / ENCODER_STEPS; // cm - distance travelled per step of rotary encoder

constexpr etl::array<float, LINEAR_SLIDE_LEVELS> LINEAR_SLIDE_HEIGHTS { 0.0, 6.0, 17.0, 38.0, 66.0, 102.0, 146.0, 190.0 }; // cm
constexpr Range<float> LINEAR_SLIDE_RANGE { 5.0, 200.0 };

// ====================
// Objects
// ====================

RotaryEncoder<float> elevatorEncoder {
    ELEVATOR_ENCODER_PIN_A,
    ELEVATOR_ENCODER_PIN_B,
    LINEAR_SLIDE_ENCODER_STEP_SIZE
};

LinearSlide elevator {
    ELEVATOR_MOTOR_PIN,
    PID(1.0, 1.0, 1.0, PWM_MOTOR_BOUNDS<float>),
    LINEAR_SLIDE_RANGE,
    []() -> float {
        return elevatorEncoder.getVal();
    }
};

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

    elevator.update(dt);
    elevatorEncoder.update();

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

    // Linear slide
    if (remoteState[LINEAR_SLIDE_MANUAL_CHANNEL]) {
        if (elevator.setManualMode()) {
            linearSlideLevel = -1;
        }

        elevator.set(
            limitSlew<int8_t>(remoteState[LINEAR_SLIDE_MANUAL_CHANNEL], lastRemoteState[LINEAR_SLIDE_MANUAL_CHANNEL], maxSlew));

    } else if (linearSlideNextButton.has_changed() && !linearSlideNextButton.is_set()) {
        if (elevator.setAutoMode()) {
            const float currHeight = elevator.getHeight();

            for (int l = 0; l < LINEAR_SLIDE_LEVELS; ++l) {
                if (LINEAR_SLIDE_HEIGHTS[l] > currHeight) {
                    linearSlideLevel = l;
                    break;
                }
            }
        }
        elevator.setHeight(LINEAR_SLIDE_HEIGHTS[linearSlideLevel]);

    } else if (linearSlidePrevButton.has_changed() && !linearSlidePrevButton.is_set()) {
        if (elevator.setAutoMode()) {
            const float currHeight = elevator.getHeight();

            for (int l = LINEAR_SLIDE_STAGES; l >= 0; --l) {
                if (LINEAR_SLIDE_HEIGHTS[l] < currHeight) {
                    linearSlideLevel = l;
                    break;
                }
            }
        }
        elevator.setHeight(LINEAR_SLIDE_HEIGHTS[linearSlideLevel]);
    }
}
