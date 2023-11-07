/**
 * @file buttons_C500.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.5
 * @date 2023-11-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <map>
#include "buttons_C500.h"
#include "../main.h"

// Map logic button number to pin. Use the same order as in OM FW so that they get scanned in a comparable order!
const std::map<uint8_t, Buttons::ButtonDef> kBtnDefByButtonNumMap = {
    {BTN_CLK_NUM, {BTN_CLK_PIN, -1}},
    {BTN_HOME_NUM, {BTN_HOME_PIN, -1}},
    {BTN_PLAY_NUM, {BTN_PLAY_PIN, -1}},
    {BTN_S1_NUM, {BTN_S1_PIN, 13}},
    {BTN_S2_NUM, {BTN_S2_PIN, 12}},
    {BTN_LOCK_NUM, {BTN_LOCK_PIN, 11}},
    {BTN_OK_NUM, {BTN_OK_PIN, -1}},
    {BTN_SUN_NUM, {BTN_SUN_PIN, 4}},
    {BTN_MON_NUM, {BTN_MON_PIN, 10}},
    {BTN_TUE_NUM, {BTN_TUE_PIN, 9}},
    {BTN_WED_NUM, {BTN_WED_PIN, 8}},
    {BTN_THU_NUM, {BTN_THU_PIN, 7}},
    {BTN_FRI_NUM, {BTN_FRI_PIN, 6}},
    {BTN_SAT_NUM, {BTN_SAT_PIN, 5}}};

Buttons buttons(kBtnDefByButtonNumMap);

/**
 * @brief Check if one of the "magic buttons" got pressed and do his function.
 * SETUP + 4H = Display FW version
 * SETUP + 10H = LED animation
 */
void magic_buttons()
{
    if (!buttons.is_pressed(BTN_OK_NUM))
        return;

    if (buttons.is_pressed(BTN_SUN_NUM))
        leds.sequence_start(&LEDcontrol::sequence_animate_handler);
    else if (buttons.is_pressed(BTN_CLK_NUM))
        leds.show_num(FIRMWARE_VERSION);
    return;
};
