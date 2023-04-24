/**
 * @file LEDcontrol.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-04-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_LEDCONTROL_H
#define YFC500_LEDCONTROL_H

#include <stdint.h>
#include <map>
#include "LEDcontrol.h"
#include "stm32cube/inc/gpio.h"
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
    struct Led_pio_def
    {
        GPIO_TypeDef *port;
        uint16_t pin;
    };
    const Led_pio_def _leds[NUM_LEDS] = {
        // Order derived from LowLevel "enum LED_id"
        {LED_CHARGE_GPIO_Port, LED_CHARGE_Pin}, //  0
        {LED_BAT_GPIO_Port, LED_BAT_Pin},       //  1
        {LED_WIRE_GPIO_Port, LED_WIRE_Pin},     //  2
        {LED_LIFTED_GPIO_Port, LED_LIFTED_Pin}, //  3
        {LED_SUN_GPIO_Port, LED_SUN_Pin},       //  4 (digit 6)
        {LED_SAT_GPIO_Port, LED_SAT_Pin},       //  5 (digit 5)
        {LED_FRI_GPIO_Port, LED_FRI_Pin},       //  6 (digit 4)
        {LED_THU_GPIO_Port, LED_THU_Pin},       //  7 (digit 3)
        {LED_WED_GPIO_Port, LED_WED_Pin},       //  8 (digit 2)
        {LED_TUE_GPIO_Port, LED_TUE_Pin},       //  9 (digit 1)
        {LED_MON_GPIO_Port, LED_MON_Pin},       // 10 (digit 0)
        {LED_LOCK_GPIO_Port, LED_LOCK_Pin},     // 11
        {LED_S2_GPIO_Port, LED_S2_Pin},         // 12
        {LED_S1_GPIO_Port, LED_S1_Pin},         // 13
        {LED_8HR_GPIO_Port, LED_8HR_Pin},       // 14
        {LED_6HR_GPIO_Port, LED_6HR_Pin},       // 15
        {LED_4HR_GPIO_Port, LED_4HR_Pin},       // 16
        {LED_2HR_GPIO_Port, LED_2HR_Pin},       // 17
        {LED_REAR_GPIO_Port, LED_REAR_Pin}      // 18
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

    void blink_timer_elapsed(LED_state blink_state);                                           // Get called by responsible blink timer
    void force_off(uint8_t led_num, bool force);                                               // Switch/force LED num off, independent of it's running state
    void force_on(uint8_t led_num, bool force);                                                // Switch/force LED num on, independent of it's running state
    LED_state get(uint8_t led_num);                                                            // Get state from _led_states_bin for the given led
    bool has(uint8_t led_num, LED_state state);                                                // Comparison if the given LED has the given state (in _led_states_bin)
    void identify(uint8_t led_num);                                                            // Identify the given LED by a special signalling
    void set(uint8_t led_num, LED_state state = LED_state::LED_off, bool change_state = true); // Set any of known LED_state states for a specific LED
    void set(uint64_t all_state);                                                              // Set any of known LED_state states for all LEDs by binary state value
    void show_num(uint16_t);                                                                   // Display a number via Mon-Sun + Lifted LED
    void toggle(uint8_t led_num);                                                              // Toggle on->off or off->on

    // ***** Sequence stuff *****
    void process_sequence();                            // Called by timer for LED sequences like animation or FW version display
    void sequence_start(void (LEDcontrol::*handler)()); // Start the given sequence method
    void sequence_animate_handler();                    // A short LED Animation handler
};

#endif /* YFC500_LEDCONTROL_H */
