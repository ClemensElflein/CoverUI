/**
 * @file model_C500.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 model header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_MDL_C500_H
#define YARDFORCE_MDL_C500_H

#include "model/LEDcontrol_C500.hpp"
#include "model/buttons_C500.h"

#ifdef MOD_EMERGENCY
#include "model/emergency_C500.h"
#endif

// LowLevel Serial Pins
#define UART_LL_RX PA3
#define UART_LL_TX PA2

#endif // YARDFORCE_MDL_C500_H
