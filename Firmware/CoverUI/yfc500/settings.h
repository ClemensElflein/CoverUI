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
#ifndef YFC500_SETTINGS_H
#define YFC500_SETTINGS_H

#include <Arduino.h>

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

#ifdef MCU_STM32
#define TIM_SLOW TIM16
#define TIM_FAST TIM15
#define TIM_QUICK TIM14
#else
#define TIM_SLOW TIMER16
#define TIM_FAST TIMER15
#define TIM_QUICK TIMER14
#endif

#endif /* YFC500_SETTINGS_H */