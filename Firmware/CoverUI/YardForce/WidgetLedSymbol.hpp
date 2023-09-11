/**
 * @file GuiLedSymbol.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief Tiny class/wrapper for a virtual LED, which get displayed as symbol (LVGL label) for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-09-11
 *
 * @copyright Copyright (c) 2023
 */
#ifndef __WIDGETLEDSYMBOL_HPP
#define __WIDGETLEDSYMBOL_HPP

#include <Arduino.h>
#include <lvgl.h>
#include "LEDcontrol.h"

namespace display
{
    class WidgetLedSymbol
    {
    public:
        std::string symbol_str;

        WidgetLedSymbol(const std::string &t_symbol_str, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) : symbol_str(t_symbol_str)
        {
            label = lv_label_create(lv_scr_act());
            lv_label_set_text(label, symbol_str.c_str());
            lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_align(label, align, x_ofs, y_ofs);
        }

        void set(LED_state state)
        {
            if (state == LED_off)
                lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
            else if (state == LED_on)
                lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
        }

    private:
        lv_obj_t *label;
    };
} // namespace display

#endif // __WIDGETLEDSYMBOL_HPP