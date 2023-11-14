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
    // Order derived from LowLevel "enum LED_id"
    //                       C500
    // Use LED         num  Original  Alternative
    LED_PIN_NC,       //  0
    LED_PIN_NC,       //  1
    LED_PIN_NC,       //  2
    LED_PIN_NC,       //  3
    LED_PIN_NC,       //  4    SUN(6)
    LED_PIN_NC,       //  5    SAT(5)
    LED_PIN_NC,       //  6    FRI(4)
    LED_PIN_NC,       //  7    THU(3)
    LED_PIN_NC,       //  8    WED(2)
    LED_PIN_NC,       //  9    TUE(1)
    LED_PIN_NC,       // 10    MON(0)
    LED_PIN_NC,       // 11
    LED_PIN_NC,       // 12              digit 5
    LED_PIN_NC,       // 13              digit 4
    LED_PIN_NC,       // 14    8HR       digit 3
    LED_PIN_NC,       // 15    6HR       digit 2
    LED_PIN_NC,       // 16    4HR       digit 1
    LED_PIN_NC,       // 17    2HR       digit 0
    LED_PIN_NC,       // 18
    LED_PIN_BACKLIGHT // 19
};

LEDcontrol leds(kPinByLedNum, sizeof(kPinByLedNum) / sizeof(uint32_t)); // Main LED controller object
