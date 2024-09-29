/**
 * @file emergency_RM-ECOW-V100.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2024-09-28
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */

#ifndef YARDFORCE_EMERGENCY_RMECOWV100_HPP
#define YARDFORCE_EMERGENCY_RMECOWV100_HPP

#include <Arduino.h>

#include "../Emergency.hpp"

#define PIN_STOP_JP5 PC10
#define PIN_STOP_JP6 PA15

#define PIN_HALL1 PA8   // LIFT
#define PIN_HALL2 PF6   // LIFTX
#define PIN_HALL3 PA12  // LBUMP
#define PIN_HALL4 PA11  // RBUMP

extern Emergency emergency;

#endif  // YARDFORCE_EMERGENCY_RMECOWV100_HPP
