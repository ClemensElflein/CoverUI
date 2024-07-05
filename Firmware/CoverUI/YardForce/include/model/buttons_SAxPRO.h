/**
 * @file buttons_SAxPRO.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO CoverUI Buttons for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_SAXPRO_H
#define YARDFORCE_BUTTONS_SAXPRO_H

#include <Arduino.h>
#include "../Buttons.hpp"

#define BTN_PLAY_PIN PC0  // or Start
#define BTN_HOME_PIN PC1
#define BTN_UP_PIN PB14
#define BTN_DOWN_PIN PB13
#define BTN_OK_PIN PB12 // or Enter
#define BTN_BACK_PIN PB15

extern Buttons buttons;

#endif // YARDFORCE_BUTTONS_SAXPRO_H
