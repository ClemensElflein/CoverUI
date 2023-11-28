/**
 * @file Display_RM-EC3-V11.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI JLX25664 display driver for NX100i OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 */
#ifndef YARDFORCE_DISPLAY_RMEC3V11_HPP
#define YARDFORCE_DISPLAY_RMEC3V11_HPP

#define ST75256_DISPLAY_WIDTH 256
#define ST75256_DISPLAY_HEIGHT 64

// JLX25664G Tscyc min = 80ns = 12.5MHz
// ST32F401 = 84MHz / 8 = 10.5MHz
#define ST75256_SPI_BAUDRATEPRESCALER SPI_BAUDRATEPRESCALER_8

#define LVGL_BUFFER_MULTIPLIER 10
#define LVGL_TIMER_HANDLER_PERIOD_MS 10 // 10ms lv_timer_handler() soft period

#define TOP_STATUS_BAR_GAP_PX 5 // Amount of (gap) pixels between top status-bar icons/symbols
#define EMERGENCY_CLEAR_TEXT "Emergency! Press [OK], close hatch and stay back, to clear emergency state."

#include "../Display.hpp"
#include "../ST75256.hpp"
#include <lvgl.h>
#include "../LEDcontrol.hpp"
#include "../WidgetLedSymbol.hpp"
#include "../WidgetBar.hpp"
#include "../WidgetTextTicker.hpp"
#include "../subscription.h"

#define BACKLIGHT_TIMEOUT_MS 120000 // 2 minutes
#define STATUS_TICKER_LENGTH 100    // TODO

namespace yardforce
{
    namespace display
    {
        class DisplayRMEC3V11 : public Display
        {
        public:
            DisplayRMEC3V11(Config t_config) : Display(t_config){};

            bool init() override; // Init GPIOs, comms, as well as LVGL & display

            void loop_low_prio() override; // Low priority loop for display changes. Also does lv_timer_handler() and subscription

            void openmower_anim();
            static void openmower_anim_delay();
            static void mainScreen();

        protected:
            lv_disp_drv_t lv_disp_drv; // LVGL driver

            static void rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *area);
            static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *t_color_p);

            static void anim_x_cb(void *var, int32_t v);
            
            static void set_undocked();
        };
    } // namespace display
} // namespace yardforce

extern yardforce::display::DisplayRMEC3V11 display;

#endif // YARDFORCE_DISPLAY_RMEC3V11_HPP