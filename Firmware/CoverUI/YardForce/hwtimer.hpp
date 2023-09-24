/**
 * @file hwtimer.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI STM32 & GD32 HW-Timer for OpenMower https://github.com/ClemensElflein/OpenMower
 *   Simple wrapper around the incompatible framework-arduinoststm32 & framework-arduinogd32 APIs so that we don't
 *   fill-up our main.hpp with #ifdef's
 * @version 0.2
 * @date 2023-09-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_HWTIMER_HPP
#define YARDFORCE_HWTIMER_HPP

#include <Arduino.h>
#include <stdint.h>
#include <HardwareTimer.h> // Required for framework-arduinogd32

#ifdef MCU_STM32
#define TIM_SLOW TIM6                         // Basic timer
#define TIM_FAST TIM17                        // General purpose timer
#define TIM_EVENT TIM14                       // General purpose timer
#define TIM_QUICK TIM16                       // General purpose timer
#define TIM_DEFAULT_PREEMPT_PRIO TIM_IRQ_PRIO
#define TIM_DEFAULT_SUB_PRIO TIM_IRQ_SUBPRIO
#else
#define TIM_SLOW TIMER16
#define TIM_FAST TIMER15
#define TIM_QUICK TIMER14
#endif

#ifdef MCU_STM32 // ---------- STM32 ----------

HardwareTimer *hwtimer(TIM_TypeDef *instance, uint32_t freq, callback_function_t callback, uint32_t preemptPriority = TIM_DEFAULT_PREEMPT_PRIO, uint32_t subPriority = TIM_DEFAULT_SUB_PRIO)
{
    HardwareTimer *Timer = new HardwareTimer(instance);
    Timer->setOverflow(freq, HERTZ_FORMAT);
    Timer->setInterruptPriority(preemptPriority, subPriority);
    Timer->attachInterrupt(callback);
    Timer->resume();
    return Timer;
}

#else // ---------- GD32 ----------

HardwareTimer *hwtimer(uint32_t instance, uint32_t freq, timerCallback_t callback, uint32_t preemptPriority = 0, uint32_t subPriority = 0)
{
    HardwareTimer *Timer = new HardwareTimer(instance);
    Timer->setPeriodTime(freq, FORMAT_HZ);
    // FIXME: GD's HW timer class doesn't has INT preemption- and sub- priorities implemented yet. Will become an issue latest with GD32 & LVGL!
    Timer->attachInterrupt(callback);
    Timer->start();
    return Timer;
}

#endif

#endif // YARDFORCE_HWTIMER_HPP
