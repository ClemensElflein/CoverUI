/**
 * @file buttons_SAxPRO.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_SAXPRO_HPP
#define YARDFORCE_BUTTONS_SAXPRO_HPP

#include <Arduino.h>
#include <map>
#include "../Buttons.hpp"
#include "../ButtonDebouncer.hpp"

#define BTN_PLAY_PIN PC0  // or Start
#define BTN_HOME_PIN PC1
#define BTN_UP_PIN PB14
#define BTN_DOWN_PIN PB13
#define BTN_OK_PIN PB12 // or Enter
#define BTN_BACK_PIN PB15

// Map logic button number to pin. Use the same order as in OM FW so that they get scanned in a comparable order!
const std::map<uint8_t, uint8_t> kPinByButtonNumMap = {
    {BTN_HOME_NUM, BTN_HOME_PIN},
    {BTN_PLAY_NUM, BTN_PLAY_PIN},
    {BTN_OK_NUM, BTN_OK_PIN},
    {BTN_UP_NUM, BTN_UP_PIN},
    {BTN_DOWN_NUM, BTN_DOWN_PIN},
    {BTN_BACK_NUM, BTN_BACK_PIN}};

Buttons buttons(kPinByButtonNumMap);

#endif // YARDFORCE_BUTTONS_SAXPRO_HPP
