/**
 * @file LEDcontrol.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_LEDCONTROL_H
#define YFC500_LEDCONTROL_H

#include <stdint.h>
#include <map>
#include "settings.h"
#include "../BttnCtl.h"

#define NUM_LEDS 19

// Some handy LED-num defines
#define LED_NUM_LIFTED 3
#define LED_NUM_WIRE 2
#define LED_NUM_BAT 1
#define LED_NUM_CHARGE 0
#define LED_NUM_REAR 18
#define LED_NUM_MON 10
#define LED_NUM_SUN 4

class LEDcontrol
{
private:
    const uint32_t _leds[NUM_LEDS] = {
        // Order derived from LowLevel "enum LED_id"
        LED_PIN_CHARGE, //  0
        LED_PIN_BAT,    //  1
        LED_PIN_WIRE,   //  2
        LED_PIN_LIFTED, //  3
        LED_PIN_SUN,    //  4 (digit 6)
        LED_PIN_SAT,    //  5 (digit 5)
        LED_PIN_FRI,    //  6 (digit 4)
        LED_PIN_THU,    //  7 (digit 3)
        LED_PIN_WED,    //  8 (digit 2)
        LED_PIN_TUE,    //  9 (digit 1)
        LED_PIN_MON,    // 10 (digit 0)
        LED_PIN_LOCK,   // 11
        LED_PIN_S2,     // 12
        LED_PIN_S1,     // 13
        LED_PIN_8HR,    // 14
        LED_PIN_6HR,    // 15
        LED_PIN_4HR,    // 16
        LED_PIN_2HR,    // 17
        LED_PIN_REAR    // 18
    };
    const uint8_t _base10_leds[10] = {
        // Numeric (base10) representation of LEDs.
        // I.e. Digit 0 = Mon(0), 9 = Thu(3) + Sun(6)
        // Bit 6 = Mon ... Bit 0 = Sun
        // MTWTFSS
        // 0123456
        0b01000000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001,
        0b00100001,
        0b00010001,
        0b00001001};
    uint64_t _led_states_bin = 0; // Binary representation of all LEDs. Each LED gets three bits (19*3=57) for the current state (see BtnCtrl.h)
    uint32_t _force_led_on = 0;   // Binary representation of a "forced LED on" _led_states_bin overrule
    uint32_t _force_led_off = 0;  // Binary representation of a "forced LED off" _led_states_bin overrule

    uint16_t _main_fw_version; // FW version of OM's

    void _change_led_states(uint8_t led_num, LED_state state);       // Change _led_states_bin for the given LED num and state
    void _force(uint8_t led_num, bool force, uint32_t *_force_type); // Switch/force LED num on or off, independent on it's running state

    // ***** Sequence stuff *****
    uint32_t _seq_start_tick = 0;           // ms tick when sequence started. Indicator for a running sequence if > 0
    uint16_t _seq_step = 0;                 // Step position of the current running sequence
    void (LEDcontrol::*_seq_handler)(void); // Member function ptr to sequence handler

    void _force_off_num_seq_leds(bool force);        // Force off all show_num() / sequence-num LEDs
    uint16_t _seq_get_next_step(uint16_t step_rate); // Get next sequence step for the given step-rate. Return 0 if the next step isn't reached now.
    uint16_t _seq_num_value;                         // To lazy now to build a common member function pointer with variable function arg length
    void _seq_num_handler();                         // Sequence handler for displaying a number via Mon-Sun + Lifted LED
    void _set_base10_leds(char digit);               // Set LED representation for a single digit

public:
    LEDcontrol();

    void setup();
    void blink_timer_elapsed(LED_state blink_state);                                          // Get called by responsible blink timer
    void force_off(uint8_t led_num, bool force);                                               // Switch/force LED num off, independent of it's running state
    void force_on(uint8_t led_num, bool force);                                                // Switch/force LED num on, independent of it's running state
    LED_state get(uint8_t led_num);                                                            // Get state from _led_states_bin for the given led
    bool has(uint8_t led_num, LED_state state);                                                // Comparison if the given LED has the given state (in _led_states_bin)
    void identify(uint8_t led_num);                                                            // Identify the given LED by a special signalling
    void set(uint8_t led_num, LED_state state = LED_state::LED_off, bool change_state = true); // Set any of known LED_state states for a specific LED
    void set(uint64_t all_state);                                                              // Set any of known LED_state states for all LEDs by binary state value
    void show_num(uint16_t);                                                                   // Display a number via Mon-Sun + Lifted LED

    // ***** Sequence stuff *****
    void process_sequence();                            // Called by timer for LED sequences like animation or FW version display
    void sequence_start(void (LEDcontrol::*handler)()); // Start the given sequence method
    void sequence_animate_handler();                    // A short LED Animation handler
};

#endif /* YFC500_LEDCONTROL_H */
