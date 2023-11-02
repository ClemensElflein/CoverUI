/**
 * @file buttons_RM-ECOW-V100.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.4
 * @date 2023-09-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_RMECOWV100_HPP
#define YARDFORCE_BUTTONS_RMECOWV100_HPP

#include <Arduino.h>
#include <map>
#include "../Buttons.hpp"
#include "../ButtonDebouncer.hpp"

#define BTN_S1_PIN PA4
#define BTN_S2_PIN PC6
#define BTN_LOCK_PIN PA0
#define BTN_4H_PIN PA6
#define BTN_6H_PIN PC4
#define BTN_8H_PIN PB0
#define BTN_10H_PIN PB2
#define BTN_PLAY_PIN PC11
#define BTN_HOME_PIN PC12
#define BTN_SETUP_PIN PF4

// Map logic button number to pin. Use the same order as in OM FW so that they get scanned in a comparable order!
const std::map<uint8_t, uint8_t> kPinByButtonNumMap = {
    {BTN_HOME_NUM, BTN_HOME_PIN},
    {BTN_PLAY_NUM, BTN_PLAY_PIN},
    {BTN_S1_NUM, BTN_S1_PIN},
    {BTN_S2_NUM, BTN_S2_PIN},
    {BTN_LOCK_NUM, BTN_LOCK_PIN},
    {BTN_4H_NUM, BTN_4H_PIN},
    {BTN_6H_NUM, BTN_6H_PIN},
    {BTN_8H_NUM, BTN_8H_PIN},
    {BTN_10H_NUM, BTN_10H_PIN},
    {BTN_SETUP_NUM, BTN_SETUP_PIN}};

Buttons buttons(kPinByButtonNumMap);

#define HAS_MAGIC_BUTTONS
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
}

#endif // YARDFORCE_BUTTONS_RMECOWV100_HPP
