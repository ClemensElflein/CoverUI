/**
 * @file WidgetLedSymbol.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief Tiny class/wrapper for a virtual LED, which get displayed as symbol (LVGL label) for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 */
#ifndef __WIDGETLEDSYMBOL_HPP
#define __WIDGETLEDSYMBOL_HPP

#include <Arduino.h>
#include <string>
#include <lvgl.h>
#include "LEDcontrol.hpp"

namespace lvgl
{
    class WidgetLedSymbol
    {
    public:
        LED_state state = LED_on;

        WidgetLedSymbol(const char *t_symbol_str, lv_align_t align, lv_coord_t x_ofs = 0, lv_coord_t y_ofs = 0);

        void set(LED_state t_state);

    private:
        lv_obj_t *label_;
        lv_anim_t anim_;

        void start_blink_anim_(uint16_t t_period_ms);
    };
} // namespace lvgl

#endif // __WIDGETLEDSYMBOL_HPP