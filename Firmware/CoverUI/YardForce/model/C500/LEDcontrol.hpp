/**
 * @file C500/LEDcontrol.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1x LED controller model for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */
#ifndef YARDFORCE_C500_LEDCONTROL_HPP
#define YARDFORCE_C500_LEDCONTROL_HPP

#include <Arduino.h>

#include "../../include/LEDcontrol.hpp"

#define LED_ANIMATE_DELAY 15  // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

// These constants should be declared i.e. in assembly.cpp
extern const uint8_t kBase10Leds[];

class LEDcontrolC500 : public LEDcontrol {
   public:
    LEDcontrolC500(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback) {};

    /**
     * @brief Animate sequence handler. Has to be started by sequence_start()
     */
    void sequence_animate_handler() override {
        unsigned int step = seq_get_next_step_(LED_ANIMATE_DELAY);  // Animation sequence runs in 15ms steps

        if (step == 0)  // Next sequence step not reached now
            return;
        else if (step >= 1 && step <= kNumLeds)  // LED on
        {
            set(kNumLeds - step, LED_state::LED_on, false);
            return;
        } else if (step >= (kNumLeds + 1) && step <= (2 * kNumLeds))  // LED off
        {
            set((2 * kNumLeds) - step, LED_state::LED_off, false);
            return;
        } else {
            seq_start_tick_ = 0;   // Sequence end
            set(led_states_bin_);  // Restore states
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
        return ((kNumLeds + 1) * LED_ANIMATE_DELAY);
    }

    /**
     * @brief Set base10 related LEDs for the given (numeric) character
     *
     * @param digit numeric character
     */
    void set_base10_leds(char digit) override {
        for (uint8_t bit = 0; bit <= 6; bit++)  // We've 6 LEDs for base10 number representation
        {
            bool on = (kBase10Leds[digit - '0'] >> bit) & 0b1;
            unsigned int led = bit + 12;
            set(bit + 4, on ? LED_state::LED_on : LED_state::LED_off, false);
        }
    }

    void force_off_num_seq_leds(bool force) override {
        force_off(LED_NUM_LIFTED, force);       // Num change signalling LED
        for (unsigned int i = 4; i <= 10; i++)  // Base10 related LEDs
            force_off(i, force);
    }
};

#endif  // YARDFORCE_C500_LEDCONTROL_HPP
