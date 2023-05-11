/**
 * @file Buttons.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Buttons class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Buttons.h"
#include "ButtonDebouncer.h"

/**
 * @brief Setup LED GPIOs
 *
 */
void Buttons::setup()
{
    printf("Buttons Setup\n");

    for (ButtonDef btn_def : ButtonNrs)
        pinMode(btn_def.digital_pin, INPUT_PULLUP);
}

void Buttons::process_states()
{
    for (uint8_t i = 0; i < NUM_GPIO_PORTS; i++)
        debouncers_[i]->process_state(kGpioPorts[i]);
}

/**
 * @brief Get (debounced) status of the given GPIO index.
 * See Buttons.h: kGpioPorts for the GPIO indexes.
 *
 * @param gpio_index
 * @return uint16_t
 */
uint16_t Buttons::get_status(uint8_t gpio_index)
{
    return debouncers_[gpio_index]->get_status();
}

/**
 * @brief Return boolean if the given button number is pressed.
 *        Take into notice that the returned state is already debounced.
 *
 * @param button_nr
 * @return true
 * @return false
 */
bool Buttons::is_pressed(uint8_t button_nr)
{
    return get_status(ButtonNrs[button_nr].debouncer_index) & digitalPinToBitMask(ButtonNrs[button_nr].digital_pin);
};