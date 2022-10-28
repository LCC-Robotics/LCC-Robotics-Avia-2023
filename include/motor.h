#ifndef __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_
#define __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_

#include "CrcLib.h"

enum PWM {
    DRIVE_FL = CRC_PWM_1,
    DRIVE_FR = CRC_PWM_2,
    DRIVE_BL = CRC_PWM_3,
    DRIVE_BR = CRC_PWM_4
};

enum Servos {

};

void initPWM();
void initServos();

#endif // __ROBOTICS_LCC_ROBOTICS_22_23_INC_MOTOR_H_
