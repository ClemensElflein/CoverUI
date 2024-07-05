/**
 * @file ButtonDebouncer.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Button-Debouncer class for OpenMower https://github.com/ClemensElflein/OpenMower
 *        Debouncing is done by continuos simple shifting the port states into an state array for later processing.
 *        See Jack Ganssle debouncing http://www.ganssle.com/debouncing-pt2.htm
 *        For code simplicity/speed, I debounce all pins, regardless if it has a button or not. Button separation has to be done by calling class.
 * @version 0.3
 * @date 2023-10-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/ButtonDebouncer.hpp"

void ButtonDebouncer::process_state(const uint32_t gpio_port_nr) // Has to get called regulary i.e. by timer (5ms) and store the (buttons) port state within states_ array
{
#ifdef MCU_STM32
    auto gpio_port = get_GPIO_Port(gpio_port_nr);
    states_[state_index_] = gpio_port->IDR ^ 0xFFFF; // XOR changes for pull-up states_
#else
    states_[state_index_] = GPIO_ISTAT(gpio_port[gpio_port_nr]) ^ 0xFFFF; // XOR changes for pull-up states_
#endif

    // Debounce
    uint16_t laststate_debounced_ = state_debounced_;
    unsigned int i;
    for (i = 0, state_debounced_ = 0xFFFF; i < NUM_BUTTON_STATES; i++)
        state_debounced_ &= states_[i];

    // Circular buffer index
    state_index_++;
    if (state_index_ >= NUM_BUTTON_STATES)
        state_index_ = 0;

    // Save what changed
    state_changed_ = state_debounced_ ^ laststate_debounced_;
};

/**
 * @brief Return boolean true if the given pin's button is pressed.
 *        Take into notice that the returned state is already debounced.
 *
 * @param pin digital_pin
 * @return true if pressed, false if not pressed
 */
bool ButtonDebouncer::is_pressed(uint8_t pin)
{
    return state_debounced_ & digitalPinToBitMask(pin);
};
