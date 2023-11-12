/**
 * @file Buttons.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Buttons class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.6
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include <stdint.h>
#include <map>
#include "include/Buttons.hpp"
#include "../BttnCtl.h"

#ifdef MCU_STM32
#define DIGITAL_PIN_TO_PORT_NR(p) (STM_PORT(digitalPinToPinName(p)))
#else // MCU_GD32
#define DIGITAL_PIN_TO_PORT_NR(p) (GD_PORT_GET(DIGITAL_TO_PINNAME(p)))
#endif

extern void sendMessage(void *message, size_t size);

/**
 * @brief Setup GPIOs
 *
 */
void Buttons::setup()
{
    for (auto const &it : kBtnDefByNumMap) // Loop over Button-Num -> button pin map
    {
        // Create debouncer if not already exists for this Pin's GPIO_Port_Nr
        uint32_t gpio_port_nr = DIGITAL_PIN_TO_PORT_NR(it.second.pin);
        auto debouncer = debouncer_by_gpio_port_nr_map.find(gpio_port_nr);
        if (debouncer == debouncer_by_gpio_port_nr_map.end())
            debouncer_by_gpio_port_nr_map.insert(std::pair<uint32_t, ButtonDebouncer>(gpio_port_nr, ButtonDebouncer()));

        pinMode(it.second.pin, INPUT_PULLUP);
    }
};

/**
 * @brief Process GPIO states by debouncer. Has to get called regulary i.e. by timer (5ms)
 *
 */
void Buttons::process_states()
{
    for (std::map<uint32_t, ButtonDebouncer>::iterator it = debouncer_by_gpio_port_nr_map.begin(); it != debouncer_by_gpio_port_nr_map.end(); ++it)
        it->second.process_state(it->first);
};

/**
 * @brief Get corresponding LED num for button num
 *
 * @param button_nr
 * @return uint8_t LED num. -1 of not exists.
 */
int8_t Buttons::get_led(uint8_t button_nr)
{
    auto btn_def_it = kBtnDefByNumMap.find(button_nr); // Find button_nr and get iterator pair
    if (btn_def_it != kBtnDefByNumMap.end())
        return btn_def_it->second.led_num;

    return -1;
}

/**
 * @brief Return boolean true if the given button number is pressed.
 *        Take into notice that the returned state is already debounced.
 *
 * @param uint8_t button_nr
 * @return true if pressed, false if not pressed
 */
bool Buttons::is_pressed(uint8_t button_nr)
{
    auto btn_def_it = kBtnDefByNumMap.find(button_nr); // Find button_nr and get iterator pair
    if (btn_def_it != kBtnDefByNumMap.end())
    {
        uint32_t gpio_port_nr = DIGITAL_PIN_TO_PORT_NR(btn_def_it->second.pin);
        auto debouncer_it = debouncer_by_gpio_port_nr_map.find(gpio_port_nr); // Find debouncer and get iterator pair
        if (debouncer_it != debouncer_by_gpio_port_nr_map.end())
        {
            return debouncer_it->second.is_pressed(btn_def_it->second.pin);
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
uint8_t Buttons::is_pressed()
{
    for (auto const &it : kBtnDefByNumMap) // Loop over Button-Num -> button pin map
        if (is_pressed(it.first))
            return it.first;

    return 0;
};

/**
 * @brief Send 'rain' message via COBS with last read rain-sensor- value (together with (currently static) threshold)
 *
 */
void Buttons::send(uint16_t button_id, uint8_t press_duration)
{
    msg_event_button msg = {
        .type = Get_Button,
        .button_id = button_id,
        .press_duration = press_duration};
    sendMessage(&msg, sizeof(msg));
}
