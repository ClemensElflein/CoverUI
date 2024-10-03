/**
 * @file Buttons.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Buttons class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.7
 * @date 2024-10-01
 *
 * @copyright Copyright (c) 2023, 2024
 *
 */
#ifndef YARDFORCE_BUTTONS_HPP
#define YARDFORCE_BUTTONS_HPP

#include <Arduino.h>
#include <etl/map.h>
#include <stdint.h>

#include <map>

#include "../../BttnCtl.h"
#include "../include/ButtonDebouncer.hpp"

// Logic button numbers. Take attention that OM known buttons need to have the same logic number!
// 0 is reserved for no-button return
#define BTN_CLK_NUM 1
#define BTN_HOME_NUM 2
#define BTN_PLAY_NUM 3  // or Start
#define BTN_S1_NUM 4
#define BTN_S2_NUM 5
#define BTN_LOCK_NUM 6
#define BTN_OK_NUM 7  // or Enter
#define BTN_MON_NUM 8
#define BTN_TUE_NUM 9
#define BTN_WED_NUM 10
#define BTN_THU_NUM 11
#define BTN_FRI_NUM 12
#define BTN_SAT_NUM 13
#define BTN_SUN_NUM 14
// (yet) unknown OM buttons. Let's start from 0x20 to have some free space for more (future) OM supported buttons
#define BTN_UP_NUM 32     // i.e. SAxPRO display
#define BTN_DOWN_NUM 33   // i.e. SAxPRO display
#define BTN_BACK_NUM 34   // i.e. SAxPRO display
#define BTN_4H_NUM 35     // i.e. RM-ECOW-V100
#define BTN_6H_NUM 36     // i.e. RM-ECOW-V100
#define BTN_8H_NUM 37     // i.e. RM-ECOW-V100
#define BTN_10H_NUM 38    // i.e. RM-ECOW-V100
#define BTN_SETUP_NUM 39  // i.e. RM-ECOW-V100 (WLAN-Setup button)
#define BTN_MENU_NUM 40   // i.e. RM-EC3-V11
#define BTN_0_NUM 48      // ASCII 0, i.e. RM-EC3-V11
#define BTN_1_NUM 49      // ASCII 1, i.e. RM-EC3-V11
#define BTN_2_NUM 50      // ASCII 2, i.e. RM-EC3-V11
#define BTN_3_NUM 51      // ASCII 3, i.e. RM-EC3-V11
#define BTN_4_NUM 52      // ASCII 4, i.e. RM-EC3-V11
#define BTN_5_NUM 53      // ASCII 5, i.e. RM-EC3-V11
#define BTN_6_NUM 54      // ASCII 6, i.e. RM-EC3-V11
#define BTN_7_NUM 55      // ASCII 7, i.e. RM-EC3-V11
#define BTN_8_NUM 56      // ASCII 8, i.e. RM-EC3-V11
#define BTN_9_NUM 57      // ASCII 9, i.e. RM-EC3-V11

#define BTN_HAS_NO_LED -1

#define MAX_BTN_GPIO_PORTS 6  // Our MCUs do have GPIO-Ports A-F = 6

#ifdef MCU_STM32
#define DIGITAL_PIN_TO_PORT_NR(p) (STM_PORT(digitalPinToPinName(p)))
#else  // MCU_GD32
#define DIGITAL_PIN_TO_PORT_NR(p) (GD_PORT_GET(DIGITAL_TO_PINNAME(p)))
#endif

extern void sendMessage(void *message, size_t size);

class Buttons {
   public:
    struct ButtonDef {
        uint8_t pin;
        int8_t led_num;  // Corresponding LED num. -1 is none.
    };
    const std::map<uint8_t, ButtonDef> &kBtnDefByNumMap;  // Ref to Map of Button-Num -> ButtonDef (pin & corresponding LED num)

    Buttons(const std::map<uint8_t, ButtonDef> &t_kBtnDefByNumMap) : kBtnDefByNumMap(t_kBtnDefByNumMap) {
        // Loop over Button-Num -> button pin map
        for (auto const &it : kBtnDefByNumMap) {
            // Create debouncer if not already exist for this Pin's GPIO_Port_Nr
            uint32_t gpio_port_nr = DIGITAL_PIN_TO_PORT_NR(it.second.pin);
            auto debouncer = debouncer_by_gpio_port_nr_map.find(gpio_port_nr);
            if (debouncer == debouncer_by_gpio_port_nr_map.end())
                debouncer_by_gpio_port_nr_map.insert(etl::pair<uint32_t, ButtonDebouncer>(gpio_port_nr, ButtonDebouncer()));

            pinMode(it.second.pin, INPUT_PULLUP);
        }
    }

    /**
     * @brief Handle CoverUI specific keys like BootUp-Anim or version number display.
     * Should be continuously called <= 500ms.
     */
    virtual void handle_sys_req() {}

    /**
     * @brief Get corresponding LED num for button num
     *
     * @param button_nr
     * @return uint8_t LED num. -1 of not exists.
     */
    int8_t get_led(uint8_t button_nr) {
        auto btn_def_it = kBtnDefByNumMap.find(button_nr);  // Find button_nr and get iterator pair
        if (btn_def_it != kBtnDefByNumMap.end())
            return btn_def_it->second.led_num;

        return -1;
    }

    /**
     * @brief Return ButtonNum of the first detected pressed button.
     *        Take into notice that the returned state is already debounced.
     *
     * @return uint8_t 0 = none pressed, >0 = ButtonNum
     */
    uint8_t is_pressed() {
        for (auto const &it : kBtnDefByNumMap)  // Loop over Button-Num -> button pin map
            if (is_pressed(it.first))
                return it.first;

        return 0;
    }

    /**
     * @brief Return boolean true if the given button number is pressed.
     *        Take into notice that the returned state is already debounced.
     *
     * @param uint8_t button_nr
     * @return true if pressed, false if not pressed
     */
    bool is_pressed(uint8_t button_nr) {
        auto btn_def_it = kBtnDefByNumMap.find(button_nr);  // Find button_nr and get iterator pair
        if (btn_def_it == kBtnDefByNumMap.end())
            return false;

        uint32_t gpio_port_nr = DIGITAL_PIN_TO_PORT_NR(btn_def_it->second.pin);
        auto debouncer_it = debouncer_by_gpio_port_nr_map.find(gpio_port_nr);  // Find debouncer and get iterator pair
        if (debouncer_it == debouncer_by_gpio_port_nr_map.end())
            return false;

        return debouncer_it->second.is_pressed(btn_def_it->second.pin);
    }

    /**
     * @brief Process GPIO states by debouncer. Has to get called regulary i.e. by timer (5ms)
     *
     */
    void process_states() {
        for (auto &it : debouncer_by_gpio_port_nr_map)
            it.second.process_state(it.first);
    }

    /**
     * @brief Send 'rain' message via COBS with last read rain-sensor- value (together with (currently static) threshold)
     *
     */
    void send(uint16_t button_id, uint8_t press_duration) {
        msg_event_button msg = {
            .type = Get_Button,
            .button_id = button_id,
            .press_duration = press_duration};
        sendMessage(&msg, sizeof(msg));
    }

   private:
    etl::map<uint32_t, ButtonDebouncer, MAX_BTN_GPIO_PORTS> debouncer_by_gpio_port_nr_map;  // Map of GPIO Port Nr -> debouncer object
};

#endif  // YARDFORCE_BUTTONS_HPP
