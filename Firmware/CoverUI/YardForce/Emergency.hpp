/**
 * @file Emergency.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Emergency class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-05-10
 *
 * @copyright Copyright (c) 2023
 *
 */

/*
 * Cabling:
 *
 * Plug              | Color  |NMOS| Modification
 * ------------------|--------|----|-------------------------------------------------------------------------
 * Stop (Hall)       | White  | Q2 | Bridge R75 (or move R52) to get signal to PC5 (Pin 25)
 * Wheel list (Hall) | Red    | Q4 | Bridge-Cable from Q4/R61 to U5-Pin3 to get signal to PB7 (Pin 59)
 * Stop (Hall)       | Yellow | Q5 | Bridge-Cable from R76/J6-Pin14 to U5-Pin2 to get signal to PB6 (Pin 58)
 * Wheel list (Hall) | Blue   | Q6 | Bridge-Cable from Q6/R83 to U5-Pin10 to get signal to PB8 (Pin 61)
 */

#ifndef YFC500_EMERGENCY_H
#define YFC500_EMERGENCY_H

#include <Arduino.h>
#include <map>
#include "../BttnCtl.h"

#define PIN_HALL_STOP_WHITE PC5
#define PIN_HALL_STOP_YELLOW PB6
#define PIN_HALL_WHEEL_RED PB7
#define PIN_HALL_WHEEL_BLUE PB8

extern void sendMessage(void *message, size_t size);

class Emergency
{
private:
    struct PinDef
    {
        uint8_t pin;
        Emergency_state state;
    };

    const PinDef kPinDefs[4] = {
        {PIN_HALL_STOP_WHITE, Emergency_state::Emergency_stop1},
        {PIN_HALL_STOP_YELLOW, Emergency_state::Emergency_stop2},
        {PIN_HALL_WHEEL_RED, Emergency_state::Emergency_lift1},
        {PIN_HALL_WHEEL_BLUE, Emergency_state::Emergency_lift2}};

    uint8_t state_ = 0;           // Current emergency state, set by read()
    uint8_t state_last_sent_ = 0; // Copy of last sent state_

public:
    void setup()
    {
        for (auto i : kPinDefs)
        {
            pinMode(i.pin, INPUT);
        }
    }

    /**
     * @brief Read all related hall-sensors and update state_
     *
     */
    void read()
    {
        for (auto i : kPinDefs)
        {
            if (digitalRead(i.pin) == HIGH)
                state_ |= i.state;
            else
                state_ &= ~i.state;
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
            send();
    }

    /**
     * @brief Periodically send emergency state.
     *   Get called by fast (100ms) timer.
     *   An active emergency state get send on each call.
     *   An inactive emergency state get only send once a second
     */
    void periodic_send()
    {
        static uint call_cnt_ = 0;

        call_cnt_++;

        // Active emergency
        if (state_ & Emergency_state::Emergency_latch)
        {
            send();
            return;
        }

        // Inactive emergency
        if (call_cnt_ > 10)
        {
            send();
            call_cnt_ = 0;
        }
    }
};

#endif /* YFC500_EMERGENCY_H */
