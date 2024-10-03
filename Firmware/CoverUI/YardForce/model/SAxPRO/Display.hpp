/**
 * @file Display_SAxPRO.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI WYM240128K1 display driver for SAxPRO OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2023-11-29
 *
 * @copyright Copyright (c) 2023
 */
#ifndef YARDFORCE_DISPLAY_SAXPRO_HPP
#define YARDFORCE_DISPLAY_SAXPRO_HPP

#define UC1698_DISPLAY_WIDTH 240
#define UC1698_DISPLAY_HEIGHT 128

#define LVGL_BUFFER_MULTIPLIER 10
#define LVGL_TIMER_HANDLER_PERIOD_MS 10  // 10ms lv_timer_handler() soft period

#define TOP_STATUS_BAR_GAP_PX 5  // Amount of (gap) pixels between top status-bar icons/symbols
#define EMERGENCY_CLEAR_TEXT "Emergency! Press [Enter], close hatch and stay back, to clear emergency state."

// clang-format off
#include "../../include/ABC_Display.hpp"
#include "../../include/UC1698.hpp"
#include <lvgl.h>
#include "../../include/WidgetLedSymbol.hpp"
#include "../../include/WidgetBar.hpp"
#include "../../include/WidgetTextTicker.hpp"
#include "../../include/subscription.h"
// clang-format on

#define BACKLIGHT_TIMEOUT_MS 120000  // 2 minutes
#define STATUS_TICKER_LENGTH 100

// Enable for benchmarking specific code
// #define BENCHMARK

#ifdef BENCHMARK
#include "include/CortexMCycleCounter.hpp"
#endif

namespace yardforce {
namespace display {
class DisplaySAXPRO : public AbstractDisplay {
   public:
    DisplaySAXPRO(Config t_config) : AbstractDisplay(t_config) {};

    bool init() override;  // Init GPIOs, comms, as well as LVGL & display

    void loop_low_prio() override;  // Low priority loop for display changes. Also does lv_timer_handler() and subscription

    void openmower_anim();
    static void mainScreen();

   protected:
    lv_disp_drv_t lv_disp_drv;  // LVGL driver

    static void rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *area);
    static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *t_color_p);

    static void anim_x_cb(void *var, int32_t v);

    static void set_undocked();
};

#ifdef BENCHMARK
static CortexMCycleCounter cycle_cnt_flush_cb_;
#endif

}  // namespace display
}  // namespace yardforce

extern yardforce::display::DisplaySAXPRO display;

#endif  // YARDFORCE_DISPLAY_SAXPRO_HPP