/**
 * @file ButtonDebouncer.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Button-Debouncer header for OpenMower https://github.com/ClemensElflein/OpenMower
 *        Debouncing is done by continuos simple shifting the port states into an state array for later processing.
 *        See Jack Ganssle debouncing http://www.ganssle.com/debouncing-pt2.htm
 *        For code simplicity/speed, I debounce all pins, regardless if it has a button or not. Button separation has to be done by calling class.
 * @version 0.3
 * @date 2023-10-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONDEBOUNCER_HPP
#define YARDFORCE_BUTTONDEBOUNCER_HPP

#include <Arduino.h>

#define NUM_BUTTON_STATES 8 // * 5ms timer = 40ms bouncing-button states = debounced after 40ms

class ButtonDebouncer
{
public:
    void process_state(const uint32_t gpio_port_nr); // Has to get called regulary i.e. by timer (5ms) and store the (buttons) port state within states_ array
    bool is_pressed(uint8_t pin);

private:
    uint16_t states_[NUM_BUTTON_STATES]; // GPIO port state recorder (every time interval = 5ms)
    uint8_t state_index_ = 0;            // Index for next states_ store positions
    volatile uint16_t state_debounced_;  // Debounced buttons state
    uint16_t state_changed_;             // Just changed buttons
};

#endif // YARDFORCE_BUTTONDEBOUNCER_HPP
