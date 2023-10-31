/**
 * @file Buttons.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Buttons class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.5
 * @date 2023-10-27
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

/*#elif defined(MDL_SAXPRO) // Model SAxPRO
#define BTN_PLAY_PIN PC0  // or Start
#define BTN_HOME_PIN PC1
#define BTN_UP_PIN PB14
#define BTN_DOWN_PIN PB13
#define BTN_OK_PIN PB12 // or Enter
#define BTN_BACK_PIN PB15
#endif*/

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

#ifdef MCU_STM32
#define DIGITAL_PIN_TO_PORT_NR(p) (STM_PORT(digitalPinToPinName(p)))
#else // MCU_GD32
#define DIGITAL_PIN_TO_PORT_NR(p) (GD_PORT_GET(DIGITAL_TO_PINNAME(p)))
#endif

class Buttons
{
public:
    const std::map<uint8_t, uint8_t> &kPpinByNumMap;                   // Map of Button-Num -> button pin
    std::map<uint32_t, ButtonDebouncer> debouncer_by_gpio_port_nr_map; // Map of GPIO Port Nr -> debouncer object

    Buttons(const std::map<uint8_t, uint8_t> &t_kPpinByNumMap) : kPpinByNumMap(t_kPpinByNumMap){};

    /*#elif defined(MDL_SAXPRO) // Model SAxPRO
        const uint8_t kOMButtonNrs[3] = { // Logic button numbers supported by OM. Use same order as in OM FW so that they get scanned in the same order!
            BTN_HOME_NUM, BTN_PLAY_NUM, BTN_OK_NUM};
    #endif*/

    /**
     * @brief Setup GPIOs
     *
     */
    void setup()
    {
        for (auto const &it : kPpinByNumMap) // Loop over Button-Num -> button pin map
        {
            // Create debouncer if not already exists for this Pin's GPIO_Port_Nr
            uint32_t gpio_port_nr = DIGITAL_PIN_TO_PORT_NR(it.second);
            auto debouncer = debouncer_by_gpio_port_nr_map.find(gpio_port_nr);
            if (debouncer == debouncer_by_gpio_port_nr_map.end())
                debouncer_by_gpio_port_nr_map.insert(std::pair<uint32_t, ButtonDebouncer>(gpio_port_nr, ButtonDebouncer()));

            pinMode(it.second, INPUT_PULLUP);
        }
    };

    /**
     * @brief Process GPIO states by debouncer. Has to get called regulary i.e. by timer (5ms)
     *
     */
    void process_states()
    {
        for (std::map<uint32_t, ButtonDebouncer>::iterator it = debouncer_by_gpio_port_nr_map.begin(); it != debouncer_by_gpio_port_nr_map.end(); ++it)
            it->second.process_state(it->first);
    };

    /**
     * @brief Return boolean true if the given button number is pressed.
     *        Take into notice that the returned state is already debounced.
     *
     * @param uint8_t button_nr
     * @return true if pressed, false if not pressed
     */
    bool is_pressed(uint8_t button_nr)
    {
        auto pin_it = kPpinByNumMap.find(button_nr); // Find button_nr and get iterator pair
        if (pin_it != kPpinByNumMap.end())
        {
            uint32_t gpio_port_nr = DIGITAL_PIN_TO_PORT_NR(pin_it->second);
            auto debouncer_it = debouncer_by_gpio_port_nr_map.find(gpio_port_nr); // Find debouncer and get iterator pair
            if (debouncer_it != debouncer_by_gpio_port_nr_map.end())
            {
                return debouncer_it->second.is_pressed(pin_it->second);
            }
        }
        return false;
    };

    /**
     * @brief Return ButtonNum of the first detected pressed button.
     *        Take into notice that the returned state is already debounced.
     *
     * @return uint8_t 0 = none pressed, >0 = ButtonNum
     */
    uint8_t is_pressed()
    {
        for (auto const &it : kPpinByNumMap) // Loop over Button-Num -> button pin map
            if (is_pressed(it.first))
                return it.first;
    };

private:
    // Somehow static initialization...
    // All ports with a button get debounced per port, via timer callback
    /*#elif defined(MDL_SAXPRO) // Model SAxPRO
        const uint32_t kGpioPorts[NUM_GPIO_PORTS] = {GPIOB_BASE, GPIOC_BASE};
        ButtonDebouncer *debouncers_[NUM_GPIO_PORTS] = { // Debouncer obj for each port in the same order as kGpioPorts
            new ButtonDebouncer(), new ButtonDebouncer()};
        // Map logic button number to Button-definiton (Again: Some-how static with debouncer_index)
        const std::map<uint8_t, ButtonDef> kButtonNum2DefMap = {
            {BTN_HOME_NUM, {1, BTN_HOME_PIN}},
            {BTN_PLAY_NUM, {1, BTN_PLAY_PIN}},
            {BTN_OK_NUM, {0, BTN_OK_PIN}},
            {BTN_UP_NUM, {0, BTN_UP_PIN}},
            {BTN_DOWN_NUM, {0, BTN_DOWN_PIN}},
            {BTN_BACK_NUM, {0, BTN_BACK_PIN}}};
    #endif*/
};

#endif // YARDFORCE_BUTTONS_HPP
