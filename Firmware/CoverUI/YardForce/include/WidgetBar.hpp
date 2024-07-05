/**
 * @file WidgetBar.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief Tiny class/wrapper for a progress-bar, which get displayed for GPS or Battery (as LVGL bar) for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 */
#ifndef __WIDGETBAR_HPP
#define __WIDGETBAR_HPP

#include <Arduino.h>
#include <string>
#include <lvgl.h>
#include "LEDcontrol.hpp"

namespace lvgl
{
    static void event_bar_label_cb(lv_event_t *e);

    class WidgetBar
    {
    public:
        std::string bar_label; // sprintf formatted bar label like "<UNICODE SYMBOL> %d %%"

        WidgetBar(const std::string &t_bar_label, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, lv_coord_t h);

        void set_value(unsigned int value);
        void set_range(int32_t min, int32_t max);

        void add_flag(lv_obj_flag_t flag);
        void clear_flag(lv_obj_flag_t flag);

    private:
        lv_obj_t *bar_;
        lv_style_t bar_style_bg_, bar_style_indic_;
    };
} // namespace lvgl

#endif // __WIDGETBAR_HPP