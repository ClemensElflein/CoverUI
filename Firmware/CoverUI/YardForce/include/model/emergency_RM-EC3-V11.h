/**
 * @file emergency_RM-EC3-V11.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V1.1 CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef YARDFORCE_EMERGENCY_RMEC3V11_H
#define YARDFORCE_EMERGENCY_RMEC3V11_H

#include <Arduino.h>
#include "../Emergency.hpp"

#define PIN_STOP1 PC15
#define PIN_STOP2 PB6

#define PIN_HALL1 PB15
#define PIN_HALL2 PC0
#define PIN_HALL3 PC8
#define PIN_HALL4 PC9

extern Emergency emergency;

#endif // YARDFORCE_EMERGENCY_RMEC3V11_H
