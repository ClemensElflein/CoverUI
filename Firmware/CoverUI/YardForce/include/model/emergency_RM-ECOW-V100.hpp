/**
 * @file emergency_RM-ECOW-V100.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef YARDFORCE_EMERGENCY_C500_H
#define YARDFORCE_EMERGENCY_C500_H

#include <Arduino.h>
#include "../../../BttnCtl.h"
#include "../Emergency.hpp"

#define PIN_STOP_JP5 PC10
#define PIN_STOP_JP6 PA15

const EmergencyPinStateDef kEmergencyPinStateDefs[] = {
    {PIN_STOP_JP5, INPUT_PULLUP, Emergency_state::Emergency_stop1},
    {PIN_STOP_JP6, INPUT_PULLUP, Emergency_state::Emergency_stop2}};

Emergency emergency(kEmergencyPinStateDefs, sizeof(kEmergencyPinStateDefs) / sizeof(EmergencyPinStateDef));

#endif // YARDFORCE_EMERGENCY_C500_H
