/**
 * @file Display.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI abstract display class header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 */
#ifndef YARDFORCE_ABC_DISPLAY_HPP
#define YARDFORCE_ABC_DISPLAY_HPP

#define LVGL_BUFFER_MULTIPLIER 10
#define LVGL_TIMER_HANDLER_PERIOD_MS 10  // 10ms lv_timer_handler() soft period

#include <Arduino.h>
#include <lvgl.h>

#include "../BttnCtl.h"
#include "include/main.h"
#include "include/subscription.h"

#define BACKLIGHT_TIMEOUT_MS 120000  // 2 minutes

namespace yardforce {
namespace display {
class AbstractDisplay {
   public:
    struct Config {
        uint8_t backlight_led_num;
    };

    enum AnncmntType {
        none,
        close_hatch,
        version,
    };

    struct Anncmnt  // Announcement
    {
        uint32_t timeout;  // Timeout (ms)
        AnncmntType type;
    };

    AbstractDisplay(Config t_config) : config(t_config) {};

    virtual bool init() = 0;  // Init GPIOs, comms, as well as display

    /**
     * @brief Tell LVGL that <ms> milliseconds have been elapsed.
     *  Required for anim or similar. Should be called in a high priority routine like hwtimer.
     *
     * @param ms
     */
    void tick_inc(uint8_t ms) {
        lv_tick_inc(ms);
    }

    // Low priority loop for display changes. Also does lv_timer_handler() and subscription
    virtual void loop_low_prio() {
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
    void set_backlight(LED_state t_state = LED_on, uint32_t t_timeout = BACKLIGHT_TIMEOUT_MS) {
        ::leds.set(config.backlight_led_num, t_state);
        if (t_state != LED_off)
            backlight_timeout = millis() + t_timeout;
        backlight_state = t_state;
    }

    LED_state check_backlight() {
        if (backlight_state == LED_off)
            return backlight_state;

        if (millis() < backlight_timeout)
            return backlight_state;

        set_backlight(LED_off);
        return backlight_state;
    }

    // Start a new announcement
    void start_anncmnt(uint32_t t_timeout_ms, AnncmntType t_type) {
        anncmnt = {
            .timeout = millis() + t_timeout_ms,
            .type = t_type};
    }

    // Get announcement type if an announcement is running
    AnncmntType get_anncmnt() {
        if (!anncmnt.timeout)
            return AnncmntType::none;

        if (millis() < anncmnt.timeout)
            return anncmnt.type;

        anncmnt.timeout = 0;
        return AnncmntType::none;
    }

   protected:
    Config config;
    Anncmnt anncmnt;

    // Backlight handling
    LED_state backlight_state;
    uint32_t backlight_timeout;
};  // class Display

}  // namespace display
}  // namespace yardforce
#endif  // YARDFORCE_ABC_DISPLAY_HPP