#include <Arduino.h>

#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview
#include <arduino-timer.h> // https://github.com/contrem/arduino-timer

#include "debounce.h"
#include "drivetrain.h"
#include "encoder.h"
#include "linearSlide.h"
#include "motor.h"
#include "pid.h"
#include "remoteState.h"
#include "utils.h"

using namespace Crc;

// ================
// CONSTANTS
// ================

#define FORWARD_CHANNEL ANALOG::JOYSTICK1_Y
#define YAW_CHANNEL ANALOG::JOYSTICK1_X

#define LINEAR_SLIDE_MANUAL_CHANNEL ANALOG::JOYSTICK2_Y
#define LINEAR_SLIDE_NEXT_BUTTON BUTTON::ARROW_UP
#define LINEAR_SLIDE_PREV_BUTTON BUTTON::ARROW_DOWN

#define MOTOR_UPDATE_INTERVAL 20 // ms
#define ENCODER_STEPS 30

#define LINEAR_SLIDE_STAGES 3
#define LINEAR_SLIDE_SPOOL_DIAMETER 20.0f // mm
#define LINEAR_SLIDE_LEVELS 8

constexpr utils::Range<float> LINEAR_SLIDE_RANGE { 50.0, 2000.0 };
constexpr float LINEAR_SLIDE_ENCODER_STEP_SIZE = LINEAR_SLIDE_SPOOL_DIAMETER * LINEAR_SLIDE_STAGES * PI / ENCODER_STEPS; // mm - distance travelled per step of rotary encoder
const float LINEAR_SLIDE_HEIGHTS[8] = { 0.0, 60.0, 170.0, 380.0, 660.0, 1020.0, 1460.0, 1900.0 }; // mm

// ===============

RState remoteState; // custom remote state that uses the forbidden arts

Debounce linearSlideNextButton;
Debounce linearSlidePrevButton;

Timer<5> timer; // allows us to

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

    // recalculate m_motor output every MOTOR_UPDATE_INTERVAL
    // still stupid proof
    timer.every(MOTOR_UPDATE_INTERVAL,
        [](void*) -> bool {
            driveTrain.update(MOTOR_UPDATE_INTERVAL);
            elevator.update(MOTOR_UPDATE_INTERVAL);
            return true;
        });

#ifdef DEBUG // only start serial if in debug mode (serial can affect performance)
    Serial.begin(BAUD); // macro defined in platformio.ini
#endif
}

void loop()
{
    CrcLib::Update();

    timer.tick<void>();
    elevatorEncoder.update();

    // Check if commands are valid
    if (!CrcLib::IsCommValid()) // controller not connected, don't run loop
    {
        driveTrain.stop(); // should fix jerking problem from last year?
        return;
    }

    const auto dt = CrcLib::GetDeltaTimeMillis();

    // Update remote state`
    remoteState = RState::Next();

    linearSlideNextButton.addSample(remoteState[LINEAR_SLIDE_NEXT_BUTTON], dt);
    linearSlidePrevButton.addSample(remoteState[LINEAR_SLIDE_PREV_BUTTON], dt);

    // move robot
    driveTrain.move(remoteState[FORWARD_CHANNEL], remoteState[YAW_CHANNEL]);

    // Linear slide
    if (remoteState[LINEAR_SLIDE_MANUAL_CHANNEL]) {
        if (elevator.setManualMode()) {
            linearSlideLevel = -1;
        }
        elevator.move(remoteState[LINEAR_SLIDE_MANUAL_CHANNEL]);

    } else if (linearSlideNextButton.fell()) {
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

    } else if (linearSlidePrevButton.fell()) {
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
