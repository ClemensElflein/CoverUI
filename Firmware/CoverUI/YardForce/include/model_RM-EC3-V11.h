/**
 * @file model_RM-EC3-V11.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V1.1 model header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_MDL_RMEC3V11_H
#define YARDFORCE_MDL_RMEC3V11_H

#include "model/LEDcontrol_RM-EC3-V11.hpp"
#include "model/buttons_RM-EC3-V11.h"
#include "model/hatch_RM-EC3-V11.hpp"
#include "model/Display_RM-EC3-V11.hpp"

#ifdef MOD_EMERGENCY
#include "model/emergency_RM-EC3-V11.h"
#endif

// LowLevel Serial Pins
#define UART_LL_RX PA3
#define UART_LL_TX PA2

#endif // YARDFORCE_MDL_RMEC3V11_H
