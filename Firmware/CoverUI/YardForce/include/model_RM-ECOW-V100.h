/**
 * @file model_RM-ECOW-V100.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 model header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_MDL_RMECOWV100_H
#define YARDFORCE_MDL_RMECOWV100_H

#include "model/LEDcontrol_RM-ECOW-V100.hpp"
#include "model/buttons_RM-ECOW-V100.h"
#include "model/hatch_RM-ECOW-V100.hpp"

#ifdef MOD_EMERGENCY
#include "model/emergency_RM-ECOW-V100.h"
#endif

// LowLevel Serial Pins
#define UART_LL_RX PA3
#define UART_LL_TX PA2

#endif // YARDFORCE_MDL_RMECOWV100_H
