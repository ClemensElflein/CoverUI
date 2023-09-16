/**
 * @file ButtonDebouncer.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Button-Debouncer class for OpenMower https://github.com/ClemensElflein/OpenMower
 *        Debouncing is done by continuos simply shift the port states into an state array for later processing.
 *        See Jack Ganssle debouncing http://www.ganssle.com/debouncing-pt2.htm
 *        For code simplicity/speed, I debounce all pins, regardless if it has a button or not. Button separation has to be done by calling class.
 * @version 0.2
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_BUTTONDEBOUNCER_HPP
#define YFC500_BUTTONDEBOUNCER_HPP

#include <Arduino.h>
#include <stdint.h>

#define NUM_BUTTON_STATES 8 // * 5ms timer = 40ms bouncing-button states = debounced after 40ms

class ButtonDebouncer
{
private:
    uint16_t states_[NUM_BUTTON_STATES]; // GPIO port state recorder (every time interval = 5ms)
    uint8_t state_index_ = 0;            // Index for next states_ store positions
    uint16_t state_debounced_;           // Debounced buttons state
    uint16_t state_changed_;             // Just changed buttons

public:
    void process_state(const uint32_t gpio_port) // Has to get called regulary i.e. by timer (5ms) and store the (buttons) port state within states_ array
    {
#ifdef MCU_STM32
        states_[state_index_] = ((GPIO_TypeDef *)gpio_port)->IDR ^ 0xFFFF; // XOR changes for pull-up states_
#else
        states_[state_index_] = GPIO_ISTAT(gpio_port) ^ 0xFFFF; // XOR changes for pull-up states_
#endif

        // Debounce
        uint16_t laststate_debounced_ = state_debounced_;
        uint i;
        for (i = 0, state_debounced_ = 0xFFFF; i < NUM_BUTTON_STATES; i++)
            state_debounced_ &= states_[i];

        // Circular buffer index
        state_index_++;
        if (state_index_ >= NUM_BUTTON_STATES)
            state_index_ = 0;

        // Save what changed
        state_changed_ = state_debounced_ ^ laststate_debounced_;
    }
    uint16_t get_status() { return state_debounced_; };
    uint16_t get_pressed() { return (state_changed_ & state_debounced_); };
};

#endif /* YFC500_BUTTONDEBOUNCER_HPP */
