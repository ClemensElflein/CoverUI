/**
 * @file MDL_RM-ECOW-V100.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_MDL_RMECOWV100_H
#define YARDFORCE_MDL_RMECOWV100_H


#define MDL_RMECOWV100
#define LEDCNTRL_HDR "YardForce/include/LEDcontrol_RM-ECOW-V100.hpp"

// LowLevel Serial
#define UART_LL_RX PA3
#define UART_LL_TX PA2

//HardwareSerial serial_ll(PA3, PA2); // Serial connection to LowLevel MCU, JP2 Pin 1+3

#endif /* YARDFORCE_MDL_RMECOWV100_H */
