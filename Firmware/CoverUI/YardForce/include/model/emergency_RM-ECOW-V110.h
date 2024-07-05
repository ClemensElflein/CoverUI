/**
 * @file emergency_RM-ECOW-V110.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.1.0 CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-06-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef YARDFORCE_EMERGENCY_RMECOWV110_HPP
#define YARDFORCE_EMERGENCY_RMECOWV110_HPP

#include <Arduino.h>
#include "../Emergency.hpp"

#define PIN_STOP_JP6 PC10
#define PIN_STOP_JP8 PA15

#define PIN_HALL1 PA8 // LIFT
#define PIN_HALL2 PA9 // LIFTX
#define PIN_HALL3 PC8 // LBUMP
#define PIN_HALL4 PC9 // RBUMP

extern Emergency emergency;

#endif  // YARDFORCE_EMERGENCY_RMECOWV110_HPP
