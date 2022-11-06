#include <Arduino.h> // https://www.arduino.cc/reference/en/

#include <Bounce2.h> // Debounce library: https://thomasfredericks.github.io/Bounce2/files/index.html
#include <CrcLib.h> // Crc dependency: https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview
#include <arduino-timer.h> // Timer libarary: https://github.com/contrem/arduino-timer

#include "motor.h"
#include "utils.h"

#define FORWARD_CHANNEL ANALOG::JOYSTICK1_Y
#define YAW_CHANNEL ANALOG::JOYSTICK1_X
#define STRAFE_CHANNEL ANALOG::JOYSTICK2_X

using namespace Crc;

Timer<5, millis> timer;

DriveTrain driveTrain = {
    .FLMotor = { .pin = CRC_PWM_1 },
    .FRMotor = { .pin = CRC_PWM_2 },
    .BLMotor = { .pin = CRC_PWM_3 },
    .BRMotor = { .pin = CRC_PWM_4 }
};

void setup()
{
    CrcLib::Initialize();

    CrcLib::InitializePwmOutput(CRC_PWM_1);
    CrcLib::InitializePwmOutput(CRC_PWM_2);
    CrcLib::InitializePwmOutput(CRC_PWM_3);
    CrcLib::InitializePwmOutput(CRC_PWM_4);

    // recalculate motor output every MOTOR_UPDATE_INTERVAL
    timer.every(MOTOR_UPDATE_INTERVAL,
        [](void*) -> bool {
            driveTrain.update();
            return true;
        });

#ifdef DEBUG // only start serial if in debug mode (serial can affect performance)
    Serial.begin(BAUD); // macro defined in platformio.ini
#endif
}

void loop()
{
    timer.tick();
    CrcLib::Update();

    if (!CrcLib::IsCommValid()) // controller not connected, don't run loop
    {
        driveTrain.stop(); // should fix jerking problem from last year?
        return; 
    }

    driveTrain.moveHolonomic(
        CrcLib::ReadAnalogChannel(FORWARD_CHANNEL),
        CrcLib::ReadAnalogChannel(YAW_CHANNEL),
        CrcLib::ReadAnalogChannel(STRAFE_CHANNEL));
}
