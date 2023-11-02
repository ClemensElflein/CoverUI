/**
 * @file LEDcontrol_RM-ECOW-V100.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_RMECOWV100_H
#define YARDFORCE_LEDCONTROL_RMECOWV100_H

#include <Arduino.h>
#include <stdint.h>
#include <map>
#include "../../../BttnCtl.h"
#include "../LEDcontrol.h"

// 1st row: Lifted, Wire (WLAN), Battery empty, Charge
#define LED_PIN_LIFTED PB11
#define LED_PIN_WIRE PB12
#define LED_PIN_BAT PB14
#define LED_PIN_CHARGE PB15

// 2nd row: 4, 6, 8, 10HR
#define LED_PIN_4H PA7
#define LED_PIN_6H PC5
#define LED_PIN_8H PB1
#define LED_PIN_10H PB10

// 3rd row: S1, Setup (WLAN), S2
#define LED_PIN_S1 PA5
#define LED_PIN_SETUP PF5
#define LED_PIN_S2 PC7

// 4th row: Lock
#define LED_PIN_LOCK PA1

// Backside
#define LED_PIN_REAR PB13

#define LED_ANIMATE_DELAY 20 // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

const uint32_t kPinByLedNum[] = {
    // Order derived from LowLevel "enum LED_id"
    //                       C500
    // Use LED         num  Original  Alternative
    LED_PIN_CHARGE, //  0
    LED_PIN_BAT,    //  1
    LED_PIN_WIRE,   //  2
    LED_PIN_LIFTED, //  3
    LED_PIN_NC,     //  4    SUN
    LED_PIN_NC,     //  5    SAT
    LED_PIN_NC,     //  6    FRI
    LED_PIN_NC,     //  7    THU
    LED_PIN_NC,     //  8    WED
    LED_PIN_NC,     //  9    TUE
    LED_PIN_NC,     // 10    MON
    LED_PIN_LOCK,   // 11
    LED_PIN_S2,     // 12              digit 5
    LED_PIN_S1,     // 13              digit 4
    LED_PIN_10H,    // 14    8HR       digit 3
    LED_PIN_8H,     // 15    6HR       digit 2
    LED_PIN_6H,     // 16    4HR       digit 1
    LED_PIN_4H,     // 17    2HR       digit 0
    LED_PIN_REAR,   // 18
    LED_PIN_SETUP   // 19
};

const unsigned int kLedAnimOrder[] = {3, 2, 1, 0, 17, 16, 15, 14, 13, 19, 12, 11};
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
const uint8_t kBase10Leds[] = {
    0b100000, // = 0
    0b010000,
    0b001000,
    0b000100,
    0b000010,
    0b000001,
    0b010001,
    0b001001,
    0b000101,
    0b000011}; // = 9

class LEDcontrolRMECOWV100 : public LEDcontrol
{
public:
    LEDcontrolRMECOWV100(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback) {}

    void sequence_animate_handler()
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

    /**
     * @brief A quick boot/power-on animation, also used as jammed button indicator
     *
     * @return unsigned int ms how long it will take to play
     */
    unsigned int boot_animation() // A short boot animation which return the amount of ms it will take
    {
        sequence_start(&LEDcontrol::sequence_animate_handler);
        return ((kNumFrontLeds + 1) * LED_ANIMATE_DELAY);
    }

    /**
     * @brief Set base10 related LEDs for the given (numeric) character
     *
     * @param digit numeric character
     */
    void set_base10_leds(char digit) override
    {
        for (uint8_t bit = 0; bit <= 5; bit++) // We've 5 LEDs for base10 number representation
        {
            bool on = (kBase10Leds[digit - '0'] >> bit) & 0b1;
            unsigned int led = bit + 12;
            set(led, on ? LED_state::LED_on : LED_state::LED_off, false);
        }
    }

    void force_off_num_seq_leds(bool force) override
    {
        force_off(LED_NUM_LIFTED, force);       // Num change signalling LED
        for (unsigned int i = 12; i <= 17; i++) // Base10 related LEDs
            force_off(i, force);
    }
};

LEDcontrolRMECOWV100 leds(kPinByLedNum, sizeof(kPinByLedNum) / sizeof(uint32_t), &LEDcontrol::set_base10_leds); // Main LED controller object

#endif /* YARDFORCE_LEDCONTROL_RMECOWV100_H */
