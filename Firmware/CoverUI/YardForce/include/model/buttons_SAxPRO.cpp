/**
 * @file buttons_SAxPRO.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "../main.h"

// Map logic button number to pin. Use the same order as in OM FW so that they get scanned in a comparable order!
const std::map<uint8_t, Buttons::ButtonDef> kBtnDefByButtonNumMap = {
    {BTN_HOME_NUM, {BTN_HOME_PIN, -1}},
    {BTN_PLAY_NUM, {BTN_PLAY_PIN, -1}},
    {BTN_OK_NUM, {BTN_OK_PIN, -1}},
    {BTN_UP_NUM, {BTN_UP_PIN, -1}},
    {BTN_DOWN_NUM, {BTN_DOWN_PIN, -1}},
    {BTN_BACK_NUM, {BTN_BACK_PIN, -1}}};

Buttons buttons(kBtnDefByButtonNumMap);
