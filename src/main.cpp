#include <Arduino.h>
#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview
#include <Encoder.h>


#include <etl/debounce.h>

#include "remoteState.h"
#include "utils.h"

using namespace Crc;

Encoder flipperEnco(CRC_ENCO_A, CRC_ENCO_B);
bool floorFlipState = true;
int flipped = 5;
int original = 5;

// ====================
// Controller Input
// ====================

constexpr ANALOG FORWARD_CHANNEL = ANALOG::JOYSTICK1_Y; //done
constexpr ANALOG YAW_CHANNEL = ANALOG::JOYSTICK1_X; //done

constexpr ANALOG LINEAR_SLIDE_CHANNEL = ANALOG::JOYSTICK2_Y; //done

constexpr BUTTON DIE_BUTTON = BUTTON::LOGO; // done

constexpr BUTTON FLOOR_FLIPPER_BUTTON = BUTTON::COLORS_DOWN; 

constexpr BUTTON EXTENDER_FORWARDS_BUTTON = BUTTON::R1; //done
constexpr BUTTON EXTENDER_BACKWARDS_BUTTON = BUTTON::L1; //done

constexpr BUTTON EXTENDER_FLIP_FORWARDS_BUTTON = BUTTON::COLORS_LEFT; //done
constexpr BUTTON EXTENDER_FLIP_BACKWARDS_BUTTON = BUTTON::COLORS_RIGHT; //done

// ====================
// Pins
// ====================

constexpr uint8_t LEFT_MOTOR_PIN = CRC_PWM_1;
constexpr uint8_t RIGHT_MOTOR_PIN = CRC_PWM_2;

constexpr uint8_t LINEAR_SLIDE_PIN = CRC_PWM_3;

constexpr uint8_t FLOOR_FLIPPER_PIN = CRC_PWM_4;
constexpr uint8_t FLOOR_FLIPPER_ENCODER_PIN = CRC_DIG_2;

constexpr uint8_t EXTENDER_MOTOR_PIN = CRC_PWM_7;
constexpr uint8_t EXTENDER_FLIPPER_PIN = CRC_PWM_8;

// ====================
// Constants
// ====================

constexpr float DEFAULT_SLEW_RATE = 0.75;

// ====================
// Objects
// ====================

RState lastRemoteState;
RState remoteState; // custom remote state that uses the forbidden arts

etl::debounce<> die_button_debounce;
etl::debounce<> floor_button_debounce;

// ====================

void setup()
{
    CrcLib::Initialize();

    // Initialize pins
    CrcLib::InitializePwmOutput(LEFT_MOTOR_PIN, false);
    CrcLib::InitializePwmOutput(RIGHT_MOTOR_PIN, false);
    CrcLib::InitializePwmOutput(LINEAR_SLIDE_PIN, false);
    CrcLib::InitializePwmOutput(FLOOR_FLIPPER_PIN, false);
    CrcLib::InitializePwmOutput(EXTENDER_MOTOR_PIN, false);
    CrcLib::InitializePwmOutput(EXTENDER_FLIPPER_PIN, false);

    CrcLib::SetDigitalPinMode(FLOOR_FLIPPER_ENCODER_PIN, INPUT);

#ifdef DEBUG // only start serial if in debug mode (serial can affect performance)
    Serial.begin(BAUD); // macro defined in platformio.ini
#endif
}

void loop()
{
    CrcLib::Update();

    const unsigned int dt = CrcLib::GetDeltaTimeMillis();

    // Update remote state
    lastRemoteState = remoteState;
    remoteState = RState::Next();

    die_button_debounce.add(remoteState[DIE_BUTTON]);
    floor_button_debounce.add(remoteState[FLOOR_FLIPPER_BUTTON]);

    // Check if commands are valid
    if (!CrcLib::IsCommValid() || die_button_debounce.is_held()) // controller not connected, don't run loop
    {
        CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, LOW);
        CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, LOW);
        CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, LOW);
        CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, LOW);
        return CrcLib::MoveArcade(0, 0, LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN); // should fix jerking problem from last year?
    }

    // max amount that input can deviate by
    const auto maxSlew = static_cast<int8_t>(DEFAULT_SLEW_RATE * dt);

    // move robot
    CrcLib::MoveArcade(
        limitSlew<int8_t>(remoteState[FORWARD_CHANNEL], lastRemoteState[FORWARD_CHANNEL], maxSlew),
        limitSlew<int8_t>(remoteState[YAW_CHANNEL], lastRemoteState[YAW_CHANNEL], maxSlew),
        LEFT_MOTOR_PIN,
        RIGHT_MOTOR_PIN);

    // extender
    if (remoteState[EXTENDER_FORWARDS_BUTTON]) {
        CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, PWM_MOTOR_BOUNDS.upper);
    }
    if (remoteState[EXTENDER_BACKWARDS_BUTTON]) {
        CrcLib::SetPwmOutput(EXTENDER_MOTOR_PIN, PWM_MOTOR_BOUNDS.lower);
    }
    
    // extender flipper
    if (remoteState[EXTENDER_FLIP_FORWARDS_BUTTON]){
        CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, PWM_MOTOR_BOUNDS.lower);
    }
    if (remoteState[EXTENDER_FLIP_BACKWARDS_BUTTON]){
        CrcLib::SetPwmOutput(EXTENDER_FLIPPER_PIN, PWM_MOTOR_BOUNDS.upper);
    }

    // linear slide
    CrcLib::SetPwmOutput(LINEAR_SLIDE_PIN, remoteState[LINEAR_SLIDE_CHANNEL]);

    // floor flipper
    if (remoteState[FLOOR_FLIPPER_BUTTON]){
        if (floorFlipState == true){
            if (flipperEnco.read() != flipped) {
                CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, PWM_MOTOR_BOUNDS.upper);
            }
            else {
                CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, 0);
                floorFlipState = false;
            }
        }
        else if (floorFlipState == false){
            if (flipperEnco.read() != original) {
                CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, PWM_MOTOR_BOUNDS.lower);
            }
            else {
                CrcLib::SetPwmOutput(FLOOR_FLIPPER_PIN, 0);
                floorFlipState = true;
            }
        }
    }
}
