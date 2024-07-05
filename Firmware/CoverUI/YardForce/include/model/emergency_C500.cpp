/**
 * @file emergency_C500.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "emergency_C500.h"
#include "../Emergency.hpp"

const EmergencyPinStateDef kEmergencyPinStateDefs[] = {
    {PIN_HALL_STOP_WHITE, INPUT, Emergency_state::Emergency_stop1},
    {PIN_HALL_STOP_YELLOW, INPUT, Emergency_state::Emergency_stop2},
    {PIN_HALL_WHEEL_RED, INPUT, Emergency_state::Emergency_lift1},
    {PIN_HALL_WHEEL_BLUE, INPUT, Emergency_state::Emergency_lift2}};

Emergency emergency(kEmergencyPinStateDefs, sizeof(kEmergencyPinStateDefs) / sizeof(EmergencyPinStateDef));
