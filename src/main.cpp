#include <Arduino.h>
#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview
#include <Encoder.h>

#include <etl/debounce.h>

// #include "motor.h"
// #include "remoteState.h"
#include "utils.h"

using namespace Crc;

// ====================
// Controller Input
// ====================

constexpr ANALOG FORWARD_CHANNEL = ANALOG::JOYSTICK1_Y;
constexpr ANALOG YAW_CHANNEL = ANALOG::JOYSTICK1_X;

constexpr ANALOG LINEAR_SLIDE_CHANNEL = ANALOG::JOYSTICK2_Y;

constexpr ANALOG STOP_IT = ANALOG::GACHETTE_L;

constexpr BUTTON DIE_BUTTON = BUTTON::LOGO;
constexpr BUTTON FLOOR_FLIPPER_BUTTON = BUTTON::COLORS_DOWN;

constexpr BUTTON EXTENDER_FORWARDS_BUTTON = BUTTON::R1;
constexpr BUTTON EXTENDER_BACKWARDS_BUTTON = BUTTON::L1;

constexpr BUTTON EXTENDER_FLIP_FORWARDS_BUTTON = BUTTON::COLORS_LEFT;
constexpr BUTTON EXTENDER_FLIP_BACKWARDS_BUTTON = BUTTON::COLORS_RIGHT;

// ====================
// Pins
// ====================

constexpr uint8_t LEFT_MOTOR_PIN = CRC_PWM_1;
constexpr uint8_t RIGHT_MOTOR_PIN = CRC_PWM_2;

constexpr uint8_t LINEAR_SLIDE_PIN = CRC_PWM_3;

constexpr uint8_t FLOOR_FLIPPER_PIN = CRC_PWM_4;

constexpr uint8_t FLOOR_FLIPPER_ENCODER_PIN_A = CRC_ENCO_A;
constexpr uint8_t FLOOR_FLIPPER_ENCODER_PIN_B = CRC_ENCO_B;

constexpr uint8_t EXTENDER_MOTOR_PIN = CRC_PWM_5;
constexpr uint8_t EXTENDER_FLIPPER_PIN = CRC_PWM_8;


// ====================
// Objects
// ====================


Encoder flipperEncoder(FLOOR_FLIPPER_ENCODER_PIN_A, FLOOR_FLIPPER_ENCODER_PIN_B);

// Range<int32_t> flipperRange;

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
    CrcLib::InitializePwmOutput(EXTENDER_FLIPPER_PIN, false);

    flipperEncoder.write(0);

#ifdef DEBUG // only start serial if in debug mode (serial can affect performance)
    Serial.begin(BAUD); // macro defined in platformio.ini
#endif
}

void loop()
{
    CrcLib::Update();
    // Update remote state
    // lastRemoteState = remoteState;
    // remoteState = RState::Next();

    // dieButtonDebounce.add(remoteState[DIE_BUTTON]);
    // FloorButtonDebounce.add(remoteState[FLOOR_FLIPPER_BUTTON]);

    // Check if commands are valid
    if (!CrcLib::IsCommValid() || CrcLib::ReadDigitalChannel(DIE_BUTTON)) // controller not connected, don't run loop
    {
        CrcLib::Update();

        CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, 0);
        CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, 0);
        CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, 0);
        CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, 0);
        CrcLib::SetPwmOutput(LEFT_MOTOR_PIN, 0);
        CrcLib::SetPwmOutput(RIGHT_MOTOR_PIN, 0); // should fix jerking problem from last year?

        return;
    }

    // const unsigned int dt = CrcLib::GetDeltaTimeMillis();

    /*
   linearSlide.update(dt);
   floorFlipper.update(dt);
   extender.update(dt);
   extenderFlipper.update(dt);
   */

    // ======================
    // MOVEMENT
    // ======================

    // const auto maxMotorChange = static_cast<int8_t>(DEFAULT_SLEW_RATE * dt); // max amount that motor output can deviate by
    CrcLib::MoveArcade(FORWARD_CHANNEL, YAW_CHANNEL, LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN);

    // ======================
    // Linear Slide
    // ======================

    // linearSlide.set(remoteState[LINEAR_SLIDE_CHANNEL]);
    int linearOutput = CrcLib::ReadAnalogChannel(LINEAR_SLIDE_CHANNEL);
    
        if (linearOutput >= 5 || linearOutput <= -5) {
            CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, linearOutput);
        } 
        //else if (CrcLib::ReadAnalogChannel(STOP_IT) > 0){
        //    CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, 1);
        //}
        else {
            CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, 0);
        }
    


    // ======================
    // EXTENDER
    // ======================

    int8_t extenderOutput = 0;

    if (CrcLib::ReadDigitalChannel(EXTENDER_FORWARDS_BUTTON)) {
        extenderOutput = 127;
    }
    if (CrcLib::ReadDigitalChannel(EXTENDER_BACKWARDS_BUTTON)) {
        extenderOutput = -128;
    }
    CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, extenderOutput);

    // ======================
    // EXTENDER FLIPPER
    // ======================

    int8_t extenderFlipperOutput = 0;

    if (CrcLib::ReadDigitalChannel(EXTENDER_FLIP_FORWARDS_BUTTON)) {
        extenderFlipperOutput = 127;
    } else if (CrcLib::ReadDigitalChannel(EXTENDER_FLIP_BACKWARDS_BUTTON)) {
        extenderFlipperOutput = -128;
    }
    CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, extenderFlipperOutput);

    // ======================
    // FLOOR FLIPPER
    // ======================

    int32_t flipperEncoderValue = flipperEncoder.read();
    int8_t flipperOutput = 0;

    if (CrcLib::ReadDigitalChannel(FLOOR_FLIPPER_BUTTON) && flipperEncoderValue < 1300) {
        flipperOutput = -64;
    } else if (flipperEncoderValue > 0 ) {
        flipperOutput = 64;
    }

    CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, flipperOutput);
}
