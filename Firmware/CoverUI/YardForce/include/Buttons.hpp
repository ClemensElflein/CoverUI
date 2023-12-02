/**
 * @file Buttons.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Buttons header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.6
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_HPP
#define YARDFORCE_BUTTONS_HPP

#include <Arduino.h>
#include <stdint.h>
#include <map>
#include "ButtonDebouncer.hpp"

// Logic button numbers. Take attention that OM known buttons need to have the same logic number!
// 0 is reserved for no-button return
#define BTN_CLK_NUM 1
#define BTN_HOME_NUM 2
#define BTN_PLAY_NUM 3 // or Start
#define BTN_S1_NUM 4
#define BTN_S2_NUM 5
#define BTN_LOCK_NUM 6
#define BTN_OK_NUM 7 // or Enter
#define BTN_MON_NUM 8
#define BTN_TUE_NUM 9
#define BTN_WED_NUM 10
#define BTN_THU_NUM 11
#define BTN_FRI_NUM 12
#define BTN_SAT_NUM 13
#define BTN_SUN_NUM 14
// (yet) unknown OM buttons. Let's start from 0x20 to have some free space for more (future) OM supported buttons
#define BTN_UP_NUM 32    // i.e. SAxPRO display
#define BTN_DOWN_NUM 33  // i.e. SAxPRO display
#define BTN_BACK_NUM 34  // i.e. SAxPRO display
#define BTN_4H_NUM 35    // i.e. RM-ECOW-V100
#define BTN_6H_NUM 36    // i.e. RM-ECOW-V100
#define BTN_8H_NUM 37    // i.e. RM-ECOW-V100
#define BTN_10H_NUM 38   // i.e. RM-ECOW-V100
#define BTN_SETUP_NUM 39 // i.e. RM-ECOW-V100 (WLAN-Setup button)
#define BTN_MENU_NUM 40  // i.e. RM-EC3-V11
#define BTN_0_NUM 48     // ASCII 0, i.e. RM-EC3-V11
#define BTN_1_NUM 49     // ASCII 1, i.e. RM-EC3-V11
#define BTN_2_NUM 50     // ASCII 2, i.e. RM-EC3-V11
#define BTN_3_NUM 51     // ASCII 3, i.e. RM-EC3-V11
#define BTN_4_NUM 52     // ASCII 4, i.e. RM-EC3-V11
#define BTN_5_NUM 53     // ASCII 5, i.e. RM-EC3-V11
#define BTN_6_NUM 54     // ASCII 6, i.e. RM-EC3-V11
#define BTN_7_NUM 55     // ASCII 7, i.e. RM-EC3-V11
#define BTN_8_NUM 56     // ASCII 8, i.e. RM-EC3-V11
#define BTN_9_NUM 57     // ASCII 9, i.e. RM-EC3-V11

class Buttons
{
public:
    struct ButtonDef
    {
        uint8_t pin;
        int8_t led_num; // Corresponding LED num. -1 is none.
    };

    const std::map<uint8_t, ButtonDef> &kBtnDefByNumMap;               // Map of Button-Num -> ButtonDef (pin & corresponding LED num)
    std::map<uint32_t, ButtonDebouncer> debouncer_by_gpio_port_nr_map; // Map of GPIO Port Nr -> debouncer object

    Buttons(const std::map<uint8_t, ButtonDef> &t_kBtnDefByNumMap) : kBtnDefByNumMap(t_kBtnDefByNumMap){};

    virtual ~Buttons() = default;

    int8_t get_led(uint8_t button_nr);
    bool is_pressed(uint8_t button_nr);
    uint8_t is_pressed();

    void process_states();

    void setup();
    void send(uint16_t button_id, uint8_t press_duration);
};

#endif // YARDFORCE_BUTTONS_HPP
