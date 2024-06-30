/**
 * @file main.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI main header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.4
 * @date 2023-11-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __YARDFORCE_MAIN_H
#define __YARDFORCE_MAIN_H

#include <Arduino.h> // Stock CoverUI is build now via Arduino framework (instead of HAL), which is ATM the only framework with STM32F030R8 and GD32F330R8 support
#include "datatypes.h"

#if defined(FIRMWARE_VERSION) && !(FIRMWARE_VERSION == 200)
#pragma GCC error "YardForce CoverUI port is based on OM- FIRMWARE_VERSION 200. Port code changes before compiling..."
#include <AbortCompile>
#else
#undef FIRMWARE_VERSION
#endif
#define FIRMWARE_VERSION 205

#if defined(MDL_C500)
#include "model_C500.h"
#elif defined(MDL_RMECOWV100)
#include "model_RM-ECOW-V100.h"
#elif defined(MDL_RMECOWV110)
#include "model_RM-ECOW-V110.h"
#elif defined(MDL_RMEC3V11)
#include "model_RM-EC3-V11.h"
#elif defined(MDL_SAXPRO)
#include "model_SAxPRO.h"
#else
#pragma GCC error "Missing model header!"
#include <AbortCompile>
#endif

#ifdef MOD_RAIN
#include "Rain.hpp"
#endif

#define BATT_ABS_MAX 28.7f
#define BATT_ABS_MIN 21.7f

#define BATT_FULL (BATT_ABS_MAX - 0.3f)
#define BATT_EMPTY (BATT_ABS_MIN + 0.3f)

// STM32/GD32 are single cores, also without threads.
// Send mutex calls of main.cpp to nirvana. Dangerous?
#define auto_init_mutex(name)
#define mutex_enter_blocking(ptr)
#define mutex_exit(ptr)

// OM names. Could also use those, but I prefer logic names instead of physical ones
extern HardwareSerial serial_ll;
#define uart1 &serial_ll

// Some dummy Pico-SDK definitions. Not used but by this we'll NOT pollution original code to much with #ifdefs
#define pio0 NULL
#define pio1 NULL
typedef bool *PIO;
#define buzzer_SM_CYCLE 10800

// Forward declaration for ../main.cpp
extern void buzzer_program_put_words(PIO pio, unsigned int sm, uint32_t repeat, uint32_t duration, uint32_t gap);
extern void uart_putc(HardwareSerial *Serial, uint8_t c);
extern bool uart_is_readable(HardwareSerial *Serial);
extern void Force_LED_off(uint8_t led_num, bool force);

#endif // __YARDFORCE_MAIN_H