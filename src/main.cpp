#include <CrcLib.h> // https://robocrc.atlassian.net/wiki/spaces/AR/pages/403767325/CrcLib+Functions+-+An+overview

#include <Bounce2.h>       // https://thomasfredericks.github.io/Bounce2/files/index.html
#include <arduino-timer.h> // https://github.com/contrem/arduino-timer

#include "motor.h"
#include "utils.h"
#include "sensor.h"

using namespace Crc;

#define FORWARD_CHANNEL ANALOG::JOYSTICK1_Y
#define YAW_CHANNEL ANALOG::JOYSTICK1_X

const int elevator_heights[8] = {0, 6, 17, 38, 66, 102, 146, 190};

Timer<5, millis> timer;

ArcadeDriveTrain driveTrain = {
    .LMotor = {.pin = CRC_PWM_1},
    .RMotor = {.pin = CRC_PWM_2}};

void setup()
{
    CrcLib::Initialize();

    CrcLib::InitializePwmOutput(CRC_PWM_1);
    CrcLib::InitializePwmOutput(CRC_PWM_2);
    CrcLib::InitializePwmOutput(CRC_PWM_3);
    CrcLib::InitializePwmOutput(CRC_PWM_4);

    // recalculate motor output every MOTOR_UPDATE_INTERVAL
    timer.every(MOTOR_UPDATE_INTERVAL,
                [](void *) -> bool
                {
                    driveTrain.update();
                    return true;
                });

#ifdef DEBUG            // only start serial if in debug mode (serial can affect performance)
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

    driveTrain.move(CrcLib::ReadAnalogChannel(FORWARD_CHANNEL),
                    CrcLib::ReadAnalogChannel(YAW_CHANNEL));
}
