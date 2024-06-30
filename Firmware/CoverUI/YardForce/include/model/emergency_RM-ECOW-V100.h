/**
 * @file emergency_RM-ECOW-V100.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef YARDFORCE_EMERGENCY_RMECOWV100_HPP
#define YARDFORCE_EMERGENCY_RMECOWV100_HPP

#include <Arduino.h>
#include "../Emergency.hpp"

#define PIN_STOP_JP5 PC10
#define PIN_STOP_JP6 PA15

extern Emergency emergency;

#endif  // YARDFORCE_EMERGENCY_RMECOWV100_HPP
