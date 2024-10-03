/**
 * @file Hatch.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Abstract Hatch class for OpenMower https://github.com/ClemensElflein/OpenMower
 * This is for YardForce models (like SA, SC or NX), whose CoverUI is behind a hatch.
 * Those need some special (button) handling because opening the hatch (to reach the buttons), triggers stop-emergency.
 * @version 0.3
 * @date 2024-09-30
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */
#ifndef YARDFORCE_ABC_HATCH_HPP
#define YARDFORCE_ABC_HATCH_HPP

#include <Arduino.h>
#include <etl/queue.h>

const unsigned int FAKE_BUTTON_QUEUE_SIZE = 10;

class AbstractHatch {
   public:
    struct FakeButton {
        uint8_t button_id;
        uint8_t press_duration;  // 0 for single press, 1 for long, 2 for very long press
        uint32_t delay_end;
    };

    /**
     * @brief Handle any kind of pressed button result like:
     * 1. Convert a button id to some other value
     * 2. Trigger count-down for hatch-close
     * 3. Queue fake (delayed) buttons
     * 4. Handle fake_button_queue and probably directly send Get_Button packet
     *
     * @param button_id
     * @param press_time
     * @return unsigned int of the probably converted or ignored button ID
     */
    virtual unsigned int handle_button(unsigned int button_id, unsigned int press_cnt) = 0;

    void queue_button(uint8_t button_id, uint8_t press_duration, uint32_t delay) {
        fake_button_queue.push({button_id, press_duration, millis() + delay});
    };

    /**
     * @brief Loop over fake_button_queue and process(send) ready ones
     */
    void process_queued() {
        if (fake_button_queue.empty() || fake_button_queue.full())
            return;

        auto first = fake_button_queue.front();
        if (millis() >= first.delay_end) {
            buttons.send(first.button_id, first.press_duration);
            fake_button_queue.pop();
        }
    };

   protected:
    etl::queue<FakeButton, FAKE_BUTTON_QUEUE_SIZE, etl::memory_model::MEMORY_MODEL_SMALL> fake_button_queue;
};

#endif  // YARDFORCE_ABC_HATCH_HPP
