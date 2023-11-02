/**
 * @file buttons_C500.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.4
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_C500_HPP
#define YARDFORCE_BUTTONS_C500_HPP

#include <Arduino.h>
#include <map>
#include "../Buttons.hpp"
#include "../ButtonDebouncer.hpp"

#define BTN_CLK_PIN PF4
#define BTN_OK_PIN PF5
#define BTN_S1_PIN PB2
#define BTN_S2_PIN PB10
#define BTN_LOCK_PIN PB11
#define BTN_MON_PIN PB12
#define BTN_TUE_PIN PB13
#define BTN_WED_PIN PB14
#define BTN_THU_PIN PB15
#define BTN_FRI_PIN PC6
#define BTN_SAT_PIN PC7
#define BTN_SUN_PIN PC8
#define BTN_PLAY_PIN PA11
#define BTN_HOME_PIN PA12

// Map logic button number to pin. Use the same order as in OM FW so that they get scanned in a comparable order!
const std::map<uint8_t, uint8_t> kPinByButtonNumMap = {
    {BTN_CLK_NUM, BTN_CLK_PIN},
    {BTN_HOME_NUM, BTN_HOME_PIN},
    {BTN_PLAY_NUM, BTN_PLAY_PIN},
    {BTN_S1_NUM, BTN_S1_PIN},
    {BTN_S2_NUM, BTN_S2_PIN},
    {BTN_LOCK_NUM, BTN_LOCK_PIN},
    {BTN_OK_NUM, BTN_OK_PIN},
    {BTN_SUN_NUM, BTN_SUN_PIN},
    {BTN_MON_NUM, BTN_MON_PIN},
    {BTN_TUE_NUM, BTN_TUE_PIN},
    {BTN_WED_NUM, BTN_WED_PIN},
    {BTN_THU_NUM, BTN_THU_PIN},
    {BTN_FRI_NUM, BTN_FRI_PIN},
    {BTN_SAT_NUM, BTN_SAT_PIN}};

Buttons buttons(kPinByButtonNumMap);

#define HAS_MAGIC_BUTTONS
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
}

#endif // YARDFORCE_BUTTONS_C500_HPP
