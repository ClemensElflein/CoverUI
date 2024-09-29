/**
 * @file emergency_RM-ECOW-V100.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2024-09-28
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */
#include "emergency_RM-ECOW-V100.h"

#include <Arduino.h>

#include "../Emergency.hpp"

const EmergencyPinStateDef kEmergencyPinStateDefs[] = {
#ifdef MOD_HALL
    {PIN_HALL1, INPUT_PULLUP, Emergency_state::Emergency_lift1},
    {PIN_HALL2, INPUT_PULLUP, Emergency_state::Emergency_lift1},
    {PIN_HALL3, INPUT_PULLUP, Emergency_state::Emergency_lift2},
    {PIN_HALL4, INPUT_PULLUP, Emergency_state::Emergency_lift2},
#endif
#ifdef MOD_STOP
    {PIN_STOP_JP5, INPUT_PULLUP, Emergency_state::Emergency_stop1},
    {PIN_STOP_JP6, INPUT_PULLUP, Emergency_state::Emergency_stop2}
#endif
};

Emergency emergency(kEmergencyPinStateDefs, sizeof(kEmergencyPinStateDefs) / sizeof(EmergencyPinStateDef));
