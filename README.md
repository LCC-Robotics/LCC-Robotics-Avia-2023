# LCC Robotics 2023 Repository

This is the repo used for the LCC '22-'23 robot. 

## Dependencies
[CrcLib@1.2.0](https://github.com/Robotique-CRC-Robotics/Robotique-CRC-Robotics/tree/master/CrcLib/CrcLib)

[Servo@1.1.8](https://github.com/arduino-libraries/Servo)

## Build Instructions

### VSCode

#### MacOS

1. Install dependencies into `/User/<USERNAME>/Documents/Arduino/libraries`. This can be done using `arduino-cli` or manually. 
2. Install the `Arduino` extension for VSCode.
3. Initialize project by executing the `Arduino: Initialize` command.
4. Create a `c_cpp_properties.json` in the `.vscode` directory, using the following template: 
```
{
    "configurations": [
        {
            "cStandard": "c17",
            "cppStandard": "c++17",
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "forcedInclude": [
                "<USERNAME>/Library/Arduino15/packages/arduino/hardware/avr/<AVR_VERSION>/cores/arduino/Arduino.h"
            ],
            "includePath": [
                "<USERNAME>/Library/Arduino15/packages/arduino/hardware/avr/<AVR_VERSION>/cores/arduino",
                "<USERNAME>/Library/Arduino15/packages/arduino/tools/avr-gcc/<VERSION>-arduino7/avr/include",
                "<USERNAME>/Library/Arduino15/packages/arduino/tools/avr-gcc/<VERSION>-arduino7/lib/gcc/avr/7.3.0/include",
                "<USERNAME>/Library/Arduino15/packages/arduino/hardware/avr/<AVR_VERSION>/variants/mega",
                "<USERNAME>/Library/Arduino15/packages/arduino/hardware/**",
                "<USERNAME>/Documents/Arduino/libraries/**",
                "<USERNAME>/Documents/Arduino/libraries/CrcLib_1.2.0/**",
                "${workspaceFolder}/lib/**",
                "${workspaceFolder}/**"
            ],
            "intelliSenseMode": "${default}"
        }
    ],
    "version": 4
```
5. ...
6. ...
7. Enjoy!

## Authors

Angela Wang

Kenneth Lew
