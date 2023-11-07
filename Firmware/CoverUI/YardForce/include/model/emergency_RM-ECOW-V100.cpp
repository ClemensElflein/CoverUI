/**
 * @file emergency_RM-ECOW-V100.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "emergency_RM-ECOW-V100.h"
#include "../Emergency.hpp"

const EmergencyPinStateDef kEmergencyPinStateDefs[] = {
    {PIN_STOP_JP5, INPUT_PULLUP, Emergency_state::Emergency_stop1},
    {PIN_STOP_JP6, INPUT_PULLUP, Emergency_state::Emergency_stop2}};

Emergency emergency(kEmergencyPinStateDefs, sizeof(kEmergencyPinStateDefs) / sizeof(EmergencyPinStateDef));
