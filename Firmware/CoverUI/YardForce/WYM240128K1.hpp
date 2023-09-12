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

#define TOP_STATUS_BAR_GAP_PX 5 // Amount of (gap) pixels between top status-bar icons/symbols

#include "UC1698.h"

#include <Arduino.h>
#include <lvgl.h>
#include "LEDcontrol.h"
#include "WidgetLedSymbol.hpp"
#include "WidgetBar.hpp"
#include "WidgetTextTicker.hpp"

// C images
LV_IMG_DECLARE(OM_Logo_120x54x1);
LV_IMG_DECLARE(OM_Wordmark_240x35x1);

namespace display
{
    UC1698 uc1698; // Display controller

    // LVGL buffers
    static lv_disp_draw_buf_t lv_disp_buf;
    static lv_color_t lv_buf_1[UC1698_DISPLAY_WIDTH * LVGL_BUFFER_MULTIPLIER];
    static lv_color_t lv_buf_2[UC1698_DISPLAY_WIDTH * LVGL_BUFFER_MULTIPLIER];

    lv_disp_drv_t lv_disp_drv; // LVGL driver

    // Status Screen Widgets
    WidgetLedSymbol *v_led_heartbeat, *v_led_ros,
        *v_led_emergency_wheel, *v_led_emergency, *v_led_emergency_stop,
        *v_led_charge;
    WidgetBar *bar_gps, *bar_bat;
    WidgetTextTicker *text_ticker_status;

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
            for (x = area->x1; x <= area->x2; x += 3) // FIXME: Might overflow buffer if area->x2 is not dividable by 3
            {
                // Color is inverted (0 = black but pixel off / >0 = white but pixel on) but UC1698 "[16] Set Inverse Display" is set
                uc1698.drawPixelTriplet(t_color_p->full, (t_color_p + 1)->full, (t_color_p + 2)->full);
                t_color_p += 3;
            }
        }
        lv_disp_flush_ready(disp_drv);
    }

    static void mainStatusScreen()
    {
        // On the left side of the status bar we do have functional status symbols like heartbeat and ROS
        v_led_heartbeat = new WidgetLedSymbol(FA_SYMBOL_HEARTBEAT, LV_ALIGN_TOP_LEFT, 0, 0); // Leftmost
        v_led_ros = new WidgetLedSymbol(FA_SYMBOL_ROS, LV_ALIGN_TOP_LEFT, 14 + TOP_STATUS_BAR_GAP_PX, 0);

        // In the middle, we do have emergencies
        v_led_emergency = new WidgetLedSymbol(FA_SYMBOL_EMERGENCY, LV_ALIGN_TOP_MID, 0, 0);                                           // Centered
        v_led_emergency_wheel = new WidgetLedSymbol(FA_SYMBOL_EMERGENCY_WHEEL, LV_ALIGN_TOP_MID, -14 - TOP_STATUS_BAR_GAP_PX - 2, 0); // Left of centered
        v_led_emergency_stop = new WidgetLedSymbol(FA_SYMBOL_EMERGENCY_STOP, LV_ALIGN_TOP_MID, 14 + TOP_STATUS_BAR_GAP_PX, 0);        // Right of centered

        // On the right side, mowing status like, charging, docking, ...
        v_led_charge = new WidgetLedSymbol(FA_SYMBOL_CHARGE, LV_ALIGN_OUT_TOP_RIGHT, (240 - (1 * 14)), 0); // Rightmost

        // GPS & Battery bars
        bar_gps = new WidgetBar(FA_SYMBOL_GPS2 " %d %%", LV_ALIGN_TOP_MID, 0, 30, UC1698_DISPLAY_WIDTH, 21);
        bar_bat = new WidgetBar(FA_SYMBOL_BATTERY " %d %%", LV_ALIGN_TOP_MID, 0, 60, UC1698_DISPLAY_WIDTH, 21);

        // Mower status text (ticker)
        text_ticker_status = new WidgetTextTicker(LV_ALIGN_TOP_MID, 0, 95, UC1698_DISPLAY_WIDTH);
        text_ticker_status->set_text("Mower status like Idle, Mowing, Emergency, ...");
    }

    static void anim_x_cb(void *var, int32_t v)
    {
        lv_obj_set_x((lv_obj_t *)var, v);
    }

    void openmower_anim()
    {
        // Mower Logo - img_logo
        lv_obj_t *img_logo = lv_img_create(lv_scr_act());
        lv_img_set_src(img_logo, &OM_Logo_120x54x1);
        lv_obj_align(img_logo, LV_ALIGN_CENTER, 0, -25);

        // OpenMower Wordmark - img_wordmark
        lv_obj_t *img_wordmark = lv_img_create(lv_scr_act());
        lv_img_set_src(img_wordmark, &OM_Wordmark_240x35x1);
        lv_obj_align(img_wordmark, LV_ALIGN_CENTER, 0, 25);

        // Anim of logo
        lv_anim_t al;
        lv_anim_init(&al);
        lv_anim_set_var(&al, img_logo);
        lv_anim_set_values(&al, 0, -((UC1698_DISPLAY_WIDTH / 2) + (OM_Logo_120x54x1.header.w / 2)));
        lv_anim_set_time(&al, 2000);
        lv_anim_set_delay(&al, 1000);
        lv_anim_set_exec_cb(&al, (lv_anim_exec_xcb_t)anim_x_cb);
        lv_anim_set_path_cb(&al, lv_anim_path_ease_in);
        lv_anim_start(&al);

        // Anim of Wordmark
        lv_anim_t aw;
        lv_anim_init(&aw);
        lv_anim_set_var(&aw, img_wordmark);
        lv_anim_set_values(&aw, 0, (UC1698_DISPLAY_WIDTH / 2) + (OM_Wordmark_240x35x1.header.w / 2) + 20);
        lv_anim_set_time(&aw, 2500);
        lv_anim_set_delay(&aw, 1700);
        lv_anim_set_exec_cb(&aw, (lv_anim_exec_xcb_t)anim_x_cb);
        lv_anim_set_path_cb(&aw, lv_anim_path_ease_in);
        lv_anim_set_deleted_cb(&aw, (lv_anim_ready_cb_t)mainStatusScreen); // Set a callback to indicate when the animation is deleted (idle)
        lv_anim_start(&aw);
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
        disp = lv_disp_drv_register(&lv_disp_drv);                               // Register the driver and save the created display objects
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), LV_PART_MAIN); // No background color

        openmower_anim();
        //mainStatusScreen();

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

            if (bar_gps != nullptr)
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
                bar_gps->set_value(gps_perc);
            }

            if (bar_bat != nullptr)
            {
                // Rev-calc Battery LEDs to percent
                uint8_t bat_perc = 0;
                for (uint8_t i = 0; i < 7; i++)
                {
                    if (::leds.get(10 - i) != LED_state::LED_on) // 10 = LED_MON
                        continue;
                    bat_perc += 15; // One LED represents 14.3%
                }
                bar_bat->set_value(bat_perc);
            }

            if (v_led_charge != nullptr && v_led_ros != nullptr)
            {
                v_led_charge->set(leds.get(LED_NUM_CHARGE));
                v_led_ros->set(leds.get(LED_NUM_S1));
            }
        }
    }
} // namespace display
#endif // __WYM240128K1_HPP