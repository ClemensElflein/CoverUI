/**
 * @file Display.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI abstract display class header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 */
#ifndef YARDFORCE_DISPLAY_HPP
#define YARDFORCE_DISPLAY_HPP

#define LVGL_BUFFER_MULTIPLIER 10
#define LVGL_TIMER_HANDLER_PERIOD_MS 10 // 10ms lv_timer_handler() soft period

#include <Arduino.h>
#include "../BttnCtl.h"

#define BACKLIGHT_TIMEOUT_MS 120000 // 2 minutes

namespace yardforce
{
    namespace display
    {
        class Display
        {
        public:
            struct Config
            {
                uint8_t backlight_led_num;
            };

            enum AnncmntType
            {
                none,
                close_hatch,
                version,
            };

            struct Anncmnt // Announcement
            {
                uint32_t timeout; // Timeout (ms)
                AnncmntType type;
            };

            Display(Config t_config) : config(t_config){};

            virtual bool init() = 0; // Init GPIOs, comms, as well as display

            void tick_inc(uint8_t ms);
            virtual void loop_low_prio(); // Low priority loop for display changes. Also does lv_timer_handler() and subscription

            void set_backlight(LED_state t_state = LED_on, uint32_t t_timeout = BACKLIGHT_TIMEOUT_MS);
            LED_state check_backlight();

            void start_anncmnt(uint32_t t_timeout_ms, AnncmntType t_type); // Start a new announcement
            AnncmntType get_anncmnt();                                     // Get announcement type if an announcement is running

        protected:
            Config config;
            Anncmnt anncmnt;

            // Backlight handling
            LED_state backlight_state;
            uint32_t backlight_timeout;
        }; // class Display

    } // namespace display
} // namespace yardforce
#endif // YARDFORCE_DISPLAY_HPP