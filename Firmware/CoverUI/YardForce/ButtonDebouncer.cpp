/**
 * @file ButtonDebouncer.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Button-Debouncer class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-04-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdio.h>
#include "ButtonDebouncer.h"

void ButtonDebouncer::process_state(const uint32_t gpio_port)
{
#ifdef MCU_STM32
    states_[state_index_] = ((GPIO_TypeDef *)gpio_port)->IDR ^ 0xFFFF; // XOR changes for pull-up states_
#else
    states_[state_index_] = GPIO_ISTAT(gpio_port) ^ 0xFFFF; // XOR changes for pull-up states_
#endif

    // Debounce
    uint16_t laststate_debounced_ = state_debounced_;
    for (uint i = 0, state_debounced_ = 0xFFFF; i < NUM_BUTTON_STATES; i++)
        state_debounced_ &= states_[i];

    // Circular buffer index
    state_index_++;
    if (state_index_ >= NUM_BUTTON_STATES)
        state_index_ = 0;

    // Save what changed
    state_changed_ = state_debounced_ ^ laststate_debounced_;
}

uint16_t ButtonDebouncer::get_pressed()
{
    return (state_changed_ & state_debounced_);
}

uint16_t ButtonDebouncer::get_status()
{
    return state_debounced_;
}