/**
 * @file display_SAxPRO.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI WYM240128K1 display driver for SAxPRO OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-07
 *
 * @copyright Copyright (c) 2023
 */
#ifndef YARDFORCE_DISPLAY_SAXPRO_H
#define YARDFORCE_DISPLAY_SAXPRO_H

#define UC1698_DISPLAY_WIDTH 240
#define UC1698_DISPLAY_HEIGHT 128
#define LVGL_BUFFER_MULTIPLIER 10
#define LVGL_TIMER_HANDLER_PERIOD_MS 10 // 10ms lv_timer_handler() soft period

#define TOP_STATUS_BAR_GAP_PX 5 // Amount of (gap) pixels between top status-bar icons/symbols
#define EMERGENCY_CLEAR_TEXT "Emergency! Press [Enter], close cover and stay back, to clear emergency state."

#include <Arduino.h>
#include "../UC1698.hpp"
#include <lvgl.h>
#include "../LEDcontrol.hpp"
#include "../WidgetLedSymbol.hpp"
#include "../WidgetBar.hpp"
#include "../WidgetTextTicker.hpp"
#include "../subscription.h"

#define BACKLIGHT_TIMEOUT_MS 120000 // 2 minutes
#define STATUS_TICKER_LENGTH 100

namespace display
{
    bool init();
    void set_backlight(LED_state state = LED_on, uint32_t timeout = BACKLIGHT_TIMEOUT_MS);
    void loop_low_prio();
    void tick_inc(uint8_t ms);
    void start_countdown(uint32_t t_countdown_ms);
    bool check_backlight();
} // namespace display

#endif // YARDFORCE_DISPLAY_SAXPRO_H