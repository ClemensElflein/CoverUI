/**
 * @file WidgetLedSymbol.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief Tiny class/wrapper for a virtual LED, which get displayed as symbol (LVGL label) for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 */
#include <Arduino.h>
#include "include/WidgetLedSymbol.hpp"

namespace lvgl
{
    static void anim_blink_cb(void *var, int32_t v)
    {
        if (v)
            lv_obj_clear_flag((lv_obj_t *)var, LV_OBJ_FLAG_HIDDEN);
        else
            lv_obj_add_flag((lv_obj_t *)var, LV_OBJ_FLAG_HIDDEN);
    }

    WidgetLedSymbol::WidgetLedSymbol(const char *t_symbol_str, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {
        label_ = lv_label_create(lv_scr_act());
        lv_label_set_text_static(label_, t_symbol_str);
        lv_obj_set_style_text_align(label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
        lv_obj_align(label_, align, x_ofs, y_ofs);
    }

    void WidgetLedSymbol::set(LED_state t_state)
    {
        if (t_state == state)
            return;

        switch (t_state)
        {
        case LED_on:
            lv_anim_del(label_, (lv_anim_exec_xcb_t)anim_blink_cb);
            lv_obj_clear_flag(label_, LV_OBJ_FLAG_HIDDEN);
            break;
        case LED_blink_slow:
            start_blink_anim_(700);
            break;
        case LED_blink_fast:
            start_blink_anim_(400);
            break;
        default: // off
            lv_anim_del(label_, (lv_anim_exec_xcb_t)anim_blink_cb);
            lv_obj_add_flag(label_, LV_OBJ_FLAG_HIDDEN);
            break;
        }
        state = t_state;
    }

    void WidgetLedSymbol::start_blink_anim_(uint16_t t_period_ms)
    {
        lv_anim_init(&anim_);
        lv_anim_set_exec_cb(&anim_, (lv_anim_exec_xcb_t)anim_blink_cb);
        lv_anim_set_var(&anim_, label_);
        lv_anim_set_time(&anim_, t_period_ms);
        lv_anim_set_repeat_delay(&anim_, t_period_ms);
        lv_anim_set_values(&anim_, 0, 1);
        lv_anim_set_repeat_count(&anim_, LV_ANIM_REPEAT_INFINITE);
        lv_anim_start(&anim_);
    }
} // namespace lvgl
