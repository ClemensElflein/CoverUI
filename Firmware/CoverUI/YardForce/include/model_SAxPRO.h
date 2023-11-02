/**
 * @file model_SAxPRO.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO model header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_MDL_SAXPRO_H
#define YARDFORCE_MDL_SAXPRO_H

#define MDL_SAXPRO

#define HAS_LEDS
#define LEDCTRL_HDR "YardForce/include/model/LEDcontrol_SAxPRO.hpp"

#define BUTTONS_HDR "YardForce/include/model/buttons_SAxPRO.hpp"

#define HAS_DISPLAY
#define DISPLAY_HDR "YardForce/include/WYM240128K1.hpp"

//#include "../WYM240128K1.hpp" // display namespace

//#ifdef MOD_EMERGENCY
//#define EMERGENCY_HDR "YardForce/emergency_RM-ECOW-V100.hpp"
//#endif

// LowLevel Serial Pins
#define UART_LL_RX PA10
#define UART_LL_TX PA9

#endif // YARDFORCE_MDL_SAXPRO_H
