/**
 * @file buttons_RM-ECOW-V100.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Buttons header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.5
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_RMECOWV100_H
#define YARDFORCE_BUTTONS_RMECOWV100_H

#include <Arduino.h>
#include "../Buttons.hpp"

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

extern Buttons buttons;

#define HAS_MAGIC_BUTTONS
extern void magic_buttons();

#endif // YARDFORCE_BUTTONS_RMECOWV100_H
