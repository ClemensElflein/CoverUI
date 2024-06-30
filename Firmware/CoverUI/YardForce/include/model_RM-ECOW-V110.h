/**
 * @file model_RM-ECOW-V110.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.1.0 model header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-06-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_MDL_RMECOWV110_H
#define YARDFORCE_MDL_RMECOWV110_H

#include "model/LEDcontrol_RM-ECOW-V110.hpp"
#include "model/buttons_RM-ECOW-V110.h"
#include "model/hatch_RM-ECOW-V110.hpp"

#ifdef MOD_EMERGENCY
#include "model/emergency_RM-ECOW-V110.h"
#endif

// LowLevel Serial Pins
#define UART_LL_RX PA3
#define UART_LL_TX PA2

#endif // YARDFORCE_MDL_RMECOWV110_H
