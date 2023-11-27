/**
 * @file LEDcontrol_RM-EC3-V11.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "LEDcontrol_RM-EC3-V11.hpp"

const uint32_t kPinByLedNum[] = {
    // Order derived from LowLevel "enum LED_id"
    //                       C500
    // Use LED         num  Original  Alternative
    LED_PIN_NC,       //  0
    LED_PIN_NC,       //  1
    LED_PIN_NC,       //  2
    LED_PIN_NC,       //  3
    LED_PIN_NC,       //  4    SUN
    LED_PIN_NC,       //  5    SAT
    LED_PIN_NC,       //  6    FRI
    LED_PIN_NC,       //  7    THU
    LED_PIN_NC,       //  8    WED
    LED_PIN_NC,       //  9    TUE
    LED_PIN_NC,       // 10    MON
    LED_PIN_NC,       // 11
    LED_PIN_S2,       // 12              digit 5
    LED_PIN_S1,       // 13              digit 4
    LED_PIN_NC,       // 14    8HR       digit 3
    LED_PIN_NC,       // 15    6HR       digit 2
    LED_PIN_NC,       // 16    4HR       digit 1
    LED_PIN_NC,       // 17    2HR       digit 0
    LED_PIN_NC,       // 18
    LED_PIN_SETUP,    // 19
    LED_PIN_BACKLIGHT // 20
};

const unsigned int kLedAnimOrder[] = {13, 19, 12};
const unsigned int kNumFrontLeds = sizeof(kLedAnimOrder) / sizeof(unsigned int);

// Numeric (base10) representation of LEDs.
// I.e. Digit 0 = 4H(0), ... Then S2(5) + S1(4) = 9.
//
//    |------ Bit 5 =  4H(0)
//    ||----- Bit 4 =  6H(1)
//    |||---- Bit 3 =  8H(2)
//    ||||--- Bit 2 = 10H(3)
//    |||||-- Bit 1 =  S1(4)
//    ||||||- Bit 0 =  S2(5)
/*const uint8_t kBase10Leds[] = {
    0b100000, // = 0
    0b010000,
    0b001000,
    0b000100,
    0b000010,
    0b000001,
    0b010001,
    0b001001,
    0b000101,
    0b000011}; // = 9*/

void LEDcontrolRMEC3V11::sequence_animate_handler()
{
    uint16_t step = seq_get_next_step_(LED_ANIMATE_DELAY); // Animation sequence runs in 15ms steps

    if (step == 0) // Next sequence step not reached now
        return;
    else if (step >= 1 && step <= kNumFrontLeds) // LED on
    {
        set(kLedAnimOrder[step - 1], LED_state::LED_on, false);
        return;
    }
    else if (step >= (kNumFrontLeds + 1) && step <= (2 * kNumFrontLeds)) // LED off
    {
        set(kLedAnimOrder[step - kNumFrontLeds - 1], LED_state::LED_off, false);
        return;
    }
    else
    {
        seq_start_tick_ = 0;  // Sequence end
        set(led_states_bin_); // Restore states
        return;
    }
}

void LEDcontrolRMEC3V11::show_countdown_state(unsigned int sec, LED_state state)
{
    unsigned int sec_left = 5 - sec;

    for (size_t i = 1; i <= 4; i++)
    {
        uint8_t led_num = 18 - i;
        if (sec_left >= i) // Remaining secs is greater than this LED
        {
            leds.set(led_num, state, false);
        }
        else
        {
            leds.set(led_num, LED_off, false);
            force_off(led_num, true);
        }
    }
};

/**
 * @brief Countdown LED animation handler
 */
void LEDcontrolRMEC3V11::sequence_countdown_handler()
{
    uint16_t step = seq_get_next_step_(100); // Animation sequence run in 1000ms steps

    if (step == 0) // Next sequence step not reached now
        return;
    else if (step == 1 || step == 11 || step == 21 || step == 31 || step == 41)
    {
        show_countdown_state(((step - 1) / 10) + 1, LED_blink_fast);
        return;
    }
    else if (step < 41)
    {
        return;
    }
    else
    {
        seq_start_tick_ = 0;  // Sequence end
        set(led_states_bin_); // Restore states
        for (size_t i = 1; i <= 4; i++)
            force_off(18 - i, false);
        return;
    }
}

/**
 * @brief A quick boot/power-on animation, also used as jammed button indicator
 *
 * @return unsigned int ms how long it will take to play
 */
unsigned int LEDcontrolRMEC3V11::boot_animation() // A short boot animation which return the amount of ms it will take
{
    sequence_start(&LEDcontrol::sequence_animate_handler);
    return ((kNumFrontLeds + 1) * LED_ANIMATE_DELAY);
}

/**
 * @brief Set base10 related LEDs for the given (numeric) character
 *
 * @param digit numeric character
 */
/*void LEDcontrolRMEC3V11::set_base10_leds(char digit)
{
    for (uint8_t bit = 0; bit <= 5; bit++) // We've 5 LEDs for base10 number representation
    {
        bool on = (kBase10Leds[digit - '0'] >> bit) & 0b1;
        unsigned int led = bit + 12;
        set(led, on ? LED_state::LED_on : LED_state::LED_off, false);
    }
}

void LEDcontrolRMEC3V11::force_off_num_seq_leds(bool force)
{
    force_off(LED_NUM_LIFTED, force);       // Num change signalling LED
    for (unsigned int i = 12; i <= 17; i++) // Base10 related LEDs
        force_off(i, force);
}*/

LEDcontrolRMEC3V11 leds(kPinByLedNum, sizeof(kPinByLedNum) / sizeof(uint32_t), &LEDcontrol::set_base10_leds); // Main LED controller object
