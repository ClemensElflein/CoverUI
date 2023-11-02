/**
 * @file LEDcontrol_SAxPRO.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_SAXPRO_H
#define YARDFORCE_LEDCONTROL_SAXPRO_H

#include <Arduino.h>
#include <stdint.h>
#include <map>
#include "../../../BttnCtl.h"
#include "../LEDcontrol.h"

#define LED_PIN_BACKLIGHT PA11 // SAxPRO only has one LED, the backlight LED
#define LED_NUM_BACKLIGHT 0

const uint32_t kPinByLedNum[] = {
    // Use LED         num  Original  Alternative
    LED_PIN_BACKLIGHT //  0
};

LEDcontrol leds(kPinByLedNum, sizeof(kPinByLedNum) / sizeof(uint32_t)); // Main LED controller object

#endif // YARDFORCE_LEDCONTROL_SAXPRO_H
