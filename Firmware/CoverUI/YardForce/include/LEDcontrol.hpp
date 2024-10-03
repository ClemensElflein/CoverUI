/**
 * @file LEDcontrol.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.4
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_HPP
#define YARDFORCE_LEDCONTROL_HPP

#include <Arduino.h>
#include <stdint.h>

#include "../../BttnCtl.h"

#define LED_PIN_NC 0xffffffff  // Not Connected (virtual LED)

#define LED_NUM_OM_MAX 17  // Highest LED num controlled by OM LL FW

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

class LEDcontrol {
   public:
    const uint32_t *kPinByLedNumPtr;                     // Pointer to an array of LED pins, indexed by OM-LED-Num
    const size_t kNumLeds;                               // Number of defined LEDs
    void (LEDcontrol::*set_base10_leds_cb)(char digit);  // Callback pointer to set_base10_leds_cb(char digit) method

    LEDcontrol(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_cb)(char digit) = nullptr);

    virtual ~LEDcontrol() = default;

    void blink_timer_elapsed(LED_state blink_state);  // Get called by responsible blink timer

    void force_off(uint8_t led_num, bool force);         // Switch/force LED num off, independent of it's running state
    virtual void force_off_num_seq_leds(bool force) {};  // Force off all show_num() / sequence-num LEDs
    void force_on(uint8_t led_num, bool force);          // Switch/force LED num on, independent of it's running state

    LED_state get(uint8_t led_num);                                                             // Get state from led_states_bin_ for the given led
    bool has(uint8_t led_num, LED_state state);                                                 // Comparison if the given LED has the given state (in led_states_bin_)
    void identify(uint8_t led_num);                                                             // Identify the given LED by a special signalling
    void set(uint8_t led_num, LED_state state = LED_state::LED_off, bool change_state = true);  // Set any of known LED_state states for a specific LED
    void set(uint64_t all_state);                                                               // Set any of known LED_state states for all LEDs by binary state value
    virtual void set_base10_leds(char digit) {};                                                // Set LED representation for a single digit (by default empty)
    void toggle(uint8_t led_num, bool change_state = true);                                     // Set any of known LED_state states for a specific LED

    virtual unsigned int boot_animation() { return 0; };  // A short boot animation which return the amount of ms it will take

    // ***** Sequence stuff *****
    void process_sequence();                                                         // Called by timer for LED sequences like animation or FW version display
    void sequence_start(void (LEDcontrol::*handler)(), bool abort_running = false);  // Start the given sequence method
    virtual void sequence_animate_handler() {};                                      // A short LED Animation handler
    virtual void sequence_countdown_handler() {};                                    // A short LED Countdown handler
    void show_num(uint16_t);                                                         // Display a number via Mon-Sun + Lifted LED

   protected:
    uint64_t led_states_bin_ = 0;                     // Binary representation of all LEDs. Each LED gets three bits (19*3=57) for the current state (see BtnCtrl.h)
    uint32_t seq_start_tick_ = 0;                     // ms tick when sequence started. Indicator for a running sequence if > 0
    uint16_t seq_get_next_step_(uint16_t step_rate);  // Get next sequence step for the given step-rate. Return 0 if the next step isn't reached now.

   private:
    uint32_t force_led_on_ = 0;   // Binary representation of a "forced LED on" led_states_bin_ overrule
    uint32_t force_led_off_ = 0;  // Binary representation of a "forced LED off" led_states_bin_ overrule

    void change_led_states_(uint8_t led_num, LED_state state);        // Change led_states_bin_ for the given LED num and state
    void force_(uint8_t led_num, bool force, uint32_t *force_type_);  // Switch/force LED num on or off, independent on it's running state

    uint16_t seq_step_ = 0;   // Step position of the current running sequence
    uint16_t seq_num_value_;  // To lazy now to build a common member function pointer with variable function arg length

    void seq_num_handler_();                 // Sequence handler for displaying a number via Mon-Sun + Lifted LED
    void (LEDcontrol::*seq_handler_)(void);  // Member function ptr to sequence handler
};

#endif  // YARDFORCE_LEDCONTROL_HPP
