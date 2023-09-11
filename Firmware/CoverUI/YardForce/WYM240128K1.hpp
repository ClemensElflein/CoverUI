/**
 * @file WYM240128K1.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI WYM240128K1 display driver class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-09-03
 *
 * @copyright Copyright (c) 2023
 */
#ifndef __WYM240128K1_HPP
#define __WYM240128K1_HPP

#define UC1698_DISPLAY_WIDTH 240
#define UC1698_DISPLAY_HEIGHT 128
#define LVGL_BUFFER_MULTIPLIER 10
#define LVGL_TIMER_HANDLER_PERIOD_MS 10 // 10ms lv_timer_handler() soft period

#include "UC1698.h"

#include <Arduino.h>
#include <lvgl.h>
#include "LEDcontrol.h"
#include "WidgetLedSymbol.hpp"

// C images
LV_IMG_DECLARE(OM_Logo_Inv_120x54x1);
LV_IMG_DECLARE(OM_Wordmark_Inv_240x35x1);

namespace display
{
    UC1698 uc1698; // Display controller

    // LVGL buffers
    static lv_disp_draw_buf_t lv_disp_buf;
    static lv_color_t lv_buf_1[UC1698_DISPLAY_WIDTH * LVGL_BUFFER_MULTIPLIER];
    static lv_color_t lv_buf_2[UC1698_DISPLAY_WIDTH * LVGL_BUFFER_MULTIPLIER];

    lv_disp_drv_t lv_disp_drv; // LVGL driver

    // Widgets
    lv_obj_t *gps_bar = nullptr; // GPS Bar
    const std::string gps_bar_label_format = FA_SYMBOL_GPS2 " %d %%";
    lv_obj_t *bat_bar = nullptr; // Battery Bar
    const std::string bat_bar_label_format = FA_SYMBOL_BATTERY " %d %%";
    lv_style_t bar_style_bg;
    lv_style_t bar_style_indic;

    // Status symbols
    WidgetLedSymbol *v_led_ros, *v_led_charge;

    /**
     * @brief Rounder callback will round the display area to a multiple of 3, on x axis (RGB control lines of a pixel are connected to 3 monochrome pixels)
     *
     * @param disp_drv
     * @param area
     */
    static void rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *area)
    {
        area->x1 = area->x1 - (area->x1 % 3);           // Round down to neares multiple of 3
        area->x2 = (area->x2 + 3) - (area->x2 % 3) - 1; // Round up to nearest multiple of 3, minus -1
    }

    /**
     * @brief Flush display buffer to display controller.
     * Done via uc1698::drawPixelTriplet() method, which doesn't look as efficient like direct data write,
     * but save the call to a further pixel-color-callback, as well as another buffer, which sounds more expensive.
     *
     * @param disp_drv
     * @param area
     * @param t_color_p
     */
    static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *t_color_p)
    {
        uint8_t x, y;
        lv_color_t *color_p;
        uc1698.setWindowProgramArea(area->x1, area->x2, area->y1, area->y2);

        for (y = area->y1; y <= area->y2; y++)
        {
            // color_p = t_color_p + (y * UC1698_DISPLAY_WIDTH) + area->x1;
            for (x = area->x1; x <= area->x2; x += 3) // FIXME: Might overflow buffer if area->x2 is not dividable by 3
            {
                uc1698.drawPixelTriplet(t_color_p->full, (t_color_p + 1)->full, (t_color_p + 2)->full);
                t_color_p += 3;
            }
        }
        lv_disp_flush_ready(disp_drv);
    }

    static void event_bar_label_cb(lv_event_t *e)
    {
        lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
        if (dsc->part != LV_PART_INDICATOR)
            return;

        lv_obj_t *obj = lv_event_get_target(e);
        int32_t bar_value = lv_bar_get_value(obj);
        lv_draw_label_dsc_t label_dsc;
        lv_draw_label_dsc_init(&label_dsc);

        char buf[12];
        lv_snprintf(buf, sizeof(buf), (const char *)lv_obj_get_user_data(obj), bar_value);

        lv_point_t txt_size;
        lv_txt_get_size(&txt_size, buf, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX,
                        label_dsc.flag);

        lv_area_t txt_area;
        // If the indicator is long enough put the text inside on the right
        if (lv_area_get_width(dsc->draw_area) > txt_size.x + 20)
        {
            txt_area.x2 = dsc->draw_area->x2 - 5;
            txt_area.x1 = txt_area.x2 - txt_size.x + 1;
            label_dsc.color = lv_color_black();
        }
        // If the indicator is still short put the text out of it on the right
        else
        {
            txt_area.x1 = dsc->draw_area->x2 + 5;
            txt_area.x2 = txt_area.x1 + txt_size.x - 1;
            label_dsc.color = lv_color_white();
        }

        txt_area.y1 = (dsc->draw_area->y1 + (lv_area_get_height(dsc->draw_area) - txt_size.y) / 2) + 1;
        txt_area.y2 = txt_area.y1 + txt_size.y;

        lv_draw_label(dsc->draw_ctx, &label_dsc, &txt_area, buf, NULL);
    }

    static void drawGPSBar()
    {
        gps_bar = lv_bar_create(lv_scr_act());
        lv_obj_set_user_data(gps_bar, (void *)gps_bar_label_format.c_str()); // Save label prefix to object user data
        lv_obj_remove_style_all(gps_bar);                                    // To have a clean start
        lv_obj_add_style(gps_bar, &bar_style_bg, 0);
        lv_obj_add_style(gps_bar, &bar_style_indic, LV_PART_INDICATOR);

        lv_obj_add_event_cb(gps_bar, event_bar_label_cb, LV_EVENT_DRAW_PART_END, NULL);
        lv_obj_set_size(gps_bar, UC1698_DISPLAY_WIDTH, 21);
        lv_obj_align(gps_bar, LV_ALIGN_TOP_MID, 0, 30);
    }

    static void drawBatBar()
    {
        bat_bar = lv_bar_create(lv_scr_act());
        lv_obj_set_user_data(bat_bar, (void *)bat_bar_label_format.c_str()); // Save label prefix to object user data
        lv_obj_remove_style_all(bat_bar);                                    // To have a clean start
        lv_obj_add_style(bat_bar, &bar_style_bg, 0);
        lv_obj_add_style(bat_bar, &bar_style_indic, LV_PART_INDICATOR);

        lv_obj_add_event_cb(bat_bar, event_bar_label_cb, LV_EVENT_DRAW_PART_END, NULL);
        lv_obj_set_size(bat_bar, UC1698_DISPLAY_WIDTH, 21);
        lv_obj_align(bat_bar, LV_ALIGN_TOP_MID, 0, 60);
    }

    static void mainStatusScreen()
    {
        // Status symbols, from right to left
        v_led_charge = new WidgetLedSymbol(FA_SYMBOL_CHARGE, LV_ALIGN_OUT_TOP_RIGHT, (240 - (1 * 14)), 0);
        v_led_ros = new WidgetLedSymbol(FA_SYMBOL_ROS, LV_ALIGN_OUT_TOP_RIGHT, (240 - (2 * 14) - 5), 0);

        drawGPSBar();
        drawBatBar();
    }

    static void anim_x_cb(void *var, int32_t v)
    {
        lv_obj_set_x((lv_obj_t *)var, v);
    }

    void openmower_anim()
    {
        // Mower Logo - img_logo
        lv_obj_t *img_logo = lv_img_create(lv_scr_act());
        lv_img_set_src(img_logo, &OM_Logo_Inv_120x54x1);
        lv_obj_align(img_logo, LV_ALIGN_CENTER, 0, -25);

        // OpenMower Wordmark - img_wordmark
        lv_obj_t *img_wordmark = lv_img_create(lv_scr_act());
        lv_img_set_src(img_wordmark, &OM_Wordmark_Inv_240x35x1);
        lv_obj_align(img_wordmark, LV_ALIGN_CENTER, 0, 25);

        // Anim of logo
        lv_anim_t al;
        lv_anim_init(&al);
        lv_anim_set_var(&al, img_logo);
        lv_anim_set_values(&al, 0, -((UC1698_DISPLAY_WIDTH / 2) + (OM_Logo_Inv_120x54x1.header.w / 2)));
        lv_anim_set_time(&al, 2000);
        lv_anim_set_delay(&al, 1000);
        lv_anim_set_exec_cb(&al, (lv_anim_exec_xcb_t)anim_x_cb);
        lv_anim_set_path_cb(&al, lv_anim_path_ease_in);
        lv_anim_start(&al);

        // Anim of Wordmark
        lv_anim_t aw;
        lv_anim_init(&aw);
        lv_anim_set_var(&aw, img_wordmark);
        lv_anim_set_values(&aw, 0, (UC1698_DISPLAY_WIDTH / 2) + (OM_Wordmark_Inv_240x35x1.header.w / 2));
        lv_anim_set_time(&aw, 2500);
        lv_anim_set_delay(&aw, 1700);
        lv_anim_set_exec_cb(&aw, (lv_anim_exec_xcb_t)anim_x_cb);
        lv_anim_set_path_cb(&aw, lv_anim_path_ease_in);
        lv_anim_set_deleted_cb(&aw, (lv_anim_ready_cb_t)mainStatusScreen); // Set a callback to indicate when the animation is deleted (idle)
        lv_anim_start(&aw);
    }

    static void test1()
    {
        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "test1() " FA_SYMBOL_GPS1 " " FA_SYMBOL_GPS2 " " FA_SYMBOL_BATTERY);
        lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);
    }

    bool init()
    {
        // Init UC1698 display controller
        if (!uc1698.init())
        {
            return false;
        }

        // Init LVGL
        lv_init();
        lv_disp_drv_init(&lv_disp_drv);                                                                         // Basic LVGL display driver initialization
        lv_disp_drv.draw_buf = &lv_disp_buf;                                                                    // Set an initialized buffer
        lv_disp_drv.rounder_cb = rounder_cb;                                                                    // Round x coordinated so that it fit for our 3 RGB pixel/per dot display
        lv_disp_drv.flush_cb = flush_cb;                                                                        // Set a flush callback to draw to the display
        lv_disp_drv.hor_res = UC1698_DISPLAY_WIDTH;                                                             // Set the horizontal resolution in pixels
        lv_disp_drv.ver_res = UC1698_DISPLAY_HEIGHT;                                                            // Set the vertical resolution in pixels
        lv_disp_draw_buf_init(&lv_disp_buf, lv_buf_1, lv_buf_2, UC1698_DISPLAY_WIDTH * LVGL_BUFFER_MULTIPLIER); // Initialize `disp_buf` with the buffer(s)
        lv_disp_t *disp;
        disp = lv_disp_drv_register(&lv_disp_drv);                                     // Register the driver and save the created display objects
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN); // No background color

        // Init bar style settings
        lv_style_init(&bar_style_bg);
        lv_style_set_border_color(&bar_style_bg, lv_color_hex(0xffffff));
        lv_style_set_border_width(&bar_style_bg, 2);
        lv_style_set_pad_all(&bar_style_bg, 3); // To make the indicator smaller
        lv_style_set_radius(&bar_style_bg, 4);

        lv_style_init(&bar_style_indic);
        lv_style_set_bg_opa(&bar_style_indic, LV_OPA_COVER);
        lv_style_set_bg_color(&bar_style_indic, lv_color_hex(0xffffff));
        lv_style_set_radius(&bar_style_indic, 1);

        // openmower_anim();
        mainStatusScreen();
        // test1();
        // testCanvas();

        return true;
    }

    /**
     * @brief Tell LVGL that <ms> milliseconds were elapsed.
     *  Required for anim or similar. Should be called in a high priority routine like interrupt.
     *
     * @param ms
     */
    void tick_inc(uint8_t ms)
    {
        lv_tick_inc(ms);
    }

    /**
     * @brief Regular loop() function, has to be called in Arduino's main loop.
     * Handles i.e. LVGL timers.
     * Has to be a lower priority routine than tick_inc(), otherwise all LVGL timers (or animations) do not work
     */
    void loop()
    {
        static uint32_t next_lv_timer_ms = 0;
        static uint32_t next_display_data_ms = 0;
        uint32_t now = millis();

        if (now >= next_lv_timer_ms)
        {
            next_lv_timer_ms = now + LVGL_TIMER_HANDLER_PERIOD_MS;
            lv_timer_handler();
        }

        // Refresh display data
        if (now >= next_display_data_ms)
        {
            next_display_data_ms = now + 100; // 100ms display data refresh

            if (gps_bar != nullptr)
            {
                // Rev-calc GPS LEDs to percent
                int8_t gps_perc = 0;
                for (uint8_t i = 0; i < 4; i++)
                {
                    if (::leds.get(17 - i) == LED_state::LED_blink_fast) // 17 = LED_2HR
                    {
                        gps_perc = -1; // No fix?
                        break;
                    }
                    if (::leds.get(17 - i) != LED_state::LED_on) // 17 =
                    {
                        gps_perc += 25; // One LED represents 25%
                    }
                }
                lv_bar_set_value(gps_bar, gps_perc, LV_ANIM_OFF);
            }

            if (bat_bar != nullptr)
            {
                // Rev-calc Battery LEDs to percent
                uint8_t bat_perc = 0;
                for (uint8_t i = 0; i < 7; i++)
                {
                    if (::leds.get(10 - i) != LED_state::LED_on) // 10 = LED_MON
                        continue;
                    bat_perc += 15; // One LED represents 14.3%
                }
                lv_bar_set_value(bat_bar, bat_perc, LV_ANIM_OFF);
            }

            v_led_charge->set(leds.get(LED_NUM_CHARGE));
        }
    }
} // namespace display
#endif // __WYM240128K1_HPP