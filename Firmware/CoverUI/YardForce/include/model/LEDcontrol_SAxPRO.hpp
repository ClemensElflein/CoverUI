/**
 * @file LEDcontrol_SAxPRO.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_SAXPRO_HPP
#define YARDFORCE_LEDCONTROL_SAXPRO_HPP

#include <Arduino.h>
#include "../LEDcontrol.hpp"

#define LED_PIN_BACKLIGHT PA11 // SAxPRO only has one LED, the backlight LED
#define LED_NUM_BACKLIGHT 19

extern LEDcontrol leds; // Main LED controller object

#endif // YARDFORCE_LEDCONTROL_SAXPRO_HPP
