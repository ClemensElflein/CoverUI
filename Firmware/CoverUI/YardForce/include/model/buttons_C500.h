/**
 * @file buttons_C500.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Buttons header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.5
 * @date 2023-11-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_C500_H
#define YARDFORCE_BUTTONS_C500_H

#include <Arduino.h>
#include "../Buttons.hpp"

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

extern Buttons buttons;

#define HAS_MAGIC_BUTTONS
void magic_buttons();

#endif // YARDFORCE_BUTTONS_C500_H
