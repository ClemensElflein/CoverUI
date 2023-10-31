/**
 * @file Emergency.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef YARDFORCE_EMERGENCY_H
#define YARDFORCE_EMERGENCY_H

#include <Arduino.h>
#include "../BttnCtl.h"

#define PERIODIC_SEND_CYCLE 1000 // Periodic cycle (ms) how often to send emergency state (if there's no active emergency)

struct EmergencyPinStateDef
{
    uint8_t pin;
    uint8_t pin_mode;
    Emergency_state state;
};

extern void sendMessage(void *message, size_t size);

class Emergency
{
public:
    const EmergencyPinStateDef *kEmergencyPinStatesPtr; // Pointer to an array of EmergencyPinStateDef's (order doesn't matter)
    const size_t kNumEmergencies;

    Emergency(const EmergencyPinStateDef *t_kEmergencyPinStatesPtr, const size_t t_kNumEmergencies) : kEmergencyPinStatesPtr(t_kEmergencyPinStatesPtr), kNumEmergencies(t_kNumEmergencies){};

    void setup()
    {
        for (size_t i = 0; i < kNumEmergencies; i++)
        {
            auto pin_state = *(kEmergencyPinStatesPtr + i);
            pinMode(pin_state.pin, pin_state.pin_mode);
        }
    }

    /**
     * @brief Read all related hall-sensors and update state_
     *
     */
    void read()
    {
        for (size_t i = 0; i < kNumEmergencies; i++)
        {
            auto pin_state = *(kEmergencyPinStatesPtr + i);
            if (digitalRead(pin_state.pin) == HIGH)
                state_ |= pin_state.state;
            else
                state_ &= ~pin_state.state;
        }
        if (state_ & ~Emergency_state::Emergency_latch)
            state_ |= Emergency_state::Emergency_latch;
        else
            state_ &= ~Emergency_state::Emergency_latch; // Logically same as 0
    }

    /**
     * @brief Send 'emergency' message via COBS with latest state_
     *
     */
    void send()
    {
        msg_event_emergency msg = {
            .type = Get_Emergency,
            .state = state_};
        sendMessage(&msg, sizeof(msg));
        state_last_sent_ = state_;
    }

    /**
     * @brief read() related hall sensors and send if a new emergency occurred.
     *   Get called constantly by quick (5ms) timer
     *
     */
    void read_and_send_if_emergency()
    {
        read();

        if (state_ & Emergency_state::Emergency_latch && !(state_last_sent_ & Emergency_state::Emergency_latch))
        {
            send();
            next_periodic_cycle = millis() + PERIODIC_SEND_CYCLE;
        }
    }

    /**
     * @brief Periodically send emergency state.
     *   Get called by fast (100ms) timer.
     *   An active emergency state get send on each call.
     *   An inactive emergency state get only every PERIODIC_SEND_CYCLE
     */
    void periodic_send()
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
    }

private:
    volatile uint32_t next_periodic_cycle = PERIODIC_SEND_CYCLE;
    volatile uint8_t state_ = 0;  // Current emergency state, set by read()
    uint8_t state_last_sent_ = 0; // Copy of last sent state_
};

#endif // YARDFORCE_EMERGENCY_H
