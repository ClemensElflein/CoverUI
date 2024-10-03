/**
 * @file Emergency.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.4
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */

#ifndef YARDFORCE_EMERGENCY_HPP
#define YARDFORCE_EMERGENCY_HPP

#include <Arduino.h>

#include "../../BttnCtl.h"

#define PERIODIC_SEND_CYCLE 1000  // Periodic cycle (ms) how often to send emergency state (if there's no active emergency)

extern void sendMessage(void *message, size_t size);

class Emergency {
   public:
    struct PinStateDef {
        uint8_t pin;
        uint8_t pin_mode;
        Emergency_state state;
    };

    const PinStateDef *kPinStatesPtr;  // Pointer to an array of PinStateDef's (order doesn't matter)
    const size_t kNumEmergencies;

    Emergency(const PinStateDef *t_kPinStatesPtr, const size_t t_kNumEmergencies) : kPinStatesPtr(t_kPinStatesPtr), kNumEmergencies(t_kNumEmergencies) {
        for (size_t i = 0; i < kNumEmergencies; i++) {
            auto pin_state = *(kPinStatesPtr + i);
            pinMode(pin_state.pin, pin_state.pin_mode);
        }
    };

    /**
     * @brief Read all related hall-sensors and update state_
     *
     */
    void read() {
        state_ = 0;  // We might have more emergency sensors and switch than OM Emergency_states. So we need to OR them instead of assign them 1:1
        for (size_t i = 0; i < kNumEmergencies; i++) {
            auto pin_state = *(kPinStatesPtr + i);
            if (digitalRead(pin_state.pin) == HIGH)
                state_ |= pin_state.state;
        }
        if (state_)
            state_ |= Emergency_state::Emergency_latch;
    }

    /**
     * @brief Send 'emergency' message via COBS with latest state_
     *
     */
    void send() {
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
    void read_and_send_if_emergency() {
        read();

        if (state_ & Emergency_state::Emergency_latch && !(state_last_sent_ & Emergency_state::Emergency_latch)) {
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
    void periodic_send() {
        // Active emergency
        if (state_ & Emergency_state::Emergency_latch) {
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
    volatile uint8_t state_ = 0;   // Current emergency state, set by read()
    uint8_t state_last_sent_ = 0;  // Copy of last sent state_
};

#endif  // YARDFORCE_EMERGENCY_HPP
