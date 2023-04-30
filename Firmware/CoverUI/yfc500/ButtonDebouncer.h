/**
 * @file ButtonDebouncer.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Button-Debouncer class for OpenMower https://github.com/ClemensElflein/OpenMower
 *        Debouncing is done by continuos simply shift the port states into an state array for later processing.
 *        See Jack Ganssle debouncing http://www.ganssle.com/debouncing-pt2.htm
 *        For code simplicity/speed, I debounce all pins, regardless if it has a button or not. Button separation has to be done by calling class.
 * @version 0.1
 * @date 2023-04-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_BUTTONDEBOUNCER_H
#define YFC500_BUTTONDEBOUNCER_H

#include <stdint.h>
#ifdef MCU_STM32
//#include "stm32cube/inc/gpio.h"
#else // MCU_GD32
#endif

#define NUM_BUTTON_STATES 6 // * 5ms timer = 30ms bouncing-button states = debounced after 30ms

class ButtonDebouncer
{
private:
    uint16_t _states[NUM_BUTTON_STATES]; // GPIO port state recorder (every time interval = 5ms)
    uint8_t _state_index = 0;            // Index for next _states store positions
    uint16_t _state_debounced;           // Debounced buttons state
    uint16_t _state_changed;             // Just changed buttons

public:
    ButtonDebouncer();

    //void process_state(const GPIO_TypeDef *gpio_port); // Has to get called regulary i.e. by timer (5ms) and store the (buttons) port state within _states array
    uint16_t get_status();
    uint16_t get_pressed();
};

#endif /* YFC500_BUTTONDEBOUNCER_H */