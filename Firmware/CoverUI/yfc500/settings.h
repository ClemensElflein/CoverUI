/**
 * @file setup.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI setting (LEDs, Btns, Timer, ...) for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-04-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_SETUP_H
#define YFC500_SETUP_H

#include <Arduino.h>

//#ifdef MCU_STM32
#define LED_PIN_2HR PA4
#define LED_PIN_4HR PA5
#define LED_PIN_6HR PA6
#define LED_PIN_8HR PA7

#define LED_PIN_S1 PA0
#define LED_PIN_S2 PA1
#define LED_PIN_LOCK PC4

#define LED_PIN_MON PA15
#define LED_PIN_TUE PC10
#define LED_PIN_WED PC11
#define LED_PIN_THU PC12
#define LED_PIN_FRI PD2
#define LED_PIN_SAT PB3
#define LED_PIN_SUN PB4

#define LED_PIN_LIFTED PC0
#define LED_PIN_WIRE PC1
#define LED_PIN_BAT PC2
#define LED_PIN_CHARGE PC3

#define LED_PIN_REAR PB0
/*#else
#define LED_PIN_2HR PA4
#define LED_PIN_4HR PA5
#define LED_PIN_6HR PA6
#define LED_PIN_8HR PA7

#define LED_PIN_S1 PA0
#define LED_PIN_S2 PA1
#define LED_PIN_LOCK PA11

#define LED_PIN_MON PA15
#define LED_PIN_TUE 16
#define LED_PIN_WED 31
#define LED_PIN_THU 17
#define LED_PIN_FRI 32
#define LED_PIN_SAT PB3
#define LED_PIN_SUN PB4

#define LED_PIN_LIFTED A5
#define LED_PIN_WIRE A4
#define LED_PIN_BAT A8
#define LED_PIN_CHARGE A9

#define LED_PIN_REAR PB0

#endif*/

#endif /* YFC500_SETUP_H */