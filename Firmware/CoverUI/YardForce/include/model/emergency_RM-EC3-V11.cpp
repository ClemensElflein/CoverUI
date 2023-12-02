/**
 * @file emergency_RM-EC3-V11.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V1.1 CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "emergency_RM-EC3-V11.h"
#include "../Emergency.hpp"

const EmergencyPinStateDef kEmergencyPinStateDefs[] = {
#ifdef MOD_HALL
    {PIN_HALL1, INPUT_PULLUP, Emergency_state::Emergency_lift1},
    {PIN_HALL2, INPUT_PULLUP, Emergency_state::Emergency_lift1},
    {PIN_HALL3, INPUT_PULLUP, Emergency_state::Emergency_lift2},
    {PIN_HALL4, INPUT_PULLUP, Emergency_state::Emergency_lift2},
#endif
#ifdef MOD_STOP
    {PIN_STOP1, INPUT_PULLUP, Emergency_state::Emergency_stop1},
    {PIN_STOP2, INPUT_PULLUP, Emergency_state::Emergency_stop2},
#endif
};

Emergency emergency(kEmergencyPinStateDefs, sizeof(kEmergencyPinStateDefs) / sizeof(EmergencyPinStateDef));
