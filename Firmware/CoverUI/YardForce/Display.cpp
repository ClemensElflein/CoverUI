/**
 * @file Display.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI abstract display class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 */
#include "include/main.h"
#include "include/Display.hpp"
#include "include/subscription.h"

namespace yardforce
{
    namespace display
    {

        /**
         * @brief Tell LVGL that <ms> milliseconds have been elapsed.
         *  Required for anim or similar. Should be called in a high priority routine like hwtimer.
         *
         * @param ms
         */
        void Display::tick_inc(uint8_t ms)
        {
            lv_tick_inc(ms);
        }

        void Display::loop_low_prio()
        {
            lv_timer_handler();

            subscription::subscribe((Topic_set_ll_status | Topic_set_hl_state), 500);

            check_backlight();
        }

        /**
         * @brief Set backlight LED state and set/reset timeout counter
         *
         * @param state LED_state, default LED_on
         * @param timeout in ms when to switch off
         */
        void Display::set_backlight(LED_state t_state, uint32_t t_timeout)
        {
            ::leds.set(config.backlight_led_num, t_state);
            if (t_state != LED_off)
            {
                backlight_timeout = millis() + t_timeout;
            }
            backlight_state = t_state;
        }

        LED_state Display::check_backlight()
        {
            if (backlight_state == LED_off)
                return backlight_state;

            if (millis() < backlight_timeout)
                return backlight_state;

            set_backlight(LED_off);
            return backlight_state;
        }

        void Display::start_anncmnt(uint32_t t_timeout_ms, AnncmntType t_type)
        {
            anncmnt = {
                .timeout = millis() + t_timeout_ms,
                .type = t_type};
        }

        Display::AnncmntType Display::get_anncmnt()
        {
            if (!anncmnt.timeout)
                return AnncmntType::none;

            if (millis() < anncmnt.timeout)
                return anncmnt.type;

            anncmnt.timeout = 0;
            return AnncmntType::none;
        }
    } // namespace display
} // namespace yardforce