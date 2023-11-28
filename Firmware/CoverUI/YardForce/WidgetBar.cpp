/**
 * @file WidgetBar.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief Tiny class/wrapper for a progress-bar, which get displayed for GPS or Battery (as LVGL bar) for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 */
#include "include/WidgetBar.hpp"

namespace lvgl
{
    static void event_bar_label_cb(lv_event_t *e)
    {
        lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
        if (dsc->part != LV_PART_INDICATOR)
            return;

        lv_obj_t *obj = lv_event_get_target(e);
        int32_t bar_value = lv_bar_get_value(obj);
        lv_draw_label_dsc_t label_dsc;
        lv_draw_label_dsc_init(&label_dsc);

        char buf[12]; // FIXME: Static value is risky
        lv_snprintf(buf, sizeof(buf), (const char *)lv_obj_get_user_data(obj), bar_value);

        lv_point_t txt_size;
        lv_txt_get_size(&txt_size, buf, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

        lv_area_t txt_area;
        // If the indicator is long enough put the text inside on the right
        if (lv_area_get_width(dsc->draw_area) > txt_size.x + 20)
        {
            txt_area.x2 = dsc->draw_area->x2 - 5;
            txt_area.x1 = txt_area.x2 - txt_size.x + 1;
            label_dsc.color = lv_color_white();
        }
        // If the indicator is still short put the text out of it on the right
        else
        {
            txt_area.x1 = dsc->draw_area->x2 + 5;
            txt_area.x2 = txt_area.x1 + txt_size.x - 1;
            label_dsc.color = lv_color_black();
        }

        txt_area.y1 = (dsc->draw_area->y1 + (lv_area_get_height(dsc->draw_area) - txt_size.y) / 2) + 1;
        txt_area.y2 = txt_area.y1 + txt_size.y;

        lv_draw_label(dsc->draw_ctx, &label_dsc, &txt_area, buf, NULL);
    }

    WidgetBar::WidgetBar(const std::string &t_bar_label, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, lv_coord_t h) : bar_label(t_bar_label)
    {
        // Init bar style settings for custom (labeled) bar graph
        lv_style_init(&bar_style_bg_);
        lv_style_set_border_color(&bar_style_bg_, lv_color_black());
        lv_style_set_border_width(&bar_style_bg_, 2);
        lv_style_set_pad_all(&bar_style_bg_, 3); // To make the indicator smaller
        lv_style_set_radius(&bar_style_bg_, 4);

        lv_style_init(&bar_style_indic_);
        lv_style_set_bg_opa(&bar_style_indic_, LV_OPA_COVER);
        lv_style_set_bg_color(&bar_style_indic_, lv_color_black());
        lv_style_set_radius(&bar_style_indic_, 1);

        // Draw bar
        bar_ = lv_bar_create(lv_scr_act());
        lv_obj_set_user_data(bar_, (void *)bar_label.c_str()); // Store bar label to object user data
        lv_obj_remove_style_all(bar_);                         // To have a clean start
        lv_obj_add_style(bar_, &bar_style_bg_, 0);
        lv_obj_add_style(bar_, &bar_style_indic_, LV_PART_INDICATOR);

        lv_obj_add_event_cb(bar_, event_bar_label_cb, LV_EVENT_DRAW_PART_END, NULL);
        lv_obj_set_size(bar_, w, h);
        lv_obj_align(bar_, align, x_ofs, y_ofs);
    }

    void WidgetBar::set_value(unsigned int value)
    {
        lv_bar_set_value(bar_, value, LV_ANIM_OFF);
    }

    void WidgetBar::set_range(int32_t min, int32_t max)
    {
        lv_bar_set_range(bar_, min, max);
    }

    void WidgetBar::add_flag(lv_obj_flag_t flag)
    {
        lv_obj_add_flag(bar_, flag);
    }

    void WidgetBar::clear_flag(lv_obj_flag_t flag)
    {
        lv_obj_clear_flag(bar_, flag);
    }

} // namespace lvgl
