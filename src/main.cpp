#include <Arduino.h>
#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview

#include <etl/array.h>
#include <etl/debounce.h>

#include "drivetrain.h"
#include "encoder.h"
#include "linearSlide.h"
#include "motor.h"
#include "pid.h"
#include "remoteState.h"
#include "utils.h"

using namespace Crc;
using utils::Range;

// ================
// CONSTANTS
// ================

#define FORWARD_CHANNEL ANALOG::JOYSTICK1_Y
#define YAW_CHANNEL ANALOG::JOYSTICK1_X

#define LINEAR_SLIDE_MANUAL_CHANNEL ANALOG::JOYSTICK2_Y
#define LINEAR_SLIDE_NEXT_BUTTON BUTTON::ARROW_UP
#define LINEAR_SLIDE_PREV_BUTTON BUTTON::ARROW_DOWN

#define ENCODER_STEPS 30

#define LINEAR_SLIDE_STAGES 3
#define LINEAR_SLIDE_SPOOL_DIAMETER 20.0f // mm
#define LINEAR_SLIDE_LEVELS 8

constexpr float LINEAR_SLIDE_ENCODER_STEP_SIZE = LINEAR_SLIDE_SPOOL_DIAMETER * LINEAR_SLIDE_STAGES * PI / ENCODER_STEPS; // mm - distance travelled per step of rotary encoder

constexpr Range<float> LINEAR_SLIDE_RANGE { 5.0, 200.0 };
constexpr etl::array<float, LINEAR_SLIDE_LEVELS> LINEAR_SLIDE_HEIGHTS = { 0.0, 6.0, 17.0, 38.0, 66.0, 102.0, 146.0, 190.0 }; // cm

etl::debounce<> linearSlideNextButton;
etl::debounce<> linearSlidePrevButton;

RState remoteState; // custom remote state that uses the forbidden arts

ArcadeDriveTrain driveTrain {
    Motor(CRC_PWM_5, false),
    Motor(CRC_PWM_6, true)
};

RotaryEncoder<float> elevatorEncoder {
    CRC_DIG_2,
    CRC_DIG_3,
    LINEAR_SLIDE_ENCODER_STEP_SIZE
};

LinearSlide elevator {
    Motor(CRC_PWM_7),
    PID(1.0, 1.0, 1.0, PWM_MOTOR_BOUNDS<float>),
    LINEAR_SLIDE_RANGE,
    []() -> float {
        return elevatorEncoder.getVal();
    }
};

int linearSlideLevel = 0;

void setup()
{
    CrcLib::Initialize();

    // Initialize pins
    CrcLib::SetDigitalPinMode(CRC_DIG_2, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_3, INPUT);

    CrcLib::InitializePwmOutput(CRC_PWM_5);
    CrcLib::InitializePwmOutput(CRC_PWM_6);
    CrcLib::InitializePwmOutput(CRC_PWM_7);

#ifdef DEBUG // only start serial if in debug mode (serial can affect performance)
    Serial.begin(BAUD); // macro defined in platformio.ini
#endif
}

void loop()
{
    CrcLib::Update();

    const unsigned int dt = CrcLib::GetDeltaTimeMillis();

    driveTrain.update(dt);
    elevator.update(dt);
    elevatorEncoder.update();

    // Check if commands are valid
    if (!CrcLib::IsCommValid()) // controller not connected, don't run loop
    {
        driveTrain.stop(); // should fix jerking problem from last year?
        return;
    }

    // Update remote state`
    remoteState = RState::Next();

    linearSlideNextButton.add(remoteState[LINEAR_SLIDE_NEXT_BUTTON]);
    linearSlidePrevButton.add(remoteState[LINEAR_SLIDE_PREV_BUTTON]);

    // move robot
    driveTrain.move(remoteState[FORWARD_CHANNEL], remoteState[YAW_CHANNEL]);

    // Linear slide
    if (remoteState[LINEAR_SLIDE_MANUAL_CHANNEL]) {
        if (elevator.setManualMode()) {
            linearSlideLevel = -1;
        }
        elevator.move(remoteState[LINEAR_SLIDE_MANUAL_CHANNEL]);

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
