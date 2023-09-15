/**
 * @file Buttons.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI Buttons class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.4
 * @date 2023-09-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_BUTTONS_HPP
#define YARDFORCE_BUTTONS_HPP

#include <Arduino.h>
#include <stdint.h>
#include <map>
#include "ButtonDebouncer.h"

#ifdef MCU_GD32
#define GPIOA_BASE GPIOA
#define GPIOB_BASE GPIOB
#define GPIOC_BASE GPIOC
#define GPIOF_BASE GPIOF
#endif

#ifdef MDL_C500 // Model Classic 500
#define BTN_CLK_PIN PF4
#define BTN_OK_PIN PF5
#define BTN_S1_PIN PB2
#define BTN_S2_PIN PB10
#define BTN_LOCK_PIN PB11
#define BTN_MON_PIN PB12
#define BTN_TUE_PIN PB13
#define BTN_WED_PIN PB14
#define BTN_THU_PIN PB15
#define BTN_FRI_PIN PC6
#define BTN_SAT_PIN PC7
#define BTN_SUN_PIN PC8
#define BTN_PLAY_PIN PA11
#define BTN_HOME_PIN PA12

#define NUM_BUTTONS 14
#define NUM_GPIO_PORTS 4

#elif defined(MDL_SAXPRO) // Model SAxPRO
#define BTN_PLAY_PIN PC0  // or Start
#define BTN_HOME_PIN PC1
#define BTN_UP_PIN PB14
#define BTN_DOWN_PIN PB13
#define BTN_OK_PIN PB12 // or Enter
#define BTN_BACK_PIN PB15

#define NUM_BUTTONS 6
#define NUM_GPIO_PORTS 2
#endif

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
// OM unknown buttons. Let's start from 0x20 to have some free space for more (future) OM supported buttons
#define BTN_UP_NUM 32
#define BTN_DOWN_NUM 33
#define BTN_BACK_NUM 34

#ifndef NUM_GPIO_PORTS
#define NUM_GPIO_PORTS 0
#endif

class Buttons
{
public:
    typedef uint8_t ButtonNum;
    struct ButtonDef
    {
        uint8_t debouncer_index; // Debouncer index as defined in debouncers_ array
        uint8_t digital_pin;
    };

#ifdef MDL_C500                        // Model Classic 500
    const uint8_t kOMButtonNrs[14] = { // Logic button numbers supported by OM. Use same order as in OM FW so that they get scanned in the same order!
        BTN_CLK_NUM, BTN_HOME_NUM, BTN_PLAY_NUM, BTN_S1_NUM, BTN_S2_NUM, BTN_LOCK_NUM, BTN_OK_NUM,
        BTN_SUN_NUM, BTN_MON_NUM, BTN_TUE_NUM, BTN_WED_NUM, BTN_THU_NUM, BTN_FRI_NUM, BTN_SAT_NUM};
#elif defined(MDL_SAXPRO) // Model SAxPRO
    const uint8_t kOMButtonNrs[3] = { // Logic button numbers supported by OM. Use same order as in OM FW so that they get scanned in the same order!
        BTN_HOME_NUM, BTN_PLAY_NUM, BTN_OK_NUM};
#endif

    /**
     * @brief Setup GPIOs
     *
     */
    void setup()
    {
        printf("Buttons Setup\n");

        for (auto const &btn_def : kButtonNum2DefMap)
            pinMode(btn_def.second.digital_pin, INPUT_PULLUP);
    };

    /**
     * @brief Process GPIO states by debouncer. Has to get called regulary i.e. by timer (5ms)
     *
     */
    void process_states()
    {
        for (uint8_t i = 0; i < NUM_GPIO_PORTS; i++)
            debouncers_[i]->process_state(kGpioPorts[i]);
    };

    /**
     * @brief Return boolean true if the given button number is pressed.
     *        Take into notice that the returned state is already debounced.
     *
     * @param button_nr
     * @return true = pressed
     * @return false = not pressed
     */
    bool is_pressed(ButtonNum button_nr)
    {
        try
        {
            const ButtonDef btn_def = kButtonNum2DefMap.at(button_nr);
            return get_status(btn_def.debouncer_index) & digitalPinToBitMask(btn_def.digital_pin);
        }
        catch (const std::out_of_range &e)
        {
            return false;
        }
    };

    /**
     * @brief Return ButtonNum of the first detected pressed button.
     *        Take into notice that the returned state is already debounced.
     *
     * @return ButtonNum 0 = none pressed, >0 = ButtonNum
     */
    ButtonNum is_pressed()
    {
        for (auto const &btn_def : kButtonNum2DefMap)
            if(get_status(btn_def.second.debouncer_index) & digitalPinToBitMask(btn_def.second.digital_pin))
                return btn_def.first;
        return 0;
    };

private:
    // Somehow static initialization...
    // All ports with a button get debounced per port, via timer callback
#ifdef MDL_C500 // Model Classic 500
    const uint32_t kGpioPorts[NUM_GPIO_PORTS] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOF_BASE};
    ButtonDebouncer *debouncers_[NUM_GPIO_PORTS] = { // Debouncer obj for each port in the same order as kGpioPorts
        new ButtonDebouncer(), new ButtonDebouncer(),
        new ButtonDebouncer(), new ButtonDebouncer()};
    // Map logic button number to Button-definiton (Again: Some-how static with debouncer_index)
    const std::map<ButtonNum, ButtonDef> kButtonNum2DefMap = {
        {BTN_CLK_NUM, {3, BTN_CLK_PIN}},
        {BTN_HOME_NUM, {0, BTN_HOME_PIN}},
        {BTN_PLAY_NUM, {0, BTN_PLAY_PIN}},
        {BTN_S1_NUM, {1, BTN_S1_PIN}},
        {BTN_S2_NUM, {1, BTN_S2_PIN}},
        {BTN_LOCK_NUM, {1, BTN_LOCK_PIN}},
        {BTN_OK_NUM, {3, BTN_OK_PIN}},
        {BTN_MON_NUM, {1, BTN_MON_PIN}},
        {BTN_TUE_NUM, {1, BTN_TUE_PIN}},
        {BTN_WED_NUM, {1, BTN_WED_PIN}},
        {BTN_THU_NUM, {1, BTN_THU_PIN}},
        {BTN_FRI_NUM, {2, BTN_FRI_PIN}},
        {BTN_SAT_NUM, {2, BTN_SAT_PIN}},
        {BTN_SUN_NUM, {2, BTN_SUN_PIN}}};
#elif defined(MDL_SAXPRO) // Model SAxPRO
    const uint32_t kGpioPorts[NUM_GPIO_PORTS] = {GPIOB_BASE, GPIOC_BASE};
    ButtonDebouncer *debouncers_[NUM_GPIO_PORTS] = { // Debouncer obj for each port in the same order as kGpioPorts
        new ButtonDebouncer(), new ButtonDebouncer()};
    // Map logic button number to Button-definiton (Again: Some-how static with debouncer_index)
    const std::map<ButtonNum, ButtonDef> kButtonNum2DefMap = {
        {BTN_HOME_NUM, {1, BTN_HOME_PIN}},
        {BTN_PLAY_NUM, {1, BTN_PLAY_PIN}},
        {BTN_OK_NUM, {0, BTN_OK_PIN}},
        {BTN_UP_NUM, {0, BTN_UP_PIN}},
        {BTN_DOWN_NUM, {0, BTN_DOWN_PIN}},
        {BTN_BACK_NUM, {0, BTN_BACK_PIN}}};
#endif

    /**
     * @brief Get (debounced) status of the whole GPIO port by debouncer index.
     * See kGpioPorts definition for the GPIO debouncer indexes.
     *
     * @param gpio_index
     * @return uint16_t
     */
    uint16_t get_status(uint8_t debouncer_index)
    {
        return debouncers_[debouncer_index]->get_status();
    };
};

#endif // YARDFORCE_BUTTONS_HPP
