/**
 * @file WidgetLedSymbol.hpp
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
#include <string>
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
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
            lv_obj_align(label, align, x_ofs, y_ofs);
        }

        static void anim_blink_cb(void *var, int32_t v)
        {
            if (v)
                lv_obj_clear_flag((lv_obj_t *)var, LV_OBJ_FLAG_HIDDEN);
            else
                lv_obj_add_flag((lv_obj_t *)var, LV_OBJ_FLAG_HIDDEN);
        }

        void set(LED_state t_state)
        {
            if (t_state == state)
                return;

            switch (t_state)
            {
            case LED_on:
                lv_anim_del(label, (lv_anim_exec_xcb_t)anim_blink_cb);
                lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
                break;
            case LED_blink_slow:
                startBlinkAnim(500);
                break;
            case LED_blink_fast:
                startBlinkAnim(100);
                break;
            default: // off
                lv_anim_del(label, (lv_anim_exec_xcb_t)anim_blink_cb);
                lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
                break;
            }
            state = t_state;
        }

    private:
        lv_obj_t *label;
        lv_anim_t anim;
        LED_state state = LED_on;

        void startBlinkAnim(uint16_t t_period_ms)
        {
            lv_anim_init(&anim);
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)anim_blink_cb);
            lv_anim_set_var(&anim, label);
            lv_anim_set_time(&anim, t_period_ms);
            lv_anim_set_repeat_delay(&anim, t_period_ms);
            lv_anim_set_values(&anim, 0, 1);
            lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
            lv_anim_start(&anim);
        }
    };
} // namespace display

#endif // __WIDGETLEDSYMBOL_HPP