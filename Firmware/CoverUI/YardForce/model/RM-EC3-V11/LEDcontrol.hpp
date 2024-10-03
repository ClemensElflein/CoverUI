/**
 * @file RM-EC3-V11/LEDcontrol.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V11 LED controller model for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */
#ifndef YARDFORCE_RMEC3V11_LEDCONTROL_HPP
#define YARDFORCE_RMEC3V11_LEDCONTROL_HPP

#include <Arduino.h>

#include "../../include/LEDcontrol.hpp"

#define LED_ANIMATE_DELAY 50  // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

// These constants should be declared i.e. in assembly.cpp
extern const unsigned int kNumFrontLeds;
extern const unsigned int kLedAnimOrder[];

class LEDcontrolRMEC3V11 : public LEDcontrol {
   public:
    LEDcontrolRMEC3V11(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback) {};

    /**
     * @brief Animate sequence handler. Has to be started by sequence_start()
     */
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

    void show_countdown_state(unsigned int sec, LED_state state) {
        unsigned int sec_left = 5 - sec;

        for (size_t i = 1; i <= 4; i++) {
            uint8_t led_num = 18 - i;
            if (sec_left >= i)  // Remaining secs is greater than this LED
            {
                set(led_num, state, false);
            } else {
                set(led_num, LED_off, false);
                force_off(led_num, true);
            }
        }
    };

    /**
     * @brief Countdown LED animation handler
     */
    void sequence_countdown_handler() {
        uint16_t step = seq_get_next_step_(100);  // Animation sequence run in 1000ms steps

        if (step == 0)  // Next sequence step not reached now
            return;
        else if (step == 1 || step == 11 || step == 21 || step == 31 || step == 41) {
            show_countdown_state(((step - 1) / 10) + 1, LED_blink_fast);
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
    unsigned int boot_animation()  // A short boot animation which return the amount of ms it will take
    {
        sequence_start(&LEDcontrol::sequence_animate_handler);
        return ((kNumFrontLeds + 1) * LED_ANIMATE_DELAY);
    }
};

#endif  // YARDFORCE_RMEC3V11_LEDCONTROL_HPP
