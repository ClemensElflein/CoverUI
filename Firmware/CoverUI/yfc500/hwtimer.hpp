/**
 * @file timer_setup.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI STM32 & GD32 HW-Timer setup for OpenMower https://github.com/ClemensElflein/OpenMower
 *   Required because framework-arduino & framework-arduinogd32 have incompatible definitions and I don't fill-up our
 *   main.hpp with #ifdef's
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

#ifdef MCU_STM32

void hwtimer_setup(TIM_TypeDef *instance, uint32_t freq, callback_function_t callback)
{
    uint32_t cycle, prescale_fact;

    HardwareTimer *Timer = new HardwareTimer(instance);

    // Prepare calculation
    cycle = Timer->getTimerClkFreq() / freq;
    prescale_fact = (cycle / 0x10000) + 1;
    Timer->setPrescaleFactor(prescale_fact - 1);
    //Timer->attachInterrupt(std::bind(&LEDcontrol::blink_timer_elapsed, &LedControl, LED_state::LED_blink_slow));
    Timer->attachInterrupt(callback);
    Timer->resume();
}

#else

#include <HardwareTimer.h>
void todo_wrapper()
{
    LedControl.blink_timer_elapsed(LED_state::LED_blink_slow);
}

void hwtimer_setup(uint32_t instance, uint32_t freq)
{
    uint32_t cycle, prescale_fact;

    HardwareTimer *Timer = new HardwareTimer(instance);

    // Prepare calculation
    cycle = Timer->getTimerClkFre() / freq;
    prescale_fact = (cycle / 0x10000) + 1;
    Timer->setPrescaler(prescale_fact - 1);
    Timer->attachInterrupt(todo_wrapper); // framework-arduinogd32 only support void (*timerCallback_t)()
    Timer->start();
}

#endif

#endif /* YFC500_HWTIMER_H */
