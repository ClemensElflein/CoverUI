/**
 * @file LEDcontrol_RM-ECOW-V100.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Clasic 500 LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_C500_H
#define YARDFORCE_LEDCONTROL_C500_H

#include <Arduino.h>
#include <stdint.h>
#include <map>
#include "../../../BttnCtl.h"
#include "../LEDcontrol.h"

// 1st row: 2,4,6,8HR
#define LED_PIN_2HR PA4
#define LED_PIN_4HR PA5
#define LED_PIN_6HR PA6
#define LED_PIN_8HR PA7
// 2nd row: S1, S2, LOCK
#define LED_PIN_S1 PA0
#define LED_PIN_S2 PA1
#define LED_PIN_LOCK PC4
// 3rd row: Mon-Sun
#define LED_PIN_MON PA15
#define LED_PIN_TUE PC10
#define LED_PIN_WED PC11
#define LED_PIN_THU PC12
#define LED_PIN_FRI PD2
#define LED_PIN_SAT PB3
#define LED_PIN_SUN PB4
// 4th row: Lifted, Wire, Bat, Charge
#define LED_PIN_LIFTED PC0
#define LED_PIN_WIRE PC1
#define LED_PIN_BAT PC2
#define LED_PIN_CHARGE PC3
// Backside
#define LED_PIN_REAR PB0

#define LED_ANIMATE_DELAY 15 // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

const uint32_t kPinByLedNum[] = {
    // Order derived from LowLevel "enum LED_id"
    //                       C500
    // Use LED         num  Original  Alternative
    LED_PIN_CHARGE, //  0
    LED_PIN_BAT,    //  1
    LED_PIN_WIRE,   //  2
    LED_PIN_LIFTED, //  3
    LED_PIN_SUN,    //  4    SUN(6)
    LED_PIN_SAT,    //  5    SAT(5)
    LED_PIN_FRI,    //  6    FRI(4)
    LED_PIN_THU,    //  7    THU(3)
    LED_PIN_WED,    //  8    WED(2)
    LED_PIN_TUE,    //  9    TUE(1)
    LED_PIN_MON,    // 10    MON(0)
    LED_PIN_LOCK,   // 11
    LED_PIN_S2,     // 12              digit 5
    LED_PIN_S1,     // 13              digit 4
    LED_PIN_8HR,    // 14    8HR       digit 3
    LED_PIN_6HR,    // 15    6HR       digit 2
    LED_PIN_4HR,    // 16    4HR       digit 1
    LED_PIN_2HR,    // 17    2HR       digit 0
    LED_PIN_REAR    // 18
};

// Numeric (base10) representation of LEDs.
// I.e. Digit 0 = 4H(0), ... Then S2(5) + S1(4) = 9.
//
//    |------- Bit 6 = MON(0)
//    ||------ Bit 5 = TUE(1)
//    |||----- Bit 4 = WED(2)
//    ||||---- Bit 3 = THU(3)
//    |||||--- Bit 2 = FRI(4)
//    ||||||-- Bit 1 = SAT(5)
//    |||||||- Bit 0 = SUN(6)
const uint8_t kBase10Leds[] = {
    0b1000000, // = 0
    0b0100000,
    0b0010000,
    0b0001000,
    0b0000100,
    0b0000010,
    0b0000001,
    0b0100001,
    0b0010001,
    0b0001001}; // = 9

class LEDcontrolC500 : public LEDcontrol
{
public:
    LEDcontrolC500(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback) {}

    /**
     * @brief Animate sequence handler. Has to be started by sequence_start()
     */
    void sequence_animate_handler()
    {
        unsigned int step = seq_get_next_step_(LED_ANIMATE_DELAY); // Animation sequence runs in 15ms steps

        if (step == 0) // Next sequence step not reached now
            return;
        else if (step >= 1 && step <= kNumLeds) // LED on
        {
            set(kNumLeds - step, LED_state::LED_on, false);
            return;
        }
        else if (step >= (kNumLeds + 1) && step <= (2 * kNumLeds)) // LED off
        {
            set((2 * kNumLeds) - step, LED_state::LED_off, false);
            return;
        }
        else
        {
            seq_start_tick_ = 0;  // Sequence end
            set(led_states_bin_); // Restore states
            return;
        }
    }

    /**
     * @brief A quick boot/power-on animation, also used as jammed button indicator
     *
     * @return unsigned int ms how long it will take to play
     */
    unsigned int boot_animation() // A short boot animation which return the amount of ms it will take
    {
        sequence_start(&LEDcontrol::sequence_animate_handler);
        return ((kNumLeds + 1) * LED_ANIMATE_DELAY);
    }

    /**
     * @brief Set base10 related LEDs for the given (numeric) character
     *
     * @param digit numeric character
     */
    void set_base10_leds(char digit)
    {
        for (uint8_t bit = 0; bit <= 6; bit++) // We've 6 LEDs for base10 number representation
        {
            bool on = (kBase10Leds[digit - '0'] >> bit) & 0b1;
            unsigned int led = bit + 12;
            set(bit + 4, on ? LED_state::LED_on : LED_state::LED_off, false);
        }
    }

    void force_off_num_seq_leds(bool force)
    {
        force_off(LED_NUM_LIFTED, force);      // Num change signalling LED
        for (unsigned int i = 4; i <= 10; i++) // Base10 related LEDs
            force_off(i, force);
    }
};

LEDcontrolC500 leds(kPinByLedNum, sizeof(kPinByLedNum) / sizeof(uint32_t), &LEDcontrol::set_base10_leds); // Main LED controller object

#endif // YARDFORCE_LEDCONTROL_C500_H
