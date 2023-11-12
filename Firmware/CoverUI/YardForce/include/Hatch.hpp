/**
 * @file Hatch.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Hatch header for OpenMower https://github.com/ClemensElflein/OpenMower
 * This is for YardForce models (like SA, SC or NX), whose CoverUI is behind a hatch.
 * Those need some special (button) handling because opening the hatch (to reach the buttons), triggers stop-emergency.
 * @version 0.2
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_HATCH_HPP
#define YARDFORCE_HATCH_HPP

#include <Arduino.h>
#include <list>

class Hatch
{
public:
    struct FakeButton
    {
        uint8_t button_id;
        uint8_t press_duration; // 0 for single press, 1 for long, 2 for very long press
        uint32_t delay_end;
    };

    virtual ~Hatch() = default;

    unsigned int handle_button(unsigned int button_id, uint32_t press_time);
    void queue_button(uint8_t button_id, uint8_t press_duration, uint32_t delay);
    void process_queued();

protected:
    std::list<FakeButton> fake_button_queue;
};

#endif // YARDFORCE_HATCH_HPP
