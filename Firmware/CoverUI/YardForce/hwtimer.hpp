/**
 * @file hwtimer.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI STM32 & GD32 HW-Timer for OpenMower https://github.com/ClemensElflein/OpenMower
 *   Simple wrapper around the incompatible framework-arduinoststm32 & framework-arduinogd32 APIs so that we don't
 *   fill-up our main.hpp with #ifdef's
 * @version 0.1
 * @date 2023-05-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_HWTIMER_H
#define YFC500_HWTIMER_H

#include <Arduino.h>
#include <stdint.h>
#include <HardwareTimer.h> // Required for framework-arduinogd32

#ifdef MCU_STM32
#define TIM_SLOW TIM6   // Basic timer
#define TIM_FAST TIM17  // General purpose timer
#define TIM_QUICK TIM16 // General purpose timer
#else
#define TIM_SLOW TIMER16
#define TIM_FAST TIMER15
#define TIM_QUICK TIMER14
#endif

#ifdef MCU_STM32 // ---------- STM32 ----------

HardwareTimer *hwtimer(TIM_TypeDef *instance, uint32_t freq, callback_function_t callback)
{
    HardwareTimer *Timer = new HardwareTimer(instance);
    Timer->setOverflow(freq, HERTZ_FORMAT);
    Timer->attachInterrupt(callback);
    Timer->resume();
    return Timer;
}

#else // ---------- GD32 ----------

HardwareTimer *hwtimer(uint32_t instance, uint32_t freq, timerCallback_t callback)
{
    HardwareTimer *Timer = new HardwareTimer(instance);
    Timer->setPeriodTime(freq, FORMAT_HZ);
    Timer->attachInterrupt(callback);
    Timer->start();
    return Timer;
}

#endif

#endif /* YFC500_HWTIMER_H */
