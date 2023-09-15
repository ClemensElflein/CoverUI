/**
 * @file LEDcontrol.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2023-09-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_H
#define YARDFORCE_LEDCONTROL_H

#include <Arduino.h>
#include <stdint.h>
#include <map>
#include "../BttnCtl.h"

#ifdef MDL_C500 // Model Classic 500

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

#define NUM_LEDS 19

#elif defined(MDL_SAXPRO) // Model SAxPRO

#define LED_PIN_BACKLIGHT PA11
#define NUM_LEDS 19

#define LED_NUM_BACKLIGHT 18
#define BACKLIGHT_TIMEOUT_STEP_RATE_MS 100
#define BACKLIGHT_TIMEOUT_MS 30000 // Should be int dividable by BACKLIGHT_TIMEOUT_STEP_RATE_MS

#else
#define NUM_LEDS 0

#endif

// Some handy LED-num defines
#define LED_NUM_CHARGE 0
#define LED_NUM_BAT 1
#define LED_NUM_WIRE 2
#define LED_NUM_LIFTED 3
#define LED_NUM_SUN 4
#define LED_NUM_MON 10
#define LED_NUM_S2 12
#define LED_NUM_S1 13
#define LED_NUM_8HR 14
#define LED_NUM_2HR 17
#define LED_NUM_REAR 18

#define LED_PIN_NC 0xffffffff // Not Connected (virtual LED)

class LEDcontrol
{
private:
#ifdef MDL_C500 // Model Classic 500
    const uint32_t kLeds[NUM_LEDS] = {
        // FIXME: Why is it uint32_t large? Test with C500 in STM & GD variant. Should become uint16_t (not uint8_t because of NC case)
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
    const uint8_t kBase10Leds[10] = {
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
#elif defined(MDL_SAXPRO) // Model SAxPRO
    const uint32_t kLeds[NUM_LEDS] = {
        // Order derived from LowLevel "enum LED_id"
        // TODO: As long as we don't have a separate proto for gfx display (value instead of LED), we need to simulate with virtual LEDs and translate them accordingly
        LED_PIN_NC,       //  0 = Charge
        LED_PIN_NC,       //  1 = Bat
        LED_PIN_NC,       //  2 = Wire
        LED_PIN_NC,       //  3 = Lifted
        LED_PIN_NC,       //  4 = Sun (digit 6)
        LED_PIN_NC,       //  5 = Sat (digit 5)
        LED_PIN_NC,       //  6 = Fri (digit 4)
        LED_PIN_NC,       //  7 = Thu (digit 3)
        LED_PIN_NC,       //  8 = Wed (digit 2)
        LED_PIN_NC,       //  9 = Tue (digit 1)
        LED_PIN_NC,       // 10 = Mon (digit 0)
        LED_PIN_NC,       // 11 = Lock
        LED_PIN_NC,       // 12 = S2
        LED_PIN_NC,       // 13 = S1
        LED_PIN_NC,       // 14 = 8hr
        LED_PIN_NC,       // 15 = 6hr
        LED_PIN_NC,       // 16 = 4hr
        LED_PIN_NC,       // 17 = 2hr
        LED_PIN_BACKLIGHT // 18
    };
#else
    const uint32_t kLeds[0] = {};
#endif
    uint64_t led_states_bin_ = 0; // Binary representation of all LEDs. Each LED gets three bits (19*3=57) for the current state (see BtnCtrl.h)
    uint32_t force_led_on_ = 0;   // Binary representation of a "forced LED on" led_states_bin_ overrule
    uint32_t force_led_off_ = 0;  // Binary representation of a "forced LED off" led_states_bin_ overrule

    void change_led_states_(uint8_t led_num, LED_state state);       // Change led_states_bin_ for the given LED num and state
    void force_(uint8_t led_num, bool force, uint32_t *force_type_); // Switch/force LED num on or off, independent on it's running state

    // ***** Sequence stuff *****
    uint32_t seq_start_tick_ = 0;           // ms tick when sequence started. Indicator for a running sequence if > 0
    uint16_t seq_step_ = 0;                 // Step position of the current running sequence
    void (LEDcontrol::*seq_handler_)(void); // Member function ptr to sequence handler

    uint16_t seq_get_next_step_(uint16_t step_rate); // Get next sequence step for the given step-rate. Return 0 if the next step isn't reached now.
    uint16_t seq_num_value_;                         // To lazy now to build a common member function pointer with variable function arg length

#ifdef MDL_C500                               // Classic 500 FIXME: Should go either into a superclass or need a more generic parent class, on next model
    void force_off_num_seq_leds_(bool force); // Force off all show_num() / sequence-num LEDs
    void seq_num_handler_();                  // Sequence handler for displaying a number via Mon-Sun + Lifted LED
    void set_base10_leds_(char digit);        // Set LED representation for a single digit
#endif                                        // MDL_C500

public:
    void setup();
    void blink_timer_elapsed(LED_state blink_state);                                           // Get called by responsible blink timer
    void blink_slow_timer_elapsed() { blink_timer_elapsed(LED_state::LED_blink_slow); };       // STM32/GD32 compatibility method due to framework-arduino[gd32] differences
    void blink_fast_timer_elapsed() { blink_timer_elapsed(LED_state::LED_blink_fast); };       // STM32/GD32 compatibility method due to framework-arduino[gd32] differences
    void force_off(uint8_t led_num, bool force);                                               // Switch/force LED num off, independent of it's running state
    void force_on(uint8_t led_num, bool force);                                                // Switch/force LED num on, independent of it's running state
    LED_state get(uint8_t led_num);                                                            // Get state from led_states_bin_ for the given led
    bool has(uint8_t led_num, LED_state state);                                                // Comparison if the given LED has the given state (in led_states_bin_)
    void identify(uint8_t led_num);                                                            // Identify the given LED by a special signalling
    void set(uint8_t led_num, LED_state state = LED_state::LED_off, bool change_state = true); // Set any of known LED_state states for a specific LED
    void set(uint64_t all_state);                                                              // Set any of known LED_state states for all LEDs by binary state value

    // ***** Sequence stuff *****
    void process_sequence();                                                        // Called by timer for LED sequences like animation or FW version display
    void sequence_start(void (LEDcontrol::*handler)(), bool abort_running = false); // Start the given sequence method

#ifdef MDL_C500                      // Classic 500 FIXME: Should go either into a superclass or need a more generic parent class, on next model
    void sequence_animate_handler(); // A short LED Animation handler
    void show_num(uint16_t);         // Display a number via Mon-Sun + Lifted LED
#elif defined(MDL_SAXPRO)            // Model SAxPRO
    void sequence_backlight_timeout_handler();
#endif                               // MDL_
};

#endif /* YARDFORCE_LEDCONTROL_H */
