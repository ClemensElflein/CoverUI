/**
 * @file hatch_RM-EC3-V11.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V1.1 CoverUI Hatch for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "../model_RM-EC3-V11.h"

unsigned int HatchRMEC3V11::handle_button(unsigned int button_id, unsigned int press_cnt)
{
    // If backlight is off, skip first button press
    if (display.check_backlight() == LED_off)
    {
        display.set_backlight();
        return 0; // Skip handling of first button-press if backlight was off
    }
    display.set_backlight();

    // Clear emergency = Enter (OK) button
    if (button_id == BTN_OK_NUM)
    {
        display.start_anncmnt(4000, yardforce::display::Display::AnncmntType::close_hatch);
        queue_button(BTN_LOCK_NUM, 2, 4500); // Send long-press Lock button in 4.5 sec
        return 0;                            // Don't return button now (hatch is still open)
    }

    // Play & Home button
    if (button_id == BTN_HOME_NUM || button_id == BTN_PLAY_NUM)
    {
        display.start_anncmnt(4000, yardforce::display::Display::AnncmntType::close_hatch);
        queue_button(BTN_LOCK_NUM, 2, 4500); // Send long-press Lock button in 4.5 sec
        queue_button(button_id, 0, 5000);
        return 0; // Don't return button now (hatch is still open)
    }

    if (button_id == BTN_1_NUM)
        return BTN_MON_NUM; // Volume up

    if (button_id == BTN_2_NUM)
        return BTN_TUE_NUM; // Volume down

    if (button_id == BTN_3_NUM)
        return BTN_WED_NUM; // Next language

    return button_id;
};

HatchRMEC3V11 hatch;
