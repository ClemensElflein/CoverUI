/**
 * @file emergency_RM-ECOW-V110.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.1.0 CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-06-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <Arduino.h>
#include "emergency_RM-ECOW-V110.h"
#include "../Emergency.hpp"

const EmergencyPinStateDef kEmergencyPinStateDefs[] = {
#ifdef MOD_HALL
    {PIN_HALL1, INPUT_PULLUP, Emergency_state::Emergency_lift1},
    {PIN_HALL2, INPUT_PULLUP, Emergency_state::Emergency_lift1},
    {PIN_HALL3, INPUT_PULLUP, Emergency_state::Emergency_lift2},
    {PIN_HALL4, INPUT_PULLUP, Emergency_state::Emergency_lift2},
#endif
#ifdef MOD_STOP
    {PIN_STOP_JP6, INPUT_PULLUP, Emergency_state::Emergency_stop1},
    {PIN_STOP_JP8, INPUT_PULLUP, Emergency_state::Emergency_stop2},
#endif
};

Emergency emergency(kEmergencyPinStateDefs, sizeof(kEmergencyPinStateDefs) / sizeof(EmergencyPinStateDef));
