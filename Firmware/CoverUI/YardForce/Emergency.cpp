/**
 * @file Emergency.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "include/Emergency.hpp"
#include "../BttnCtl.h"

extern void sendMessage(void *message, size_t size);

void Emergency::setup()
{
    for (size_t i = 0; i < kNumEmergencies; i++)
    {
        auto pin_state = *(kEmergencyPinStatesPtr + i);
        pinMode(pin_state.pin, pin_state.pin_mode);
    }
};

/**
 * @brief Read all related hall-sensors and update state_
 *
 */
void Emergency::read()
{
    state_ = 0; // We might have more emergency sensors and switch than OM Emergency_states. So we need to OR them instead of assign them 1:1
    for (size_t i = 0; i < kNumEmergencies; i++)
    {
        auto pin_state = *(kEmergencyPinStatesPtr + i);
        if (digitalRead(pin_state.pin) == HIGH)
            state_ |= pin_state.state;
    }
    if (state_)
        state_ |= Emergency_state::Emergency_latch;
};

/**
 * @brief Send 'emergency' message via COBS with latest state_
 *
 */
void Emergency::send()
{
    msg_event_emergency msg = {
        .type = Get_Emergency,
        .state = state_};
    sendMessage(&msg, sizeof(msg));
    state_last_sent_ = state_;
};

/**
 * @brief read() related hall sensors and send if a new emergency occurred.
 *   Get called constantly by quick (5ms) timer
 *
 */
void Emergency::read_and_send_if_emergency()
{
    read();

    if (state_ & Emergency_state::Emergency_latch && !(state_last_sent_ & Emergency_state::Emergency_latch))
    {
        send();
        next_periodic_cycle = millis() + PERIODIC_SEND_CYCLE;
    }
};

/**
 * @brief Periodically send emergency state.
 *   Get called by fast (100ms) timer.
 *   An active emergency state get send on each call.
 *   An inactive emergency state get only every PERIODIC_SEND_CYCLE
 */
void Emergency::periodic_send()
{
    // Active emergency
    if (state_ & Emergency_state::Emergency_latch)
    {
        send();
        next_periodic_cycle = millis() + PERIODIC_SEND_CYCLE;
        return;
    }

    // Inactive emergency
    if (millis() < next_periodic_cycle)
        return;

    send();
    next_periodic_cycle = millis() + PERIODIC_SEND_CYCLE;
};
