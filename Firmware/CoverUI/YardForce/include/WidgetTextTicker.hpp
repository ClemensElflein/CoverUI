/**
 * @file WidgetTextTicker.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief Tiny class/wrapper for a text-ticker, which get displayed for mower status messages (as LVGL label + anim) for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-09-12
 *
 * @copyright Copyright (c) 2023
 */
#ifndef __WIDGETTEXTTICKER_HPP
#define __WIDGETTEXTTICKER_HPP

#include <Arduino.h>
#include <lvgl.h>
#include "LEDcontrol.hpp"

LV_FONT_DECLARE(PerfectPixel_20);

namespace lvgl
{
    class WidgetTextTicker
    {
    public:
        WidgetTextTicker(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, uint32_t anim_speed = 100)
        {
            label = lv_label_create(lv_scr_act());
            set_text(""); // Don't show "Text" during initialization
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_font(label, &PerfectPixel_20, LV_PART_MAIN);
            lv_obj_set_style_anim_speed(label, anim_speed, LV_PART_MAIN);
            lv_obj_align(label, align, x_ofs, y_ofs);
            lv_obj_set_width(label, w);
            lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        }

        void set_text(const char *t_text)
        {
            if(strcmp(t_text, lv_label_get_text(label)) == 0)
                return; // Same string as before, do not set again as it would disturb long-text- circular animation

            lv_label_set_text(label, t_text);
        }

    private:
        lv_obj_t *label;
    };
} // namespace lvgl

#endif // __WIDGETTEXTTICKER_HPP