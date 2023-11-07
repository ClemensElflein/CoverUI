/**
 * @file emergency_C500.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef YARDFORCE_EMERGENCY_C500_H
#define YARDFORCE_EMERGENCY_C500_H

#include <Arduino.h>
#include "../Emergency.hpp"

#define PIN_HALL_STOP_WHITE PC5
#define PIN_HALL_STOP_YELLOW PB6
#define PIN_HALL_WHEEL_RED PB7
#define PIN_HALL_WHEEL_BLUE PB8

extern Emergency emergency;

#endif // YARDFORCE_EMERGENCY_C500_H
