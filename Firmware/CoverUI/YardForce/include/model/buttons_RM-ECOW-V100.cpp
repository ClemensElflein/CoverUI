/**
 * @file buttons_RM-ECOW-V100.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.5
 * @date 2023-11-05
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
    {BTN_LOCK_NUM, {BTN_LOCK_PIN, 11}},
    {BTN_4H_NUM, {BTN_4H_PIN, 17}},
    {BTN_6H_NUM, {BTN_6H_PIN, 16}},
    {BTN_8H_NUM, {BTN_8H_PIN, 15}},
    {BTN_10H_NUM, {BTN_10H_PIN, 14}},
    {BTN_SETUP_NUM, {BTN_SETUP_PIN, 19}}};

Buttons buttons(kBtnDefByButtonNumMap);

/**
 * @brief Check if one of the "magic buttons" got pressed and do his function.
 * SETUP + 4H = Display FW version
 * SETUP + 10H = LED animation
 */
void magic_buttons()
{
    if (!buttons.is_pressed(BTN_SETUP_NUM))
        return;

    if (buttons.is_pressed(BTN_10H_NUM))
        leds.sequence_start(&LEDcontrol::sequence_animate_handler);
    else if (buttons.is_pressed(BTN_4H_NUM))
        leds.show_num(FIRMWARE_VERSION);
    return;
};
