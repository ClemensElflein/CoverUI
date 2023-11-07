/**
 * @file LEDcontrol_SAxPRO.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "LEDcontrol_SAxPRO.hpp"

const uint32_t kPinByLedNum[] = {
    // Use LED         num  Original  Alternative
    LED_PIN_BACKLIGHT //  0
};

LEDcontrol leds(kPinByLedNum, sizeof(kPinByLedNum) / sizeof(uint32_t)); // Main LED controller object
