/**
 * @file hatch_SAxPRO.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO CoverUI Hatch for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "../model_SAxPRO.h"

unsigned int HatchSAXPRO::handle_button(unsigned int button_id, unsigned int press_cnt)
{
    // Clear emergency = Enter (OK) button
    if (button_id == BTN_OK_NUM)
    {
        display::start_countdown(4000);
        queue_button(BTN_LOCK_NUM, 2, 4500); // Send long-press Lock button in 4.5 sec
        return 0;                            // Don't return button now (hatch is still open)
    }

    // Play & Home button
    if (button_id == BTN_HOME_NUM || button_id == BTN_PLAY_NUM)
    {
        display::start_countdown(4000);
        queue_button(BTN_LOCK_NUM, 2, 4500); // Send long-press Lock button in 4.5 sec
        queue_button(button_id, 0, 5000);
        return 0; // Don't return button now (hatch is still open)
    }

    if (button_id == BTN_UP_NUM)
        return BTN_MON_NUM; // Volume up

    if (button_id == BTN_DOWN_NUM)
        return BTN_TUE_NUM; // Volume down

    if (button_id == BTN_BACK_NUM)
        return BTN_WED_NUM; // Next language

    return button_id;
};

HatchSAXPRO hatch;
