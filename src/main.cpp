#include <Arduino.h>
#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview
#include <Encoder.h>

#include <etl/debounce.h>

#include "motor.h"
#include "remoteState.h"
#include "utils.h"

using Crc::CrcLib;

// ====================
// Controller Input
// ====================

constexpr ANALOG FORWARD_CHANNEL = ANALOG::JOYSTICK1_Y; // done
constexpr ANALOG YAW_CHANNEL = ANALOG::JOYSTICK1_X; // done

constexpr ANALOG LINEAR_SLIDE_CHANNEL = ANALOG::JOYSTICK2_Y; // done

constexpr BUTTON DIE_BUTTON = BUTTON::LOGO; // done
constexpr BUTTON FLOOR_FLIPPER_BUTTON = BUTTON::COLORS_DOWN;

constexpr BUTTON EXTENDER_FORWARDS_BUTTON = BUTTON::R1; // done
constexpr BUTTON EXTENDER_BACKWARDS_BUTTON = BUTTON::L1; // done

constexpr BUTTON EXTENDER_FLIP_FORWARDS_BUTTON = BUTTON::COLORS_LEFT; // done
constexpr BUTTON EXTENDER_FLIP_BACKWARDS_BUTTON = BUTTON::COLORS_RIGHT; // done

// ====================
// Pins
// ====================

constexpr uint8_t LEFT_MOTOR_PIN = CRC_PWM_1;
constexpr uint8_t RIGHT_MOTOR_PIN = CRC_PWM_2;

constexpr uint8_t LINEAR_SLIDE_PIN = CRC_PWM_3;

constexpr uint8_t FLOOR_FLIPPER_PIN = CRC_PWM_4;

constexpr uint8_t FLOOR_FLIPPER_ENCODER_PIN_A = CRC_ENCO_A;
constexpr uint8_t FLOOR_FLIPPER_ENCODER_PIN_B = CRC_ENCO_B;

constexpr uint8_t EXTENDER_MOTOR_PIN = CRC_PWM_7;
constexpr uint8_t EXTENDER_FLIPPER_PIN = CRC_PWM_8;

// ====================
// Constants
// ====================

constexpr int32_t FLIPPER_RANGE = 3000; // steps

// ====================
// Objects
// ====================

RState lastRemoteState;
RState remoteState; // custom remote state that uses the forbidden arts

etl::debounce<> dieButtonDebounce;
etl::debounce<> FloorButtonDebounce;

SlewLimitingMotor linearSlide { LINEAR_SLIDE_PIN };
SlewLimitingMotor floorFlipper { FLOOR_FLIPPER_PIN };
SlewLimitingMotor extender { EXTENDER_MOTOR_PIN };
SlewLimitingMotor extenderFlipper { EXTENDER_MOTOR_PIN };

Encoder flipperEncoder(FLOOR_FLIPPER_ENCODER_PIN_A, FLOOR_FLIPPER_ENCODER_PIN_B);
Range<int32_t> flipperRange;

// ====================

void setup()
{
    CrcLib::Initialize();

    // Initialize pins
    CrcLib::InitializePwmOutput(LEFT_MOTOR_PIN, false);
    CrcLib::InitializePwmOutput(RIGHT_MOTOR_PIN, true);
    CrcLib::InitializePwmOutput(LINEAR_SLIDE_PIN, false);
    CrcLib::InitializePwmOutput(FLOOR_FLIPPER_PIN, false);
    CrcLib::InitializePwmOutput(EXTENDER_MOTOR_PIN, false);
    CrcLib::InitializePwmOutput(EXTENDER_MOTOR_PIN, false);

    CrcLib::SetDigitalPinMode(FLOOR_FLIPPER_ENCODER_PIN_A, INPUT);
    CrcLib::SetDigitalPinMode(FLOOR_FLIPPER_ENCODER_PIN_B, INPUT);

    flipperRange.lower = flipperEncoder.read();
    flipperRange.upper = flipperRange.lower + FLIPPER_RANGE;

#ifdef DEBUG // only start serial if in debug mode (serial can affect performance)
    Serial.begin(BAUD); // macro defined in platformio.ini
#endif
}

void loop()
{
    CrcLib::Update();
    // Update remote state
    lastRemoteState = remoteState;
    remoteState = RState::Next();

    dieButtonDebounce.add(remoteState[DIE_BUTTON]);
    FloorButtonDebounce.add(remoteState[FLOOR_FLIPPER_BUTTON]);

    // Check if commands are valid
    if (!CrcLib::IsCommValid() || dieButtonDebounce.is_held()) // controller not connected, don't run loop
    {
        CrcLib::Update();

        CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, 0);
        CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, 0);
        CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, 0);
        CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, 0);
        CrcLib::MoveArcade(0, 0, LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN); // should fix jerking problem from last year?

        return;
    }

    const unsigned int dt = CrcLib::GetDeltaTimeMillis();

    linearSlide.update(dt);
    floorFlipper.update(dt);
    extender.update(dt);
    extenderFlipper.update(dt);

    // ======================
    // MOVEMENT
    // ======================

    const auto maxMotorChange = static_cast<int8_t>(DEFAULT_SLEW_RATE * dt); // max amount that motor output can deviate by
    CrcLib::MoveArcade(
        static_cast<int8_t>(limitSlew(remoteState[FORWARD_CHANNEL], lastRemoteState[FORWARD_CHANNEL], maxMotorChange)),
        static_cast<int8_t>(limitSlew(remoteState[YAW_CHANNEL], lastRemoteState[YAW_CHANNEL], maxMotorChange)),
        LEFT_MOTOR_PIN,
        RIGHT_MOTOR_PIN);

    // ======================
    // Linear Slide
    // ======================

    linearSlide.set(remoteState[LINEAR_SLIDE_CHANNEL]);

    // ======================
    // EXTENDER
    // ======================

    int8_t extenderOutput = 0;

    if (remoteState[EXTENDER_FORWARDS_BUTTON])
        extenderOutput = PWM_MOTOR_BOUNDS.upper;

    if (remoteState[EXTENDER_BACKWARDS_BUTTON])
        extenderOutput = PWM_MOTOR_BOUNDS.lower;

    extender.set(extenderOutput);

    // ======================
    // EXTENDER FLIPPER
    // ======================

    int8_t extenderFlipperOutput = 0;

    if (remoteState[EXTENDER_FLIP_FORWARDS_BUTTON]) {
        extenderFlipperOutput = PWM_MOTOR_BOUNDS.upper;
    } else if (remoteState[EXTENDER_FLIP_BACKWARDS_BUTTON]) {
        extenderFlipperOutput = PWM_MOTOR_BOUNDS.lower;
    }
    extenderFlipper.set(extenderFlipperOutput);

    // ======================
    // FLOOR FLIPPER (checked)
    // ======================

    int32_t flipperEncoderValue = flipperEncoder.read();

    int8_t flipperOutput = 0;

    if (FloorButtonDebounce.is_held() && flipperEncoderValue <= flipperRange.upper) {
        flipperOutput = PWM_MOTOR_BOUNDS.lower;
    } else if (flipperEncoderValue > 0) {
        flipperOutput = PWM_MOTOR_BOUNDS.upper;
    } else {
        flipperOutput = 0;
    }

    floorFlipper.set(flipperOutput);
}
