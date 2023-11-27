/**
 * @file display_RM-EC3-V11.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI JLX25664 display driver for NX100i OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 */
#ifndef YARDFORCE_DISPLAY_RMEC3V11_H
#define YARDFORCE_DISPLAY_RMEC3V11_H

#define ST75256_DISPLAY_WIDTH 256
#define ST75256_DISPLAY_HEIGHT 64

// JLX25664G Tscyc min = 80ns = 12.5MHz
// ST32F401 = 84MHz / 8 = 10.5MHz
#define ST75256_SPI_BAUDRATEPRESCALER SPI_BAUDRATEPRESCALER_8

#define LVGL_BUFFER_MULTIPLIER 10
#define LVGL_TIMER_HANDLER_PERIOD_MS 10 // 10ms lv_timer_handler() soft period

#define TOP_STATUS_BAR_GAP_PX 5 // Amount of (gap) pixels between top status-bar icons/symbols
#define EMERGENCY_CLEAR_TEXT "Emergency! Press [OK], close hatch and stay back, to clear emergency state."

#define HAS_DISPLAY

#include <Arduino.h>
#include "../ST75256.hpp"
#include <lvgl.h>
#include "../LEDcontrol.hpp"
#include "../WidgetLedSymbol.hpp"
#include "../WidgetBar.hpp"
#include "../WidgetTextTicker.hpp"
#include "../subscription.h"

#define BACKLIGHT_TIMEOUT_MS 120000 // 2 minutes
#define STATUS_TICKER_LENGTH 100    // TODO

namespace display
{
    enum StatusTicker
    {
        normal,
        close_hatch,
        version,
    };

    bool init();
    void set_backlight(LED_state state = LED_on, uint32_t timeout = BACKLIGHT_TIMEOUT_MS);
    void loop_low_prio();
    void tick_inc(uint8_t ms);
    void start_countdown(uint32_t t_countdown_ms);
    bool check_backlight();
} // namespace display

#endif // YARDFORCE_DISPLAY_RMEC3V11_H