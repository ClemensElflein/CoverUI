/**
 * @file subscription.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI subscriptions for SAxPRO OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-09
 *
 * @copyright Copyright (c) 2023
 */
#ifndef YARDFORCE_SUBSCRIPTION_H
#define YARDFORCE_SUBSCRIPTION_H

#define SUBS_CYCLE 2000 // Cycle between subscription requests. Has to be > than msg_event_subscribe.interval

#include <Arduino.h>
#include "../../BttnCtl.h"
#include "datatypes.h"

namespace subscription
{
    extern ll_status recv_ll_status;
    extern ll_high_level_state recv_hl_state;

    void subscribe(uint8_t t_topic_bitmask, uint16_t t_interval);
    void ack();
}

#endif // YARDFORCE_SUBSCRIPTION_H