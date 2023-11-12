/**
 * @file Hatch.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Hatch for OpenMower https://github.com/ClemensElflein/OpenMower
 * This is for YardForce models (like SA, SC or NX), whose CoverUI is behind a hatch.
 * Those need some special (button) handling because opening the hatch (to reach the buttons), triggers stop-emergency.
 * @version 0.2
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
// #include <stdint.h>
#include <list>
// #include "include/Hatch.hpp"
// #include "include/LEDcontrol.hpp"
#include "include/main.h"

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
unsigned int Hatch::handle_button(unsigned int button_id, uint32_t press_time)
{
    leds.set(LED_NUM_LIFTED, LED_blink_slow);

    return button_id + 0;
};

void Hatch::queue_button(uint8_t button_id, uint8_t press_duration, uint32_t delay)
{
    fake_button_queue.push_back({button_id, press_duration, millis() + delay});
};

/**
 * @brief Loop over fake_button_queue and process(send) ready ones
 */
void Hatch::process_queued()
{
    if (fake_button_queue.empty())
        return;

    for (auto it = fake_button_queue.begin(); it != fake_button_queue.end(); ++it)
    {
        if (millis() >= it->delay_end)
        {
            buttons.send(it->button_id, it->press_duration);
            it = fake_button_queue.erase(it);
        }
    }
};
