#include <Arduino.h>
#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview
#include <Encoder.h>

#include <etl/debounce.h>

#include "motor.h"
#include "remoteState.h"
#include "utils.h"

using namespace Crc;

// ====================
// Controller Input
// ====================

constexpr ANALOG FORWARD_CHANNEL = ANALOG::JOYSTICK1_Y; 
constexpr ANALOG YAW_CHANNEL = ANALOG::JOYSTICK1_X; 

constexpr ANALOG LINEAR_SLIDE_CHANNEL = ANALOG::JOYSTICK2_Y; 

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

constexpr uint8_t EXTENDER_MOTOR_PIN = CRC_PWM_7;
constexpr uint8_t EXTENDER_FLIPPER_PIN = CRC_PWM_8;

// ====================
// Constants
// ====================

constexpr int32_t flipperUpper = 1300; 
constexpr int32_t flipperLower = 0;

// ====================
// Objects
// ====================

RState lastRemoteState;
RState remoteState; // custom remote state that uses the forbidden arts

etl::debounce<> dieButtonDebounce;
etl::debounce<> FloorButtonDebounce;

/*
SlewLimitingMotor linearSlide { LINEAR_SLIDE_PIN };
SlewLimitingMotor floorFlipper { FLOOR_FLIPPER_PIN };
SlewLimitingMotor extender { EXTENDER_MOTOR_PIN };
SlewLimitingMotor extenderFlipper { EXTENDER_FLIPPER_PIN };
*/

Encoder flipperEncoder(FLOOR_FLIPPER_ENCODER_PIN_A, FLOOR_FLIPPER_ENCODER_PIN_B);

//Range<int32_t> flipperRange;

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
    if ((CrcLib::IsCommValid() == false) || (CrcLib::ReadDigitalChannel(DIE_BUTTON) == 1)) // controller not connected, don't run loop
    {
        CrcLib::Update();

        CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, 0);
        CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, 0);
        CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, 0);
        CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, 0);
        CrcLib::SetPwmOutput(LEFT_MOTOR_PIN, 0); 
        CrcLib::SetPwmOutput(RIGHT_MOTOR_PIN, 0);// should fix jerking problem from last year?

        return;
    }

    //const unsigned int dt = CrcLib::GetDeltaTimeMillis();
    
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

    //linearSlide.set(remoteState[LINEAR_SLIDE_CHANNEL]);
    int linearSlideOutput = CrcLib::ReadAnalogChannel(LINEAR_SLIDE_CHANNEL);
    CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, linearSlideOutput);
    
    // ======================
    // EXTENDER
    // ======================

    int8_t extenderOutput = 0;

    if (remoteState[EXTENDER_FORWARDS_BUTTON]){
        extenderOutput = PWM_MOTOR_BOUNDS.upper;
    }
    if (remoteState[EXTENDER_BACKWARDS_BUTTON]){
        extenderOutput = PWM_MOTOR_BOUNDS.lower;
    }
    CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, extenderOutput);

    // ======================
    // EXTENDER FLIPPER
    // ======================

    int8_t extenderFlipperOutput = 0;

    if (remoteState[EXTENDER_FLIP_FORWARDS_BUTTON]) {
        extenderFlipperOutput = PWM_MOTOR_BOUNDS.upper;
    } 
    else if (remoteState[EXTENDER_FLIP_BACKWARDS_BUTTON]) {
        extenderFlipperOutput = PWM_MOTOR_BOUNDS.lower;
    }
    CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, extenderFlipperOutput);

    // ======================
    // FLOOR FLIPPER
    // ======================

    int32_t flipperEncoderValue = flipperEncoder.read();
    int8_t flipperOutput = 0;

    if (CrcLib::ReadDigitalChannel(FLOOR_FLIPPER_BUTTON) && flipperEncoderValue <= 1300) {
        flipperOutput = -128;
    }
    else if (flipperEncoderValue > 0) {
        flipperOutput = 127;
    }
    else {
        flipperOutput = 0;
    }
    
    CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, flipperOutput); 
}
