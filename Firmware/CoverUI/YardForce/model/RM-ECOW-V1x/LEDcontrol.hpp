/**
 * @file RM-ECOW-V1x/LEDcontrol.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1x LED controller model for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */
#ifndef YARDFORCE_RMECOWV1X_LEDCONTROL_HPP
#define YARDFORCE_RMECOWV1X_LEDCONTROL_HPP

#include <Arduino.h>

#include "../../include/LEDcontrol.hpp"

#define LED_ANIMATE_DELAY 20  // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

// These constants should be declared i.e. in assembly.cpp
extern const unsigned int kNumFrontLeds;
extern const unsigned int kLedAnimOrder[];
extern const uint8_t kBase10Leds[];

class LEDcontrolRMECOWV1x : public LEDcontrol {
   public:
    LEDcontrolRMECOWV1x(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback) {};

    void sequence_animate_handler() override {
        uint16_t step = seq_get_next_step_(LED_ANIMATE_DELAY);  // Animation sequence runs in 15ms steps

        if (step == 0)  // Next sequence step not reached now
            return;
        else if (step >= 1 && step <= kNumFrontLeds) {  // LED on
            set(kLedAnimOrder[step - 1], LED_state::LED_on, false);
            return;
        } else if (step >= (kNumFrontLeds + 1) && step <= (2 * kNumFrontLeds)) {  // LED off
            set(kLedAnimOrder[step - kNumFrontLeds - 1], LED_state::LED_off, false);
            return;
        } else {
            seq_start_tick_ = 0;   // Sequence end
            set(led_states_bin_);  // Restore states
            return;
        }
    }

    /**
     * @brief Countdown LED animation handler
     */
    void sequence_countdown_handler() override {
        uint16_t step = seq_get_next_step_(100);  // Animation sequence run in 1000ms steps

        if (step == 0)  // Next sequence step not reached now
            return;
        else if (step == 1 || step == 11 || step == 21 || step == 31 || step == 41) {
            show_countdown_state_(((step - 1) / 10) + 1, LED_blink_fast);
            return;
        } else if (step < 41) {
            return;
        } else {
            seq_start_tick_ = 0;   // Sequence end
            set(led_states_bin_);  // Restore states
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
    unsigned int boot_animation() override {
        sequence_start(&LEDcontrol::sequence_animate_handler);
        return ((kNumFrontLeds + 1) * LED_ANIMATE_DELAY);
    }

    /**
     * @brief Set base10 related LEDs for the given (numeric) character
     *
     * @param digit numeric character
     */
    void set_base10_leds(char digit) override {
        for (uint8_t bit = 0; bit <= 5; bit++)  // We've 5 LEDs for base10 number representation
        {
            bool on = (kBase10Leds[digit - '0'] >> bit) & 0b1;
            unsigned int led = bit + 12;
            set(led, on ? LED_state::LED_on : LED_state::LED_off, false);
        }
    }

    void force_off_num_seq_leds(bool force) override {
        force_off(LED_NUM_LIFTED, force);        // Num change signalling LED
        for (unsigned int i = 12; i <= 17; i++)  // Base10 related LEDs
            force_off(i, force);
    }

   private:
    void show_countdown_state_(unsigned int sec, LED_state state) {
        unsigned int sec_left = 5 - sec;

        for (size_t i = 1; i <= 4; i++) {
            uint8_t led_num = 18 - i;
            if (sec_left >= i) {            // Remaining secs is greater than this LED
                set(led_num, state, true);  // change_state = true because blink states get handled by timer and this need to be stored
            } else {
                set(led_num, LED_off, true);
                force_off(led_num, true);
            }
        }
    }
};

#endif  // YARDFORCE_RMECOWV1X_LEDCONTROL_HPP
