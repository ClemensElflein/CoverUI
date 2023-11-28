/**
 * @file buttons_RM-EC3-V11.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V1.1 CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <map>
#include "../main.h"

// Map logic button number to pin. Use the same order as in OM FW so that they get scanned in a comparable order!
const std::map<uint8_t, Buttons::ButtonDef> kBtnDefByButtonNumMap = {
    {BTN_HOME_NUM, {BTN_HOME_PIN, -1}},
    {BTN_PLAY_NUM, {BTN_PLAY_PIN, -1}},
    {BTN_S1_NUM, {BTN_S1_PIN, LED_NUM_S1}},
    {BTN_S2_NUM, {BTN_S2_PIN, LED_NUM_S2}},
    {BTN_SETUP_NUM, {BTN_SETUP_PIN, 19}},
    {BTN_MENU_NUM, {BTN_MENU_PIN, -1}},
    {BTN_1_NUM, {BTN_1_PIN, -1}},
    {BTN_2_NUM, {BTN_2_PIN, -1}},
    {BTN_3_NUM, {BTN_3_PIN, -1}},
    {BTN_4_NUM, {BTN_4_PIN, -1}},
    {BTN_5_NUM, {BTN_5_PIN, -1}},
    {BTN_6_NUM, {BTN_6_PIN, -1}},
    {BTN_7_NUM, {BTN_7_PIN, -1}},
    {BTN_8_NUM, {BTN_8_PIN, -1}},
    {BTN_9_NUM, {BTN_9_PIN, -1}},
    {BTN_BACK_NUM, {BTN_BACK_PIN, -1}},
    {BTN_0_NUM, {BTN_0_PIN, -1}},
    {BTN_OK_NUM, {BTN_OK_PIN, -1}}};

Buttons buttons(kBtnDefByButtonNumMap);

/**
 * @brief Check if one of the "magic buttons" got pressed and do his function.
 * SETUP + 4H = Display FW version
 * SETUP + 10H = LED animation
 */
void magic_buttons()
{
    if (!buttons.is_pressed(BTN_MENU_NUM))
        return;

    if (buttons.is_pressed(BTN_BACK_NUM))
        display.openmower_anim();
    else if (buttons.is_pressed(BTN_0_NUM))
        display.start_anncmnt(2000, yardforce::display::Display::AnncmntType::version);

    return;
};
