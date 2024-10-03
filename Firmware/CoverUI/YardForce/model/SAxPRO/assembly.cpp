/**
 * @file SAxPRO/assembly.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO CoverUI Assembly for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "assembly.hpp"

#include "../../include/Emergency.hpp"
#include "Display.hpp"

// ----- LEDs -----

const uint32_t kPinByLedNum[] = {
    // Order derived from LowLevel "enum LED_id"
    //                       C500
    // Use LED         num  Original  Alternative
    LED_PIN_NC,        //  0
    LED_PIN_NC,        //  1
    LED_PIN_NC,        //  2
    LED_PIN_NC,        //  3
    LED_PIN_NC,        //  4    SUN(6)
    LED_PIN_NC,        //  5    SAT(5)
    LED_PIN_NC,        //  6    FRI(4)
    LED_PIN_NC,        //  7    THU(3)
    LED_PIN_NC,        //  8    WED(2)
    LED_PIN_NC,        //  9    TUE(1)
    LED_PIN_NC,        // 10    MON(0)
    LED_PIN_NC,        // 11
    LED_PIN_NC,        // 12              digit 5
    LED_PIN_NC,        // 13              digit 4
    LED_PIN_NC,        // 14    8HR       digit 3
    LED_PIN_NC,        // 15    6HR       digit 2
    LED_PIN_NC,        // 16    4HR       digit 1
    LED_PIN_NC,        // 17    2HR       digit 0
    LED_PIN_NC,        // 18
    LED_PIN_BACKLIGHT  // 19
};

// Main LED controller object
LEDcontrol leds(kPinByLedNum, sizeof(kPinByLedNum) / sizeof(uint32_t));  // Main LED controller object

// ----- Buttons -----

// Map logic button number to pin. Use the same order as in OM FW so that they get scanned in a comparable order!
const std::map<uint8_t, Buttons::ButtonDef> kBtnDefByButtonNumMap = {
    {BTN_HOME_NUM, {BTN_HOME_PIN, -1}},
    {BTN_PLAY_NUM, {BTN_PLAY_PIN, -1}},
    {BTN_OK_NUM, {BTN_OK_PIN, -1}},
    {BTN_UP_NUM, {BTN_UP_PIN, -1}},
    {BTN_DOWN_NUM, {BTN_DOWN_PIN, -1}},
    {BTN_BACK_NUM, {BTN_BACK_PIN, -1}}};

Buttons buttons(kBtnDefByButtonNumMap);

// ----- Hatch -----

unsigned int HatchSAXPRO::handle_button(unsigned int button_id, unsigned int press_cnt) {
    // If backlight is off, skip first button press
    if (display.check_backlight() == LED_off) {
        display.set_backlight();
        return 0;  // Skip handling of first button-press if backlight was off
    }
    display.set_backlight();

    // Clear emergency = Enter (OK) button
    if (button_id == BTN_OK_NUM) {
        display.start_anncmnt(4000, yardforce::display::AbstractDisplay::AnncmntType::close_hatch);
        queue_button(BTN_LOCK_NUM, 2, 4500);  // Send long-press Lock button in 4.5 sec
        return 0;                             // Don't return button now (hatch is still open)
    }

    // Play & Home button
    if (button_id == BTN_HOME_NUM || button_id == BTN_PLAY_NUM) {
        display.start_anncmnt(4000, yardforce::display::AbstractDisplay::AnncmntType::close_hatch);
        queue_button(BTN_LOCK_NUM, 2, 4500);  // Send long-press Lock button in 4.5 sec
        queue_button(button_id, 0, 5000);
        return 0;  // Don't return button now (hatch is still open)
    }

    if (button_id == BTN_UP_NUM)
        return BTN_MON_NUM;  // Volume up

    if (button_id == BTN_DOWN_NUM)
        return BTN_TUE_NUM;  // Volume down

    return button_id;
};

HatchSAXPRO hatch;