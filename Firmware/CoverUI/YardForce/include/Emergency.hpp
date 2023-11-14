/**
 * @file Emergency.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Emergency header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef YARDFORCE_EMERGENCY_HPP
#define YARDFORCE_EMERGENCY_HPP

#include <Arduino.h>
#include "../../BttnCtl.h"

#define PERIODIC_SEND_CYCLE 1000 // Periodic cycle (ms) how often to send emergency state (if there's no active emergency)

struct EmergencyPinStateDef
{
    uint8_t pin;
    uint8_t pin_mode;
    Emergency_state state;
};


class Emergency
{
public:
    const EmergencyPinStateDef *kEmergencyPinStatesPtr; // Pointer to an array of EmergencyPinStateDef's (order doesn't matter)
    const size_t kNumEmergencies;

    Emergency(const EmergencyPinStateDef *t_kEmergencyPinStatesPtr, const size_t t_kNumEmergencies) : kEmergencyPinStatesPtr(t_kEmergencyPinStatesPtr), kNumEmergencies(t_kNumEmergencies){};

    void setup();
    void read();
    void send();
    void read_and_send_if_emergency();
    void periodic_send();

private:
    volatile uint32_t next_periodic_cycle = PERIODIC_SEND_CYCLE;
    volatile uint8_t state_ = 0;  // Current emergency state, set by read()
    uint8_t state_last_sent_ = 0; // Copy of last sent state_
};

#endif // YARDFORCE_EMERGENCY_HPP
