/**
 * @file subscription.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI subscriptions for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-09
 *
 * @copyright Copyright (c) 2023
 */
#include "include/subscription.h"

extern void sendMessage(void *message, size_t size);

namespace subscription
{
    ll_status recv_ll_status;
    ll_high_level_state recv_hl_state;

    namespace // anonymous (private) namespace
    {
        uint32_t next_subscribe_ms_ = 0;
    }

    void subscribe(uint8_t t_topic_bitmask, uint16_t t_interval)
    {
        if (millis() < next_subscribe_ms_)
            return;

        msg_event_subscribe msg = {
            .type = Get_Subscribe,
            .topic_bitmask = t_topic_bitmask,
            .interval = t_interval};
        sendMessage(&msg, sizeof(msg));
        ack();
    }

    void ack()
    {
        next_subscribe_ms_ = millis() + SUBS_CYCLE;
    }
} // namespace subscription
