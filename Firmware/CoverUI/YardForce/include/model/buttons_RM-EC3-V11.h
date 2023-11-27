/**
 * @file buttons_RM-EC3-V11.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V1.1 CoverUI Buttons header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_RMEC3V11_H
#define YARDFORCE_BUTTONS_RMEC3V11_H

#include <Arduino.h>
#include "../Buttons.hpp"

#define BTN_S1_PIN PC14
#define BTN_S2_PIN PC13
#define BTN_PLAY_PIN PC7
#define BTN_HOME_PIN PC2
#define BTN_SETUP_PIN PA0

#define BTN_MENU_PIN PB4

#define BTN_1_PIN PB8
#define BTN_2_PIN PB5
#define BTN_3_PIN PB3
#define BTN_4_PIN PD2
#define BTN_5_PIN PC12
#define BTN_6_PIN PC11
#define BTN_7_PIN PC10
#define BTN_8_PIN PB14
#define BTN_9_PIN PC6
#define BTN_BACK_PIN PA15
#define BTN_0_PIN PB9
#define BTN_OK_PIN PB13

extern Buttons buttons;

#define HAS_MAGIC_BUTTONS
extern void magic_buttons();

#endif // YARDFORCE_BUTTONS_RMEC3V11_H
