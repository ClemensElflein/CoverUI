/**
 * @file buttons_RM-ECOW-V110.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.1.0 CoverUI Buttons header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-06-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_BUTTONS_RMECOWV110_H
#define YARDFORCE_BUTTONS_RMECOWV110_H

#include <Arduino.h>
#include "../Buttons.hpp"

#define BTN_S1_PIN PB11
#define BTN_S2_PIN PC6
#define BTN_LOCK_PIN PB12
#define BTN_4H_PIN PB14
#define BTN_6H_PIN PB15
#define BTN_8H_PIN PB13
#define BTN_10H_PIN PB2
#define BTN_PLAY_PIN PC11
#define BTN_HOME_PIN PC12

extern Buttons buttons;

#define HAS_MAGIC_BUTTONS
extern void magic_buttons();

#endif // YARDFORCE_BUTTONS_RMECOWV110_H
